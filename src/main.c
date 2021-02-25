#include <stdint.h>
#include "smd/megadrive.h"
#include "../res/res.h"

static const uint16_t data[10] = {0x1111, 0x2222, 0x3333, 0x4444, 0x5555,
                                  0x6666, 0x7777, 0x8888, 0x9999, 0xAAAA};


int main()
{
    uint8_t color = 1;


    smd_ints_enable();
    dma_vram_transfer(font00_tiles, 0, 96 * 8 * 2, 2);
    dma_cram_transfer(font00_pal, 0, 16*2, 2);
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
        if (pad_btn_pressed(PAD_1, PAD_BTN_B))
        {
            vid_display_enable();
        }
        if (pad_btn_pressed(PAD_1, PAD_BTN_C))
        {
            vid_background_color_set(color);
            dma_queue_vram_transfer(bgB_tiles, 0, 352 * 8 * 4, 2);
            dma_queue_cram_transfer(bgB_pal, 32, 16*2, 2);
            ++color;
        }

        //dma_queue_vram_transfer(font00_tiles, 100 * 16, 96 * 8 * 2, 2);
        //dma_queue_cram_transfer(font00_pal, 16, 16, 2);
        //dma_queue_vsram_transfer(data, 0, 2, 2);

        vid_vsync_wait();
        dma_queue_flush();
    }
}
