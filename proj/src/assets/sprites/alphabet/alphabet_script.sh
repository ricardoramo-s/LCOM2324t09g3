#!/bin/bash

# ImageMagick

FONT="Retro-Gaming"         # Font, might be changed
FONT_SIZE=64
TARGET_WIDTH=44             # Target width might need to change to accommodate font changes

for CHAR in {A..Z} 0 1 2 3 4 5 6 7 8 9 . , : / - \|; do
    ASCII=$(printf '%s' "$CHAR" | od -An -tuC)
    convert -font "$FONT" -pointsize "$FONT_SIZE" -colors 2 -gravity center -extent "${TARGET_WIDTH}x" caption:$CHAR "${ASCII}.png"
    echo "Generated $CHAR PNG!"
done

convert *.png -append -colors 2 alphabet_sprites.xpm
echo "Combined into XPM!"

sed -i '1s/^/#pragma once\n\n/' alphabet_sprites.xpm
sed -i "4s/.*/static xpm_row_t const alphabet_sprites[\] = \{/" alphabet_sprites.xpm
sed -i '6s/ 1 //g' alphabet_sprites.xpm
sed -i '7s/.*/\"  0\",/g' alphabet_sprites.xpm
sed -i '8s/.*/\". 63\",/g' alphabet_sprites.xpm
mv alphabet_sprites.xpm alphabet_sprites.h
echo "Performed C conversions!"

sed -i 's/ \.\ /   /g' alphabet_sprites.h  # EXPERIMENTAL: Try to remove random artifacts from XPM (origin unknown)
echo "Removed XPM artifacts"

rm *.png 

echo "Sprite sheet generated: alphabet_sprites.h"
