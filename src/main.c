#include <stdint.h>
#include "smd/megadrive.h"
#include "../res/res.h"

static const uint16_t data[10] = {0x1111, 0x2222, 0x3333, 0x4444, 0x5555,
                                  0x6666, 0x7777, 0x8888, 0x9999, 0xAAAA};


int main()
{
    uint8_t color = 1;


    smd_ints_enable();
    pal_primary_set(PAL_0, 16, font00_pal);
    pal_primary_set(PAL_1, 16, font00_pal);            
    pal_primary_set(PAL_2, 16, font00_pal);
    pal_primary_set(PAL_3, 16, font00_pal);            
    vid_display_enable();
    while (1)
    {
        pad_update();

        /* Check press button  */
        if (pad_btn_pressed(PAD_1, PAD_BTN_A))
        {
            //vid_display_disable();
            vid_cram_clear();
            pal_primary_set(PAL_0, 16, font00_pal);
            pal_primary_set(PAL_1, 16, font00_pal);            
            pal_primary_set(PAL_2, 16, font00_pal);
            pal_primary_set(PAL_3, 16, font00_pal);
            pal_alternate_set(PAL_0, 16, bgB_pal);
            pal_alternate_set(PAL_1, 16, bgB_pal);            
            pal_alternate_set(PAL_2, 16, bgB_pal);
            pal_alternate_set(PAL_3, 16, bgB_pal);
        }
        /* Check release button  */        
        if (pad_btn_pressed(PAD_1, PAD_BTN_B))
        {
            if (!pal_is_fading())
            {
                pal_fade(2);
            }
            pal_fade_step();
            //vid_display_enable();
        }
        if (pad_btn_pressed(PAD_1, PAD_BTN_C))
        {
            vid_background_color_set(color);
            ++color;
            //if (!pal_is_fading())
            {
                pal_fade(3);
                pal_fade_wait();
            pal_primary_set(PAL_0, 16, font00_pal);
            pal_primary_set(PAL_1, 16, font00_pal);            
            pal_primary_set(PAL_2, 16, font00_pal);
            pal_primary_set(PAL_3, 16, font00_pal);  
            pal_swap();                
            }
                        
        }

        //dma_queue_vram_transfer(font00_tiles, 100 * 16, 96 * 8 * 2, 2);
        //dma_queue_cram_transfer(font00_pal, 16, 16, 2);
        //dma_queue_vsram_transfer(data, 0, 2, 2);

        vid_vsync_wait();
        pal_update();
        dma_queue_flush();

    }
}
