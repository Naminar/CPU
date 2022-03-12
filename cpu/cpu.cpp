
#include "../include/cpu.h"


void cpu_work(void)
{
    Cpu cpu = {};

    cpu_init(&cpu);

    size_t ip       = 0,
           hlt_key  = 1;

    int cmd_key     = 0,
        codeIP      = 0,
        video_ip    = 0;

    Stack ptr_stk = {};

    stack_init(&ptr_stk, 3, NULL);

    while(hlt_key)
    {
        codeIP  = (int) cpu.code[ip];
        //printf("!!%d!!",codeIP);
        cmd_key = -1;

        if (codeIP > 127)
        {
            cmd_key = (codeIP - 128) % 3;
            codeIP  = (codeIP - 128) / 3;
        }

        switch(codeIP)
        {
            #define DEF_CMD(name, num, asm_code, cpu_code)\
                case CMD_##name:                           \
                {                                           \
                    ip += sizeof(char);                      \
                                                              \
                    cpu_code                                   \
                                                                \
                    break;                                       \
                }

            #define DEF_JMP(name, num, sign)        \
                case JMP_##name:                     \
                {                                     \
                    static DATA first_element  = 0;    \
                    static DATA second_element = 0;     \
                                                         \
                    stack_pop(&cpu.STK, &first_element);  \
                                                           \
                    stack_pop(&cpu.STK, &second_element);   \
                                                             \
                    if (first_element sign second_element)    \
                    {                                          \
                        IP_UP(sizeof(char))                     \
                                                                 \
                        ip = CELL_VALUE(int, cpu.code);           \
                    }                                              \
                    else                                            \
                    {                                                \
                        IP_UP(sizeof(char) + sizeof(int))             \
                    }                                                  \
                                                                        \
                                                                         \
                                                                          \
                    break;                                                 \
                }


            #include "../include/commands.h"

            #undef DEF_CMD
            #undef DEF_JMP

            default :
            {
                printf ("LOOOOLka, %c %d", cpu.code[ip], ip);

                cpu_dump(&cpu, &ptr_stk, __LINE__, __FILE__, __PRETTY_FUNCTION__);

                exit(1);
            }
        }
    }

    free(ptr_stk.data);

    //printf("__%d__reg__", cpu.REG[0]);

    //printf("__%d__ram__", cpu.RAM[10]);

    cpu_destructor(&cpu);
}

static unsigned char* code_init(void)
{
    //assert(text);


    FILE* bin = fopen("../assembler/bin.txt", "rb");


    assert(bin);


    size_t size_bin = 0;

    fseek(bin, 0, SEEK_END);

    size_bin = ftell(bin);

    fseek(bin, 0, SEEK_SET);

    unsigned char* code = (unsigned char*) calloc(size_bin, sizeof(char));


    assert(code);


    fread(code, size_bin, sizeof(char), bin);

    fclose(bin);

    return code;
}

static void cpu_init(Cpu* cpu)
{
    //DATA* RAM = (DATA*) calloc(20, sizeof(DATA));
    //DATA* REG = (DATA*) calloc(5, sizeof(DATA)); // abcde
    //int* VIDEO = (int*) calloc(120 * 5, sizeof(int));
    assert(cpu);

    void* MEMORY = (void*) calloc((RAM_SIZE + REG_SIZE + WITH_CANARY_CONTROL_DIMENS) * sizeof(DATA) + VIDEO_SIZE * sizeof(int), 1); //sizeof(chat)

    cpu->RAM = (DATA*) MEMORY;

    cpu->REG = cpu->RAM + RAM_SIZE;

    cpu->VIDEO = (int*) (cpu->REG + REG_SIZE);

    cpu->STK = {};

    stack_init(&cpu->STK, CONTROL_DIMENS, cpu->VIDEO + VIDEO_SIZE);   //maybe (void)

    //unsigned char* code ;

    cpu->code = code_init();

    //Stack stk = {};


}

static void cpu_destructor(Cpu* cpu)
{
    assert(cpu);


    free(cpu->RAM);

    stack_dtor(&cpu->STK);

    free(cpu->code);
}

