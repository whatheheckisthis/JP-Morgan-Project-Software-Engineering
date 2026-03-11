# Source Modules (C/C++)

The executable analytics flow is implemented in C and C++:

- `pipeline/ingestion.c` + `pipeline.h`: ingestion, schema alignment, and normalization.
- `pipeline/feature_engineering.c`: NLP-style and financial feature generation.
- `pipeline/pca.cpp`: deterministic dimensionality reduction stage.
- `pipeline/modeling.cpp`: anomaly scoring and risk profiling.
- `pipeline/validation.cpp`: run-level validation and reporting metrics.

These modules map directly to the lifecycle: data sources -> ingestion -> feature engineering -> PCA -> modeling/scoring -> validation outputs.
