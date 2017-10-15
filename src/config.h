#ifndef GUARD_IMPSBRU_CONFIG_H
#define GUARD_IMPSBRU_CONFIG_H

#define CROSS_SIDE_A 0
#define CROSS_SIDE_B 1
#define CROSS_SIDE_C 2
#define CROSS_SIDE_D 3
#define CROSS_SIDE_E 4
#define CROSS_SIDE_F 5
#define CROSS_SIDE_COUNT 6
#define CROSS_SIDE_OPPOSITE_SHIFT 3

#define CROSS_SIDE_A_BIT (1 << CROSS_SIDE_A)
#define CROSS_SIDE_B_BIT (1 << CROSS_SIDE_B)
#define CROSS_SIDE_C_BIT (1 << CROSS_SIDE_C)
#define CROSS_SIDE_D_BIT (1 << CROSS_SIDE_D)
#define CROSS_SIDE_E_BIT (1 << CROSS_SIDE_E)
#define CROSS_SIDE_F_BIT (1 << CROSS_SIDE_F)

#define CROSS_SIDE_A_FINAL 6
#define CROSS_SIDE_C_FINAL 7
#define CROSS_SIDE_E_FINAL 8
#define CROSS_CENTER 9
#define CROSS_CURSOR 10
#define CROSS_CLEANUP 11

#define CROSS_SIDE_DESTINATION_OFFSET 12
/* CROSS_SIDE_DESTINATION FROM 13 TO 17 */
#define CROSS_ATLAS_SIZE 18

#define CROSS_CURSOR_TOP 26
#define CROSS_CURSOR_RIGHT 14
#define CROSS_CURSOR_BOTTOM 29
#define CROSS_CURSOR_LEFT 12

#define CROSS_BYTE_SIZE sizeof(UBYTE)
#define CROSS_WIDTH 64
#define CROSS_HEIGHT 64
#define CROSS_X_SHIFT 50
#define CROSS_Y_SHIFT 50
#define CROSS_ODD_Y_SHIFT 25

#define CUBE_ATLAS_SIZE 1

#define CUBE_WIDTH 18
#define CUBE_HEIGHT 18
#define CUBE_BITMAP_WIDTH 32
#define CUBE_BITMAP_HEIGHT 32

#define CUBE_ADJUST_COUNT 1

#define MAP_WIDTH 6
#define MAP_HEIGHT 4
#define MAP_Y_OFFSET 17

#endif