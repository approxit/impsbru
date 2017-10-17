#include "cube.h"

#include <ace/managers/blit.h>

#include "config.h"
#include "game.h"
#include "atlas.h"

tBitMap *g_pCubeBitMapAtlas[CUBE_ATLAS_SIZE] = {0};
tBitmapMask *g_pCubeBitMapMaskAtlas[CUBE_ATLAS_SIZE] = {0};
tBitMap *g_pCubeUndrawBitMap = 0;
UBYTE g_pCubeCrossSideAdjust[CROSS_SIDE_COUNT][CROSS_SIDE_COUNT][2] = {
	{ /* Cross Side A */
		{22, 12}, // N
		{30, 8}, // NE
		{32, 17}, // SE
		{0, 0}, /* S is impossible opposite */
		{12, 17}, // SW
		{14, 8} // NW
	},
	{ /* Cross Side B */
		{30, 8}, // N
		{30, 17}, // NE
		{40, 22}, // SE
		{30, 27}, // S
		{0, 0}, /* SW is impossible opposite */
		{22, 13}, // NW
	},
	{ /* Cross Side C */
		{32, 17}, // N
		{40, 22}, // NE
		{32, 26}, // SE
		{32, 37}, // S
		{22, 31}, // SW
		{0, 0} // NW is impossible opposite */
	},
	{ /* Cross Side D */
		{0, 0}, /* N is impossible opposite */
		{30, 27}, // NE
		{32, 37}, // SE
		{22, 31}, // S
		{12, 36}, // SW
		{14, 27} // NW
	},
	{ /* Cross Side E */
		{12, 17}, // N
		{0, 0}, /* NE is impossible opposite */
		{22, 31}, // SE
		{12, 36}, // S
		{12, 26}, // SW
		{4, 22} // NW
	},
	{ /* Cross Side F */
		{14, 8}, // N
		{22, 13}, // NE
		{0, 0}, /* SE is impossible opposite */
		{14, 27}, // S
		{4, 22}, // SW
		{14, 17} // NW
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