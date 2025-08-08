#include "parser.h"

#include <stdio.h>

struct keyword_function_pair {
    char* keyword;
    
}

struct cimple_parser_state {
    int(*process)(struct cimple_parser*, char);
};

struct cimple_parser {
    struct cimple_parser_state* state_stack;
    size_t num_states;
    char* error;
};

void parser_push_state(struct cimple_parser* parser) {
    parser->state_stack = reallocarray(parser->state_stack, ++parser->num_states, sizeof(struct cimple_parser_state));
}

int parser_process_char(struct cimple_parser* parser, char c) {
    return parser->state_stack[parser->num_states].process(parser, c);
}

int parse(struct cimple_parser* parser, struct cimple_instruction_set* instructions, const char* expr) {
    // TODO: This should be dynamic
    char token[256];
    size_t i = 0;
    while (expr[i] != '\0') {
        if (parser_process_char(parser, expr[i])) {
            printf("Error processing characer '%c' whilst parsing!\n%s", expr[i], parser->error);
            return -1;
        }

    }

    return 0;
}