static void cpu_dump(Cpu* cpu, Stack* ptr_stk, int line, const char* file, const char* function)
{
    FILE* log = fopen("CPU_Log.txt", "w");


    assert(log);


    fprintf(log, "Dump is open!\n"
            "CPU STACK:\n");

    stack_dump(&cpu->STK, 0, line, file, function);

    fprintf(log, "RETURN PTR STACK:\n");

    stack_dump(ptr_stk, 0, line, file, function);


    fprintf(log, "RAM: %p\n"
                 "REG: %p\n"
                 "VIDEO: %p\n",
            cpu->RAM, cpu->REG, cpu->VIDEO);

    fclose(log);
}


/*/========================================================


int stack_init(Stack* stk, int capacity, void* stack_place)
{
    assert(stk);

    if (capacity <= 0)
    {
        printf(" Your capacity is wrong, it must be positive.");

        return 0;//exit(1);
    }

    if (stack_place)                                                        //change position in memory
        stk->data = (DATA*) stack_place;//calloc(capacity + 2, sizeof(DATA)); //the space for canary
    else
        stk->data = (DATA*) calloc(capacity + 2, sizeof(DATA));

    assert(stk->data);

    stk->capacity = capacity;
    stk->size = 0;

    stk->data[0] = CANARY;
    stk->data[capacity + 1] = CANARY;

    stk->first_canary  = CANARY; ////
    stk->second_canary = CANARY;////

    DragonHash(stk);

    return 1;
}

void stack_dtor(Stack* stk)
{
    assert(stk);

    free(stk->data);

    stk->data = ERR_PTR;
}

int stack_push(Stack* stk, DATA element)
{
    assert(stk);

    int key = 0;

    if(key = is_stack_spoiled(stk))
    {
        stack_dump(stk, key, __LINE__, __FILE__, __PRETTY_FUNCTION__);
    }

    if(stk->size >= stk->capacity)
        if(!stack_resize(stk))
            return ERR_RESIZE;

    stk->data[stk->size + 1] = element;

    ++(stk->size);

    DragonHash(stk);

    return 1;
}

int stack_pop(Stack* stk, DATA* element)
{
    assert(stk);
    assert(stk->size >= 1);

    int key = 0;

    if(key = is_stack_spoiled(stk))
    {
        stack_dump(stk, key, __LINE__, __FILE__, __PRETTY_FUNCTION__);
    }

    *element = stk->data[stk->size]; //go to the cell taking into account the canary

    //if(stk->size == 0)
    //   return ERR_POP_SIZE;   may change assert

    --(stk->size);

    DragonHash(stk);

    return 1;
}

int stack_resize(Stack* stk)
{
    stk->data = (DATA*) realloc(stk->data, (stk->capacity + 1) * 2
                                            * sizeof(DATA));  //size was changed

    if(stk->data == NULL)
        return 0;

    stk->capacity = 2 * stk->capacity;
    stk->data[stk->capacity + 1] = CANARY;

    return 1;
}

void stack_dump(Stack* stk, int key, int line, const char* file, const char* function)
{
    printf("ERROR WAS HAPPENED IN FUNCTION:\n%s\n LINE = %d\n FILE = %s\n\n",function, line, file);

    switch(key)
    {
        case -1:
        {
            printf("YOUR STRUCT CANARY WAS CHANGED!!!");

            break;
        }

        case -2:
        {
            printf("YOUR DATA CANARY WAS CHANGED!!!!!!");

            break;
        }

        case -3:
        {
            printf("SIZE OF DATA BIGGER THAN CONTROL_DIMENS!!! GO AWAY!!! SAVE YOUR (dark) SOULS!!!");

            break;
        }

        case -4:
        {
            printf("CAPACITY OF DATA BIGGER THAN CONTROL_DIMENS!!! ZOMBIE EMERGENCY!!!!!!!!");

            break;
        }

        case -5:
        {
            printf("SIZE BIGGER THAN CAPACITY. THAT'S NOT SO GOOD. FORMATTING YOUR DRIVES...");

            break;
        }

        case -6:
        {
            printf("DATA POINTER IS NULL. SEND $100 TO THE AUTHOR.");

            break;
        }

        case -7:
        {
            printf("HASH WAS CHANGED. GO LEARN MATHAN, BABY");

            break;
        }

        default:
        {
            printf("DUMP ERROR. I CANT BEAR IT. ABORTING... (no)");

            break;
        }
    }

    printf("\n\nDUMP RESULT:"                    "\n"
           "first canary:   %3d;  REAL CANARY = %d\n"
           "second canary:  %3d;  REAL CANARY = %d\n"
           "capacity:       %3d"                 "\n"
           "size:           %3d"                 "\n"
           "data[%p]\n{ \n",

            stk->first_canary,
            CANARY,
            stk->second_canary,
            CANARY,
            stk->capacity,
            stk->size,
            stk->data);

    int ind_capacity = stk->capacity;

    printf("  data[%d] = " ELEMENT_FMT " This is a CANARY; "
           "REAL CANARY = %d\n", ind_capacity + 1, stk->data[ind_capacity + 1], CANARY);

    for (; ind_capacity > 0; --ind_capacity)
    {
        printf("  data[%d] = " ELEMENT_FMT "\n", ind_capacity, stk->data[ind_capacity]);

    }

    printf("  data[%d] = " ELEMENT_FMT " This is a CANARY; "
           "REAL CANARY = %d\n} \n", 0, stk->data[0], CANARY);

    return;//exit(2);
}

int is_stack_spoiled(Stack* stk)
{
    assert(stk);

    if(stk->first_canary != CANARY || stk->second_canary != CANARY)
        return -1;

    if(stk->data[0] != CANARY || stk->data[stk->capacity + 1] != CANARY)
        return -2;

    if(stk->size > CONTROL_DIMENS)
        return -3;

    if(stk->capacity > CONTROL_DIMENS)
        return -4;

    if(stk->size > stk->capacity)
        return -5;

    if(stk->data == NULL)
        return -6;
    //////////////////////////////////

    size_t hash_copy = stk->hash;

    DragonHash(stk);

    if(hash_copy != stk->hash)
        return -7;

    return 0;
}


void DragonHash(struct Stack* stk)
{
    assert(stk);

    stk->hash = 0;

    size_t records_key = 0;

    const unsigned char* struct_records = (const unsigned char*) stk;
    const unsigned char* stack_data     = (const unsigned char*) stk->data;

    for(size_t ind = 3; ind < sizeof(struct Stack); ind++)
    {
        records_key += 2 * struct_records[ind - 1] + struct_records[ind - 2] % 2 + struct_records[ind - 3];
    }

    switch(records_key % 4)
    {
        case 0:
        {
            records_key = records_key << 4 + 1;

            break;
        }

        case 1:
        {
            records_key = records_key << 8 + 3;

            break;
        }

        case 2:
        {
            records_key = records_key << 12 + 5;

            break;
        }

        case 3:
        {
            records_key = records_key << 16 + 7;

            break;
        }
    }

    size_t data_key = NULL_HASH;

    const size_t size_converter_value = sizeof(DATA)/sizeof(char);

    for(size_t ind = 1; ind < (stk->capacity + 2) * size_converter_value ; ind++)
    {
        data_key = data_key << 3 + 1;
        data_key = data_key ^ stack_data[ind] + data_key % 3 + stack_data[ind] + stack_data[ind - 1] | stack_data[ind];
    }

    size_t secureID[3 + sizeof(struct Stack)] = {NULL_HASH, records_key, data_key};

    size_t ind = 0;

    for(ind = 3; ind < sizeof(struct Stack) / 2; ind++)
    {
        secureID[ind] = secureID[ind - 1] + secureID[ind - 1] % 5 + 2 * secureID[ind - 2] + secureID[ind - 3] % 13 ;
    }

    stk->hash = secureID[ind - 1];
} //*/
