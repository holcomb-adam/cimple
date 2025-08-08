struct cimple_instruction_set;
struct cimple_parser_state;
struct cimple_parser;

int parse(struct cimple_parser* parser, struct cimple_instruction_set* instructions, const char* expr);