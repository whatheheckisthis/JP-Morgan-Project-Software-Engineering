#include <cassert>
#include <cstdint>
#include <cstring>

extern "C" {
#include "../../src/security/crypto.h"
}

int main() {
    assert(enron_crypto_init() == 0);

    const uint8_t payload[] = "enron-corpus-integrity";
    uint8_t digest_a[ENRON_SHA256_SIZE];
    uint8_t digest_b[ENRON_SHA256_SIZE];

    assert(enron_sha256(payload, sizeof(payload) - 1, digest_a) == 0);
    assert(enron_sha256(payload, sizeof(payload) - 1, digest_b) == 0);
    assert(std::memcmp(digest_a, digest_b, ENRON_SHA256_SIZE) == 0);

    const uint8_t key_material[16] = {
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
        0x40, 0x41, 0x42, 0x43,
    };

    uint32_t key_id = 0;
    assert(enron_store_aes_key(key_material, sizeof(key_material), &key_id) == 0);
    assert(key_id != 0);
    assert(enron_destroy_key(key_id) == 0);

    uint32_t ed_private = 0;
    uint32_t ed_public = 0;
    assert(enron_generate_ed25519_keypair(&ed_private, &ed_public) == 0);

    uint8_t signature[ENRON_ED25519_SIGNATURE_SIZE];
    size_t signature_len = 0;
    assert(enron_sign_ed25519(ed_private, payload, sizeof(payload) - 1, signature, &signature_len) == 0);
    assert(signature_len > 0);
    assert(enron_verify_ed25519(ed_public, payload, sizeof(payload) - 1, signature, signature_len) == 0);

    signature[0] ^= 0x01;
    assert(enron_verify_ed25519(ed_public, payload, sizeof(payload) - 1, signature, signature_len) != 0);

    assert(enron_destroy_key(ed_private) == 0);
#ifdef ENRON_USE_MBEDTLS
    assert(enron_destroy_key(ed_public) == 0);
#endif

    enron_crypto_free();
    return 0;
}
