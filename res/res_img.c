#include "res_img.h"

const uint16_t res_img_skel[RES_IMG_SKEL_WIDTH * RES_IMG_SKEL_HEIGHT] = {
    0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 
    0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F, 
    0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 
    0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F, 
    0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 
    0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F, 
    0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 
    0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F, 0x0040
};
const uint32_t res_img_skel_tileset[RES_IMG_SKEL_TILESET_SIZE * 8] = {
    0x44444444, 0x44444444, 0x44444444, 0x44444444, 0x44444444, 0x44444444, 0x44444444, 0x44444444,
    0x44444444, 0x44444444, 0x44444445, 0x44444555, 0x44445555, 0x4455555B, 0x455555BB, 0x555555B3,
    0x444BB3FF, 0x45BB3FFF, 0x5BB3FFFF, 0xBB3FFFFF, 0xB3FFFFFF, 0xB3FFFFFF, 0x3FFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFEE, 0xFFFFFEEE, 0xFFFFEEEE, 0xFFEEEEEE,
    0xFFFFFEEE, 0xFFFEEEEE, 0xFEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE,
    0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE,
    0xEEE33AAA, 0xEEECC33A, 0xEEECCC73, 0xEEECCC77, 0xEECCCCC7, 0xEECCCCCC, 0xEECCCCCC, 0xECCCCCCC,
    0x44444444, 0xA4444444, 0xAA444444, 0x3AA44444, 0x3AA44444, 0x73BB4444, 0x73BB4444, 0x773B4444,
    0x44444445, 0x44444455, 0x44444555, 0x55555666, 0x55556666, 0x55566666, 0x55566666, 0x55666666,
    0x55555BB3, 0x55555B3F, 0x55555B3F, 0x6666BB3F, 0x6666BB3F, 0x6668BB3F, 0x668BB3FF, 0x66BB3EEF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE, 0xFFFFFFFE, 0xFFFFFFFE, 0xFFFFFFEE,
    0xFFEEEEEE, 0xFEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE,
    0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEFFF, 0xEEEFFFFE, 0xEFFFFFEE, 0xFFFFFEEE,
    0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEC, 0xEEEEEECC, 0xEEEEECCC, 0xEEECCCCC, 0xEECCCCCC,
    0xECCCCCCC, 0xECCCCCCC, 0xCCCCCCCC, 0xCCCCCCCC, 0xCCCCCCC7, 0xCCCCCCC7, 0xCCCCCCC7, 0xCCCCCC77,
    0x773B4444, 0x773BB444, 0x773BB444, 0x773AA555, 0x773BA555, 0x773BB555, 0x773BB555, 0x773BB655,
    0x55666666, 0x56666666, 0x56666666, 0x66666668, 0x66666668, 0x66666688, 0x66666688, 0x88888888,
    0x68B3EEEE, 0x88D3CCEE, 0x8DD3CCCE, 0x88DD377C, 0x88DD0377, 0x88DD0033, 0x88DD0012, 0x888D0101,
    0xFFFFFFEE, 0xEEFFFFFE, 0xEEEFFFFE, 0xCEEEEFFF, 0xCCEEEFFF, 0x7CCEEEFF, 0x37CCEEFE, 0x23CCCEEE,
    0xEEEEEEEF, 0xEEEEEEFF, 0xEEEEEFFF, 0xEEECEFFF, 0xEEECFFFE, 0xECCEFFEE, 0xECCEFEEE, 0xECCEEEEC,
    0xFFFFEEEC, 0xFFEEECCC, 0xFEECCCC7, 0xEECCCC77, 0xECCC7773, 0xCCC77330, 0xCC773100, 0xC7731000,
    0xCCCCC3CC, 0xCCC773CC, 0x77773CCC, 0x7333CCCC, 0x3000CCC7, 0x0010CCC7, 0x0110CCC7, 0x1120CC77,
    0xCCCCC777, 0xCCCC7777, 0xC7777777, 0x77377777, 0x73777777, 0x73777777, 0x37777777, 0x37777773,
    0x773BB655, 0x773BB665, 0x773B6665, 0x773B6666, 0x73BB6666, 0x3DBB6666, 0x3DB66666, 0xDDD88888,
    0x88888888, 0x88888888, 0x88888888, 0x88888888, 0x88888888, 0x88889999, 0x88889999, 0x88889999,
    0x888DD010, 0x888DD021, 0x888DD022, 0x888DD022, 0x888DD302, 0x999DD3E0, 0x9999D3EE, 0x9999D3EF,
    0x137CCCEE, 0x0037CCEE, 0x1137CCCE, 0x11137CC3, 0x21113CC3, 0x22113CC3, 0x02211337, 0xE0000C7C,
    0xCCCEEECC, 0xCCCEEEC7, 0xCC3EEEC7, 0xCC3EECC7, 0xCC3CEC77, 0x7C73CC73, 0xCCC73773, 0xCCCC733C,
    0x77310011, 0x73100112, 0x73001122, 0x30001122, 0x30011222, 0x00112220, 0x0112200C, 0x00000CCC,
    0x1220CC77, 0x220CCC77, 0x220CCC77, 0x20CCC777, 0x0CCCC777, 0xCCCC7777, 0xCCC77773, 0xCC777773,
    0x3777733D, 0x37733DDD, 0x333DDDD8, 0x3DDDDD88, 0x3333DDDD, 0x3777333D, 0x77777773, 0x77777777,
    0xDD888888, 0x88888888, 0x88888888, 0x88DDBB88, 0xDDDDBBBB, 0xD333333D, 0x3CCC7773, 0x73EECCC7,
    0x88889999, 0x88889999, 0x88889999, 0x88889999, 0x88889999, 0x88888999, 0x88888999, 0x88888999,
    0x9999DD3F, 0x9999DD3F, 0x99999DD3, 0x999999DD, 0x999999DD, 0x9999999D, 0x99999999, 0x99999999,
    0xFEEEEECC, 0xFFEEEEEC, 0xFFFFEEEE, 0x3FFFFFEC, 0xD3EFFFFE, 0xDD3EFFFE, 0xDDD3EEFF, 0x9DDD3EEF,
    0xCCCCCCCC, 0xCC7CEEEC, 0x3C73CEEE, 0xC777CEEE, 0xECCEEEEE, 0xEEEEEEEE, 0xEEEEFFEE, 0xFFEEFFEE,
    0xCCCCCCCC, 0xCCCCCCCC, 0xEEECCC77, 0xEECCC777, 0xECCC7733, 0xCCC773D3, 0xCC773D3C, 0xC773DD3C,
    0xC7777733, 0x777333C3, 0x733CCCC3, 0x3777CC3C, 0x777CC3EE, 0x77CC3E33, 0x3333E3EE, 0xCEEEE3EE,
    0xCCCC7777, 0xCCCCCC33, 0xCCC333EE, 0xCC3CEEEE, 0xCC3ECCEE, 0x33EEE7C3, 0xECEEEC73, 0xEEC3ECC7,
    0x73EEECC0, 0x3E3EE001, 0xC3E00111, 0xEC311100, 0xEC30000C, 0xEC730CCC, 0xEE73C333, 0x3E773DDD,
    0x88888999, 0x999999AA, 0x999999AA, 0x9999999A, 0x9999999A, 0x89999999, 0x9AAAAAAA, 0x99AAAAAA,
    0x99999999, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
    0x99DD3CEE, 0xAADD3CE3, 0xAAAD3CC3, 0xAAADD3C3, 0xAAAADD37, 0xAAAAADD3, 0xAAAAAADD, 0xAAAAAAAA,
    0xEFEEEFEC, 0xEEEE3EEC, 0xEEEC3CC7, 0xCCCC3777, 0x37773777, 0x33333333, 0xDDDDDDDD, 0xDDDDADDD,
    0xC773DD3C, 0x373DD3CE, 0x373DD3EF, 0x33DDD3FF, 0x3DDDD3E0, 0xDDDDD001, 0xDDD00100, 0xD0010000,
    0xEEEEE33E, 0xEEEEE003, 0xFEE00000, 0xE0011000, 0x0100000E, 0x000003CC, 0x000EEE3C, 0x0EEEEEE3,
    0xEECC3EC7, 0xEEEC73C7, 0x33EE7737, 0x0E3CC737, 0xEE3CC733, 0xCCC3C73C, 0xC77733E3, 0x33333EEE,
    0x3CC73DDD, 0x3CC3DDDD, 0x333733DD, 0x37777733, 0x77777777, 0x77777777, 0xCCCCC777, 0x33CCCCC7,
    0x99AAAAAA, 0x999AAAAA, 0x999AAAAA, 0x9999AAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
    0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAD, 0xAAAAADDD, 0xAAADDDD3, 0xAADDDD3F, 0xAADDD3FF, 0xADD333FF,
    0xAADDDDDD, 0xADDD33DD, 0xDDD3FE33, 0x33D3FCC7, 0xEC3FEC73, 0xEC3FEC73, 0xCC3ECC30, 0xC73EC730,
    0xAADDDDD0, 0xDDDDD001, 0x3DD00110, 0x70010000, 0x0100000C, 0x00000CCC, 0x000CCCCC, 0x0C73CCCC,
    0x0110000E, 0x1000073E, 0x000C773E, 0x0CCC773E, 0xCCC77773, 0xCC777777, 0xCC777773, 0xC777773D,
    0xEC3FFEEE, 0xC3FFEEEE, 0xC3FFEEFF, 0xC3FEC3FF, 0x3FFEC3FE, 0x3FFC3FFE, 0x3EEC3FFE, 0x3EC73FEC,
    0xEEEEEEEE, 0xFEEEEEEE, 0xEEEEECCC, 0xEEECCCC7, 0xECCC7733, 0xCC773377, 0xCC737CCC, 0xC733CCEE,
    0xEC33CCCC, 0xEEEC33CC, 0xCCCCCC33, 0x7777CCCC, 0x33337773, 0x7773333D, 0xC7773DDD, 0xECC773DD,
    0xAAAAAAAA, 0xAAAAAAAA, 0xBBBBBBBD, 0xBBBBBBDD, 0xBBBBDDD0, 0xBDDDD001, 0xDDD00110, 0xD0011110,
    0xAD3EC3EE, 0xAD3ECC3E, 0xDD3ECC3E, 0xD03EECC3, 0x0003EEC7, 0x000033EC, 0x000DDD33, 0x0DDDDBBD,
    0xC73EC73C, 0xC73EC73C, 0xC73EC773, 0xC773CC73, 0x3C73CC73, 0x73C7333D, 0x3D33DDDD, 0xDDDDDDBB,
    0xC7737777, 0xC7377777, 0x73777777, 0x33333333, 0xDDDDDDDD, 0xDDDDDDDD, 0xBBBBBBBB, 0xBBBBBBBB,
    0x777773DD, 0x77773DDD, 0x7333DDDB, 0x3DDDDBBB, 0xDDDDBBBB, 0xDBBBBBBB, 0xBBBBBBBB, 0xBBBBBBBB,
    0xD3C73EEC, 0xDD373ECC, 0xBDD33EC7, 0xBDDD3EC7, 0xBBDDD3C7, 0xBBBBDD33, 0xBBBBBDDD, 0xBBBBBBBB,
    0x73DD3EEE, 0x73DDD3FE, 0x73DDD3FF, 0x73DDDD3F, 0x3DDBDD3F, 0xDDDBBDD3, 0xDBBBBDD3, 0xBBBBBDD3,
    0xEECC773D, 0xEECC773D, 0xEEECC73D, 0xFEEEC773, 0xFEEECC73, 0xFEEECC77, 0xFFEEEC77, 0xFFEEECC7
};

