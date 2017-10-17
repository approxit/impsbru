#ifndef GUARD_IMPSBRU_CUBE_H
#define GUARD_IMPSBRU_CUBE_H

#include <ace/utils/bitmap.h>
#include <ace/utils/bitmapmask.h>

#include "config.h"

extern UBYTE g_pCubeCrossSideAdjust[CROSS_SIDE_COUNT][CROSS_SIDE_COUNT][2];

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

#endif