#ifndef ENRON_MODELING_HPP
#define ENRON_MODELING_HPP

#include <string>
#include <vector>

struct ScoreRow {
    std::string employee_id;
    double anomaly_score;
};

struct ProfileRow {
    std::string employee_id;
    double anomaly_score;
    std::string risk_band;
};

std::vector<ScoreRow> score_anomalies(const std::vector<std::string> &employee_ids,
                                      const std::vector<std::vector<double>> &projected_rows);
std::vector<ProfileRow> build_risk_profiles(const std::vector<ScoreRow> &scored_rows);

#endif
