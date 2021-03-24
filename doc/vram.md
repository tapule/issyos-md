# VRam layout
##  Index   (start..end)   - tile count
##  0x000 (0x0000..0xBFFF) - 1536 tiles

##  0xC000..0xCFFF - Plane A (64x32, 4096 Bytes)

##  0xD000..0xD000 - Plane W (0x0)

##  0x680 (0xD000..0xDFFF) - 128 tiles

##  0xE000..0xEFFF - Plane B (64x32, 4096 Bytes)

##  0x780 (0xF000..0xF7FF) - 64 Tiles - VRAM_INDEX_FONT
Tiles with ASCII glyphs between 32-95 (64 glyphs) 

##  0xF800..0xFBBF - HScroll table (960 Bytes)

##  0x7DE (0xFBC0..0xFBFF) - 2 tiles - TILE_INDEX_
- 1 tile Helmet menu marker
- 1 tile dialog box continue reading marker
* También podría poner el sprite del arrow 

##  0xFC00..0xFE7F - Sprite table (640 Bytes)

##  0x7F4 (0xFE80..0xFFFF) - 12 tiles
- 1 tile OSD hart icon
- 1 tile OSD key icon
- 1 tile OSD coin icon
- 1 tile OSD arrows icon
- 1 tile OSD health full
- 1 tile OSD health half
- 1 tile OSD health empty
* 5 tiles left 

 *  -> 1742 free tiles
 */