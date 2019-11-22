#ifndef __SECURE_ELEM_ABSTR__
#define __SECURE_ELEM_ABSTR__

#ifdef __cplusplus
extern "C" {
#endif


#include "types.h"

/**
 * Retrieves a random byte array of size randomLen from the Secure Module.
 * @param[in,out] random  IN: buffer to contain random value (at least of size randomLen);
                         OUT: retrieved random data
 * @param[in] randomLen Amount of byte to retrieve
 * @retval ::SE_SUCCESS Upon successful execution
 */
SE_STATUS se_get_random(uint8_t* rand_out , uint8_t randomLen);



/**
 * Retrieves one of the public keys stored in one of the slots.
 * @param[in] index The slot number for reading the public key
 * @param[in,out] publicKey  IN: buffer to contain public key with th length of publicKeyLen;
                         OUT: retrieved public key
 * @param[in] publicKeyLen Pointer to the length of the retrieved public key
 * @retval ::SE_SUCCESS Upon successful execution
 *         ::BAD_PARAM In case of a key_id too large for ATECC608
 */
SE_STATUS se_get_pubkey(uint8_t index, uint8_t* publicKey , uint16_t* publicKeyLen);



/**
 * Verifies a given signature using the key stored in one of the slots and the original message.
 * @param[in] index The slot number for reading the public key
 * @param[in] pHash buffer to the content to be verified;
 * @param[in] hashLen The length of the content to be verified.
 * @param[in] pSignature  IN: buffer to contain the signed message;
 * @param[in] msglen The length of the signed message
 * @retval ::SE_SUCCESS Upon successful execution
 */
SE_STATUS se_verify(uint8_t index, const uint8_t *pHash, uint16_t hashLen, const uint8_t *pSignature, uint16_t signatureLen);



/**
 * Verifies a certain signature using an external(explicit) key and the original message.
 * @param[in] index The slot number for reading the public key
 * @param[in] pKeyData Pointer to the given key
 * @param[in] keyDataLen Size of the given key
 * @param[in] pHash buffer to the content to be verified;
 * @param[in] hashLen The length of the content to be verified.
 * @param[in] pSignature  IN: buffer to contain the signed message;
 * @param[in] msglen The length of the signed message
 *
 * @retval ::SE_SUCCESS Upon successful execution
 *         ::BAD_PARAM In case of a message not being the right length(32Byte) for ATECC608
 */
SE_STATUS se_verify_external(uint8_t index, const uint8_t *pKeyData, uint16_t keyDataLen, const uint8_t *pHash, uint16_t hashLen, const uint8_t *pSignature, uint16_t signatureLen);



/**
 * Initilazes the secure element.
 * @param[in] Use mode ESTABLISH_SCI2C first time initializing the device
 * Use RESUME_SCI2C if the i2c line is inactive for 312 ms to wake up the device.
 * @retval ::SE_SUCCESS Upon successful execution
 */
SE_STATUS se_init(uint8_t mode);



/**
 * Generates a key pair inside one of the slots.
 * @param[in] index The slot number
 * @retval ::SE_SUCCESS Upon successful execution
 */
SE_STATUS se_generate_keypair(uint8_t index);


/**
 * Signs a given message using the key stored in one of the slots..
 * @param[in] index The slot number for reading the public key.
 * @param[in] msg buffer to contain the message to be signed.
 * @param[in] msglen The length of the message to be signed.
 * @param[in,out] pSignature  IN: buffer to contain the signed message
                         OUT: the signed message
 * @param[in] msglen Pointer to the length of the signed message
 * @retval ::SE_SUCCESS Upon successful execution
 *         ::BAD_PARAM In case of a message not being the right length(32Byte) for ATECC608
 */
SE_STATUS se_sign(uint8_t index, const uint8_t *msg, uint16_t msglen, uint8_t *psignature, uint16_t *pSignatureLen);



/**
 * @brief Signs a message with an embedded priv key which is not visible to the client and converts the signature from ASN format to raw format.
 * Only applies for V2 since it needed the raw format for it's verify function .
 *
 * @param index index The slot number for reading the public key.
 * @param msg msg buffer to contain the message to be signed.
 * @param msglen msglen The length of the message to be signed.
 * @param pSignature pointer to the buffer which will store the output signature.
 * @param pSignatureLen msglen Pointer to the length of the signed message.
 * @param rawSign pointer to the buffer which will store the output RAW signature.
 * @return SE_STATUS Upon successful execution
 */
SE_STATUS se_sign_raw(uint8_t index, const uint8_t *msg, uint16_t msglen, uint8_t *pSignature, uint16_t *pSignatureLen, uint8_t *rawSign);



/**
 * Save a key pair into one of the slots
 * @param[in] index The slot number for storing the key pait
 * @param[in] publicKey Pointer to the public key
 * @param[in] publicKeyLen Size of the public key
 * @param[in] pKeyData Pointer to the private key
 * @param[in] privateKeyLen Size of the private key
 * @retval ::SE_SUCCESS Upon successful execution
 */
SE_STATUS se_save_key_pair(uint8_t index, const uint8_t *publicKey, uint16_t publicKeyLen, const uint8_t *privateKey, uint16_t privateKeyLen);



/**
 * Writes data to a certain address
 * @param[in] dataOffset Position of the data to be written
 * @param[in] data Pointer to the data to be written;
 * @param[in] dataLen The size of the data
 * @retval ::SE_SUCCESS Upon successful execution
 */
SE_STATUS se_write_data(uint16_t dataOffset, uint8_t *data, uint16_t dataLen);



/**
 * Reads data from a certain address
 * @param[in] dataOffset Position of the data to be read
 * @param[in,out] data  IN: Pointer to the output field;
                         OUT: the data
 * @param[in] dataLen The size of the data
 * @retval ::SE_SUCCESS Upon successful execution
 */
SE_STATUS se_read_data(uint16_t dataOffset, uint8_t *data, uint16_t dataLen);



/**
 * @brief Hashes the input with SHA256 Algorithm and returns a 32 byte digest
 *
 * @param pMessage Message to be hashed
 * @param msgLen Lenght of the mesage
 * @param sha Output buffer for the digest
 * @param shaLen Lenght of the output buffer
 * @return SE_STATUS Upon successful execution
 */
SE_STATUS se_get_sha256(uint8_t* pMessage, uint16_t msgLen, uint8_t* sha, uint16_t*shaLen);



/**
 * @brief Clears all the data slots, only valid for V2
 *
 * @param index  Data slot to be deleted
 * @return SE_STATUS Upon successful execution
 */
SE_STATUS se_wipe_device(uint8_t index);



/**
 * @brief Releases the i2c line.
 *
 * @return SE_STATUS Upon successful execution
 */
SE_STATUS se_close(void);



/**
 * @brief Data storage with authentication,
 * SLOTS[0-4]  Requires no authentication
 * SLOT[5]  Unlocked with the Key at slot 0
 * SLOT[6]  Unlocked with the Key at slot 1
 * SLOTS[7-15]  Unlocked with the Key at slot 2
 *
 * @param slot Number of the slot to be written, each slot is 256 bytes long
 * @param data Pointer to the data object to be stored
 * @param len Length of the data object
 * @return SE_STATUS Upon successful execution
 */
SE_STATUS se_secure_store(uint8_t slot ,uint8_t * data, uint16_t len);



/**
 * @brief Accessing the data with authentication,
 * SLOTS[0-4]  Requires no authentication
 * SLOT[5]  Unlocked with the Key at slot 0
 * SLOT[6]  Unlocked with the Key at slot 1
 * SLOTS[7-15]  Unlocked with the Key at slot 2
 *
 * @param slot Number of the slot to be read, each slot is 256 bytes long
 * @param data Pointer to the data object to be read
 * @param len Length of the data object
 * @return SE_STATUS Upon successful execution
 */
SE_STATUS se_secure_read(uint8_t slot ,uint8_t * data, uint16_t len);



/**
 * @brief Authenticate the slot with the given key, no authentication is required for slots [0-4]
 * only one authentication scheme can be active at any time
 *
 * @param slot Slot to be authenticated.
 * @param key Key value to authenticate the slot.
 * @return SE_STATUS Upon successful execution
 */
SE_STATUS se_authenticate(uint8_t slot, uint8_t *key);



/**
 * @brief Apply the provisioning scheme on the SES chip with the provided keys.
 * SLOTS[0-4] Requires no authentication, can be used without restriction
 * SLOT[5] key0 is used to lock this slot , authentication is required for read/write operations.
 * SLOT[6] key1 is used to lock this slot , authentication is required for read/write operations.
 * SLOT[7-15] key2 is used to lock this slot , authentication is required for read/write operations.
 * @param lock Locks the config to avoid future modifications to the config. (EXPERIMENTAL in this version)
 * @param key_0 Key to be used to unlock slot 5
 * @param key_1 Key to be used to unlock slot 6
 * @param key_2 Key to be used to unlock slot 7-15
 * @return SE_STATUS Upon successful execution
 */
SE_STATUS se_secure_storage_personalize(bool lock, uint8_t* key_0, uint8_t* key_1, uint8_t* key_2);



/**
 * @brief Release i2c bus for SES, only needs to be used in standalone mode.
 *
 * @return SE_STATUS Upon successful execution
 */
SE_STATUS se_secure_storage_close();



/**
 * @brief Initializes the i2c bus for SES, only needs to be used in stand alone mode.
 *
 * @return SE_STATUS Upon successful execution
 */
SE_STATUS se_secure_storage_init();



#ifdef __cplusplus
}
#endif

#endif
