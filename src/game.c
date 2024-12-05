/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            game.c
 * \brief           Main game object
 */

#include "game.h"
#include "smd.h"
#include "vram.h"
#include "../build/assets/res.h"

void game_load_resources(void)
{
    /* CHECKME: Resource loading must be here? */
    smd_kdebug_alert("Loading resources...");

    /* Palettes */
    smd_pal_primary_set(SMD_PAL_0_INDEX, RES_PAL_PLAYER_SIZE, res_pal_player);
    smd_pal_primary_set(SMD_PAL_1_INDEX, RES_PAL_COLLECTIBLES_SIZE, res_pal_collectibles);

    /* System font */
    smd_tile_load(smd_dma_transfer_fast, res_font_sys, VRAM_INDEX_FONT, RES_FONT_SYS_SIZE);
    smd_text_font_set(VRAM_INDEX_FONT);
    smd_text_pal_set(SMD_PAL_0);

    /* Other resource load examples */
    smd_z80_bus_request();
        smd_xgm_sfx_set(64, res_sfx_door, RES_SFX_DOOR_SIZE);
        smd_xgm_sfx_set(65, res_sfx_spit, RES_SFX_SPIT_SIZE);
        smd_xgm_sfx_set(66, sfx_snd_death14, SFX_SND_DEATH14_SIZE);
    smd_z80_bus_release();
    smd_xgm_music_play(mus_credits);
}

void game_init(void)
{
    smd_sys_ints_disable();
    game_load_resources();
    smd_vdp_display_enable();
    smd_sys_ints_enable();
}

void game_run(void)
{
    uint16_t text[128];
    uint16_t size;
    uint16_t status = 0;
    uint16_t song = 0;
    uint16_t sfx = 0;
//    uint16_t i;

    while (1)
    {
        /* Game body background color */
        smd_vdp_background_color_set(5);

        smd_pad_update();

        /* Check press button  */
        if (smd_pad_btn_pressed(SMD_PAD_1, SMD_PAD_BTN_A))
        {
            switch (sfx)
            {
            case 0:
                ++sfx;
                smd_kdebug_alert("Boton A: Start sound 0");
                size = smd_text_write("SOUND 0 PLAYING", text);
                smd_xgm_sfx_play_auto(64, 15);
                break;
            case 1:
                ++sfx;
                smd_kdebug_alert("Boton A: Start sound 1");
                size = smd_text_write("SOUND 1 PLAYING", text);
                smd_xgm_sfx_play_auto(65, 15);
                break;
            case 2:
                sfx = 0;
                smd_kdebug_alert("Boton A: Start sound 0");
                size = smd_text_write("SOUND 2 PLAYING", text);
                smd_xgm_sfx_play_auto(66, 15);
                break;
            }
            smd_plane_row_draw(smd_dma_transfer, &(smd_plane_draw_desc_t) {
                .plane = SMD_PLANE_A,
                .cells = text,
                .x = 2,
                .y = 4,
                .length = size
            });

            smd_plane_cell_draw(&(smd_plane_draw_desc_t) {
                .plane = SMD_PLANE_B,
                .cell = *text,
                .x = 0,
                .y = 0
            });

            smd_plane_column_draw(smd_dma_transfer, &(smd_plane_draw_desc_t) {
                .plane = SMD_PLANE_A,
                .cells = text,
                .x = 1,
                .y = 8,
                .length = size
            });

           smd_plane_rect_draw(smd_dma_transfer, &(smd_plane_draw_desc_t) {
                .plane = SMD_PLANE_A,
                .cells = text,
                .x = 3,
                .y = 8,
                .width = 4,
                .height = 4
            });

           smd_plane_rect_fill(&(smd_plane_draw_desc_t) {
                .plane = SMD_PLANE_A,
                .cell = *text,
                .x = 9,
                .y = 8,
                .width = 5,
                .height = 5
            });



        }
        /* Check press button  */
        if (smd_pad_btn_pressed(SMD_PAD_1, SMD_PAD_BTN_B))
        {
            smd_kdebug_alert("Boton B: Pause/Resume/Switch song");
            switch (status)
            {
            // Pause
            case 0:
                size = smd_text_write("MUSIC PAUSE   ", text);
                smd_xgm_music_pause();
                status = 1;
                break;
            // Resume
            case 1:
                size = smd_text_write("MUSIC RESUME   ", text);
                smd_xgm_music_resume();
                status = 0;
                break;
            // Start new song
            case 4:
                switch (song)
                {
                case 0:
                    size = smd_text_write("MUSIC SONG 0", text);
                    smd_xgm_music_play(mus_credits);
                    break;
                case 1:
                    size = smd_text_write("MUSIC SONG 1", text);
                    smd_xgm_music_play(mus_demo);
                    break;
                case 2:
                    size = smd_text_write("MUSIC SONG 2", text);
                    smd_xgm_music_play(mus_caves);
                    break;
                }
                status = 0;
                break;
            }
            smd_plane_row_draw(smd_dma_transfer, &(smd_plane_draw_desc_t) {
                .plane = SMD_PLANE_A,
                .cells = text,
                .x = 2,
                .y = 6,
                .length = size
            });
        }
        if (smd_pad_btn_pressed(SMD_PAD_1, SMD_PAD_BTN_C))
        {
            smd_xgm_music_stop();
            status = 4;
            ++song;
            if (song == 3)
            {
                song = 0;
            }
            size = smd_text_write("MUSIC STOP  ", text);
            smd_plane_row_draw(smd_dma_transfer, &(smd_plane_draw_desc_t) {
                .plane = SMD_PLANE_A,
                .cells = text,
                .x = 2,
                .y = 6,
                .length = size
            });
            smd_kdebug_warning_if(smd_pad_type(SMD_PAD_1) == SMD_PAD_TYPE_3BTN, "PAD 1 3botones");
            smd_kdebug_warning_if(!smd_pad_is_plugged(SMD_PAD_2), "PAD 2 DESCONECTADO");

        }

        /* Main game body goes here */
        //smd_dma_queue_vram_transfer(font00_tiles, 100 * 16, 96 * 8 * 2, 2);
        //smd_dma_queue_cram_transfer(font00_pal, 16, 16, 2);
        //smd_dma_queue_vsram_transfer(data, 0, 2, 2);

        /* Wait vsync background color */
        smd_vdp_background_color_set(6);
        //wait(10);
        smd_vdp_vsync_wait();
        // smd_xgm_update(); // Ojo, hecho automáticamente en el vint
        smd_pal_update();
        smd_spr_update();

        /* Vertical blank background color */
        smd_vdp_background_color_set(2);
        smd_dma_queue_flush();
    }
}