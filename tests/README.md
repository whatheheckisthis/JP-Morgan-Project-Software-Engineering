# Test Layout

- `unit/test_outlier_cleaner.py`: existing legacy Python compatibility test.
- `unit/test_pipeline_flow.cpp`: compiled C/C++ integration-style flow test for ingestion -> features -> PCA -> scoring -> validation.

Run checks:

```bash
pytest -q
g++ -std=c++17 -I./src/pipeline tests/unit/test_pipeline_flow.cpp src/pipeline/ingestion.c src/pipeline/feature_engineering.c src/pipeline/pca.cpp src/pipeline/modeling.cpp src/pipeline/validation.cpp -o /tmp/test_pipeline_flow && /tmp/test_pipeline_flow
```
