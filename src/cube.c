#include "cube.h"

#include <ace/managers/blit.h>

#include "config.h"
#include "game.h"
#include "atlas.h"

tBitmapMask *g_pCubeBitMapMaskAtlas[CUBE_ATLAS_SIZE] = {0};
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
tBitMap *g_pCubeMapDepthMask = 0;
tBitMap *g_pCubeDepthMask = 0;

tBitMap *s_pCubeBitMapAtlas[CUBE_ATLAS_SIZE] = {0};
tBitMap *s_pCubeUndrawBitMap = 0;

void createCubeAtlas() {
	createAtlasFileWithMask(s_pCubeBitMapAtlas, g_pCubeBitMapMaskAtlas, 0, "/data/cube_frames/cube.bm", "/data/cube_frames/cube.msk");
	s_pCubeUndrawBitMap = bitmapCreate(CUBE_BITMAP_WIDTH, CUBE_BITMAP_HEIGHT, WINDOW_SCREEN_BPP, BMF_CLEAR);
	g_pCubeMapDepthMask = bitmapCreate(WINDOW_SCREEN_WIDTH, WINDOW_SCREEN_HEIGHT, 1, BMF_CLEAR);
	g_pCubeDepthMask = bitmapCreate(CUBE_BITMAP_WIDTH, CUBE_BITMAP_HEIGHT, 1, BMF_CLEAR);
}

void destroyCubeAtlas() {
	destroyAtlasFilesWithMask(s_pCubeBitMapAtlas, g_pCubeBitMapMaskAtlas, CUBE_ATLAS_SIZE);

	bitmapDestroy(s_pCubeUndrawBitMap);
	bitmapDestroy(g_pCubeMapDepthMask);
	bitmapDestroy(g_pCubeDepthMask);
}

UBYTE getCubeDepth(UBYTE ubCrossSide, UBYTE ubCrossSideAdjustRotation) {
	if (ubCrossSide & 1) { /* B D E cross sides */
		return ubCrossSideAdjustRotation & 1 ? CUBE_DEPTH_BOTTOM : CUBE_DEPTH_MIDDLE;
	}
	else { /* A C E cross sides */
		return ubCrossSideAdjustRotation & 1 ? CUBE_DEPTH_MIDDLE : CUBE_DEPTH_TOP;
	}
}

void drawCube(UWORD uwX, UWORD uwY) {
	blitCopy(
		g_pBufferManager->pBuffer, uwX, uwY,
		s_pCubeUndrawBitMap, 0, 0,
		CUBE_WIDTH, CUBE_HEIGHT,
		MINTERM_COOKIE, 0xFF
	);

	drawCubeAtlasIndex(uwX, uwY, 0);
}

void undrawCube(UWORD uwX, UWORD uwY) {
	blitCopyMask(
		s_pCubeUndrawBitMap, 0, 0,
		g_pBufferManager->pBuffer, uwX, uwY,
		CUBE_WIDTH, CUBE_HEIGHT,
		g_pCubeBitMapMaskAtlas[0]->pData
	);
}

void drawCubeWithDepth(UWORD uwX, UWORD uwY) {
	blitCopy(
		g_pBufferManager->pBuffer, uwX, uwY,
		s_pCubeUndrawBitMap, 0, 0,
		CUBE_WIDTH, CUBE_HEIGHT,
		MINTERM_COOKIE, 0xFF
	);

	logWrite("%hhux%hhu\n", uwX, uwY);

//	blitRect(
//		g_pCubeDepthMask, 0, 0,
//		CUBE_BITMAP_WIDTH, CUBE_BITMAP_HEIGHT,
//		0
//	);

	BltBitMap(
		g_pCubeMapDepthMask, uwX, uwY,
		g_pCubeDepthMask, 0, 0,
		CUBE_BITMAP_WIDTH, CUBE_BITMAP_HEIGHT,
		MINTERM_COOKIE, 0xFF, 0
	);

//	tBitmapMask sBottomDepthMask;
//	sBottomDepthMask.pData = (UWORD *) g_pCubeDepthMask->Planes[0];
//	sBottomDepthMask.uwWidth = CUBE_BITMAP_WIDTH;
//	sBottomDepthMask.uwHeight = CUBE_BITMAP_HEIGHT;
//	bitmapMaskSaveBmp(&sBottomDepthMask, "fragment_mask.bmp");

	tBitMap sCrossSideMaskBitmap;
	InitBitMap(&sCrossSideMaskBitmap, 1, CUBE_BITMAP_WIDTH, CUBE_BITMAP_HEIGHT);
	sCrossSideMaskBitmap.Planes[0] = (PLANEPTR) g_pCubeBitMapMaskAtlas[0]->pData;

	BltBitMap(
		&sCrossSideMaskBitmap, 0, 0,
		g_pCubeDepthMask, 0, 0,
		CUBE_BITMAP_WIDTH, CUBE_BITMAP_HEIGHT,
		0x44, 0xFF, 0
	);

//	sBottomDepthMask.pData = (UWORD *) g_pCubeDepthMask->Planes[0];
//	sBottomDepthMask.uwWidth = CUBE_BITMAP_WIDTH;
//	sBottomDepthMask.uwHeight = CUBE_BITMAP_HEIGHT;
//	bitmapMaskSaveBmp(&sBottomDepthMask, "merge_mask.bmp");

	blitCopyMask(
		s_pCubeBitMapAtlas[0], 0, 0,
		g_pBufferManager->pBuffer, uwX, uwY,
		CUBE_WIDTH, CUBE_HEIGHT,
		(UWORD *) g_pCubeDepthMask->Planes[0]
	);
}

