#include "gamestates/test.h"

#include <ace/managers/log.h>
#include <ace/managers/game.h>
#include <ace/managers/key.h>
#include <ace/managers/blit.h>
#include <ace/managers/viewport/simplebuffer.h>
#include <ace/utils/extview.h>
#include <ace/utils/palette.h>

#include "cross.h"

static tView *s_pView;
static tVPort *s_pVPort;
static tSimpleBufferManager *s_pBufferManager;
static UWORD uwCrossData = CROSS_SIDE_A_OPEN |
	CROSS_SIDE_B_OPEN |
	CROSS_SIDE_C_OPEN |
	CROSS_SIDE_D_OPEN |
	CROSS_SIDE_E_OPEN |
	CROSS_SIDE_F_OPEN;

void gsTestCreate() {
	logWrite("gsTestCreate\n");

	s_pView = (tView *) viewCreate(0,
		TAG_VIEW_GLOBAL_CLUT, 1,
		TAG_DONE
	);

	s_pVPort = vPortCreate(0,
		TAG_VPORT_VIEW, s_pView,
		TAG_DONE
	);

	s_pBufferManager = simpleBufferCreate(0,
		TAG_SIMPLEBUFFER_VPORT, s_pVPort,
		TAG_DONE
	);

	paletteLoad("/data/impsbru.plt", s_pVPort->pPalette, 1 << WINDOW_SCREEN_BPP);
	createCrossAtlas();

	drawMap();

	viewLoad(s_pView);
}

UBYTE s_pKeyIndexArray[] = {KEY_W, KEY_E, KEY_C, KEY_X, KEY_Z, KEY_Q};

void gsTestLoop() {
	if (keyUse(KEY_ESCAPE)) {
		gameClose();
	}

	for (UBYTE ubKeyIndex = 0; ubKeyIndex < CROSS_SIDE_COUNT; ++ubKeyIndex) {
		if (keyUse(s_pKeyIndexArray[ubKeyIndex])) {
			uwCrossData = changeCrossSideMode(uwCrossData, ubKeyIndex);

			drawMap();
		}
	}
}

void drawMap() {
	UWORD uwX, uwY;
	for (UBYTE ubCrossXIndex = 0; ubCrossXIndex < 5; ++ubCrossXIndex) {
		for (UBYTE ubCrossYIndex = 0; ubCrossYIndex < 4; ++ubCrossYIndex) {
			uwX = CROSS_X_SHIFT * ubCrossXIndex;
			uwY = ((ubCrossXIndex % 2) * CROSS_EVEN_Y_SHIFT) + (CROSS_Y_SHIFT * ubCrossYIndex);

			drawCross(uwCrossData, uwX, uwY);
		}
	}
}

UWORD changeCrossSideMode(UWORD uwCrossData, UBYTE ubCrossSide) {
	UBYTE ubCrossSideMode = getCrossSideMode(uwCrossData, ubCrossSide);
	UBYTE ubNewCrossSideValue = (ubCrossSideMode + 1) % (CROSS_SIDE_MODE_COUNT + 1);

	uwCrossData &= ~(~(0xFFFF << CROSS_SIDE_MODE_BYTES) << (CROSS_SIDE_MODE_BYTES * ubCrossSide));
	uwCrossData |= ubNewCrossSideValue << (CROSS_SIDE_MODE_BYTES * ubCrossSide);

	return uwCrossData;
}

void gsTestDestroy() {
	logWrite("gsTestDestroy\n");

	destroyCrossAtlas();

	viewDestroy(s_pView);
}

void drawCross(UWORD uwCrossData, UWORD uwX, UWORD uwY) {
	if (uwCrossData) {
		drawAtlasIndex(0, uwX, uwY);
	}

	for (UBYTE ubCrossSide = 1; ubCrossSide < CROSS_SIDE_COUNT; ubCrossSide += 2) {
		drawCrossSide(uwCrossData, ubCrossSide, uwX, uwY);
	}

	for (UBYTE ubCrossSide = CROSS_SIDE_COUNT; 0 < ubCrossSide; ubCrossSide -= 2) {
		drawCrossSide(uwCrossData, ubCrossSide - 2, uwX, uwY);
	}
}

void drawCrossSide(UWORD uwCrossData, UBYTE ubCrossSide, UWORD uwX, UWORD uwY) {
	UBYTE ubCrossSideMode = getCrossSideMode(uwCrossData, ubCrossSide);
	if (ubCrossSideMode) {
		UBYTE ubAtlasIndex = getCrossSideAtlasIndex(ubCrossSide, ubCrossSideMode);
		drawAtlasIndex(ubAtlasIndex, uwX, uwY);
	}
}

void drawAtlasIndex(UBYTE ubAtlasIndex, UWORD uwX, UWORD uwY) {
	if (g_pCrossBitMapAtlas[ubAtlasIndex] && g_pCrossBitMapMaskAtlas[ubAtlasIndex]) {
		blitCopyMask(
			g_pCrossBitMapAtlas[ubAtlasIndex], 0, 0,
			s_pBufferManager->pBuffer, uwX, uwY,
			CROSS_WIDTH, CROSS_HEIGHT,
			g_pCrossBitMapMaskAtlas[ubAtlasIndex]->pData
		);
	}
}
