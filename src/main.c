#include <stdint.h>
#include "smd/megadrive.h"

int main()
{
    smd_interrupts_enable();
    vid_display_enable();
    while (1)
    {
        pad_update();

        /* Check press button  */
        if (pad_btn_pressed(PAD_1, PAD_BTN_A))
        {
            vid_display_disable();
        }
        /* Check release button  */        
        if (pad_btn_released(PAD_1, PAD_BTN_B))
        {
            vid_display_enable();
        }

        vid_vsync_wait();
    }
}
