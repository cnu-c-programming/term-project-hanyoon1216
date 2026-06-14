#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "student.h"
#include "file_io.h"
#include "command.h"

#define LINE_SIZE 256

static void print_usage(const char* program_name) {
    (void)program_name;

#ifdef ADMIN_MODE
    printf("Usage: ./admin_shell <csv_file> [-f command_file]\n");
#endif

#ifdef CLIENT_MODE
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

static void trim(char* str) {
    int start = 0;
    int end;
    int i;

    if (str == NULL) {
        return;
    }

    while (str[start] != '\0' && isspace((unsigned char)str[start])) {
        start++;
    }

    if (start > 0) {
        i = 0;

        while (str[start] != '\0') {
            str[i++] = str[start++];
        }

        str[i] = '\0';
    }

    end = (int)strlen(str) - 1;

    while (end >= 0 && isspace((unsigned char)str[end])) {
        str[end] = '\0';
        end--;
    }
}

static int is_comment_or_empty(const char* line) {
    if (line == NULL) {
        return 1;
    }

    if (line[0] == '\0') {
        return 1;
    }

    if (line[0] == '#') {
        return 1;
    }

    return 0;
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

static void print_skipped_message(ShellResult result, int line_number) {
    if (result == SHELL_OK || result == SHELL_EXIT) {
        return;
    }

    printf("Skipped line %d.\n", line_number);
}

static ShellResult run_command_file(const char* command_filename, Student** head, ShellContext* context) {
    FILE* fp;
    char line[LINE_SIZE];
    int line_number = 0;
    ShellResult result;

    fp = fopen(command_filename, "r");

    if (fp == NULL) {
        printf("Error: cannot open command file.\n");
        return SHELL_ERR_FILE_OPEN;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        line_number++;

        remove_newline(line);
        trim(line);

        if (is_comment_or_empty(line)) {
            continue;
        }

        printf("[command file:%d] %s\n", line_number, line);

        result = execute_command(line, head, context);

        if (result == SHELL_EXIT) {
            fclose(fp);
            return SHELL_EXIT;
        }

        print_skipped_message(result, line_number);
    }

    fclose(fp);

    return SHELL_OK;
}

static ShellResult run_interactive_shell(Student** head, ShellContext* context) {
    char line[LINE_SIZE];
    ShellResult result;

    while (1) {
        printf("%s", get_prompt());

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        remove_newline(line);

        result = execute_command(line, head, context);

        if (result == SHELL_EXIT) {
            return SHELL_EXIT;
        }
    }

    return SHELL_OK;
}

int main(int argc, char* argv[]) {
    Student* head = NULL;
    ShellContext context;
    const char* command_filename = NULL;
    int loaded_count;
    ShellResult result;

    if (argc != 2 && argc != 4) {
        print_usage(argv[0]);
        return 1;
    }

    if (argc == 4) {
        if (strcmp(argv[2], "-f") != 0) {
            print_usage(argv[0]);
            return 1;
        }

        command_filename = argv[3];
    }

    context.csv_filename = argv[1];

    print_program_title();

    loaded_count = load_csv(context.csv_filename, &head);

    if (loaded_count < 0) {
        free_students(head);
        return 1;
    }

    printf("Loaded %d students from %s.\n", loaded_count, context.csv_filename);

    if (command_filename != NULL) {
        result = run_command_file(command_filename, &head, &context);

        if (result == SHELL_EXIT) {
            free_students(head);
            return 0;
        }
    }

    run_interactive_shell(&head, &context);

    free_students(head);

    return 0;
}
