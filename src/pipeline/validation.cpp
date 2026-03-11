#include "validation.hpp"

ValidationSummary summarize_run(std::size_t feature_rows,
                                std::size_t projected_rows,
                                std::size_t profiles_emitted,
                                std::size_t high_risk_count) {
    ValidationSummary summary{};
    summary.records_ingested = feature_rows;
    summary.records_projected = projected_rows;
    summary.profiles_emitted = profiles_emitted;
    summary.high_risk_count = high_risk_count;
    summary.validation_passed = (feature_rows == projected_rows) && (projected_rows == profiles_emitted);
    return summary;
}
