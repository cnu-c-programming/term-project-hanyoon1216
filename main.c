#include <stdio.h>
#include <string.h>
#include "student.h"
#include "file_io.h"
#include "command.h"

#define LINE_SIZE 256

static void print_usage(const char* program_name) {
#ifdef ADMIN_MODE
    (void)program_name;
    printf("Usage: ./admin_shell <csv_file> [-f command_file]\n");
#endif

#ifdef CLIENT_MODE
    (void)program_name;
    printf("Usage: ./client_shell <csv_file> [-f command_file]\n");
#endif
}

static void remove_newline(char* line) {
    size_t len;

    if (line == NULL) {
        return;
    }

    len = strlen(line);

    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
        line[len - 1] = '\0';
        len--;
    }
}

static const char* get_prompt(void) {
#ifdef ADMIN_MODE
    return "admin> ";
#endif

#ifdef CLIENT_MODE
    return "client> ";
#endif
}

static void print_program_title(void) {
#ifdef ADMIN_MODE
    printf("[Admin Program]\n");
#endif

#ifdef CLIENT_MODE
    printf("[Client Program]\n");
#endif
}

int main(int argc, char* argv[]) {
    Student* head = NULL;
    ShellContext context;
    char line[LINE_SIZE];
    int loaded_count;
    ShellResult result;

    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    }

    context.csv_filename = argv[1];

    print_program_title();

    loaded_count = load_csv(context.csv_filename, &head);

    if (loaded_count < 0) {
        free_students(head);
        return 1;
    }

    printf("Loaded %d students from %s.\n", loaded_count, context.csv_filename);

    while (1) {
        printf("%s", get_prompt());

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        remove_newline(line);

        result = execute_command(line, &head, &context);

        if (result == SHELL_EXIT) {
            break;
        }
    }

    free_students(head);

    return 0;
}
