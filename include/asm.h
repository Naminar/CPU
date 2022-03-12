
#ifndef ASM_

#define ASM_

#include "../include/global.h"

#define DBG

#define IP_UP_BY_PTR(arg)      *ip += (arg);

#define IP_UP(arg)              ip += (arg);

//#define IP_DOWN(arg)    *ip -= (arg);

#define RECORD_CELL(type)  *(type*) (code + *ip)

#define HASH_WRITTEN_CMD(type, num)                         \
            *ip -= (sizeof(type));                          \
            *(type*) (code + *ip) = CMD_NUM * 3 + 128 + num;

#define REG_NUM (input_argument[0] - 'a');

//===============================================

typedef struct Jump{

    char name[9];

    int address;

} Jump;

typedef struct JumpCenter{

    Jump* jump_labels;

    int size;

    int capacity;

} JumpCenter;

//===============================================

char* do_asm_code(const char* )                                             ;

void argument_reader  (size_t*, void*, int, FILE*, FILE*, size_t )          ;
void write_labels     (char*, size_t, size_t*, JumpCenter*, const char*)    ;  //maybe const *const

static size_t files_seek_set        (FILE*, FILE*, FILE*)                   ;
static size_t files_initialization  (const char*, FILE**, FILE**, FILE**)   ;
static size_t files_destructor      (FILE*, FILE*, FILE*)                   ;
static size_t code_init             (FILE*, char**)                         ;

//void jump_labels_init               (JumpCenter*, size_t)                   ;
//void jump_labels_extension          (JumpCenter*)                           ;
//void jump_labels_destructor         (JumpCenter* jump_struct)               ;

//===============================================

#ifdef LABELS_

void jump_labels_init(JumpCenter* jump_struct, size_t capacity)
{
    assert(jump_struct);

    jump_struct->jump_labels = (Jump*) calloc(capacity, sizeof(Jump));

    assert(jump_struct->jump_labels);

    jump_struct->size = 0;

    jump_struct->capacity = capacity;
}

void jump_labels_extension(JumpCenter* jump_struct)
{
    assert(jump_struct);

    jump_struct->jump_labels = (Jump*) realloc(jump_struct->jump_labels,
                                2 * jump_struct->capacity * sizeof(Jump));

    assert(jump_struct->jump_labels);

    jump_struct->capacity *= 2;
}

void jump_labels_destructor(JumpCenter* jump_struct)
{
    assert(jump_struct);

    free(jump_struct->jump_labels);
}

#endif

#endif