const uint16_t res_img_2tile[RES_IMG_2TILE_WIDTH * RES_IMG_2TILE_HEIGHT] = {
    0x0000, 0x0001
};
const uint32_t res_img_2tile_tileset[RES_IMG_2TILE_TILESET_SIZE * 8] = {
    0x33333333, 0x34434433, 0x34444444, 0x33411111, 0x33411111, 0x33411333, 0x33411344, 0x34411344,
    0x55555555, 0x52252255, 0x52222222, 0x55200000, 0x55200000, 0x55200555, 0x55200522, 0x52200522
};

const uint16_t res_img_6tiles[RES_IMG_6TILES_WIDTH * RES_IMG_6TILES_HEIGHT] = {
    0x0000, 0x0001, 0x1001, 0x0801, 0x1801, 0x0000
};
const uint32_t res_img_6tiles_tileset[RES_IMG_6TILES_TILESET_SIZE * 8] = {
    0x88888888, 0x8AA8AA88, 0x8AAAAAAA, 0x88A66666, 0x88A66666, 0x88A66888, 0x88A668AA, 0x8AA668AA,
    0xDDDDDDDD, 0xD77D77DD, 0xD7777777, 0xDD744444, 0xDD744444, 0xDD744DDD, 0xDD744D77, 0xD7744D77
};

