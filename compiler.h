#ifndef PEACHCOMPILER_H
#define PEACHCOMPILER_H

#include <stdio.h>
#include <stdbool.h>

#include "helpers/vector.h"
#include "helpers/buffer.h"

#define NUMERIC_CASE \
case '0': \
case '1': \
case '2': \
case '3': \
case '4': \
case '5': \
case '6': \
case '7': \
case '8': \
case '9'

enum {
    COMPILER_FILE_COMPILED_OK,
    COMPILER_FAILED_WITH_ERRORS,
};

enum {
    LEXICAL_ANALYSIS_ALL_OK,
    LEXICAL_ANALYSIS_INPUT_ERROR,
};

enum {
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_SYMBOL,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_COMMENT,
    TOKEN_TYPE_NEWLINE,
};

typedef struct pos {
    int line;
    int col;
    const char* filename;
} pos;

typedef struct token {
    int type;
    int flags;
    pos pos;
    union {
        char cval;
        const char* sval;
        unsigned int inum;
        unsigned long lnum;
        unsigned long long llnum;
        void *any;
    };
    bool whitespace;
    const char* between_brackets;
} token;

typedef struct lex_process lex_process;
typedef struct compile_process compile_process;

typedef char (*LEX_PROCESS_NEXT_CHAR)(struct lex_process* process);
typedef char (*LEX_PROCESS_PEEK_CHAR)(struct lex_process* process);
typedef void (*LEX_PROCESS_PUSH_CHAR)(struct lex_process* process, char c);

typedef struct lex_process_functions {
    LEX_PROCESS_NEXT_CHAR next_char;
    LEX_PROCESS_PEEK_CHAR peek_char;
    LEX_PROCESS_PUSH_CHAR push_char;
} lex_process_functions;

struct lex_process {
    pos pos;
    struct vector* token_vec;
    compile_process* compiler;
    int current_expression_count;
    struct buffer* parentheses_buffer;
    lex_process_functions* function;
    void* private_data;
};

struct compile_process {
    int flags;
    pos pos;
    struct compile_process_input_file {
        FILE* fp;
        const char* abs_path;
    } cfile;

    FILE* ofile;
};

int compile_file(const char* filename, const char* out_filename, int flags);
void compiler_error(compile_process* compiler, const char* msg, ...);
void compiler_warning(compile_process* compiler, const char* msg, ...);

compile_process* compile_process_create(const char* filename, const char* out_filename, int flags);
char compile_process_next_char(lex_process* lex_process);
char compile_process_peek_char(lex_process* lex_process);
void compile_process_push_char(lex_process* lex_process, char c);

lex_process* lex_process_create(compile_process* compiler, lex_process_functions* functions, void* private_data) ;
void lex_process_free(lex_process* process);
void* lex_process_private(lex_process* process);
struct vector* lex_process_tokens(lex_process* process);
int lex(lex_process* process);

#endif
