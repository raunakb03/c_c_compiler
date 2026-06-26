#include <string.h>

#include "compiler.h"
#include "helpers/buffer.h"
#include "helpers/vector.h"



#define LEX_GETC_IF(buffer, c, exp)      \
for (c = peekc(); exp; c = peekc()) {   \
    buffer_write(buffer, c);            \
    nextc();                            \
}


static lex_process* curr_lex_process;
static token tmp_token;
static token* read_next_token();

/* ############################## character helper functions ############################## */
static char peekc() {
    return curr_lex_process->function->peek_char(curr_lex_process);
}

static void pushc(char c) {
    curr_lex_process->function->push_char(curr_lex_process, c);
}

static char nextc() {
    char c = curr_lex_process->function->next_char(curr_lex_process);
    curr_lex_process->pos.col += 1;
    if (c == '\n') {
        curr_lex_process->pos.line += 1;
        curr_lex_process->pos.col = 1;
    }

    return c;
}
/* ############################## character helper functions ############################## */

static token* lexer_last_token() {
    return vector_back_or_null(curr_lex_process->token_vec);
}

static token* handle_white_space() {
    token* last_token = lexer_last_token();
    if (last_token) {
        last_token->whitespace = true;
    }
    nextc();
    return read_next_token();
}

static struct pos lex_file_position() {
    return curr_lex_process->pos;
}

static token* token_create(token* _token) {
    memcpy(&tmp_token, _token, sizeof(token));
    tmp_token.pos =  lex_file_position();
    return &tmp_token;
}

token* token_make_number_for_value(unsigned long number) {
    return token_create(&(token){.type = TOKEN_TYPE_NUMBER, .llnum = number});
}

const char* read_number_str() {
    const char* num = NULL;
    struct buffer* buffer = buffer_create();
    char c = peekc();
    LEX_GETC_IF(buffer, c, (c >= '0' && c <= '9'));

    buffer_write(buffer, 0x00);
    return buffer_ptr(buffer);
}

unsigned long long read_number() {
    const char* s = read_number_str();
    return atoll(s);
}

token* token_make_number() {
    return token_make_number_for_value(read_number());
}

token* read_next_token() {
    token* token = NULL;
    char c = peekc();

    switch (c) {
        NUMERIC_CASE:
            token = token_make_number();
            break;
        case ' ':
        case '\t':
        // TODO: \n will be handled seperately. Here right now to avoid compiler compiler error :)
        case '\n':
            token = handle_white_space();
            break;
        case EOF:
            break;
        default:
            compiler_error(curr_lex_process->compiler, "Unexpected token\n");
    }

    return token;
}
 
int lex(lex_process* process) {
    process->current_expression_count = 0;
    process->parentheses_buffer = NULL;
    curr_lex_process = process;
    process->pos.filename = process->compiler->cfile.abs_path;

    token* token = read_next_token();
    while (token) {
        vector_push(process->token_vec, token);
        token = read_next_token();
    }
    return LEXICAL_ANALYSIS_ALL_OK;
}
