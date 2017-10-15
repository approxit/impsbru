#include "cube.h"

#include <ace/managers/blit.h>

#include "config.h"
#include "game.h"
#include "atlas.h"

tBitMap *g_pCubeBitMapAtlas[CUBE_ATLAS_SIZE] = {0};
tBitmapMask *g_pCubeBitMapMaskAtlas[CUBE_ATLAS_SIZE] = {0};
tBitMap *g_pCubeUndrawBitMap = 0;
UBYTE g_pCubeCrossSideAdjust[CROSS_SIDE_COUNT][CUBE_ADJUST_COUNT][2] = {
	{ /* Cross Side A */
		{22, 12}
	},
	{ /* Cross Side B */
		{31, 17}
	},
	{ /* Cross Side C */
		{32, 26}
	},
	{ /* Cross Side D */
		{22, 31}
	},
	{ /* Cross Side E */
		{13, 26}
	},
	{ /* Cross Side F */
		{13, 16}
	}
};

void createCubeAtlas() {
	createAtlasFileWithMask(g_pCubeBitMapAtlas, g_pCubeBitMapMaskAtlas, 0, "/data/cube_frames/cube.bm", "/data/cube_frames/cube.msk");
	// FIXME: +1 in bitmap width is needed to avoid blitter bug
	g_pCubeUndrawBitMap = bitmapCreate(CUBE_BITMAP_WIDTH + 1, CUBE_BITMAP_HEIGHT, WINDOW_SCREEN_BPP, BMF_CLEAR);
}

void destroyCubeAtlas() {
	destroyAtlasFilesWithMask(g_pCubeBitMapAtlas, g_pCubeBitMapMaskAtlas, CUBE_ATLAS_SIZE);
	bitmapDestroy(g_pCubeUndrawBitMap);
}

void drawCube(UWORD uwX, UWORD uwY) {
	blitCopy(
		g_pBufferManager->pBuffer, uwX, uwY,
		g_pCubeUndrawBitMap, 0, 0,
		CUBE_BITMAP_WIDTH, CUBE_BITMAP_HEIGHT,
		MINTERM_COOKIE, 0xFF
	);

	drawCubeAtlasIndex(uwX, uwY, 0);
}

void undrawCube(UWORD uwX, UWORD uwY) {
	blitCopy(
		g_pCubeUndrawBitMap, 0, 0,
		g_pBufferManager->pBuffer, uwX, uwY,
		CUBE_BITMAP_WIDTH, CUBE_BITMAP_HEIGHT,
		MINTERM_COOKIE, 0xFF
	);
}

void drawCubeAtlasIndex(UWORD uwX, UWORD uwY, UBYTE ubAtlasIndex) {
	blitCopyMask(
		g_pCubeBitMapAtlas[ubAtlasIndex], 0, 0,
		g_pBufferManager->pBuffer, uwX, uwY,
		CUBE_WIDTH, CUBE_HEIGHT,
		g_pCubeBitMapMaskAtlas[ubAtlasIndex]->pData
	);
}