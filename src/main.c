#include <stdint.h>
#include "smd/rand.h"

volatile uint16_t s1;
volatile uint16_t s2;
volatile uint16_t s3;

int main()
{
    rnd_init();
    s1 = rnd_seed_get();
    while (1)
    {
        uint16_t i;

        for (i = 0; i< 1000; ++i)
        {
            __asm__ volatile ("\tnop\n");
            __asm__ volatile ("\tnop\n");
            __asm__ volatile ("\tnop\n");
            __asm__ volatile ("\tnop\n");
        }
        s1 = rnd_get();
    }
}
