"""Data quality helpers for anomaly and outlier management."""

from __future__ import annotations


def _percentile(values, percentile):
    """Compute percentile with linear interpolation, NumPy-compatible style."""
    if not values:
        raise ValueError("values must not be empty")

    sorted_values = sorted(values)
    if len(sorted_values) == 1:
        return sorted_values[0]

    position = (len(sorted_values) - 1) * (percentile / 100)
    lower_index = int(position)
    upper_index = min(lower_index + 1, len(sorted_values) - 1)
    weight = position - lower_index
    return sorted_values[lower_index] * (1 - weight) + sorted_values[upper_index] * weight


def outlier_cleaner(predictions, ages, net_worths):
    """Remove the top 10% largest residual errors."""
    errors = [(predictions[i] - net_worths[i]) ** 2 for i in range(len(predictions))]
    max_error = _percentile(errors, 90)
    cleaned_data = []

    for i, error in enumerate(errors):
        if error < max_error:
            cleaned_data.append((ages[i], net_worths[i], error))

    return cleaned_data


# Backward-compatible alias used by legacy notebooks.
outlierCleaner = outlier_cleaner
