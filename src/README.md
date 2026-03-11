# Source Modules (C/C++)

The executable analytics flow is implemented in C and C++:

- `pipeline/ingestion.c` + `pipeline.h`: ingestion, schema alignment, and normalization.
- `pipeline/feature_engineering.c`: NLP-style and financial feature generation.
- `pipeline/pca.cpp`: deterministic dimensionality reduction stage.
- `pipeline/modeling.cpp`: anomaly scoring and risk profiling.
- `pipeline/validation.cpp`: run-level validation and reporting metrics.

These modules map directly to the lifecycle: data sources -> ingestion -> feature engineering -> PCA -> modeling/scoring -> validation outputs.


## Cryptography (Mbed TLS + PSA Crypto)

- `security/crypto.c` and `security/crypto.h` provide a crypto abstraction for SHA-256, key import/storage, and key destruction.
- Build with `-DENRON_USE_MBEDTLS` and include Mbed TLS headers to enable hardware/software-backed PSA Crypto operations.
- `security/mbedtls_config.h` and `security/psa_crypto_config.h` define the required feature flags for this repository.

- Edwards-curve mitigation: `enron_generate_ed25519_keypair`, `enron_sign_ed25519`, and `enron_verify_ed25519` use PSA Crypto Ed25519 (`PSA_ALG_PURE_EDDSA`) when enabled.
