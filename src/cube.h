#ifndef GUARD_IMPSBRU_CUBE_H
#define GUARD_IMPSBRU_CUBE_H

#include <ace/utils/bitmap.h>
#include <ace/utils/bitmapmask.h>

#include "config.h"

extern tBitMap *g_pCubeBitMapAtlas[CUBE_ATLAS_SIZE];
extern tBitmapMask *g_pCubeBitMapMaskAtlas[CUBE_ATLAS_SIZE];
extern UBYTE g_pCubeCrossSideAdjust[CROSS_SIDE_COUNT][CUBE_ADJUST_COUNT][2];

void createCubeAtlas();
void destroyCubeAtlas();

void drawCube(
	IN UWORD uwX,
	IN UWORD uwY
);

void undrawCube();

void drawCubeAtlasIndex(
	IN UWORD uwX,
	IN UWORD uwY,
	IN UBYTE ubAtlasIndex
);

#endif