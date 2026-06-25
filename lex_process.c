#include <stdlib.h>

#include "helpers/vector.h"
#include "compiler.h"

lex_process* lex_process_create(compile_process* compiler, lex_process_functions* functions, void* private_data) {
    lex_process* process = calloc(1, sizeof(lex_process));
    process->function = functions;
    process->token_vec = vector_create(sizeof(token));
    process->compiler = compiler;
    process->private_data = private_data;
    process->pos.line = 1;
    process->pos.col = 1;
    return process;
}

void lex_process_free(lex_process* process) {
    vector_free(process->token_vec);
    free(process);
}

void* lex_process_private(lex_process* process) {
    return process->private_data;
}

struct vector* lex_process_tokens(lex_process* process) {
    return process->token_vec;
}
