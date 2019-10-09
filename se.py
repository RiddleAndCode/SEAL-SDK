import base58
from SEAL import SEAL
from bigchaindb.common.crypto import key_pair_from_ed25519_key
from bigchaindb_driver.crypto import generate_keypair
from cryptoconditions import crypto
from bigchaindb_driver import BigchainDB
from collections import namedtuple
import binascii
import os
import json

CryptoKeypair = namedtuple('CryptoKeypair', ('private_key', 'public_key'))
current_dir = os.path.dirname(os.path.abspath(__file__))
bdb_root_url = 'https://ipdb-eu2.riddleandcode.com'

def generate_identity(se):
    rnd = raspberry.get_random()
    rndHex = (binascii.hexlify(rnd)).decode()
    CryptoKeypair = key_pair_from_ed25519_key(rndHex)
    SECRET=CryptoKeypair.private_key
    PUBLIC=CryptoKeypair.public_key
    se.save_keypair(PUBLIC,SECRET)
    print("Hash of priv key :")
    print(raspberry.get_hash(SECRET,len(SECRET)).hex())
    print("pub key :")
    print(PUBLIC)

def read_identity(se):
    return CryptoKeypair(private_key=str(base58.b58encode(raspberry.read_data(0,32)).decode()),
                         public_key=str(base58.b58encode(raspberry.get_public_key()).decode()))

def hash_data_sources(paths,id):

    file = open(paths,"r")
    content = file.read()
    content_size = len(content)
    sha = raspberry.get_hash(content,content_size)
    print(sha.hex())

    with open(paths) as f:
        data = json.load(f)
        f.close()

    a_dict = {'sha256': sha.hex()}
    data.update(a_dict)

    with open("database/"+paths.split('/')[1]+"-"+id, 'w') as f:
        json.dump(data, f)
        f.close()

def prepare_outputs(id,file):
    print("Hashing "+ file)
    hash_data_sources("source/"+data,id)

def attest_device(device_name,pair):
    data_sources = {
        'data': {
            "devices" :device_name
        },
    }
    prepared_token_tx = bdb.transactions.prepare(
        operation='CREATE',
        metadata={"ID":"Attestation of "+ device_name},
        signers=pair.public_key,
        recipients=[([pair.public_key], 10)],
        asset=data_sources)
    print("prepared_token_tx")

    # fulfill and send the transaction
    fulfilled_token_tx = bdb.transactions.fulfill(
        prepared_token_tx, private_keys=pair.private_key)
    print("fulfilled_token_tx")

    bdb.transactions.send_commit(fulfilled_token_tx)


def send_data_blockchain(device_name,pair,file):
    data_sources = {
        'data': {
            "devices" :device_name
        },
    }
    path = "source/" + file
    f = open(path,"r")
    content = f.read()
    f.close()
    content_size = len(content)
    sha = raspberry.get_hash(content,content_size)
    prepared_token_tx = bdb.transactions.prepare(
        operation='CREATE',
        metadata={(path.split('/')[1]).split('.')[0]+" SHA":sha.hex()},
        signers=pair.public_key,
        recipients=[([pair.public_key], 10)],
        asset=data_sources)

    # fulfill and send the transaction
    fulfilled_token_tx = bdb.transactions.fulfill(
        prepared_token_tx, private_keys=pair.private_key)

    prepare_outputs(fulfilled_token_tx["id"],file)

    bdb.transactions.send_commit(fulfilled_token_tx)





bdb = BigchainDB(bdb_root_url)
raspberry = SEAL("/home/pi/dodo/test/libseadyn.so")


generate_identity(raspberry)
pair = read_identity(raspberry)
print(pair.private_key)
print(pair.public_key)

data_lists = os.listdir(current_dir + "/source")

for data in data_lists:
    send_data_blockchain("dodo2",pair,data)

raspberry.close_comms()



rapper= bdb.assets.get(search='dodo2')
print("\n\n")

tx_ids = set()
for i,x in zip(range(len(rapper)),rapper):
    tx_ids.add(x["id"])

print(tx_ids)
print("\n\n")

for x in tx_ids:
    rapper= bdb.transactions.retrieve(txid=x)
    print(rapper)
    print("\n")


print("\n\n")






