#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_env_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening .env file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }

        // Remove trailing newline character
        line[strcspn(line, "\n")] = '\0';

        // Find the '=' character
        char *delimiter_pos = strchr(line, '=');
        if (!delimiter_pos) {
            fprintf(stderr, "Invalid line in .env file: %s\n", line);
            continue;
        }

        // Split the line into key and value
        *delimiter_pos = '\0';
        char *key = line;
        char *value = delimiter_pos + 1;

#ifdef _WIN32
        // Construct the key=value string for _putenv
        char env_entry[512];
        snprintf(env_entry, sizeof(env_entry), "%s=%s", key, value);

        // Set the environment variable
        if (_putenv(env_entry) != 0) {
            perror("Error setting environment variable");
        }
#else
        // Set the environment variable
        if (setenv(key, value, 1) != 0) {
            perror("Error setting environment variable");
        }
#endif
    }

    fclose(file);
}

char* get_env_variable(const char *key) {
    // Load environment variables from .env file
    load_env_file("envVariables.env");

    // Retrieve the environment variable value
    char *value = getenv(key);
    if (value == NULL) {
        fprintf(stderr, "Environment variable %s not found\n", key);
    }

    return value;
}
