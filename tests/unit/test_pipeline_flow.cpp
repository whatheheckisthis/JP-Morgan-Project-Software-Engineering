#include <cassert>
#include <iostream>
#include <string>
#include <vector>

extern "C" {
#include "../../src/pipeline/pipeline.h"
}
#include "../../src/pipeline/modeling.hpp"
#include "../../src/pipeline/pca.hpp"
#include "../../src/pipeline/validation.hpp"

int main() {
    EmailRecord email_rows[MAX_RECORDS];
    FinancialRecord financial_rows[MAX_RECORDS];
    EmailRecord aligned_emails[MAX_RECORDS];
    FinancialRecord aligned_financials[MAX_RECORDS];
    FeatureRecord features[MAX_RECORDS];

    const size_t email_count = load_email_rows("data/raw/enron/production_email_inputs.csv", email_rows, MAX_RECORDS);
    const size_t financial_count = load_financial_rows("data/raw/enron/production_financial_inputs.csv", financial_rows, MAX_RECORDS);
    assert(email_count == 8);
    assert(financial_count == 8);

    const size_t aligned_count = normalize_rows(email_rows, email_count, financial_rows, financial_count,
                                                aligned_emails, aligned_financials, MAX_RECORDS);
    assert(aligned_count == 8);

    const size_t feature_count = build_feature_rows(aligned_emails, aligned_financials, aligned_count,
                                                    features, MAX_RECORDS);
    assert(feature_count == 8);

    std::vector<std::vector<double>> matrix;
    std::vector<std::string> employee_ids;
    for (size_t i = 0; i < feature_count; ++i) {
        employee_ids.emplace_back(features[i].employee_id);
        matrix.push_back({
            features[i].token_count,
            features[i].avg_token_length,
            features[i].exclamation_count,
            features[i].uppercase_ratio,
            features[i].email_count,
            features[i].message_length,
            features[i].financial_pressure,
            features[i].compensation_spike,
        });
    }

    const auto projected = pca_fit_transform(matrix, 2);
    assert(projected.size() == feature_count);
    assert(projected[0].size() == 2);

    const auto scores = score_anomalies(employee_ids, projected);
    const auto profiles = build_risk_profiles(scores);

    size_t high_risk_count = 0;
    for (const auto &profile : profiles) {
        if (profile.risk_band == "high") {
            high_risk_count++;
        }
    }

    const ValidationSummary summary = summarize_run(feature_count, projected.size(), profiles.size(), high_risk_count);
    assert(summary.validation_passed);
    assert(summary.profiles_emitted == 8);

    std::cout << "pipeline_flow_test: PASS\n";
    return 0;
}
