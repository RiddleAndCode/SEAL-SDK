# Python wrapper around microecc library

## Install

Firstly, clone the "parent" project (git@github.com:RiddleAndCode/microecc.git) or unzip it (you can get it here: https://github.com/RiddleAndCode/microecc/archive/master.zip) somewhere (outside your project folder) and cd into the **python** folder. Just run the following commands:

```shell
$ git clone git@github.com:RiddleAndCode/microecc.git
$ cd microecc/python/
$ python setup.py install
```

or...

```shell
$ unzip microecc-master.zip
$ cd microecc-master/python/
$ python setup.py install
```

Now, the package should be installed. You can get rid of the cloned (or unziped) folder.

*Please, notice that you'll have to activate the corresponding environment where you want to install the package to beforehand.

```shell
$ source venv/bin/activate  # or similar
```

**This package requires **Python 3+**

On the other hand, in order to make the wrapper work, you must have the compiled file of _microecc_ (**libmicroecc.so**) in a common place like _/usr/local/lib_ (this is the recommended folder). After placing the file please use the _ldconfig_ command to update the dynamic-linked libraries:

```shell
$ sudo ldconfig
```

It's also possible to indicate the full path of the library when you instantiate the wrapper.

```python
m = MicroECCPy(library_path='/full/path/to/libmicroecc.so')
```

## Usage

```python
from microecc_py import MicroECCPy

m = MicroECCPy()
```

If you want to indicate the full path to the library (_libmicroecc.so_) you have to do it in the instantiation:

```python
from microecc_py import MicroECCPy

m = MicroECCPy(library_path='/full/path/to/libmicroecc.so')
```

In this step you may also define the curve to be used in all the processes (key sizes, generation of keys, signing, etc):

```python
from microecc_py import MicroECCPy

m = MicroECCPy(curve_name='secp256r1')
```

The default value for _curve_name_ is _secp256r1_. Other possible values are:
 - secp160r1
 - secp192r1
 - secp224r1
 - secp256k1

The main methods exposed by the instance of **MicroECCPy** are:

### get_random_keypair

Returns a random keypair (public key and private key) in the form of hex-strings

```python
m = MicroECCPy()  # using the default "secp256r1"
pk, pvk = m.get_random_keypair()

pk = 'E27FED0DC6F31E239B68D62229E62301176F1075A81A368DE2FA1307F2F313A290827F7DDCB351DBE7073CB4C5969B38ABCF219959D312E94A11682420268F09'

pvk = 'D2811A5F7A843FDE7813B370C630CCF7AF051D684A5E2B11BE81498F747483DA'
```

### get_public_from_private

Returns the corresponding **public key** for a given **private key**:

```python
m = MicroECCPy()  # using the default "secp256r1"
pvk = 'D2811A5F7A843FDE7813B370C630CCF7AF051D684A5E2B11BE81498F747483DA'
pk = m.get_public_from_private(pvk)

pk = 'E27FED0DC6F31E239B68D62229E62301176F1075A81A368DE2FA1307F2F313A290827F7DDCB351DBE7073CB4C5969B38ABCF219959D312E94A11682420268F09'
```

### generate_shared_secret

Returns the **shared secret** corresponding to a given **public key** and **private key**, in the form of a hex-string:

```python
m = MicroECCPy()  # using the default "secp256r1"

shared_secret = m.generate_shared_secret(public_key, private_key)

shared_secret = '1E92753158F28F3A1FEA86AE16BE9427EF5511866100E6E8E8EA9C022C812FA8'
```

### compress

Returns the **compressed** version of an **uncompressed public key**:

```python
m = MicroECCPy()  # using the default "secp256r1"
pk, pvk = m.get_random_keypair()

pk = 'E27FED0DC6F31E239B68D62229E62301176F1075A81A368DE2FA1307F2F313A290827F7DDCB351DBE7073CB4C5969B38ABCF219959D312E94A11682420268F09'

pk_compressed = m.compress(pk)

pk_compressed = '03E27FED0DC6F31E239B68D62229E62301176F1075A81A368DE2FA1307F2F313A2'
```

### decompress

Returns the **uncompressed** version of a **compressed public key**:

```python
m = MicroECCPy()  # using the default "secp256r1"
pk, pvk = m.get_random_keypair()

pk_compressed = '03E27FED0DC6F31E239B68D62229E62301176F1075A81A368DE2FA1307F2F313A2'

pk = m.decompress(pk_compressed)

pk = 'E27FED0DC6F31E239B68D62229E62301176F1075A81A368DE2FA1307F2F313A290827F7DDCB351DBE7073CB4C5969B38ABCF219959D312E94A11682420268F09'
```

### sign

Returns the **signature** for a given **private key**, **message** and **hash function** (_hashlib.sha256_, by default):

```python
m = MicroECCPy()  # using the default "secp256r1"
pk, pvk = m.get_random_keypair()

signature = m.sign(pvk, 'Hello, world!')  # hashfunc=hashlib.sha256
signature = '9133E0AA6E2174D8C5A279F342D45CBEC895B2DE2AAE12AD0AA2AF6F7B4A87B5321DB8CB0520F49065475A6EA8F5F500AC2BFB20FF8527DAAF9B70CEDADA8918'
```

### verify

Checks the **signature** for the given **public key** and **challenge** (_hashed message_):

```python
m = MicroECCPy()  # using the default "secp256r1"
pk, pvk = m.get_random_keypair()

message = 'Hello, world!'
signature = m.sign(pvk, message)  # hashfunc=hashlib.sha256

challenge = sha256(message.encode('utf8')).digest().hex().upper()
# challenge = 'D783750D37C04ABCFB29A70C453CDD81388450CD494EF557E10DC9C3C749E358'

assert(m.verify(pk, challenge, signature))
```

Please, notice we're using the same hashfunc (_hashlib.sha256_) to create the **challenge** that it was used to create the **signature**.

## Run the tests

In order to run the tests you'll need to install **pytest**. Just activate the environment

```shell
$ source venv/bin/activate  # or similar
```

and run the following command, being inside the _/microecc/python_ folder:

```shell
$ pip install -r requirements.txt
$ pytest -v microecc_py/
```

If you have problems running the tests because the file _libmicroecc.so_ can't be found (by default it tries at _/usr/local/lib/libmicroecc.so_), run it this way:

```shell
$ pip install -r requirements.txt
$ export LIBRARY_PATH="/full/path/to/libmicroecc.so" ; pytest -v microecc_py/
```