const uint16_t res_img_36tiles[RES_IMG_36TILES_WIDTH * RES_IMG_36TILES_HEIGHT] = {
    0x0000, 0x0001, 0x1001, 0x0801, 0x1801, 0x0000, 
    0x0000, 0x0001, 0x1001, 0x0801, 0x1801, 0x0000, 
    0x0000, 0x0001, 0x1001, 0x0801, 0x1801, 0x0000, 
    0x0000, 0x0001, 0x1001, 0x0801, 0x1801, 0x0000, 
    0x0000, 0x0001, 0x1001, 0x0801, 0x1801, 0x0000, 
    0x0000, 0x0001, 0x1001, 0x0801, 0x1801, 0x0000
};
const uint32_t res_img_36tiles_tileset[RES_IMG_36TILES_TILESET_SIZE * 8] = {
    0x88888888, 0x8AA8AA88, 0x8AAAAAAA, 0x88A66666, 0x88A66666, 0x88A66888, 0x88A668AA, 0x8AA668AA,
    0xDDDDDDDD, 0xD77D77DD, 0xD7777777, 0xDD744444, 0xDD744444, 0xDD744DDD, 0xDD744D77, 0xD7744D77
};

const uint16_t res_img_1tile[RES_IMG_1TILE_WIDTH * RES_IMG_1TILE_HEIGHT] = {
    0x0000
};
const uint32_t res_img_1tile_tileset[RES_IMG_1TILE_TILESET_SIZE * 8] = {
    0x33333333, 0x34434433, 0x34444444, 0x33411111, 0x33411111, 0x33411333, 0x33411344, 0x34411344
};
