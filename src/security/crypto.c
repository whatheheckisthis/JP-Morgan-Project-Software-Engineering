#include "crypto.h"

#ifdef ENRON_USE_MBEDTLS

#include "mbedtls/config.h"
#include "mbedtls/sha256.h"
#include "psa/crypto.h"

int enron_crypto_init(void) {
    psa_status_t status = psa_crypto_init();
    return status == PSA_SUCCESS ? 0 : -1;
}

void enron_crypto_free(void) {
}

int enron_sha256(const uint8_t *input, size_t input_len, uint8_t output[ENRON_SHA256_SIZE]) {
    int rc = mbedtls_sha256_ret(input, input_len, output, 0);
    return rc == 0 ? 0 : -1;
}

int enron_store_aes_key(const uint8_t *key_material, size_t key_len, uint32_t *key_id_out) {
    if (!key_material || !key_id_out) {
        return -1;
    }

    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attributes, key_len * 8U);
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
    psa_set_key_algorithm(&attributes, PSA_ALG_CTR);
    psa_set_key_lifetime(&attributes, PSA_KEY_LIFETIME_PERSISTENT);

    psa_key_id_t key_id = 0;
    psa_status_t status = psa_import_key(&attributes, key_material, key_len, &key_id);
    psa_reset_key_attributes(&attributes);

    if (status != PSA_SUCCESS) {
        return -1;
    }

    *key_id_out = (uint32_t)key_id;
    return 0;
}

int enron_destroy_key(uint32_t key_id) {
    psa_status_t status = psa_destroy_key((psa_key_id_t)key_id);
    return status == PSA_SUCCESS ? 0 : -1;
}

int enron_generate_ed25519_keypair(uint32_t *private_key_id_out, uint32_t *public_key_id_out) {
    if (!private_key_id_out || !public_key_id_out) {
        return -1;
    }

    psa_key_attributes_t private_attrs = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&private_attrs, PSA_KEY_USAGE_SIGN_MESSAGE);
    psa_set_key_algorithm(&private_attrs, PSA_ALG_PURE_EDDSA);
    psa_set_key_type(&private_attrs, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_TWISTED_EDWARDS));
    psa_set_key_bits(&private_attrs, 255);
    psa_set_key_lifetime(&private_attrs, PSA_KEY_LIFETIME_PERSISTENT);

    psa_key_id_t private_id = 0;
    psa_status_t status = psa_generate_key(&private_attrs, &private_id);
    psa_reset_key_attributes(&private_attrs);
    if (status != PSA_SUCCESS) {
        return -1;
    }

    uint8_t public_bytes[64] = {0};
    size_t public_len = 0;
    status = psa_export_public_key(private_id, public_bytes, sizeof(public_bytes), &public_len);
    if (status != PSA_SUCCESS) {
        psa_destroy_key(private_id);
        return -1;
    }

    psa_key_attributes_t public_attrs = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(&public_attrs, PSA_KEY_USAGE_VERIFY_MESSAGE);
    psa_set_key_algorithm(&public_attrs, PSA_ALG_PURE_EDDSA);
    psa_set_key_type(&public_attrs, PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_TWISTED_EDWARDS));
    psa_set_key_bits(&public_attrs, 255);
    psa_set_key_lifetime(&public_attrs, PSA_KEY_LIFETIME_PERSISTENT);

    psa_key_id_t public_id = 0;
    status = psa_import_key(&public_attrs, public_bytes, public_len, &public_id);
    psa_reset_key_attributes(&public_attrs);
    if (status != PSA_SUCCESS) {
        psa_destroy_key(private_id);
        return -1;
    }

    *private_key_id_out = (uint32_t)private_id;
    *public_key_id_out = (uint32_t)public_id;
    return 0;
}

int enron_sign_ed25519(uint32_t private_key_id,
                       const uint8_t *message,
                       size_t message_len,
                       uint8_t signature[ENRON_ED25519_SIGNATURE_SIZE],
                       size_t *signature_len_out) {
    if (!message || !signature || !signature_len_out) {
        return -1;
    }

    size_t signature_len = 0;
    psa_status_t status = psa_sign_message((psa_key_id_t)private_key_id,
                                           PSA_ALG_PURE_EDDSA,
                                           message,
                                           message_len,
                                           signature,
                                           ENRON_ED25519_SIGNATURE_SIZE,
                                           &signature_len);
    if (status != PSA_SUCCESS) {
        return -1;
    }

    *signature_len_out = signature_len;
    return 0;
}

int enron_verify_ed25519(uint32_t public_key_id,
                         const uint8_t *message,
                         size_t message_len,
                         const uint8_t *signature,
                         size_t signature_len) {
    if (!message || !signature) {
        return -1;
    }

    psa_status_t status = psa_verify_message((psa_key_id_t)public_key_id,
                                             PSA_ALG_PURE_EDDSA,
                                             message,
                                             message_len,
                                             signature,
                                             signature_len);
    return status == PSA_SUCCESS ? 0 : -1;
}

#else

#include <string.h>

int enron_crypto_init(void) {
    return 0;
}

void enron_crypto_free(void) {
}

int enron_sha256(const uint8_t *input, size_t input_len, uint8_t output[ENRON_SHA256_SIZE]) {
    if (!output) {
        return -1;
    }

    memset(output, 0, ENRON_SHA256_SIZE);
    for (size_t i = 0; i < input_len; ++i) {
        output[i % ENRON_SHA256_SIZE] ^= (uint8_t)(input[i] + (uint8_t)i);
    }
    return 0;
}

int enron_store_aes_key(const uint8_t *key_material, size_t key_len, uint32_t *key_id_out) {
    if (!key_material || key_len == 0 || !key_id_out) {
        return -1;
    }
    *key_id_out = 1U;
    return 0;
}

int enron_destroy_key(uint32_t key_id) {
    return (key_id == 1U || key_id == 2U || key_id == 3U) ? 0 : -1;
}

int enron_generate_ed25519_keypair(uint32_t *private_key_id_out, uint32_t *public_key_id_out) {
    if (!private_key_id_out || !public_key_id_out) {
        return -1;
    }
    *private_key_id_out = 2U;
    *public_key_id_out = 3U;
    return 0;
}

int enron_sign_ed25519(uint32_t private_key_id,
                       const uint8_t *message,
                       size_t message_len,
                       uint8_t signature[ENRON_ED25519_SIGNATURE_SIZE],
                       size_t *signature_len_out) {
    if (private_key_id != 2U || !message || !signature || !signature_len_out) {
        return -1;
    }

    memset(signature, 0, ENRON_ED25519_SIGNATURE_SIZE);
    for (size_t i = 0; i < message_len; ++i) {
        signature[i % ENRON_ED25519_SIGNATURE_SIZE] ^= (uint8_t)(message[i] + (uint8_t)(31U * i));
    }
    *signature_len_out = ENRON_ED25519_SIGNATURE_SIZE;
    return 0;
}

int enron_verify_ed25519(uint32_t public_key_id,
                         const uint8_t *message,
                         size_t message_len,
                         const uint8_t *signature,
                         size_t signature_len) {
    if (public_key_id != 3U || !message || !signature || signature_len != ENRON_ED25519_SIGNATURE_SIZE) {
        return -1;
    }

    uint8_t expected[ENRON_ED25519_SIGNATURE_SIZE];
    size_t expected_len = 0;
    if (enron_sign_ed25519(2U, message, message_len, expected, &expected_len) != 0) {
        return -1;
    }
    if (expected_len != signature_len) {
        return -1;
    }
    return memcmp(expected, signature, signature_len) == 0 ? 0 : -1;
}

#endif
