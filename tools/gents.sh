#!/bin/bash
# Generate titlescreen files
rm res/titlescreen/titlescreen_*

img2gb tileset \
    --name=titlescreen_tileset \
    --output-c-file=res/titlescreen/titlescreen_tileset.c \
    --output-header-file=res/titlescreen/titlescreen_tileset.h \
    --output-image=res/titlescreen/titlescreen_tileset.png \
    --deduplicate \
    res/titlescreen/titlescreen.png

img2gb tilemap \
    --name=titlescreen_tilemap \
    --output-c-file=res/titlescreen/titlescreen_tilemap.c \
    --output-header-file=res/titlescreen/titlescreen_tilemap.h \
    res/titlescreen/titlescreen_tileset.png \
    res/titlescreen/titlescreen.png
