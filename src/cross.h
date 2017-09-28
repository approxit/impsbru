#ifndef GUARD_IMPSBRU_CROSS_H
#define GUARD_IMPSBRU_CROSS_H

#include <ace/utils/bitmap.h>
#include <ace/utils/bitmapmask.h>

#include "config.h"

extern tBitMap *g_pCrossBitMapAtlas[ATLAS_COUNT];
extern tBitmapMask *g_pCrossBitMapMaskAtlas[ATLAS_COUNT];

UBYTE getCrossSideAtlasIndex(
	IN UBYTE ubCrossSide,
	IN UBYTE ubCrossSideMode
);

UBYTE getCrossSideMode(
	IN UWORD uwCrossData,
	IN UBYTE ubCrossSide
);

void createCrossAtlas();

void createCrossAtlasFile(
	IN UBYTE ubAtlasIndex,
	IN char *szBitMapFilePath,
	IN char *szBitMapMaskFilePath
);

void destroyCrossAtlas();

#endif