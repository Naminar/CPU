
#include "../include/video.h"

void show_command(size_t video_ip, int* VIDEO)
{
    txCreateWindow (xSize, ySize);

    --video_ip;

    for (int ind = 0; ind <= (int) video_ip; ind += 5)
    {
        if (!is_broadcast_normal(VIDEO[ind], VIDEO[ind + 1], VIDEO[ind + 2],
                                 VIDEO[ind + 3], VIDEO[ind + 4])
           )
        {
            VIDEO[ind]     = 0;
            VIDEO[ind + 1] = 0;
            VIDEO[ind + 2] = 0;
            VIDEO[ind + 3] = 0;
            VIDEO[ind + 4] = 0;
        }


        txSetFillColor (RGB (VIDEO[ind + 2], VIDEO[ind + 3], VIDEO[ind + 4]));

        txSetColor (RGB (VIDEO[ind + 2], VIDEO[ind + 3], VIDEO[ind + 4]));

        if (ind < 110)
            txSleep (ind / 11);
        else
            txSleep (ind % 8);

        //                 x0      y0      x1       y1
        txRectangle (bitMultiply * VIDEO[ind] + xRemoval,
                     bitMultiply * VIDEO[ind + 1] + yRemoval,
                     bitMultiply * (VIDEO[ind] + 1) + xRemoval,
                     bitMultiply * (VIDEO[ind + 1] + 1) + yRemoval);
    }

    //txDestroyWindow();
}

static size_t is_broadcast_normal(int arg1, int arg2, int arg3, int arg4, int arg5)
{
    if (arg1 >= 0 && arg2 >= 0 && arg3 >= 0 && arg4 >= 0 && arg5 >= 0
        &&
        arg1 < xSize - xRemoval && arg2 < ySize - yRemoval
        &&
        (arg3 + arg4 + arg5) < 3 * maxColor)
    {
        return 1;
    }

    return 0;
}


/*#include <TXLib.h>

const int yRemoval    =  10;

const int xRemoval    =  10;

const size_t xSize    = 640;

const size_t ySize    = 320;

const int bitMultiply =  10;

const int maxColor    = 256;


static size_t is_broadcast_normal(int arg1, int arg2, int arg3, int arg4, int arg5);

void show_command(size_t video_ip, int* VIDEO)
{
    txCreateWindow (xSize, ySize);

    --video_ip;

    for (int ind = 0; ind <= (int) video_ip; ind += 5)
    {
        if (!is_broadcast_normal(VIDEO[ind], VIDEO[ind + 1], VIDEO[ind + 2],
                                 VIDEO[ind + 3], VIDEO[ind + 4])
           )
        {
            VIDEO[ind]     = 0;
            VIDEO[ind + 1] = 0;
            VIDEO[ind + 2] = 0;
            VIDEO[ind + 3] = 0;
            VIDEO[ind + 4] = 0;
        }


        txSetFillColor (RGB (VIDEO[ind + 2], VIDEO[ind + 3], VIDEO[ind + 4]));

        txSetColor (RGB (VIDEO[ind + 2], VIDEO[ind + 3], VIDEO[ind + 4]));

        if (ind < 110)
            txSleep (ind / 11);
        else
            txSleep (ind % 8);

        //            x0                     y0                    x1                      y1
        txRectangle (bitMultiply * VIDEO[ind] + xRemoval, bitMultiply * VIDEO[ind + 1] + yRemoval,
                     bitMultiply * (VIDEO[ind] + 1) + xRemoval ,bitMultiply * (VIDEO[ind + 1] + 1) + yRemoval);
    }

    //txDestroyWindow();
}

static size_t is_broadcast_normal(int arg1, int arg2, int arg3, int arg4, int arg5)
{
    if (arg1 >= 0 && arg2 >= 0 && arg3 >= 0 && arg4 >= 0 && arg5 >= 0
        &&
        arg1 < xSize - xRemoval && arg2 < ySize - yRemoval
        &&
        (arg3 + arg4 + arg5) < 3 * maxColor)
        return 1;

    return 0;
} */
