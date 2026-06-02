#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"
#include "student.h"

#define LINE_SIZE 256

static void remove_n(char* str) {
    size_t len;

    if (str == NULL) {
        return;
    }

    len = strlen(str);

    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

static int parse_int(const char* str, int* value) {
    char* endptr;
    long result;

    if (str == NULL || value == NULL || str[0] == '\0') {
        return 0;
    }

    result = strtol(str, &endptr, 10);

    if (*endptr != '\0') {
        return 0;
    }

    *value = (int)result;
    return 1;
}

static int is_valid_header(char* line) {
    remove_n(line);
    return strcmp(line, "id,name,score") == 0;
}

int load_csv(const char* filename, Student** head) {
    FILE* fp;
    char line[LINE_SIZE];
    char* id_str;
    char* name;
    char* score_str;
    int id;
    int score;
    int count = 0;
    Student* new_head = NULL;

    if (filename == NULL || head == NULL) {
        printf("Error: invalid file argument.\n");
        return -1;
    }

    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error: cannot open CSV file.\n");
        return -1;
    }

    if (fgets(line, sizeof(line), fp) == NULL) {
        printf("Error: empty CSV file.\n");
        fclose(fp);
        return -1;
    }

    if (!is_valid_header(line)) {
        printf("Error: invalid CSV header.\n");
        fclose(fp);
        return -1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        remove_n(line);

        if (line[0] == '\0') {
            continue;
        }

        id_str = strtok(line, ",");
        name = strtok(NULL, ",");
        score_str = strtok(NULL, ",");

        if (id_str == NULL || name == NULL || score_str == NULL) {
            printf("Error: invalid CSV format.\n");
            free_students(new_head);
            fclose(fp);
            return -1;
        }

        if (strtok(NULL, ",") != NULL) {
            printf("Error: invalid CSV format.\n");
            free_students(new_head);
            fclose(fp);
            return -1;
        }

        if (!parse_int(id_str, &id)) {
            printf("Error: invalid ID in CSV.\n");
            free_students(new_head);
            fclose(fp);
            return -1;
        }

        if (!parse_int(score_str, &score)) {
            printf("Error: invalid score in CSV.\n");
            free_students(new_head);
            fclose(fp);
            return -1;
        }

        if (!is_valid_id(id) || !is_valid_name(name) || !is_valid_score(score)) {
            printf("Error: invalid student data in CSV.\n");
            free_students(new_head);
            fclose(fp);
            return -1;
        }

        if (!add_student(&new_head, id, name, score)) {
            free_students(new_head);
            fclose(fp);
            return -1;
        }

        count++;
    }

    fclose(fp);

    free_students(*head);
    *head = new_head;

    return count;
}

int save_csv(const char* filename, Student* head) {
    FILE* fp;
    Student* current;
    int count = 0;

    if (filename == NULL) {
        printf("Error: invalid file argument.\n");
        return -1;
    }

    fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Error: cannot write CSV file.\n");
        return -1;
    }

    fprintf(fp, "id,name,score\n");

    current = head;

    while (current != NULL) {
        fprintf(fp, "%d,%s,%d\n", current->id, current->name, current->score);
        count++;
        current = current->next;
    }

    fclose(fp);

    return count;
}