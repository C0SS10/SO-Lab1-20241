#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    FILE *input = NULL;
    FILE *output = NULL;

    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    // --- Verificar si input y output son el mismo archivo ---
    if (argc == 3) {
        // Caso 1: mismo nombre
        if (strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }

        // Caso 2: mismo inodo/dispositivo (hardlinks, etc.)
        struct stat st_in, st_out;
        if (stat(argv[1], &st_in) == 0 && stat(argv[2], &st_out) == 0 &&
            st_in.st_ino == st_out.st_ino && st_in.st_dev == st_out.st_dev) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }
    }

    // --- Abrir archivo de entrada o stdin ---
    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    } else {
        input = stdin;
    }

    // --- Abrir archivo de salida o stdout ---
    if (argc == 3) {
        output = fopen(argv[2], "w");
        if (!output) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
            exit(1);
        }
    } else {
        output = stdout;
    }

    // --- Leer todas las lÃ­neas en memoria ---
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    size_t capacity = 10;
    size_t count = 0;
    char **lines = malloc(capacity * sizeof(char *));
    if (!lines) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    while ((read = getline(&line, &len, input)) != -1) {
        if (count >= capacity) {
            capacity *= 2;
            char **tmp = realloc(lines, capacity * sizeof(char *));
            if (!tmp) {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
            lines = tmp;
        }
        lines[count] = strdup(line);
        if (!lines[count]) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        count++;
    }
    free(line);

    // --- Escribir en orden inverso ---
    for (ssize_t i = count - 1; i >= 0; i--) {
        fprintf(output, "%s", lines[i]);
        free(lines[i]);
    }

    free(lines);

    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    return 0;
}
