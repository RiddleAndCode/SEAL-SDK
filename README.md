# SEAL-SDK
This is the SDK including documentation and examples to work with the RIDDLE&amp;CODE SecureElements.

## Usage

To run the tests for each crypto chip, run the following command. 

target can be either v1 , v2 or v3 and regarless of the target version, secure storage tests will be built along with it.

Test source files can be found under ./tests/ folder.

Test binaries can be found under ./binary/ folder.


`make target=v1`

Precompiled static libraries for different architectures can be found under ./lib/ folder.

### Raspberry 

#### C / C++

Including the `sec_elem_abstr.h` header file and linking the static library againist your application is enough to start using

Secure-Elements. Tests under tests folder are written for raspberry-pi. They are using the same scheme described above  

which you can follow from the `Makefile`.

#### Python

Python wrapper for SEAL can be found under `python/SEAL.py`. Working example of Secure Element with IPDB blockchain.

Run the following commands to see it working :

`pip3 install --user --requirement requirements.txt`

`python3 SDK_test.py -h`




### Arduino - nRF52840-DK 

Copy the contents of arduino-nrfdk folder into `home/Arduino/libraries` folder. Include sec_elem_abstr.h` to your ino file . 

You can check `libs/arduino-nrfdk/libseal/examples/wallet/wallet.ino` file for working example.

#### WARNING
*Generate key-pairs command do not work currently due to version conflict, it will be fixed with the next week's release.*




## Supported Functionalities

* NIST-P 256 ECC Key-Generation

- General Purpose Storage up to 4 kB 

+ Elliptic Curve key-pair storage to to 10 pairs.

* 3 kB authenticated storage saved with a provision key.

* Random number generation with hardware RNG module.

* Signing and verifying on chip without private keys ever leaving out of the device EEPROM.

* SHA-256 hashing function.  

* Secure-elements come preprovisioned with a locked device identity for authenticity checks.
