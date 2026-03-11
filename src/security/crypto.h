#ifndef ENRON_CRYPTO_H
#define ENRON_CRYPTO_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ENRON_SHA256_SIZE 32
#define ENRON_ED25519_SIGNATURE_SIZE 64

int enron_crypto_init(void);
void enron_crypto_free(void);

int enron_sha256(const uint8_t *input, size_t input_len, uint8_t output[ENRON_SHA256_SIZE]);

int enron_store_aes_key(const uint8_t *key_material, size_t key_len, uint32_t *key_id_out);
int enron_destroy_key(uint32_t key_id);

/* Edwards-curve mitigation path (Ed25519). */
int enron_generate_ed25519_keypair(uint32_t *private_key_id_out, uint32_t *public_key_id_out);
int enron_sign_ed25519(uint32_t private_key_id,
                       const uint8_t *message,
                       size_t message_len,
                       uint8_t signature[ENRON_ED25519_SIGNATURE_SIZE],
                       size_t *signature_len_out);
int enron_verify_ed25519(uint32_t public_key_id,
                         const uint8_t *message,
                         size_t message_len,
                         const uint8_t *signature,
                         size_t signature_len);

#ifdef __cplusplus
}
#endif

#endif