void drawCubeAtlasIndex(UWORD uwX, UWORD uwY, UBYTE ubAtlasIndex) {
	blitCopyMask(
		s_pCubeBitMapAtlas[ubAtlasIndex], 0, 0,
		g_pBufferManager->pBuffer, uwX, uwY,
		CUBE_WIDTH, CUBE_HEIGHT,
		g_pCubeBitMapMaskAtlas[ubAtlasIndex]->pData
	);
}

void blitCubeWithDepth(
	tBitMap *pSrc, WORD wSrcX, WORD wSrcY,
	tBitMap *pDst, WORD wDstX, WORD wDstY,
	WORD wWidth, WORD wHeight, UWORD *pMsk, UWORD uwMinterm
) {
	WORD wDstModulo, wSrcModulo;
	UBYTE ubPlane;

	UBYTE ubSrcOffs = wSrcX & 0xF;
	UBYTE ubDstOffs = wDstX & 0xF;

	UWORD uwBlitWidth = (wWidth+ubDstOffs+15) & 0xFFF0;
	UWORD uwBlitWords = uwBlitWidth >> 4;

	UWORD uwFirstMask = 0xFFFF;
	UWORD uwLastMask = 0xFFFF;

	UWORD uwBltCon1 = (ubDstOffs-ubSrcOffs) << BSHIFTSHIFT;
	UWORD uwBltCon0 = USEA|USEB|USED | uwMinterm;

	ULONG ulSrcOffs = pSrc->BytesPerRow * wSrcY + (wSrcX>>3);
	ULONG ulDstOffs = pDst->BytesPerRow * wDstY + (wDstX>>3);

	wSrcModulo = pSrc->BytesPerRow - (uwBlitWords<<1);
	wDstModulo = pDst->BytesPerRow - (uwBlitWords<<1);
	WaitBlit();
	custom.bltcon0 = uwBltCon0;
	custom.bltcon1 = uwBltCon1;
	custom.bltafwm = uwFirstMask;
	custom.bltalwm = uwLastMask;

	custom.bltamod = wDstModulo;
	custom.bltbmod = wSrcModulo;
	custom.bltdmod = wDstModulo;
	for(ubPlane = pSrc->Depth; ubPlane--;) {
		WaitBlit();
		custom.bltapt  = (UBYTE*)((ULONG)pMsk + ulDstOffs);
		custom.bltbpt  = (UBYTE*)(((ULONG)(pSrc->Planes[ubPlane])) + ulSrcOffs);
		custom.bltdpt  = (UBYTE*)(((ULONG)(pDst->Planes[ubPlane])) + ulDstOffs);

		custom.bltsize = (wHeight << 6) | uwBlitWords;
	}
}

void waitForPos(UWORD uwPos) {
	UWORD uwEndPos;
	UWORD uwCurrFrame;

	// Determine VPort end position
	uwEndPos = g_pVPort->uwOffsY + uwPos + 0x2C; // Addition from DiWStrt
	if(vhPosRegs->uwPosY < uwEndPos) {
		// If current beam is before pos, wait for pos @ current frame
		while(vhPosRegs->uwPosY < uwEndPos);
	}
	else {
		uwCurrFrame = g_sTimerManager.uwFrameCounter;
		while(
			vhPosRegs->uwPosY < uwEndPos ||
			g_sTimerManager.uwFrameCounter == uwCurrFrame
		);
	}

	// Otherwise wait for pos @ next frame
}