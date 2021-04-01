#include <stdint.h>
#include "vram.h"
#include "smd/megadrive.h"
#include "../res/res.h"

int main()
{
    uint8_t color = 1;


    smd_ints_enable();
    
    tiles_load_fast(res_font_sys, VRAM_INDEX_FONT, RES_FONT_SYS_SIZE);
    tiles_load_fast(res_img_skel_tileset, 1, RES_IMG_SKEL_TILESET_SIZE);

    pal_primary_set(PAL_0, RES_PAL_PLAYER_SIZE, res_pal_player);
    pal_primary_set(PAL_1, RES_PAL_COLLECTIBLES_SIZE, res_pal_collectibles);            
    pal_primary_set(PAL_2, RES_PAL_ENEMY00_SIZE, res_pal_enemy00);
    pal_primary_set(PAL_3, RES_PAL_ENEMY01_SIZE, res_pal_enemy01);   
    /* rect fill with tile */
    //plane_rect_fill(PLANE_A, VRAM_INDEX_FONT + 1, 0, 0, 8, 8);         
    /* horizontal line draw */
    //plane_hline_draw(PLANE_A, res_img_skel, 9, 0, RES_IMG_SKEL_WIDTH, false);
    /* vertical line draw */
    //plane_vline_draw(PLANE_A, res_img_skel, 18, 0, RES_IMG_SKEL_WIDTH, false);
    /* rect draw */   
    //plane_rect_draw(PLANE_A, res_img_skel, 20, 0, RES_IMG_SKEL_WIDTH, RES_IMG_SKEL_HEIGHT, false);
    /* rect draw using queue */
    //plane_rect_draw(PLANE_A, res_img_skel, 20, 9, RES_IMG_SKEL_WIDTH, RES_IMG_SKEL_HEIGHT, true);
    /* rect draw fast */
    //plane_rect_draw_fast(PLANE_A, res_img_skel, 20, 18, RES_IMG_SKEL_WIDTH, RES_IMG_SKEL_HEIGHT);

    plane_tile_draw(PLANE_A, VRAM_INDEX_FONT + 1, 0, 0);   

    plane_tile_draw(PLANE_A, VRAM_INDEX_FONT + 1, 1, 1);
       
    plane_tile_draw(PLANE_A, plane_cell_config(VRAM_INDEX_FONT + 1, 1, 0, 1, 1), 39, 20);


    vid_display_enable();
    while (1)
    {
        pad_update();

        /* Check press button  */
        if (pad_btn_pressed(PAD_1, PAD_BTN_A))
        {
            //vid_display_disable();
            pal_primary_set(PAL_0, RES_PAL_PLAYER_SIZE, res_pal_player);
            pal_primary_set(PAL_1, RES_PAL_COLLECTIBLES_SIZE, res_pal_collectibles);            
            pal_primary_set(PAL_2, RES_PAL_ENEMY00_SIZE, res_pal_enemy00);
            pal_primary_set(PAL_3, RES_PAL_ENEMY01_SIZE, res_pal_enemy01);   
            pal_alternate_set(PAL_0, RES_PAL_COLLECTIBLES2_SIZE, res_pal_collectibles2);
            pal_alternate_set(PAL_1, RES_PAL_COLLECTIBLES2_SIZE, res_pal_collectibles2);          
            pal_alternate_set(PAL_2, RES_PAL_COLLECTIBLES2_SIZE, res_pal_collectibles2);
            pal_alternate_set(PAL_3, RES_PAL_COLLECTIBLES2_SIZE, res_pal_collectibles2);
        }
        /* Check press button  */        
        if (pad_btn_pressed(PAD_1, PAD_BTN_B))
        {
            plane_clear(PLANE_A);
            plane_clear(PLANE_B);
        }
        if (pad_btn_pressed(PAD_1, PAD_BTN_C))
        {
            vid_background_color_set(color);
            ++color;
            //if (!pal_is_fading())
            {
                pal_fade(3);
                pal_fade_wait();
            pal_primary_set(PAL_0, RES_PAL_PLAYER_SIZE, res_pal_player);
            pal_primary_set(PAL_1, RES_PAL_COLLECTIBLES_SIZE, res_pal_collectibles);            
            pal_primary_set(PAL_2, RES_PAL_ENEMY00_SIZE, res_pal_enemy00);
            pal_primary_set(PAL_3, RES_PAL_ENEMY01_SIZE, res_pal_enemy01);   
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

#if 0
int main()
{
    smd_ints_enable();
    //dma_vram_fill(VID_PLANE_A_ADDR, VID_PLANE_TILES <<, 0xFF, 2);
    plane_clear(PLANE_A);
    plane_clear(PLANE_B);
    while (1)
    {
 
        vid_vsync_wait();
    }
}
#endif