
#ifndef VIDEO_

#define VIDEO_

#include <TXLib.h>

const int yRemoval    =  10;
const int xRemoval    =  10;
const int xSize       = 640;
const int ySize       = 320;
const int bitMultiply =  10;
const int maxColor    = 256;

//===============================================

void show_command                (size_t, int*)             ;
static size_t is_broadcast_normal(int, int, int, int, int)  ;


#endif
