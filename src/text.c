#include <gb/gb.h>
#include "text.h"

#define TEXT_WIN_WIDTH 20U
#define TEXT_WIN_HEIGHT 5U

#define TEXT_LINE_SPACING 2U
#define TEXT_LINE1_Y_POS 1U
#define TEXT_LINE2_Y_POS (TEXT_LINE1_Y_POS + TEXT_LINE_SPACING)

#define TEXT_PRINT_CHAR_PAUSE 4U // Number of VBL between each char

typedef struct Cursor
{
    uint8_t x;
    uint8_t y;
    const uint8_t *c;

} Cursor;

Cursor cursor;

void text_clear()
{
    for (uint8_t x = 1; x < TEXT_WIN_WIDTH - 1; x++)
    {
        set_win_tile_xy(x, TEXT_LINE1_Y_POS, 0);
        set_win_tile_xy(x, TEXT_LINE2_Y_POS, 0);
    }
}

void text_loop()
{
    while (1)
    {
        // Wait before printing next char
        for (uint8_t i = 0; i < TEXT_PRINT_CHAR_PAUSE; i++)
        {
            wait_vbl_done();
        }

        // If there is a char to print...
        if (*cursor.c)
        {
            // Check if we printed 2 lines.
            // Wait button press to print the rest of the text.
            if (cursor.y > TEXT_LINE2_Y_POS)
            {
                if (joypad())
                {
                    cursor.y = TEXT_LINE1_Y_POS;
                    text_clear();
                }
            }
            // Print next char...
            else
            {
                set_win_tile_xy(cursor.x, cursor.y, *cursor.c);
                cursor.c++;
                cursor.x++;
            }
        }
        // If there is no char to print...
        else
        {
            // Check if we were already at the beginning of the new line.
            // Pair of zero marks end of text. Press button to exit.
            if (cursor.x == 1)
            {
                if (joypad())
                {
                    return;
                }
            }
            // Go to next line...
            else
            {
                cursor.y += TEXT_LINE_SPACING;
                cursor.x = 1;
                cursor.c++;
            }
        }
    }
}

void text_init()
{
    cursor.x = 0;
    cursor.y = 0;
    cursor.c = 0;

    for (uint8_t y = 0; y < TEXT_WIN_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < TEXT_WIN_WIDTH; x++)
        {
            set_win_tile_xy(x, y, 0);
        }
    }

    set_win_tile_xy(0, 0, 124);
    set_win_tile_xy(TEXT_WIN_WIDTH - 1, 0, 125);
    set_win_tile_xy(0, TEXT_WIN_HEIGHT - 1, 126);
    set_win_tile_xy(TEXT_WIN_WIDTH - 1, TEXT_WIN_HEIGHT - 1, 127);
    for (uint8_t i = 1; i < TEXT_WIN_WIDTH - 1; i++)
    {
        set_win_tile_xy(i, 0, 120);
        set_win_tile_xy(i, TEXT_WIN_HEIGHT - 1, 121);
    }
    for (uint8_t i = 1; i < TEXT_WIN_HEIGHT - 1; i++)
    {
        set_win_tile_xy(0, i, 122);
        set_win_tile_xy(TEXT_WIN_WIDTH - 1, i, 123);
    }

    move_win(DEVICE_WINDOW_PX_OFFSET_X, DEVICE_WINDOW_PX_OFFSET_Y + ((DEVICE_SCREEN_HEIGHT - TEXT_WIN_HEIGHT) * 8U));
}

void text_show(const char *text)
{
    cursor.x = 1;
    cursor.y = TEXT_LINE1_Y_POS;
    cursor.c = text;

    text_clear();
    SHOW_WIN;

    text_loop();

    HIDE_WIN;
    wait_vbl_done();
}
