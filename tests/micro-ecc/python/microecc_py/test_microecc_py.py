import pytest
import binascii
import hashlib
import re
import os
import random
from . import MicroECCPy


class TestMicroECCPy:

    def _get_lib(self, curve_name='secp256r1'):
        return MicroECCPy(
            library_path=os.getenv('LIBRARY_PATH', '/usr/local/lib/libmicroecc.so'),
            curve_name=curve_name,
        )

    def _get_random_curve(self):
        curves = [
            'secp160r1',
            'secp192r1',
            'secp224r1',
            'secp256k1',
            # to verify is case-insentitive
            'SECP160R1',
            'SECP192R1',
            'SECP224R1',
            'SECP256K1',
        ]

        random.seed()
        return random.choice(curves)

    def test__public_from_private(self):

        m = self._get_lib()

        public_key, private_key = m.get_random_keypair()

        public_key2 = m.get_public_from_private(private_key)

        assert(public_key == public_key2)

    def test__public_from_private__secpXXX(self):

        m = MicroECCPy(
            library_path=os.getenv('LIBRARY_PATH', '/usr/local/lib/libmicroecc.so'),
            curve_name=self._get_random_curve(),
        )

        public_key, private_key = m.get_random_keypair()

        public_key2 = m.get_public_from_private(private_key)

        assert(public_key == public_key2)

    def test__shared_secret(self):

        m = self._get_lib()

        public_key1, private_key1 = m.get_random_keypair()
        public_key2, private_key2 = m.get_random_keypair()

        secret1 = m.generate_shared_secret(public_key1, private_key2)
        secret2 = m.generate_shared_secret(public_key2, private_key1)

        assert(secret1 == secret2)

    def test__shared_secret__secpXXX(self):

        m = self._get_lib(curve_name=self._get_random_curve())

        public_key1, private_key1 = m.get_random_keypair()
        public_key2, private_key2 = m.get_random_keypair()

        secret1 = m.generate_shared_secret(public_key1, private_key2)
        secret2 = m.generate_shared_secret(public_key2, private_key1)

        assert(secret1 == secret2)

    def test__shared_secret__secp192r1(self):

        m = self._get_lib(curve_name='SECP192R1')

        public_key1, private_key1 = m.get_random_keypair()
        public_key2, private_key2 = m.get_random_keypair()

        secret1 = m.generate_shared_secret(public_key1, private_key2)
        secret2 = m.generate_shared_secret(public_key2, private_key1)

        assert(secret1 == secret2)

    def test__shared_secret__secp224r1(self):

        m = self._get_lib(curve_name='secp224r1')

        public_key1, private_key1 = m.get_random_keypair()
        public_key2, private_key2 = m.get_random_keypair()

        secret1 = m.generate_shared_secret(public_key1, private_key2)
        secret2 = m.generate_shared_secret(public_key2, private_key1)

        assert(secret1 == secret2)

    def test__compress__decompress(self):

        m = self._get_lib()

        public_key, private_key = m.get_random_keypair()

        public_key_compressed = m.compress(public_key)

        assert(re.match('(02|03)[0-9A-F]+', public_key_compressed, re.I))

        public_key_uncompressed = m.decompress(public_key_compressed)

        assert(public_key_uncompressed == public_key)

    def test__compress__decompress__secp224r1(self):

        m = self._get_lib(curve_name='secp224r1')

        public_key, private_key = m.get_random_keypair()

        public_key_compressed = m.compress(public_key)

        assert(re.match('(02|03)[0-9A-F]+', public_key_compressed, re.I))

        public_key_uncompressed = m.decompress(public_key_compressed)

        assert(public_key_uncompressed == public_key)

    def test__compress__decompress__secp160r1(self):

        m = self._get_lib(curve_name='SECP160R1')

        public_key, private_key = m.get_random_keypair()

        public_key_compressed = m.compress(public_key)

        assert(re.match('(02|03)[0-9A-F]+', public_key_compressed, re.I))

        public_key_uncompressed = m.decompress(public_key_compressed)

        assert(public_key_uncompressed == public_key)

    @pytest.mark.skip()
    def test__compress__decompress_with_04_prefix(self):

        m = self._get_lib()

        public_key, private_key = m.get_random_keypair()

        # add "04" prefix
        public_key = f'04{public_key}'

        """
        04F609F1699260467E9A3E6005E10EFDC82E7FE56143D92F5AE4451949A4DCB 48ABC027B59D619B7735D3FEA0514278F97579BD1F6D4B3CAE55D07A00CF0CF85

        04F609F1699260467E9A3E6005E10EFDC82E7FE56143D92F5AE4451949A4DCB 4936FF220204BF065B0FFA269FBF1066FFB5CC3891FF836A5D419C839690901732B
        """

        public_key_compressed = m.compress(public_key)

        assert(re.match('(02|03)[0-9A-F]+', public_key_compressed, re.I))

        public_key_uncompressed = m.decompress(public_key_compressed)

        assert(public_key_uncompressed == public_key)

    def test__sign_and_verify(self):

        m = self._get_lib()

        public_key, private_key = m.get_random_keypair()

        message = 'Hello, world!'
        challenge = hashlib.sha256(message.encode('utf8')).digest()
        signature = m.sign(private_key, message)

        challenge_ = binascii.hexlify(challenge).upper()

        assert(m.verify(public_key, challenge_, signature))

    def test__sign_and_verify__secp256k1(self):

        m = self._get_lib(curve_name='secp256k1')

        public_key, private_key = m.get_random_keypair()

        message = 'Hello, world!'
        challenge = hashlib.sha256(message.encode('utf8')).digest()
        signature = m.sign(private_key, message)

        challenge_ = binascii.hexlify(challenge).upper()

        assert(m.verify(public_key, challenge_, signature))

    def test__sign_and_verify__secpXXX(self):

        m = self._get_lib(curve_name=self._get_random_curve())

        public_key, private_key = m.get_random_keypair()

        message = 'Hello, world!'
        challenge = hashlib.sha256(message.encode('utf8')).digest()
        signature = m.sign(private_key, message)

        challenge_ = binascii.hexlify(challenge).upper()

        assert(m.verify(public_key, challenge_, signature))

    def test__sign_and_not_valid(self):

        m = self._get_lib()

        public_key, private_key = m.get_random_keypair()

        message = 'Hello, world!'
        signature = m.sign(private_key, message)

        message2 = 'Hello, world!!'
        challenge2 = hashlib.sha256(message2.encode('utf8')).digest()
        challenge_ = binascii.hexlify(challenge2).upper()

        assert(not m.verify(public_key, challenge_, signature))

    def test__sign_and_not_valid__secpXXX(self):

        m = self._get_lib(curve_name=self._get_random_curve())

        public_key, private_key = m.get_random_keypair()

        message = 'Hello, world!'
        signature = m.sign(private_key, message)

        message2 = 'Hello, world!!'
        challenge2 = hashlib.sha256(message2.encode('utf8')).digest()
        challenge_ = binascii.hexlify(challenge2).upper()

        assert(not m.verify(public_key, challenge_, signature))
