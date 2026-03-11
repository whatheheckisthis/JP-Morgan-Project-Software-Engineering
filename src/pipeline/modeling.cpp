#include "modeling.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

std::vector<ScoreRow> score_anomalies(const std::vector<std::string> &employee_ids,
                                      const std::vector<std::vector<double>> &projected_rows) {
    if (projected_rows.empty()) return {};
    std::size_t dims = projected_rows[0].size();

    std::vector<double> centroid(dims, 0.0);
    for (const auto &row : projected_rows)
        for (std::size_t d = 0; d < dims; ++d)
            centroid[d] += row[d];
    for (double &v : centroid) v /= static_cast<double>(projected_rows.size());

    std::vector<ScoreRow> scores;
    for (std::size_t i = 0; i < projected_rows.size(); ++i) {
        double sq = 0.0;
        for (std::size_t d = 0; d < dims; ++d) {
            double delta = projected_rows[i][d] - centroid[d];
            sq += delta * delta;
        }
        scores.push_back({employee_ids[i], std::sqrt(sq)});
    }
    std::sort(scores.begin(), scores.end(), [](const ScoreRow &a, const ScoreRow &b) {
        return a.anomaly_score > b.anomaly_score;
    });
    return scores;
}

std::vector<ProfileRow> build_risk_profiles(const std::vector<ScoreRow> &scored_rows) {
    if (scored_rows.empty()) return {};
    double max_score = 0.0;
    for (const auto &row : scored_rows)
        if (row.anomaly_score > max_score) max_score = row.anomaly_score;
    if (max_score == 0.0) max_score = 1.0;

    std::vector<ProfileRow> profiles;
    for (const auto &row : scored_rows) {
        double ratio = row.anomaly_score / max_score;
        std::string risk = ratio >= 0.75 ? "high" : (ratio >= 0.4 ? "medium" : "low");
        profiles.push_back({row.employee_id, row.anomaly_score, risk});
    }
    return profiles;
}
