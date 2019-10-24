import ctypes
import hashlib
import binascii


class MicroECCPy:

    def __init__(self, library_path=None, curve_name='secp256r1'):
        """
        Args:
            library_path <str> : Full path to the compiled file of microECC library (libmicroecc.so)
            curve_name <str> : "secp256r1" (default), "secp160r1", "secp192r1", "secp224r1", "secp256k1"
        """

        self.lib = ctypes.CDLL(library_path or 'libmicroecc.so')

        # curve
        get_curve = getattr(self.lib, 'uECC_{}'.format(curve_name.lower()))
        get_curve.argtypes = []
        get_curve.restype = ctypes.c_voidp

        self.curve = get_curve()

        # sizes

        # public_key
        self.lib.uECC_curve_public_key_size.argtypes = [ctypes.c_void_p]
        self.lib.uECC_curve_public_key_size.restypes = ctypes.c_int

        self.public_key_size = self.lib.uECC_curve_public_key_size(self.curve)

        # curve
        self.curve_size = self.public_key_size // 2

        # private_key
        self.lib.uECC_curve_private_key_size.argtypes = [ctypes.c_void_p]
        self.lib.uECC_curve_private_key_size.restype = ctypes.c_int

        self.private_key_size = self.lib.uECC_curve_private_key_size(self.curve)

    def int_to_hex(self, n):
        """
        Args:
            n <int> : The number (byte-0 to 255) we want to be "translated" to a hex-string

        Returns:
            <str> : The hex-string representaiton of the number (byte) ('0A', 'BB', 'F1', for example)
        """

        return '{:02x}'.format(n).upper()

    def array_to_hexstr(self, a):
        """
        Args:
            a <ctypes.Array> [(ctypes.c_uint8 * N)()]

        Returns:
            <str> : A hex-string representing the bytes in the array ('AAB203F1....', for example)
        """

        s = ''
        for b in a:
            s += self.int_to_hex(b)

        return s

    def bytes_to_array(self, b, t=ctypes.c_uint8):
        """
        Args:
            b [<bytes>, ...]

        Return:
            ctypes.Array
        """

        a = (t * len(b))()
        for i, b in enumerate(b):
            a[i] = b

        return a

    def get_rng(self):
        """
        Returns a random number (int).

        Returns:
            <int>
        """

        self.lib.uECC_get_rng.argtypes = []
        self.lib.uECC_get_rng.restype = ctypes.c_int

        return self.lib.uECC_get_rng()

    def get_random_keypair(self):
        """
        Returns:
            <tuple>
                (<str: public_key>, <str: private_key>)
        """

        self.lib.uECC_make_key.argtypes = [
            ctypes.POINTER(ctypes.c_uint8),
            ctypes.POINTER(ctypes.c_uint8),
            ctypes.c_voidp
        ]

        self.lib.uECC_make_key.restype = ctypes.c_int

        private_key = (ctypes.c_uint8 * self.private_key_size)()
        public_key = (ctypes.c_uint8 * self.public_key_size)()

        res = self.lib.uECC_make_key(public_key, private_key, self.curve)
        if res == 0:
            raise Exception('Keypair was not generated successfully')

        public_key_ = self.array_to_hexstr(public_key)
        # print('public_key={}'.format(public_key_))
        # print('public_key={}'.format(list(public_key)))

        private_key_ = self.array_to_hexstr(private_key)
        # print('private_key={}'.format(private_key_))
        # print('private_key={}'.format(list(private_key)))

        return (public_key_, private_key_)

    def get_public_from_private(self, private_key):
        """
        Args:
            private_key <str> : A hex-string representing the private key

        Returns:
            <str>

        """

        private_key_ = self.bytes_to_array(binascii.unhexlify(private_key))

        self.lib.uECC_compute_public_key.argtypes = [
            ctypes.POINTER(ctypes.c_uint8),  # const uint8_t *private_key
            ctypes.POINTER(ctypes.c_uint8),  # uint8_t *public_key
            ctypes.c_void_p  # uECC_Curve curve
        ]

        self.lib.uECC_compute_public_key.restype = ctypes.c_int

        public_key_generated = (ctypes.c_uint8 * self.public_key_size)()
        self.lib.uECC_compute_public_key(private_key_, public_key_generated, self.curve)

        return self.array_to_hexstr(public_key_generated)

    def generate_shared_secret(self, its_public_key, my_private_key):
        """
        Args:
            its_public_key <str> : A hex-string ('AAB3F1DE....' == b'\xaa\xb3\xf1\xde...') representing the public key
            my_private_key <str> : A hex-string ('AAB3F1DE....' == b'\xaa\xb3\xf1\xde...') representing the private key

        Returns:
            <str> : A hex-string ('AAB3F1DE....' == b'\xaa\xb3\xf1\xde...') representing the secret
        """

        its_public_key_ = self.bytes_to_array(binascii.unhexlify(its_public_key))
        my_private_key_ = self.bytes_to_array(binascii.unhexlify(my_private_key))

        self.lib.uECC_shared_secret.argtypes = [
            ctypes.POINTER(ctypes.c_uint8),  # const uint8_t *public_key,
            ctypes.POINTER(ctypes.c_uint8),  # const uint8_t *private_key,
            ctypes.POINTER(ctypes.c_uint8),  # uint8_t *secret
            ctypes.c_void_p,  # uECC_Curve curve
        ]

        self.lib.uECC_shared_secret.restype = ctypes.c_int

        # define "secret" based on curve size
        secret = (ctypes.c_uint8 * self.curve_size)()

        # generate secret
        self.lib.uECC_shared_secret(its_public_key_, my_private_key_, secret, self.curve)

        return self.array_to_hexstr(secret)

    def compress(self, public_key):
        """
        Args:
            public_key <str> : A hex-string ('AAB3F1DE....' == b'\xaa\xb3\xf1\xde...') representing the public key

        Returns:
            <str> : A hex-string ('AAB3F1DE....' == b'\xaa\xb3\xf1\xde...') representing the compressed public key
        """

        public_key_ = self.bytes_to_array(binascii.unhexlify(public_key))

        self.lib.uECC_compress.argtypes = [
            ctypes.POINTER(ctypes.c_uint8),  # const uint8_t *public_key
            ctypes.POINTER(ctypes.c_uint8),  # uint8_t *compressed
            ctypes.c_void_p,  # uECC_Curve curve
        ]

        self.lib.uECC_compress.restype = None

        # define compressed key based on curve size (+1)
        public_key_compressed = ((ctypes.c_uint8) * (self.curve_size + 1))()

        # compress key
        self.lib.uECC_compress(public_key_, public_key_compressed, self.curve)

        return self.array_to_hexstr(public_key_compressed)

    def decompress(self, compressed_public_key):
        """
        Args:
            compressed_public_key <str> : A hex-string ('AAB3F1DE....' == b'\xaa\xb3\xf1\xde...') representing the compressed public key.

        Returns:
            <str> : A hex-string ('AAB3F1DE....' == b'\xaa\xb3\xf1\xde...') representing the uncompressed public key
        """

        public_key_compressed_ = self.bytes_to_array(binascii.unhexlify(compressed_public_key))

        self.lib.uECC_decompress.argtypes = [
            ctypes.POINTER(ctypes.c_uint8),  # uint8_t *compressed
            ctypes.POINTER(ctypes.c_uint8),  # const uint8_t *public_key
            ctypes.c_void_p,  # uECC_Curve curve
        ]

        self.lib.uECC_decompress.restype = None

        # define uncompressed public key based on public key size
        public_key_d = ((ctypes.c_uint8) * self.public_key_size)()

        # decompress key
        self.lib.uECC_decompress(public_key_compressed_, public_key_d, self.curve)

        return self.array_to_hexstr(public_key_d)

    def sign(self, private_key, message, hashfunc=hashlib.sha256):
        """
        Args:
            private_key <str> : A hex-string ('AAB3F1DE....' == b'\xaa\xb3\xf1\xde...') representing the private key
            message <str> : The message to be hashed
            hashfunc <function> : The function to hash the message (hashlib.sha256 - default)

        Returns:
            <str> : A hex-string ('AAB3F1DE....' == b'\xaa\xb3\xf1\xde...') representing the signature
        """

        private_key_ = self.bytes_to_array(binascii.unhexlify(private_key))

        self.lib.uECC_sign.argtypes = [
            ctypes.POINTER(ctypes.c_uint8),  # const uint8_t *private_key
            ctypes.POINTER(ctypes.c_uint8),  # const uint8_t *message_hash
            ctypes.c_uint,  # unsigned hash_size
            ctypes.POINTER(ctypes.c_uint8),  # uint8_t *signature
            ctypes.c_void_p,  # uECC_Curve curve)
        ]

        self.lib.uECC_sign.restype = ctypes.c_int

        # SHA256 - 32 bytes
        message_hash_ = hashfunc(message.encode('utf8')).digest()
        # message_hash = (ctypes.c_uint8 * 32)()
        message_hash = self.bytes_to_array(message_hash_)

        hash_size = ctypes.c_uint(len(message_hash))
        signature = (ctypes.c_uint8 * (self.curve_size * 2))()

        res = self.lib.uECC_sign(private_key_, message_hash, hash_size, signature, self.curve)
        if res == 0:
            raise Exception('Signature was not successfully generated')

        return self.array_to_hexstr(signature)

    def verify(self, public_key, challenge, signature):
        """
        Args:
            public_key <str> : A hex-string ('AAB3F1DE....' == b'\xaa\xb3\xf1\xde...') representing the public key
            challenge <str> : A hex-string ('AAB3F1DE....' == b'\xaa\xb3\xf1\xde...') representing the challenge (hashed message)
            signature <str> : A hex-string ('AAB3F1DE....' == b'\xaa\xb3\xf1\xde...') representing the signature

        Retuns:
            <bool> : True if it's valid, False otherwise
        """

        public_key_ = self.bytes_to_array(binascii.unhexlify(public_key))
        challenge_  = self.bytes_to_array(binascii.unhexlify(challenge))
        signature_  = self.bytes_to_array(binascii.unhexlify(signature))

        self.lib.uECC_verify.argtypes = [
            ctypes.POINTER(ctypes.c_uint8),  # const uint8_t *public_key,
            ctypes.POINTER(ctypes.c_uint8),  # const uint8_t *message_hash,
            ctypes.c_uint,  # unsigned hash_size,
            ctypes.POINTER(ctypes.c_uint8),  # const uint8_t *signature,
            ctypes.c_void_p,  # uECC_Curve curve
        ]

        self.lib.uECC_verify.restype = ctypes.c_int

        return self.lib.uECC_verify(public_key_, challenge_, len(challenge_), signature_, self.curve) == 1
