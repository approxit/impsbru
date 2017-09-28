#ifndef GUARD_IMPSBRU_GAMESTATES_TEST_H
#define GUARD_IMPSBRU_GAMESTATES_TEST_H

#include <ace/types.h>

void gsTestCreate();
void gsTestLoop();
void gsTestDestroy();

extern UBYTE s_pKeyIndexArray[];

void drawMap();

UWORD changeCrossSideMode(
	IN UWORD uwCrossData,
	IN UBYTE ubCrossSide
);

void drawCross(
	IN UWORD uwCrossData,
	IN UWORD uwX,
	IN UWORD uwY
);

void drawCrossSide(
	IN UWORD uwCrossData,
	IN UBYTE ubCrossSide,
	IN UWORD uwX,
	IN UWORD uwY
);

void drawAtlasIndex(
	IN UBYTE ubAtlasIndex,
	IN UWORD uwX,
	IN UWORD uwY
);

#endif