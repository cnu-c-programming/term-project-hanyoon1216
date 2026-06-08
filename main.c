#include <stdio.h>
#include <string.h>
#include "student.h"
#include "file_io.h"
#include "command.h"

int main(void) {
    Student* head = NULL;
    ShellContext context;
    char line[256];

    context.csv_filename = "students.csv";

    if (load_csv(context.csv_filename, &head) < 0) {
        printf("Failed to load CSV.\n");
        return 1;
    }

    printf("Type commands.\n");

    while (1) {
        printf("test: ");

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        if (execute_command(line, &head, &context) == SHELL_EXIT) {
            break;
        }
    }

    free_students(head);

    return 0;
}
