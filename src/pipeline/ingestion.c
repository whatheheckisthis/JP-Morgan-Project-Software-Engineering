#include "pipeline.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void trim_newline(char *text) {
    size_t length = strlen(text);
    if (length && text[length - 1] == '\n') {
        text[length - 1] = '\0';
    }
}

size_t load_email_rows(const char *path, EmailRecord *rows, size_t max_rows) {
    FILE *file = fopen(path, "r");
    if (!file) {
        return 0;
    }

    char buffer[2048];
    size_t count = 0;
    fgets(buffer, sizeof(buffer), file);

    while (count < max_rows && fgets(buffer, sizeof(buffer), file)) {
        EmailRecord row;
        memset(&row, 0, sizeof(row));

        char *token = strtok(buffer, ",");
        if (!token) break;
        strncpy(row.employee_id, token, sizeof(row.employee_id) - 1);

        token = strtok(NULL, ",");
        if (!token) break;
        row.email_count = atoi(token);

        token = strtok(NULL, ",");
        if (!token) break;
        row.message_length = atoi(token);

        token = strtok(NULL, "\n");
        if (!token) break;
        trim_newline(token);
        if (token[0] == '"') {
            token++;
            char *end_quote = strrchr(token, '"');
            if (end_quote) *end_quote = '\0';
        }
        strncpy(row.email_text, token, sizeof(row.email_text) - 1);

        rows[count++] = row;
    }

    fclose(file);
    return count;
}

size_t load_financial_rows(const char *path, FinancialRecord *rows, size_t max_rows) {
    FILE *file = fopen(path, "r");
    if (!file) {
        return 0;
    }

    char buffer[512];
    size_t count = 0;
    fgets(buffer, sizeof(buffer), file);

    while (count < max_rows && fgets(buffer, sizeof(buffer), file)) {
        FinancialRecord row;
        memset(&row, 0, sizeof(row));
        char poi_text[16] = {0};

        if (sscanf(buffer, "%15[^,],%lf,%lf,%lf,%15s", row.employee_id, &row.salary, &row.bonus,
                   &row.expenses, poi_text) != 5) {
            break;
        }
        trim_newline(poi_text);
        row.poi = strcmp(poi_text, "true") == 0 ? 1 : 0;
        rows[count++] = row;
    }

    fclose(file);
    return count;
}

size_t normalize_rows(const EmailRecord *emails, size_t email_count,
                      const FinancialRecord *financials, size_t financial_count,
                      EmailRecord *aligned_emails, FinancialRecord *aligned_financials,
                      size_t max_rows) {
    size_t out = 0;
    for (size_t i = 0; i < email_count && out < max_rows; ++i) {
        for (size_t j = 0; j < financial_count; ++j) {
            if (strcmp(emails[i].employee_id, financials[j].employee_id) == 0) {
                aligned_emails[out] = emails[i];
                aligned_financials[out] = financials[j];
                ++out;
                break;
            }
        }
    }
    return out;
}
