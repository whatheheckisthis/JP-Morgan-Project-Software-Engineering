#include "pca.hpp"

#include <cmath>
#include <cstddef>
#include <vector>

static std::vector<std::vector<double>> center_scale(const std::vector<std::vector<double>> &matrix) {
    if (matrix.empty()) return {};
    std::size_t rows = matrix.size();
    std::size_t cols = matrix[0].size();
    std::vector<double> means(cols, 0.0), stds(cols, 1.0);

    for (std::size_t c = 0; c < cols; ++c) {
        for (std::size_t r = 0; r < rows; ++r) means[c] += matrix[r][c];
        means[c] /= static_cast<double>(rows);
    }
    for (std::size_t c = 0; c < cols; ++c) {
        double var = 0.0;
        for (std::size_t r = 0; r < rows; ++r) {
            double d = matrix[r][c] - means[c];
            var += d * d;
        }
        var /= rows > 1 ? static_cast<double>(rows - 1) : 1.0;
        stds[c] = std::sqrt(var);
        if (stds[c] == 0.0) stds[c] = 1.0;
    }

    std::vector<std::vector<double>> scaled(rows, std::vector<double>(cols, 0.0));
    for (std::size_t r = 0; r < rows; ++r)
        for (std::size_t c = 0; c < cols; ++c)
            scaled[r][c] = (matrix[r][c] - means[c]) / stds[c];
    return scaled;
}

std::vector<std::vector<double>> pca_fit_transform(const std::vector<std::vector<double>> &matrix,
                                                   std::size_t components) {
    auto scaled = center_scale(matrix);
    if (scaled.empty()) return {};

    std::size_t rows = scaled.size();
    std::size_t cols = scaled[0].size();
    std::size_t out_cols = components < cols ? components : cols;

    std::vector<std::vector<double>> transformed(rows, std::vector<double>(out_cols, 0.0));
    for (std::size_t c = 0; c < out_cols; ++c) {
        for (std::size_t r = 0; r < rows; ++r) {
            transformed[r][c] = scaled[r][c];
        }
    }
    return transformed;
}
