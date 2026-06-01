#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

Student* create_student(int id, const char* name, int score) {
    Student* new_student = (Student*)malloc(sizeof(Student));

    if (new_student == NULL) {
        return NULL;
    }

    new_student->id = id;
    strncpy(new_student->name, name, sizeof(new_student->name) - 1);
    new_student->name[sizeof(new_student->name) - 1] = '\0';
    new_student->score = score;
    new_student->next = NULL;

    return new_student;
}

int add_student(Student** head, int id, const char* name, int score) {
    Student* current;
    Student* new_student;

    if (head == NULL) {
        return 0;
    }

    if (!is_valid_id(id)) {
        printf("Error: invalid ID.\n");
        return 0;
    }

    if (!is_valid_name(name)) {
        printf("Error: invalid name.\n");
        return 0;
    }

    if (!is_valid_score(score)) {
        printf("Error: invalid score.\n");
        return 0;
    }

    if (find_student(*head, id) != NULL) {
        printf("Error: duplicate ID.\n");
        return 0;
    }

    new_student = create_student(id, name, score);

    if (new_student == NULL) {
        printf("Error: memory allocation failed.\n");
        return 0;
    }

    if (*head == NULL) {
        *head = new_student;
        return 1;
    }

    current = *head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = new_student;

    return 1;
}

int delete_student(Student** head, int id) {
    Student* current;
    Student* prev;

    if (head == NULL || *head == NULL) {
        printf("Error: student not found.\n");
        return 0;
    }

    current = *head;
    prev = NULL;

    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }

            free(current);
            return 1;
        }

        prev = current;
        current = current->next;
    }

    printf("Error: student not found.\n");
    return 0;
}

Student* find_student(Student* head, int id) {
    Student* current = head;

    while (current != NULL) {
        if (current->id == id) {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

int update_student(Student* head, int id, int score) {
    Student* target;

    if (!is_valid_score(score)) {
        printf("Error: invalid score.\n");
        return 0;
    }

    target = find_student(head, id);

    if (target == NULL) {
        printf("Error: student not found.\n");
        return 0;
    }

    target->score = score;

    return 1;
}

void list_students(Student* head) {
    Student* current = head;

    if (current == NULL) {
        printf("No students found.\n");
        return;
    }

    printf("id, name, score\n");

    while (current != NULL) {
        printf("%d, %s, %d\n", current->id, current->name, current->score);
        current = current->next;
    }
}

void free_students(Student* head) {
    Student* current = head;
    Student* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

int count_students(Student* head) {
    int count = 0;
    Student* current = head;

    while (current != NULL) {
        count++;
        current = current->next;
    }

    return count;
}

int get_sum_score(Student* head) {
    int sum = 0;
    Student* current = head;

    while (current != NULL) {
        sum += current->score;
        current = current->next;
    }

    return sum;
}

int get_max_score(Student* head) {
    int max;
    Student* current;

    if (head == NULL) {
        return -1;
    }

    max = head->score;
    current = head->next;

    while (current != NULL) {
        if (current->score > max) {
            max = current->score;
        }

        current = current->next;
    }

    return max;
}

int get_min_score(Student* head) {
    int min;
    Student* current;

    if (head == NULL) {
        return -1;
    }

    min = head->score;
    current = head->next;

    while (current != NULL) {
        if (current->score < min) {
            min = current->score;
        }

        current = current->next;
    }

    return min;
}

int is_valid_id(int id) {
    return id > 0;
}

int is_valid_name(const char* name) {
    int i;

    if (name == NULL) {
        return 0;
    }

    if (name[0] == '\0') {
        return 0;
    }

    for (i = 0; name[i] != '\0'; i++) {
        if (name[i] == ',') {
            return 0;
        }
    }

    return 1;
}

int is_valid_score(int score) {
    return score >= 0 && score <= 100;
}
