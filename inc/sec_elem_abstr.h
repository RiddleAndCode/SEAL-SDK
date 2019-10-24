#ifndef __SECURE_ELEM_ABSTR__
#define __SECURE_ELEM_ABSTR__

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
 * Signs a given message using the key stored in one of the slots.
 * @param[in] index The slot number for reading the public key
 * @param[in] msg buffer to contain the message to be signed;
 * @param[in] msglen The length of the message to be signed.
 * @param[in,out] pSignature  IN: buffer to contain the signed message;
                         OUT: the signed message
 * @param[in] msglen Pointer to the length of the signed message
 * @retval ::SE_SUCCESS Upon successful execution
 *         ::BAD_PARAM In case of a message not being the right length(32Byte) for ATECC608
 */
SE_STATUS se_sign(uint8_t index, const uint8_t *msg, uint16_t msglen, uint8_t *psignature, uint16_t *pSignatureLen);

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
 * @param[in] Use mode ESTABLISH_SCI2Cfirst time initializing the device
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


SE_STATUS se_get_sha256(uint8_t* pMessage, uint16_t msgLen, uint8_t* sha, uint16_t*shaLen);

SE_STATUS se_wipe_device(uint8_t index);

SE_STATUS se_close(void);

SE_STATUS se_secure_store(uint8_t zone ,uint8_t * data, int16_t len);

SE_STATUS se_secure_read(uint8_t zone ,uint8_t * data, int16_t len);

SE_STATUS se_authenticate(uint8_t slot);

SE_STATUS se_secure_storage_personalize(bool lock);

SE_STATUS se_secure_storage_close();

SE_STATUS se_secure_storage_init();

#endif
