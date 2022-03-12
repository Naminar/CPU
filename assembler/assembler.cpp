
#define LABELS_

#include "../include/asm.h"

char* do_asm_code(const char* file)
{
    //==============INITIALIZATION===============

    assert(file);

    FILE* read_file = NULL,
        * out_file  = NULL,
        * bin_file  = NULL;

    char* code      = NULL,
        * ptr_point = NULL;

    JumpCenter jump_struct = {};

    size_t jump_count = 0;

    size_t ip = 0;

    int do_double_asm_key  = 1,   //do_double_asm_key
        do_labels_key = 1;   //do_labels_key

    char input_argument [(sizeof (DATA) > 10)? sizeof (DATA) : 10] = {};

    files_initialization(file, &read_file, &bin_file, &out_file);

    size_t  read_file_size = code_init(read_file, &code);

    jump_labels_init(&jump_struct, 1);

    //==================ASM WORK=================

     while (do_double_asm_key)
     {
        files_seek_set(read_file, bin_file, out_file);

        while (fscanf(read_file,"%s", input_argument) != EOF)
        {
            #define DEF_CMD(name, num, asm_code, ...)\
                if (!strcmp (input_argument, #name)) \
                asm_code else

            #define DEF_JMP(naming, num, sign)                                       \
                if (!strcmp (input_argument, #naming))                               \
                {                                                                    \
                    argument_reader(&ip, code, JMP_##naming, read_file, out_file, 0);\
                                                                                     \
                    fscanf(read_file, "%s",   input_argument);                       \
                                                                                     \
                    int ind = 0;                                                     \
                                                                                     \
                    DBG printf(" "#naming" ");                                       \
                                                                                     \
                    while (strcmp(jump_struct.jump_labels[ind].name, input_argument) \
                           &&                                                        \
                           ind < jump_struct.size + 1)                               \
                        ++ind;                                                        \
                                                                                       \
                    DBG printf("%d", ind);                                              \
                                                                                         \
                    if (ind != jump_struct.size + 1)                                      \
                    {                                                                      \
                        *(int*) (code + ip) = jump_struct.jump_labels[ind].address;         \
                                                                                             \
                        DBG printf("!!!!%d!!!!", jump_struct.jump_labels[ind].address);       \
                                                                                               \
                                                                                                \
                    }                                                                            \
                    else                                                                          \
                    {                                                                              \
                        do_double_asm_key = 2;                                                      \
                                                                                                     \
                    }                                                                                 \
                                                                                                       \
                    IP_UP(sizeof(int))/*ip += sizeof(int);*/                                            \
                                                                                                         \
                } else


            #include "../include/commands.h"

            #undef DEF_CMD
            #undef DEF_JMP

            {
                if ((ptr_point = strchr(input_argument, ':')) && do_labels_key == 1)
                {
                    write_labels(ptr_point, ip, &jump_count, &jump_struct, input_argument);
                }
            }


        }

        --do_double_asm_key;

        do_labels_key = 0;
        ip            = 0;
      }
    //DBG printf(ELEMENT_FMT, *(DATA*) (code+1));
    //printf(ELEMENT_FMT, sizeof(double));

    fwrite(code, sizeof(char), read_file_size, bin_file);

    files_destructor(read_file, bin_file, out_file);

    free(code);

    //return;*/
}

void write_labels(char* ptr_point, size_t ip, size_t* jump_count,  JumpCenter* jump_struct, const char* input_argument)
{
    *ptr_point = '\0';

    if (jump_struct->size == jump_struct->capacity)
        jump_labels_extension(jump_struct);

    strcpy(jump_struct->jump_labels[*jump_count].name, input_argument);

    jump_struct->jump_labels[*jump_count].address = ip;

    DBG printf("%s%d___%d", jump_struct->jump_labels[*jump_count].name,
               jump_struct->jump_labels[*jump_count].address, ip);

    ++jump_struct->size;

    ++(*jump_count);

    return;
}

void argument_reader(size_t* ip, void* code, int CMD_NUM, FILE* read_file, FILE* out_file, size_t number_arg)
{
    //assert(number_arg >= 0);


    DBG printf("%%%d ", CMD_NUM);
    DBG fprintf(out_file, "\n%d ", CMD_NUM);

    RECORD_CELL(char) = CMD_NUM;

    IP_UP_BY_PTR(sizeof(char))

    static char input_argument [(sizeof(DATA) > 10)? sizeof(DATA) : 10] = {},

                input_arg_help [2] = {};

    while (number_arg)
    {
        fscanf(read_file, " %c", input_argument);

        DBG printf("%c", input_argument[0]);

        if (input_argument[0] == '[')
        {
            if (fscanf(read_file, ELEMENT_FMT"%c", (DATA*) input_argument, input_arg_help)
                &&
                input_arg_help[0] == ']')
            {
                DBG fprintf(out_file, "arg"ELEMENT_FMT, *(DATA*) input_argument);

                RECORD_CELL(DATA)= *(DATA*) input_argument;

                HASH_WRITTEN_CMD(char, 0)

                IP_UP_BY_PTR(sizeof(DATA) + sizeof(char))
            }
            else if (fscanf(read_file, "%c%c%c", input_argument, input_arg_help, input_arg_help + 1)
                     &&
                     input_arg_help[0] == 'x'
                     &&
                     input_arg_help[1] == ']')
            {
                DBG fprintf(out_file, "arg%c", input_argument[0]);

                RECORD_CELL(DATA) = (DATA) REG_NUM

                HASH_WRITTEN_CMD(char, 2)

                IP_UP_BY_PTR(sizeof(DATA) + sizeof(char))
            }
            else
            {
                printf("SYNTAX ERROR!!!");

                assert(0);
            }

        }
        else
        {
            fseek(read_file, ftell(read_file)-1, 0);

            if (fscanf(read_file, ELEMENT_FMT, (DATA*) input_argument))
            {
                DBG fprintf(out_file, "arg"ELEMENT_FMT, *(DATA*) input_argument);

                RECORD_CELL(DATA) = *(DATA*) input_argument;

                IP_UP_BY_PTR(sizeof(DATA))
            }
            else if (fscanf(read_file, "%c%c", input_argument, input_arg_help)
                     &&
                     input_arg_help[0] == 'x')
            {
                DBG fprintf(out_file, "arg%c", input_argument[0]);

                RECORD_CELL(DATA) = (DATA) REG_NUM

                HASH_WRITTEN_CMD(char, 1)

                IP_UP_BY_PTR(sizeof(DATA) + sizeof(char))
            }
            else
            {
                printf("SYNTAX ERROR!!!\n"
                       "Check the correctness of the input");


                assert(0);
            }
        }

        --number_arg;
    }


    return;
}

static size_t files_seek_set(FILE* read_file, FILE* bin_file, FILE* out_file)
{
    assert(read_file && bin_file && out_file);


    fseek(read_file, 0, SEEK_SET);

    fseek(bin_file,  0, SEEK_SET);

    fseek(out_file,  0, SEEK_SET);

    return 1;
}

static size_t files_initialization(const char* file, FILE** read_file, FILE** bin_file, FILE** out_file)
{
    assert(file);


    *read_file = fopen(file, "r");

    *out_file  = fopen("asm.txt", "w");

    *bin_file  = fopen("bin.txt", "wb");


    assert(*read_file && *bin_file && *out_file);


    return 1;
}

static size_t files_destructor(FILE* read_file, FILE* bin_file, FILE* out_file)
{
    if (!fclose(read_file) && !fclose(out_file) && !fclose(bin_file))
        return 1;


    assert(0);
}

static size_t code_init(FILE* read_file, char** code)
{
    size_t read_file_size = 0;

    //code = NULL;

    fseek(read_file, 0, SEEK_END);

    read_file_size = ftell(read_file);

    fseek(read_file, 0, SEEK_SET);

    *code = (char*) calloc(read_file_size, sizeof(char));


    assert(code);


    return read_file_size;
}
