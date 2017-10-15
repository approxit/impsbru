#ifndef GUARD_IMPSBRU_MAP_H
#define GUARD_IMPSBRU_MAP_H

#include <ace/types.h>

#include "config.h"

extern UBYTE g_pMapData[MAP_WIDTH][MAP_HEIGHT];
extern UBYTE g_ubMapStartPointX;
extern UBYTE g_ubMapStartPointY;
extern UBYTE g_ubMapStartPointCrossSide;
extern UBYTE g_ubMapDestinationPointX;
extern UBYTE g_ubMapDestinationPointY;
extern UBYTE g_ubMapDestinationPointCrossSide;
extern BYTE g_pMapCursorShifts[2][CROSS_SIDE_COUNT][2];

UWORD getMapCrossX(
	IN UBYTE ubCrossXIndex
);

UWORD getMapCrossY(
	IN UBYTE ubCrossXIndex,
	IN UBYTE ubCrossXIndex
);

BYTE getMapShiftX(
	IN UBYTE ubMapCursorX,
	IN UBYTE ubCrossSide
);

BYTE getMapShiftY(
	IN UBYTE ubMapCursorX,
	IN UBYTE ubCrossSide
);

UBYTE getRowParity(
	IN UBYTE ubMapCursorX
);

UBYTE getMapNeighborX(
	IN UBYTE ubMapCursorX,
	IN UBYTE ubCrossSide
);

UBYTE getMapNeighborY(
	IN UBYTE ubMapCursorX,
	IN UBYTE ubMapCursorY,
	IN UBYTE ubCrossSide
);

UBYTE getMapNeighborXRestrictive(
	IN UBYTE ubMapCursorX,
	IN UBYTE ubCrossSide
);

UBYTE getMapNeighborYRestrictive(
	IN UBYTE ubMapCursorX,
	IN UBYTE ubMapCursorY,
	IN UBYTE ubCrossSide
);

void drawMap();
void undrawMap();

void drawMapCross(
	IN UBYTE ubCrossXIndex,
	IN UBYTE ubCrossYIndex
);

void undrawMapCross(
	IN UBYTE ubCrossXIndex,
	IN UBYTE ubCrossYIndex
);

void drawMapStartPoint();
void undrawMapStartPoint();
void drawMapDestinationPoint();
void undrawMapDestinationPoint();

UBYTE saveMapToFile(
	IN char *szFilePath
);

UBYTE loadMapFromFile(
	IN char *szFilePath
);

#endif