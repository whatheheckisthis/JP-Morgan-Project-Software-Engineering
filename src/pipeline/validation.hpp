#ifndef ENRON_VALIDATION_HPP
#define ENRON_VALIDATION_HPP

#include <cstddef>

struct ValidationSummary {
    std::size_t records_ingested;
    std::size_t records_projected;
    std::size_t profiles_emitted;
    std::size_t high_risk_count;
    bool validation_passed;
};

ValidationSummary summarize_run(std::size_t feature_rows,
                                std::size_t projected_rows,
                                std::size_t profiles_emitted,
                                std::size_t high_risk_count);

#endif
