#ifndef STUDENT_H
#define STUDENT_H

typedef struct Student {
    int id;
    char name[32];
    int score;
    struct Student* next;
} Student;

Student* create_student(int id, const char* name, int score);

int add_student(Student** head, int id, const char* name, int score);
int delete_student(Student** head, int id);
Student* find_student(Student* head, int id);
int update_student(Student* head, int id, int score);

void list_students(Student* head);
void free_students(Student* head);

int count_students(Student* head);
int get_sum_score(Student* head);
int get_max_score(Student* head);
int get_min_score(Student* head);

int is_valid_id(int id);
int is_valid_name(const char* name);
int is_valid_score(int score);

#endif