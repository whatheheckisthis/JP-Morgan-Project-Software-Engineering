#ifndef ENRON_PIPELINE_H
#define ENRON_PIPELINE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_RECORDS 128
#define MAX_TEXT 1024

typedef struct {
    char employee_id[16];
    int email_count;
    int message_length;
    char email_text[MAX_TEXT];
} EmailRecord;

typedef struct {
    char employee_id[16];
    double salary;
    double bonus;
    double expenses;
    int poi;
} FinancialRecord;

typedef struct {
    char employee_id[16];
    int poi;
    double token_count;
    double avg_token_length;
    double exclamation_count;
    double uppercase_ratio;
    double email_count;
    double message_length;
    double financial_pressure;
    double compensation_spike;
} FeatureRecord;

size_t load_email_rows(const char *path, EmailRecord *rows, size_t max_rows);
size_t load_financial_rows(const char *path, FinancialRecord *rows, size_t max_rows);
size_t normalize_rows(const EmailRecord *emails, size_t email_count,
                      const FinancialRecord *financials, size_t financial_count,
                      EmailRecord *aligned_emails, FinancialRecord *aligned_financials,
                      size_t max_rows);
size_t build_feature_rows(const EmailRecord *emails, const FinancialRecord *financials,
                          size_t count, FeatureRecord *features, size_t max_rows);

#ifdef __cplusplus
}
#endif

#endif
