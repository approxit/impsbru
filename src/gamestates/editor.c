#include "gamestates/editor.h"

#include <ace/managers/log.h>
#include <ace/managers/key.h>
#include <ace/managers/game.h>
#include <ace/managers/blit.h>

#include "game.h"
#include "cross.h"
#include "map.h"
#include "atlas.h"

static UBYTE s_ubMapCursorX = 0;
static UBYTE s_ubMapCursorY = 0;
static UBYTE s_pKeysForCrossSide[CROSS_SIDE_COUNT] = {
	KEY_W, // N
	KEY_E, // NE
	KEY_C, // SE
	KEY_X, // S
	KEY_Z, // SW
	KEY_Q // NW
};
static UBYTE s_pKeysForMapCursor[CROSS_SIDE_COUNT] = {
	KEY_I, // N
	KEY_O, // NE
	KEY_PERIOD, // SE
	KEY_COMMA, // S
	KEY_M, // SW
	KEY_U // NW
};
static UBYTE s_ubEditorStep = EDITOR_STEP_CREATE;
static tBitMap *s_pEditorStepBitMapAtlas[EDITOR_STEP_ATLAS_SIZE] = {0};

void gsEditorCreate() {
	logWrite("gsEditorCreate\n");

	createCrossAtlas();
	createEditorStepAtlas();

	loadMapFromFile("/data/maps/1.map");

	drawEditorStep();
	drawMap();
	drawCursor();
}

void gsEditorLoop() {
	handleMapEditActions();
	handleMapCursorActions();
	handleEditorStepActions();
	handleMapLoadSaveActions();
}

void gsEditorDestroy() {
	logWrite("gsEditorDestroy\n");

	destroyCrossAtlas();
	destroyEditorStepAtlas();
}

void createEditorStepAtlas() {
	createAtlasFile(s_pEditorStepBitMapAtlas, EDITOR_STEP_CREATE, "/data/editor_frames/create.bm");
	createAtlasFile(s_pEditorStepBitMapAtlas, EDITOR_STEP_START_POINT, "/data/editor_frames/start_point.bm");
	createAtlasFile(s_pEditorStepBitMapAtlas, EDITOR_STEP_DESTINATION_POINT, "/data/editor_frames/destination_point.bm");
	createAtlasFile(s_pEditorStepBitMapAtlas, EDITOR_STEP_PLAY_TEST, "/data/editor_frames/play_test.bm");
	createAtlasFile(s_pEditorStepBitMapAtlas, EDITOR_STEP_SHARE, "/data/editor_frames/share.bm");
}

void destroyEditorStepAtlas() {
	destroyAtlasFiles(s_pEditorStepBitMapAtlas, EDITOR_STEP_ATLAS_SIZE);
}

void handleMapEditActions() {
	for (UBYTE ubKeyIndex = 0; ubKeyIndex < CROSS_SIDE_COUNT; ++ubKeyIndex) {
		if (keyUse(s_pKeysForCrossSide[ubKeyIndex])) {
			BYTE ubCursorX = getMapNeighborXRestrictive(s_ubMapCursorX, ubKeyIndex);
			BYTE ubCursorY = getMapNeighborYRestrictive(s_ubMapCursorX, s_ubMapCursorY, ubKeyIndex);

			if ((ubCursorX != MAP_WIDTH) && (ubCursorY != MAP_HEIGHT)) {
				toggleCrossSideState(s_ubMapCursorX, s_ubMapCursorY, ubKeyIndex);
				toggleNeighborCrossSideState(s_ubMapCursorX, s_ubMapCursorY, ubKeyIndex);

				drawCursor();
			}
		}
	}
}

void handleMapCursorActions() {
	for (UBYTE ubKeyIndex = 0; ubKeyIndex < CROSS_SIDE_COUNT; ++ubKeyIndex) {
		if (keyUse(s_pKeysForMapCursor[ubKeyIndex])) {
			UBYTE ubCursorX = getMapNeighborX(s_ubMapCursorX, ubKeyIndex);
			UBYTE ubCursorY = getMapNeighborY(s_ubMapCursorX, s_ubMapCursorY, ubKeyIndex);

			if ((ubCursorX != MAP_WIDTH) && (ubCursorY != MAP_HEIGHT)) {
				undrawCursor();

				s_ubMapCursorX = ubCursorX;
				s_ubMapCursorY = ubCursorY;

				drawCursor();
			}
		}
	}
}

void handleEditorStepActions() {
	if (keyUse(KEY_ESCAPE) || keyUse(KEY_BACKSPACE)) {
		if (!s_ubEditorStep) {
			gameClose();
		}
		else {
			--s_ubEditorStep;
			drawEditorStep();
		}
	}

	if (keyUse(KEY_RETURN) || keyUse(KEY_SPACE)) {
		if ((s_ubEditorStep + 1) < EDITOR_STEP_COUNT) {
			++s_ubEditorStep;
			drawEditorStep();
		}
	}
}

void handleMapLoadSaveActions() {
	for (UBYTE ubKey = KEY_1; ubKey <= KEY_0; ++ubKey) {
		if (keyUse(ubKey)) {
			char szMapFilePath[255];
			sprintf(szMapFilePath, "/data/maps/%u.map", ubKey);

			if (keyCheck(KEY_LSHIFT) || keyCheck(KEY_RSHIFT)) {
				saveMapToFile(szMapFilePath);
			}
			else {
				if (loadMapFromFile(szMapFilePath)) {
					undrawMap();
					drawMap();
					drawCursor();
				}
			}
		}
	}
}

