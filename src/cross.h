#ifndef GUARD_IMPSBRU_CROSS_H
#define GUARD_IMPSBRU_CROSS_H

#include <ace/utils/bitmap.h>
#include <ace/utils/bitmapmask.h>

#include "config.h"

extern tBitMap *g_pCrossBitMapAtlas[CROSS_ATLAS_SIZE];
extern tBitmapMask *g_pCrossBitMapMaskAtlas[CROSS_ATLAS_SIZE];

UBYTE getCrossSideState(
	IN UBYTE ubCrossData,
	IN UBYTE ubCrossSide
);

UBYTE getOppositeCrossSide(
	IN UBYTE ubCrossSide
);

void createCrossAtlas();

void createCrossAtlasFile(
	IN UBYTE ubAtlasIndex,
	IN char *szBitMapFilePath,
	IN char *szBitMapMaskFilePath
);

void destroyCrossAtlas();

void drawCross(
	IN UWORD uwX,
	IN UWORD uwY,
	IN UBYTE ubCrossData
);

void undrawCross(
	IN UWORD uwX,
	IN UWORD uwY
);

void drawCrossSide(
	IN UWORD uwX,
	IN UWORD uwY,
	IN UBYTE ubCrossData,
	IN UBYTE ubCrossSide
);

void drawDestinationPoint(
	IN UWORD uwX,
	IN UWORD uwY,
	IN UBYTE ubDestinationPointSide
);

void undrawDestinationPoint(
	IN UWORD uwX,
	IN UWORD uwY
);

void drawAtlasIndex(
	IN UWORD uwX,
	IN UWORD uwY,
	IN UBYTE ubAtlasIndex
);

#endif