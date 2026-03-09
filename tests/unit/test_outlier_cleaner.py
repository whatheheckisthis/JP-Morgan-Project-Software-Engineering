from src.analytics.outliers.outlier_cleaner import outlier_cleaner


def test_outlier_cleaner_removes_top_10_percent_errors():
    predictions = [10, 20, 30, 40, 500]
    ages = [20, 30, 40, 50, 60]
    net_worths = [12, 18, 29, 41, 5]

    cleaned = outlier_cleaner(predictions, ages, net_worths)

    assert len(cleaned) == 4
    assert all(row[0] != 60 for row in cleaned)
