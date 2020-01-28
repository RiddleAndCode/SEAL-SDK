import requests
import base64
import json
import os
from Crypto.PublicKey import ECC
from SEAL import SEAL

current_dir = os.path.dirname(os.path.abspath(__file__))
raspberry = SEAL(current_dir + "/../libs/raspberry/v1/libseadyn.so")


class JSONConstructor:
    @staticmethod
    def construct_body_json(signature, public_key):
        data = {'ephemeral_public_key': public_key, 'signature': public_key}
        json_data = json.dumps(data)
        return json_data

    @staticmethod
    def construct_header_json(public_key):
        data = {'PublicKey': public_key}
        json_data = json.dumps(data)
        return json_data


class GenerateEphemeral:
    def __init__(self):
        self.key = ECC.generate(curve='P-256')
        self.ephemeral_public_x = self.key.pointQ.x
        self.ephemeral_public_y = self.key.pointQ.y
        self.ephemeral_secret = self.key.d
        self.ephemeral_public = self.key.public_key().export_key(format="DER")[27:]
        self.ephemeral_public_base64 = base64.b64encode(self.ephemeral_public).decode("utf-8")


class CryptoSeal:
    def __init__(self, ephemeral):
        self.ephemeral = ephemeral
        self.static_public = raspberry.get_public_key(0)
        self.static_public_base64 = base64.b64encode(self.static_public).decode("utf-8")
        self.sha = raspberry.get_hash(self.ephemeral.ephemeral_public, len(self.ephemeral.ephemeral_public))
        self.signature = (raspberry.sign(0, self.sha))
        self.signature_base64 = base64.b64encode(self.signature).decode("utf-8")


myEphemeral = GenerateEphemeral()
varCryptoSeal = CryptoSeal(myEphemeral)

print("Signature : " + varCryptoSeal.signature_base64)
print("Ephemeral Public Key : " + myEphemeral.ephemeral_public_base64)
print("Static Public Key : " + varCryptoSeal.static_public_base64)
print("Verify result is : ",
      (raspberry.verify(varCryptoSeal.sha, varCryptoSeal.signature, varCryptoSeal.static_public)))

point = ECC.EccPoint(myEphemeral.ephemeral_public_x, myEphemeral.ephemeral_public_y)
new_point = point * myEphemeral.ephemeral_secret
print(new_point.x)
print(new_point.y)
print((JSONConstructor.construct_body_json(varCryptoSeal.signature_base64, myEphemeral.ephemeral_public_base64)))
print((JSONConstructor.construct_header_json(varCryptoSeal.static_public_base64)))

r = requests.post("http://httpbin.org/post",
                  headers={"Public_Key": varCryptoSeal.static_public_base64},
                  data=JSONConstructor.construct_body_json(varCryptoSeal.signature_base64, myEphemeral.ephemeral_public_base64))
print(r.text)