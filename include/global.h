
#ifndef GLOBAL_

#define GLOBAL_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

enum Commands{
    #define DEF_CMD(name, num, ...) \
        CMD_##name = num,

    #define DEF_JMP(name, num, sign)\
        JMP_##name = num,

    #include "../include/commands.h"

    #undef DEF_CMD
    #undef DEF_JMP
};

//===============================================

#define ELEMENT_FMT "%d"

typedef int DATA;

#endif