void toggleCrossSideState(UBYTE ubCrossXIndex, UBYTE ubCrossYIndex, UBYTE ubCrossSide) {
	g_pMapData[ubCrossXIndex][ubCrossYIndex] ^= 1 << ubCrossSide;

	UWORD uwX = getMapCrossX(s_ubMapCursorX);
	UWORD uwY = getMapCrossY(s_ubMapCursorX, s_ubMapCursorY);

	undrawCross(uwX, uwY);
	drawCross(uwX, uwY, g_pMapData[s_ubMapCursorX][s_ubMapCursorY]);

//	logWrite("New CrossData:\t\t\tA: %u, B: %u, C: %u, D: %u, E: %u, F: %u\n",
//		getCrossSideState(g_pMapData[ubCrossXIndex][ubCrossYIndex], CROSS_SIDE_A),
//		getCrossSideState(g_pMapData[ubCrossXIndex][ubCrossYIndex], CROSS_SIDE_B),
//		getCrossSideState(g_pMapData[ubCrossXIndex][ubCrossYIndex], CROSS_SIDE_C),
//		getCrossSideState(g_pMapData[ubCrossXIndex][ubCrossYIndex], CROSS_SIDE_D),
//		getCrossSideState(g_pMapData[ubCrossXIndex][ubCrossYIndex], CROSS_SIDE_E),
//		getCrossSideState(g_pMapData[ubCrossXIndex][ubCrossYIndex], CROSS_SIDE_F)
//	);
}

void toggleNeighborCrossSideState(UBYTE ubCrossXIndex, UBYTE ubCrossYIndex, UBYTE ubCrossSide) {
	UBYTE ubMapNeighborX = getMapNeighborX(ubCrossXIndex, ubCrossSide);
	UBYTE ubMapNeighborY = getMapNeighborY(ubCrossXIndex, ubCrossYIndex, ubCrossSide);

	UBYTE ubOppositeCrossSide = getOppositeCrossSide(ubCrossSide);

	g_pMapData[ubMapNeighborX][ubMapNeighborY] &= ~(1 << ubOppositeCrossSide);
	if (getCrossSideState(g_pMapData[ubCrossXIndex][ubCrossYIndex], ubCrossSide)) {
		g_pMapData[ubMapNeighborX][ubMapNeighborY] |= (1 << ubOppositeCrossSide);
	}

	UWORD uwX = getMapCrossX(ubMapNeighborX);
	UWORD uwY = getMapCrossY(ubMapNeighborX, ubMapNeighborY);

	undrawCross(uwX, uwY);
	drawCross(uwX, uwY, g_pMapData[ubMapNeighborX][ubMapNeighborY]);

//	logWrite("New NeighborCrossData:\tA: %u, B: %u, C: %u, D: %u, E: %u, F: %u\n",
//		getCrossSideState(g_pMapData[ubCrossXIndex][ubCrossYIndex], CROSS_SIDE_A),
//		getCrossSideState(g_pMapData[ubCrossXIndex][ubCrossYIndex], CROSS_SIDE_B),
//		getCrossSideState(g_pMapData[ubCrossXIndex][ubCrossYIndex], CROSS_SIDE_C),
//		getCrossSideState(g_pMapData[ubCrossXIndex][ubCrossYIndex], CROSS_SIDE_D),
//		getCrossSideState(g_pMapData[ubCrossXIndex][ubCrossYIndex], CROSS_SIDE_E),
//		getCrossSideState(g_pMapData[ubCrossXIndex][ubCrossYIndex], CROSS_SIDE_F)
//	);
}

void drawEditorStep() {
	blitCopyAligned(
		s_pEditorStepBitMapAtlas[s_ubEditorStep], 0, 0,
		g_pBufferManager->pBuffer, 0, 0,
		EDITOR_STEP_WIDTH, EDITOR_STEP_HEIGHT
	);
}

void drawCursor() {
	UWORD uwX = getMapCrossX(s_ubMapCursorX);
	UWORD uwY = getMapCrossY(s_ubMapCursorX, s_ubMapCursorY);
	UBYTE ubWidth = CROSS_WIDTH;
	UBYTE ubHeight = CROSS_HEIGHT;
	UBYTE ubStartX = 0;
	UBYTE ubStartY = 0;

	if (!s_ubMapCursorX) {
		uwX += CROSS_CURSOR_LEFT;
		ubWidth -= CROSS_CURSOR_LEFT;
		ubStartX = CROSS_CURSOR_LEFT;
	}
	else if (s_ubMapCursorX == (MAP_WIDTH - 1)) {
		ubWidth -= CROSS_CURSOR_RIGHT;
	}

	if (!s_ubMapCursorY) {
		uwY += CROSS_CURSOR_TOP;
		ubHeight -= CROSS_CURSOR_TOP;
		ubStartY = CROSS_CURSOR_TOP;
	}
	else if (s_ubMapCursorY == (MAP_HEIGHT - 1)) {
		ubHeight -= CROSS_CURSOR_BOTTOM;
	}

	blitCopyMask(
		g_pCrossBitMapAtlas[CROSS_CURSOR], ubStartX, ubStartY,
		g_pBufferManager->pBuffer, uwX, uwY,
		ubWidth, ubHeight,
		g_pCrossBitMapMaskAtlas[CROSS_CURSOR]->pData
	);
}

void undrawCursor() {
	blitCopyMask(
		g_pCrossBitMapAtlas[CROSS_CLEANUP], 0, 0,
		g_pBufferManager->pBuffer, getMapCrossX(s_ubMapCursorX), getMapCrossY(s_ubMapCursorX, s_ubMapCursorY),
		CROSS_WIDTH, CROSS_HEIGHT,
		g_pCrossBitMapMaskAtlas[CROSS_CURSOR]->pData
	);
}