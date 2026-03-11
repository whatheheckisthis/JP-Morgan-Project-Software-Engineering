#include "pipeline.h"

#include <ctype.h>
#include <string.h>

static void text_metrics(const char *text, double *token_count, double *avg_token_length,
                         double *exclamation_count, double *uppercase_ratio) {
    int chars = 0;
    int upper = 0;
    int exclam = 0;
    int words = 0;
    int current_word = 0;
    int total_word_chars = 0;

    for (size_t i = 0; text[i] != '\0'; ++i) {
        char ch = text[i];
        chars++;
        if (ch == '!') exclam++;
        if (isupper((unsigned char)ch)) upper++;
        if (isspace((unsigned char)ch)) {
            if (current_word > 0) {
                words++;
                total_word_chars += current_word;
                current_word = 0;
            }
        } else {
            current_word++;
        }
    }

    if (current_word > 0) {
        words++;
        total_word_chars += current_word;
    }

    *token_count = (double)words;
    *avg_token_length = words ? (double)total_word_chars / (double)words : 0.0;
    *exclamation_count = (double)exclam;
    *uppercase_ratio = chars ? (double)upper / (double)chars : 0.0;
}

size_t build_feature_rows(const EmailRecord *emails, const FinancialRecord *financials,
                          size_t count, FeatureRecord *features, size_t max_rows) {
    size_t out = count < max_rows ? count : max_rows;
    for (size_t i = 0; i < out; ++i) {
        FeatureRecord feature;
        memset(&feature, 0, sizeof(feature));

        strncpy(feature.employee_id, emails[i].employee_id, sizeof(feature.employee_id) - 1);
        feature.poi = financials[i].poi;
        feature.email_count = (double)emails[i].email_count;
        feature.message_length = (double)emails[i].message_length;

        text_metrics(emails[i].email_text, &feature.token_count, &feature.avg_token_length,
                     &feature.exclamation_count, &feature.uppercase_ratio);

        feature.financial_pressure = financials[i].salary > 0.0 ?
            financials[i].expenses / financials[i].salary : 0.0;
        feature.compensation_spike = financials[i].salary > 0.0 ?
            financials[i].bonus / financials[i].salary : 0.0;

        features[i] = feature;
    }
    return out;
}
