#include "gamestates/editor.h"

#include <ace/managers/log.h>
#include <ace/managers/key.h>
#include <ace/managers/game.h>
#include <ace/managers/blit.h>

#include "game.h"
#include "cross.h"
#include "map.h"

static UBYTE g_ubMapCursorX = 0;
static UBYTE g_ubMapCursorY = 0;
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

void gsEditorCreate() {
	logWrite("gsEditorCreate\n");

	createCrossAtlas();

//	blitRect(
//		g_pBufferManager->pBuffer, 0, 0,
//		WINDOW_SCREEN_WIDTH, WINDOW_SCREEN_HEIGHT,
//		2
//	);

	drawMap();
	drawCursor();
}

void gsEditorLoop() {
	if (keyUse(KEY_ESCAPE)) {
		gameClose();
	}

	handleMapEditActions();
	handleMapCursorActions();
	handleMapLoadSaveActions();
}

void gsEditorDestroy() {
	logWrite("gsEditorDestroy\n");

	destroyCrossAtlas();
}

void handleMapEditActions() {
	for (UBYTE ubKeyIndex = 0; ubKeyIndex < CROSS_SIDE_COUNT; ++ubKeyIndex) {
		if (keyUse(s_pKeysForCrossSide[ubKeyIndex])) {
			BYTE ubCursorX = getMapNeighborXRestrictive(g_ubMapCursorX, ubKeyIndex);
			BYTE ubCursorY = getMapNeighborYRestrictive(g_ubMapCursorX, g_ubMapCursorY, ubKeyIndex);

			if ((ubCursorX != MAP_WIDTH) && (ubCursorY != MAP_HEIGHT)) {
				toggleCrossSideState(g_ubMapCursorX, g_ubMapCursorY, ubKeyIndex);
				toggleNeighborCrossSideState(g_ubMapCursorX, g_ubMapCursorY, ubKeyIndex);

				drawCursor();
			}
		}
	}
}

void handleMapCursorActions() {
	for (UBYTE ubKeyIndex = 0; ubKeyIndex < CROSS_SIDE_COUNT; ++ubKeyIndex) {
		if (keyUse(s_pKeysForMapCursor[ubKeyIndex])) {
			UBYTE ubCursorX = getMapNeighborX(g_ubMapCursorX, ubKeyIndex);
			UBYTE ubCursorY = getMapNeighborY(g_ubMapCursorX, g_ubMapCursorY, ubKeyIndex);

			if ((ubCursorX != MAP_WIDTH) && (ubCursorY != MAP_HEIGHT)) {
				undrawCursor();

				g_ubMapCursorX = ubCursorX;
				g_ubMapCursorY = ubCursorY;

				drawCursor();
			}
		}
	}
}

void handleMapLoadSaveActions() {
	for (UBYTE ubKey = KEY_1; ubKey < KEY_0; ++ubKey) {
		if (keyUse(ubKey)) {
			char szMapFilePath[255];
			sprintf(szMapFilePath, "map%u.map", ubKey);

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

	UWORD uwX = getMapCrossX(g_ubMapCursorX);
	UWORD uwY = getMapCrossY(g_ubMapCursorX, g_ubMapCursorY);

	undrawCross(uwX, uwY);
	drawCross(uwX, uwY, g_pMapData[g_ubMapCursorX][g_ubMapCursorY]);

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

void drawCursor() {
	UWORD uwX = getMapCrossX(g_ubMapCursorX);
	UWORD uwY = getMapCrossY(g_ubMapCursorX, g_ubMapCursorY);
	UBYTE ubWidth = CROSS_WIDTH;
	UBYTE ubHeight = CROSS_HEIGHT;
	UBYTE ubStartX = 0;
	UBYTE ubStartY = 0;

	if (!g_ubMapCursorX) {
		uwX += CROSS_CURSOR_LEFT;
		ubWidth -= CROSS_CURSOR_LEFT;
		ubStartX = CROSS_CURSOR_LEFT;
	}
	else if (g_ubMapCursorX == (MAP_WIDTH - 1)) {
		ubWidth -= CROSS_CURSOR_RIGHT;
	}

	if (!g_ubMapCursorY) {
		uwY += CROSS_CURSOR_TOP;
		ubHeight -= CROSS_CURSOR_TOP;
		ubStartY = CROSS_CURSOR_TOP;
	}
	else if (g_ubMapCursorY == (MAP_HEIGHT - 1)) {
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
		g_pBufferManager->pBuffer, getMapCrossX(g_ubMapCursorX), getMapCrossY(g_ubMapCursorX, g_ubMapCursorY),
		CROSS_WIDTH, CROSS_HEIGHT,
		g_pCrossBitMapMaskAtlas[CROSS_CURSOR]->pData
	);
}