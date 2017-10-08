#include "cross.h"

#include <ace/managers/blit.h>

#include "game.h"

tBitMap *g_pCrossBitMapAtlas[ATLAS_COUNT] = {0};
tBitmapMask *g_pCrossBitMapMaskAtlas[ATLAS_COUNT] = {0};

UBYTE getCrossSideState(UBYTE ubCrossData, UBYTE ubCrossSide) {
	return (ubCrossData >> ubCrossSide) & 1;
}

UBYTE getOppositeCrossSide(UBYTE ubCrossSide) {
	return (ubCrossSide + CROSS_SIDE_OPPOSITE_SHIFT) % CROSS_SIDE_COUNT;
}

void createCrossAtlas() {
	createCrossAtlasFile(CROSS_SIDE_A, "/data/cross_frames/a.bm", "/data/cross_frames/a.msk");
	createCrossAtlasFile(CROSS_SIDE_B, "/data/cross_frames/b.bm", "/data/cross_frames/b.msk");
	createCrossAtlasFile(CROSS_SIDE_C, "/data/cross_frames/c.bm", "/data/cross_frames/c.msk");
	createCrossAtlasFile(CROSS_SIDE_D, "/data/cross_frames/d.bm", "/data/cross_frames/d.msk");
	createCrossAtlasFile(CROSS_SIDE_E, "/data/cross_frames/e.bm", "/data/cross_frames/e.msk");
	createCrossAtlasFile(CROSS_SIDE_F, "/data/cross_frames/f.bm", "/data/cross_frames/f.msk");
	createCrossAtlasFile(CROSS_SIDE_A_FINAL, "/data/cross_frames/a_final.bm", "/data/cross_frames/a_final.msk");
	createCrossAtlasFile(CROSS_SIDE_C_FINAL, "/data/cross_frames/c_final.bm", "/data/cross_frames/c_final.msk");
	createCrossAtlasFile(CROSS_SIDE_E_FINAL, "/data/cross_frames/e_final.bm", "/data/cross_frames/e_final.msk");
	createCrossAtlasFile(CROSS_CENTER, "/data/cross_frames/center.bm", "/data/cross_frames/center.msk");
	createCrossAtlasFile(CROSS_CURSOR, "/data/cross_frames/cursor.bm", "/data/cross_frames/cursor.msk");
	createCrossAtlasFile(CROSS_CLEANUP, "/data/cross_frames/cleanup.bm", "/data/cross_frames/cleanup.msk");
}

void createCrossAtlasFile(UBYTE ubAtlasIndex, char *szBitMapFilePath, char *szBitMapMaskFilePath) {
	g_pCrossBitMapAtlas[ubAtlasIndex] = bitmapCreateFromFile(szBitMapFilePath);
	g_pCrossBitMapMaskAtlas[ubAtlasIndex] = bitmapMaskCreateFromFile(szBitMapMaskFilePath);
}

void destroyCrossAtlas() {
	UBYTE ubAtlasIndex = ATLAS_COUNT;
	while (ubAtlasIndex--) {
		bitmapDestroy(g_pCrossBitMapAtlas[ubAtlasIndex]);
		bitmapMaskDestroy(g_pCrossBitMapMaskAtlas[ubAtlasIndex]);
	}
}

void drawCross(UWORD uwX, UWORD uwY, UBYTE ubCrossData) {
	if (ubCrossData) {
		drawAtlasIndex(uwX, uwY, CROSS_CENTER);
	}

	for (UBYTE ubCrossSide = 1; ubCrossSide < CROSS_SIDE_COUNT; ubCrossSide += 2) {
		drawCrossSide(uwX, uwY, ubCrossData, ubCrossSide);
	}

	for (UBYTE ubCrossSide = CROSS_SIDE_COUNT; 0 < ubCrossSide; ubCrossSide -= 2) {
		drawCrossSide(uwX, uwY, ubCrossData, ubCrossSide - 2);
	}
}

void undrawCross(UWORD uwX, UWORD uwY) {
	drawAtlasIndex(uwX, uwY, CROSS_CLEANUP);
}

void drawCrossSide(UWORD uwX, UWORD uwY, UBYTE ubCrossData, UBYTE ubCrossSide) {
	UBYTE ubCrossSideState = getCrossSideState(ubCrossData, ubCrossSide);
	if (ubCrossSideState) {
		drawAtlasIndex(uwX, uwY, ubCrossSide);
	}
}

void drawAtlasIndex(UWORD uwX, UWORD uwY, UBYTE ubAtlasIndex) {
//	blitRect(
//		g_pBufferManager->pBuffer, uwX, uwY,
//		CROSS_WIDTH, CROSS_HEIGHT,
//		1
//	);

	blitCopyMask(
		g_pCrossBitMapAtlas[ubAtlasIndex], 0, 0,
		g_pBufferManager->pBuffer, uwX, uwY,
		CROSS_WIDTH, CROSS_HEIGHT,
		g_pCrossBitMapMaskAtlas[ubAtlasIndex]->pData
	);
}