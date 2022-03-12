
#ifndef CPU_

#define CPU_

#include <math.h>
#include "../include/global.h"
#include "../include/stack.h"

#define IP_UP(arg)      ip += (arg);

//#define IP_DOWN(arg)    *ip -= (arg);

#define CELL_VALUE(type, code)  *(type*) (code + ip)

//===============================================

typedef struct Cpu{

  DATA* RAM  ;

  DATA* REG  ;

  int* VIDEO ;

  Stack STK  ;

  //not in RAM memory:

  unsigned char* code ;

} Cpu;

//===============================================

const size_t RAM_SIZE    = 20;

const size_t REG_SIZE    = 5;// abcde

const size_t VIDEO_SIZE  = 160 * 5;

//===============================================

void cpu_work(void)                                              ;
void show_command(size_t video_ip, int* VIDEO)              ;

static void cpu_destructor      (Cpu*)                      ;
static void cpu_init            (Cpu*)                      ;
static unsigned char* code_init (void)                      ;
static void cpu_dump            (Cpu*, Stack*, int,
                                 const char*, const char*)  ;

//===============================================

#endif
