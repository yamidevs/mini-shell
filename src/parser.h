#ifndef PARSER_H
#define PARSER_H

typedef enum {
    SEMICOL,
    AND,
    OR,
    PARAL,
    PIPE,
    END
} separator_t;

typedef enum {
    INPUT,
    OUTPUT,
    OUTPUT_APPEND,
    ERROR,
    ERROR_APPEND,
    ERROR_OUTPUT
} redirection_t;

int trim(char* str);
int clean(char* str);
int split_str(char* str, char* tab[], int max_size);
int split_command(char* tab[], separator_t* sep);
int redirection(char* tab[], redirection_t* redir);

#endif
