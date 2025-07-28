#include <stdio.h>



typedef struct {
    char* file_buffer;
    size_t pos;
} file_reader;

int open_file(file_reader* reader, const char* filename) {
    FILE* file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        return -1;
    }
    reader->pos = 0;

    size_t file_size = 0;
    while (fgetc(file_ptr) != EOF) {
        reader->file_buffer = realloc(reader->file_buffer, file_size + 1);
        reader->file_buffer[file_size] = fgetc(file_ptr);
        file_size++;
    }
    reader->file_buffer = realloc(reader->file_buffer, file_size + 1);
    reader->file_buffer[file_size] = '\0';

    fclose(file_ptr);
    return 0;
}

void close_file(file_reader* reader) {
    reader->pos = 0;
    free(reader->file_buffer);
    fclose(reader->file_ptr);
}

char read_file_char(file_reader* reader) {
    char c = fgetc(reader->file_ptr);
    return c;
}

size_t file_seek_char(file_reader* reader, char c) {
    size_t pos = reader->pos;
    while (fgetc(reader->file_ptr) != c) {
        if (fgetc(reader->file_ptr) == EOF) {
            return -1;
        }
        pos++;
    }
    return pos;
}



typedef struct {
    char* instructions;
    size_t num_instructions;
} cimple_instruction_set;

typedef struct {
    char* error;
} cimple_parser;

typedef struct {
    char* name;
    char* type;
    char* attribs;
    void* value;
} variable;

#define UNNAMED_SCOPE "unnamed scope"
typedef struct scope {
    char* name;
    variable* variables;
    size_t num_variables;
    struct scope* parent;
} scope;

typedef struct {
    scope* scopes;
    size_t num_scopes;
} scope_stack;

typedef struct cimple_interpreter {
    scope_stack* scopes;
    scope* curent_scope;
    char* error;
} cimple_interpreter;



cimple_instruction_set* parse(cimple_parser* parser, const char* filename) {
    file_reader reader;
    if (open_file(&reader, filename)) {
        return 1;
    }

    // TODO: This should be dynamic
    char cmd_buffer[256];
    size_t i = 0;
    reader.pos = 0;
    char current_char;
    while ((current_char = read_file_char(&reader)) != EOF) {
        if (i < sizeof(cmd_buffer) - 1) {
            cmd_buffer[i++] = (char)current_char;
            break;
        }

        if (current_char == '@') {
            size_t cmd_end = file_seek_char(&reader, '\n');
            strncpy(cmd_buffer, reader.file_buffer + reader.pos, cmd_end - reader.pos);
            cmd_buffer[cmd_end - reader.pos] = '\0';
        }
        
        reader.pos++;
    }

    close_file(&reader);
    return 0;
}



int main(int argc, char* argv[]) {
    cimple_parser parser;
    cimple_instruction_set instructions;
    if (parse(&parser, &instructions, argv[1])) {
        printf("Error parsing file %s\n\n========================================================================\n\n%s", argv[1], parser.error);
        return 1;
    }

    cimple_interpreter interpreter;
    if (interpret(&interpreter, &instructions)) {
        printf("Error interpreting generated instruction set from file %s!\n\n========================================================================\n\n%s", argv[1], interpreter.error);
        return 1;
    }

    

    return 0;
}

