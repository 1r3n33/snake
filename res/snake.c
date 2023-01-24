/*

 SNAKE.C

 Tile Source File.

 Info:
  Form                 : All tiles as one unit.
  Format               : Gameboy 4 color.
  Compression          : None.
  Counter              : None.
  Tile size            : 8 x 8
  Tiles                : 0 to 64

  Palette colors       : None.
  SGB Palette          : None.
  CGB Palette          : None.

  Convert to metatiles : No.

 This file was generated by GBTD v2.2

*/

/* Start of tile array. */
const unsigned char gfx_snake[] =
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x07,0x07,0x18,0x18,0x20,0x20,0x40,0x40,
  0x40,0x40,0x80,0x80,0x81,0x81,0x83,0x83,
  0x83,0x83,0x81,0x81,0x80,0x80,0x40,0x40,
  0x40,0x40,0x20,0x20,0x18,0x18,0x07,0x07,
  0xE0,0xE0,0x18,0x18,0x04,0x04,0x02,0x02,
  0x02,0x02,0x01,0x01,0x81,0x81,0xC1,0xC1,
  0xC1,0xC1,0x81,0x81,0x01,0x01,0x02,0x02,
  0x02,0x02,0x04,0x04,0x18,0x18,0xE0,0xE0,
  0x07,0x07,0x18,0x1F,0x20,0x3F,0x40,0x7F,
  0x40,0x7F,0x80,0xFF,0x80,0xFF,0x80,0xFF,
  0x80,0xFF,0x80,0xFF,0x80,0xFF,0x80,0xFF,
  0x80,0xFF,0x80,0xFF,0x80,0xFF,0x80,0xFF,
  0xE0,0xE0,0x18,0xF8,0x04,0xFC,0x02,0xFE,
  0x02,0xFE,0x01,0xFF,0x01,0xFF,0x01,0xFF,
  0x01,0xFF,0x01,0xFF,0x01,0xFF,0x01,0xFF,
  0x01,0xFF,0x01,0xFF,0x01,0xFF,0x01,0xFF,
  0xFF,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0xFF,0xFF,
  0xE0,0xE0,0x18,0xF8,0x04,0xFC,0x02,0xFE,
  0x02,0xFE,0x01,0xFF,0x01,0xFF,0x01,0xFF,
  0x01,0xFF,0x01,0xFF,0x01,0xFF,0x02,0xFE,
  0x02,0xFE,0x04,0xFC,0x18,0xF8,0xE0,0xE0,
  0x80,0xFF,0x80,0xFF,0x80,0xFF,0x80,0xFF,
  0x80,0xFF,0x80,0xFF,0x80,0xFF,0x80,0xFF,
  0x80,0xFF,0x80,0xFF,0x80,0xFF,0x40,0x7F,
  0x40,0x7F,0x20,0x3F,0x18,0x1F,0x07,0x07,
  0x01,0xFF,0x01,0xFF,0x01,0xFF,0x01,0xFF,
  0x01,0xFF,0x01,0xFF,0x01,0xFF,0x01,0xFF,
  0x01,0xFF,0x01,0xFF,0x01,0xFF,0x02,0xFE,
  0x02,0xFE,0x04,0xFC,0x18,0xF8,0xE0,0xE0,
  0x07,0x07,0x18,0x1F,0x20,0x3F,0x40,0x7F,
  0x40,0x7F,0x80,0xFF,0x80,0xFF,0x80,0xFF,
  0x80,0xFF,0x80,0xFF,0x80,0xFF,0x40,0x7F,
  0x40,0x7F,0x20,0x3F,0x18,0x1F,0x07,0x07,
  0xFF,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0xFF,0xFF,
  0x80,0xFF,0x80,0xFF,0x80,0xFF,0x80,0xFF,
  0x80,0xFF,0x80,0xFF,0x80,0xFF,0x80,0xFF,
  0x80,0xFF,0x80,0xFF,0x80,0xFF,0x80,0xFF,
  0x80,0xFF,0x80,0xFF,0x80,0xFF,0x80,0xFF,
  0x01,0xFF,0x01,0xFF,0x01,0xFF,0x01,0xFF,
  0x01,0xFF,0x01,0xFF,0x01,0xFF,0x01,0xFF,
  0x01,0xFF,0x01,0xFF,0x01,0xFF,0x01,0xFF,
  0x01,0xFF,0x01,0xFF,0x01,0xFF,0x01,0xFF,
  0xFF,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0xFF,0xFF,
  0xFF,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0xFF,0xFF,
  0x00,0x00,0x00,0x00,0x03,0x03,0x04,0x07,
  0x08,0x0F,0x10,0x1F,0x20,0x3F,0x20,0x3F,
  0x40,0x7F,0x40,0x7F,0x80,0xFF,0x80,0xFF,
  0x80,0xFF,0x80,0xFF,0x80,0xFF,0x80,0xFF,
  0x3F,0x3F,0xC0,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0xFC,0xFC,0x03,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0x00,0x00,0x00,0xC0,0xC0,0x20,0xE0,
  0x10,0xF0,0x08,0xF8,0x04,0xFC,0x04,0xFC,
  0x02,0xFE,0x02,0xFE,0x01,0xFF,0x01,0xFF,
  0x01,0xFF,0x01,0xFF,0x01,0xFF,0x01,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x03,0xFF,0xFC,0xFC,
  0x01,0xFF,0x01,0xFF,0x01,0xFF,0x01,0xFF,
  0x01,0xFF,0x01,0xFF,0x02,0xFE,0x02,0xFE,
  0x04,0xFC,0x04,0xFC,0x08,0xF8,0x10,0xF0,
  0x20,0xE0,0xC0,0xC0,0x00,0x00,0x00,0x00,
  0x80,0xFF,0x80,0xFF,0x80,0xFF,0x80,0xFF,
  0x80,0xFF,0x80,0xFF,0x40,0x7F,0x40,0x7F,
  0x20,0x3F,0x20,0x3F,0x10,0x1F,0x08,0x0F,
  0x04,0x07,0x03,0x03,0x00,0x00,0x00,0x00,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0xC0,0xFF,0x3F,0x3F,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

/* End of SNAKE.C */
