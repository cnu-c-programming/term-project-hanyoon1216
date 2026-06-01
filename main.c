#include <stdio.h>
#include "student.h"

int main(void) {
    Student* head = NULL;

    add_student(&head, 1, "Alice", 90);
    add_student(&head, 2, "Bob", 85);
    add_student(&head, 3, "Charlie", 95);

    list_students(head);

    printf("Count: %d\n", count_students(head));
    printf("Sum: %d\n", get_sum_score(head));
    printf("Max: %d\n", get_max_score(head));
    printf("Min: %d\n", get_min_score(head));

    update_student(head, 2, 100);
    delete_student(&head, 1);

    list_students(head);

    free_students(head);

    return 0;
}
