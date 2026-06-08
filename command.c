#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "command.h"
#include "file_io.h"

#define TOKEN_SIZE 64

static void trim(char* str);
static int parse_int_arg(const char* str, int* value);

#ifdef ADMIN_MODE
static Command commands[] = {
    {"save", handle_save, "save", "Save students to CSV"},
    {"reload", handle_reload, "reload", "Reload students from CSV"},
    {"add", handle_add, "add <id> <name> <score>", "Add a student"},
    {"delete", handle_delete, "delete <id>", "Delete a student"},
    {"update", handle_update, "update <id> <score>", "Update student score"},
    {"find", handle_find, "find <id>", "Find student by ID"},
    {"list", handle_list, "list", "List all students"},
    {"stats", handle_stats, "stats", "Show statistics"},
    {"help", handle_help, "help", "Show help"},
    {"clear", handle_clear, "clear", "Clear screen"},
    {"exit", handle_exit, "exit", "Exit shell"}
};
#endif

#ifdef CLIENT_MODE
static Command commands[] = {
    {"reload", handle_reload, "reload", "Reload students from CSV"},
    {"find", handle_find, "find <id>", "Find student by ID"},
    {"list", handle_list, "list", "List all students"},
    {"stats", handle_stats, "stats", "Show statistics"},
    {"help", handle_help, "help", "Show help"},
    {"clear", handle_clear, "clear", "Clear screen"},
    {"exit", handle_exit, "exit", "Exit shell"}
};
#endif

static int command_count = sizeof(commands) / sizeof(commands[0]);

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

static int parse_int_arg(const char* str, int* value) {
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

ShellResult execute_command(char* line, Student** head, ShellContext* context) {
    char* command_name;
    char* args;
    int i;

    if (line == NULL) {
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    trim(line);

    if (line[0] == '\0') {
        return SHELL_OK;
    }

    command_name = strtok(line, " \t");

    if (command_name == NULL) {
        return SHELL_OK;
    }

    args = strtok(NULL, "");

    if (args != NULL) {
        trim(args);
    }

    for (i = 0; i < command_count; i++) {
        if (strcmp(command_name, commands[i].name) == 0) {
            return commands[i].handler(args, head, context);
        }
    }

    printf("Unknown command or permission denied.\n");
    return SHELL_ERR_UNKNOWN_COMMAND;
}

ShellResult handle_save(char* args, Student** head, ShellContext* context) {
    (void)args;
    (void)head;
    (void)context;

    printf("save command is not implemented yet.\n");
    return SHELL_OK;
}

ShellResult handle_reload(char* args, Student** head, ShellContext* context) {
    (void)args;
    (void)head;
    (void)context;

    printf("reload command is not implemented yet.\n");
    return SHELL_OK;
}

ShellResult handle_add(char* args, Student** head, ShellContext* context) {
    (void)args;
    (void)head;
    (void)context;

    printf("add command is not implemented yet.\n");
    return SHELL_OK;
}

ShellResult handle_delete(char* args, Student** head, ShellContext* context) {
    (void)args;
    (void)head;
    (void)context;

    printf("delete command is not implemented yet.\n");
    return SHELL_OK;
}

ShellResult handle_update(char* args, Student** head, ShellContext* context) {
    (void)args;
    (void)head;
    (void)context;

    printf("update command is not implemented yet.\n");
    return SHELL_OK;
}

ShellResult handle_find(char* args, Student** head, ShellContext* context) {
    int id;
    Student* target;

    (void)context;

    if (head == NULL) {
        printf("Error: invalid argument.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    if (args == NULL || args[0] == '\0') {
        printf("Error: missing argument.\n");
        return SHELL_ERR_MISSING_ARGUMENT;
    }

    if (!parse_int_arg(args, &id)) {
        printf("Error: invalid ID.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    target = find_student(*head, id);

    if (target == NULL) {
        printf("Error: student not found.\n");
        return SHELL_ERR_STUDENT_NOT_FOUND;
    }

    printf("ID: %d\n", target->id);
    printf("Name: %s\n", target->name);
    printf("Score: %d\n", target->score);

    return SHELL_OK;
}


ShellResult handle_list(char* args, Student** head, ShellContext* context) {
    (void)args;
    (void)context;

    if (head == NULL) {
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    list_students(*head);
    return SHELL_OK;
}

ShellResult handle_stats(char* args, Student** head, ShellContext* context) {
    int count;
    int sum;
    int max;
    int min;
    double average;

    (void)args;
    (void)context;

    if (head == NULL) {
        printf("Error: invalid argument.\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    count = count_students(*head);

    if (count == 0) {
        printf("No student data available.\n");
        return SHELL_OK;
    }

    sum = get_sum_score(*head);
    max = get_max_score(*head);
    min = get_min_score(*head);
    average = (double)sum / count;

    printf("Count: %d\n", count);
    printf("Average: %.1f\n", average);
    printf("Max: %d\n", max);
    printf("Min: %d\n", min);

    return SHELL_OK;
}


ShellResult handle_help(char* args, Student** head, ShellContext* context) {
    int i;

    (void)args;
    (void)head;
    (void)context;

    printf("Commands:\n");

    for (i = 0; i < command_count; i++) {
        printf("%-25s %s\n", commands[i].usage, commands[i].description);
    }

    return SHELL_OK;
}

ShellResult handle_clear(char* args, Student** head, ShellContext* context) {
    (void)args;
    (void)head;
    (void)context;

    printf("\033[2J\033[H");
    return SHELL_OK;
}

ShellResult handle_exit(char* args, Student** head, ShellContext* context) {
    (void)args;
    (void)head;
    (void)context;

    printf("Goodbye.\n");
    return SHELL_EXIT;
}
