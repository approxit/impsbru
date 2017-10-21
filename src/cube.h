#ifndef GUARD_IMPSBRU_CUBE_H
#define GUARD_IMPSBRU_CUBE_H

#include <ace/utils/bitmap.h>
#include <ace/utils/bitmapmask.h>

#include "config.h"

extern UBYTE g_pCubeCrossSideAdjust[CROSS_SIDE_COUNT][CROSS_SIDE_COUNT][2];
extern tBitmapMask *g_pCubeBitMapMaskAtlas[CUBE_ATLAS_SIZE];
extern tBitMap *g_pCubeMapDepthMask;
extern tBitMap *g_pCubeDepthMask;

void createCubeAtlas();
void destroyCubeAtlas();

UBYTE getCubeDepth(
	IN UBYTE ubCrossSide,
	IN UBYTE ubCrossSideAdjustRotation
);

void drawCube(
	IN UWORD uwX,
	IN UWORD uwY
);

void undrawCube(
	IN UWORD uwX,
	IN UWORD uwY
);

void drawCubeAtlasIndex(
	IN UWORD uwX,
	IN UWORD uwY,
	IN UBYTE ubAtlasIndex
);

void drawCubeWithDepth(
	IN UWORD uwX,
	IN UWORD uwY
);

void blitCubeWithDepth(
	tBitMap *pSrc, WORD wSrcX, WORD wSrcY,
	tBitMap *pDst, WORD wDstX, WORD wDstY,
	WORD wWidth, WORD wHeight, UWORD *pMsk,
	UWORD uwMinterm
);

#endif