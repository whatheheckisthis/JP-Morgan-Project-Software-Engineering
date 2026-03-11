#ifndef ENRON_PCA_HPP
#define ENRON_PCA_HPP

#include <vector>

std::vector<std::vector<double>> pca_fit_transform(
    const std::vector<std::vector<double>> &matrix,
    std::size_t components);

#endif
