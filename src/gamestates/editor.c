#include "gamestates/editor.h"

#include <ace/managers/log.h>
#include <ace/managers/key.h>
#include <ace/managers/game.h>
#include <ace/managers/blit.h>
#include <ace/macros.h>

#include "game.h"
#include "cross.h"
#include "cube.h"
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
static UBYTE s_ubCubeCrossSide = CROSS_SIDE_COUNT;
static UBYTE s_ubCubeMapStartPointX = 0;
static UBYTE s_ubCubeMapStartPointY = 0;
static UBYTE s_ubCubeMapStartPointCrossSide = 0;
static UBYTE s_ubCubeMapDestinationPointX = 0;
static UBYTE s_ubCubeMapDestinationPointY = 0;
static UBYTE s_ubCubeMapDestinationPointCrossSide = 0;
static UWORD s_uwCubeX = 0;
static UWORD s_uwCubeY = 0;
static UWORD s_uwCubeStartPointX = 0;
static UWORD s_uwCubeStartPointY = 0;
static UWORD s_uwCubeDestinationPointX = 0;
static UWORD s_uwCubeDestinationPointY = 0;
static BYTE s_pCubeStep[CROSS_SIDE_COUNT][2] = {
	{0, -2}, // N
	{2, -1}, // NE
	{2, 1}, // SE
	{0, 2}, // S
	{-2, 1}, // SW
	{-2, -1} // NW
};

void gsEditorCreate() {
	logWrite("gsEditorCreate\n");

	createCrossAtlas();
	createCubeAtlas();
	createEditorStepAtlas();

	drawEditorStep();
	drawMap();
	drawCursor();
}

void gsEditorLoop() {
	switch (s_ubEditorStep) {
		case EDITOR_STEP_CREATE:
			handleEditorStepCreateActions();
			handleMapCursorActions();
			break;
		case EDITOR_STEP_START_POINT:
			handleEditorStepStartPointActions();
			handleMapCursorActions();
			break;
		case EDITOR_STEP_DESTINATION_POINT:
			handleEditorStepDestinationPointActions();
			handleMapCursorActions();
			break;
		case EDITOR_STEP_PLAY_TEST:
			handleEditorStepPlayTestActions();
			moveCube();
			break;
		default:
			handleEditorStepActions();
	}

	handleMapLoadSaveActions();
}

void gsEditorDestroy() {
	logWrite("gsEditorDestroy\n");

	destroyCrossAtlas();
	destroyCubeAtlas();
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

void handleEditorStepCreateActions() {
	/* Cross sides toggling */
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

	/* Cancel buttons handling */
	if (keyUse(KEY_ESCAPE) || keyUse(KEY_BACKSPACE)) {
		UBYTE ubIsMapEmpty = 1;
		for (UBYTE ubX = 0; ubX < MAP_WIDTH; ++ubX) {
			for (UBYTE ubY = 0; ubY < MAP_HEIGHT; ++ubY) {
				if (g_pMapData[ubX][ubY]) {
					ubIsMapEmpty = 0;
					break;
				}
			}
		}

		if (ubIsMapEmpty) {
			gameClose();
		}
		else {
			memset(g_pMapData, 0, MAP_WIDTH * MAP_HEIGHT);
			undrawMap();
			drawMap();
			drawCursor();
			drawEditorStep();
		}
	}

	/* Accept buttons handling */
	if (keyUse(KEY_RETURN) || keyUse(KEY_SPACE)) {
		++s_ubEditorStep;
		drawEditorStep();
	}
}

void handleEditorStepStartPointActions() {
	/* Cross sides toggling */
	for (UBYTE ubKeyIndex = 0; ubKeyIndex < CROSS_SIDE_COUNT; ++ubKeyIndex) {
		if (keyUse(s_pKeysForCrossSide[ubKeyIndex])) {
			UBYTE ubCrossData = g_pMapData[s_ubMapCursorX][s_ubMapCursorY];
			if (ubCrossData && !getCrossSideState(ubCrossData, ubKeyIndex)) {
				undrawMapStartPoint();

				g_ubMapStartPointX = s_ubMapCursorX;
				g_ubMapStartPointY = s_ubMapCursorY;
				g_ubMapStartPointCrossSide = ubKeyIndex;

				drawMapStartPoint();
				drawCursor();
			}
		}
	}

	/* Cancel buttons handling */
	if (keyUse(KEY_ESCAPE) || keyUse(KEY_BACKSPACE)) {
		undrawMapStartPoint();
		if ((g_ubMapStartPointX == s_ubMapCursorX) && (g_ubMapStartPointY == s_ubMapCursorY)) {
			drawCursor();
		}

		g_ubMapStartPointX = MAP_WIDTH;
		g_ubMapStartPointY = MAP_HEIGHT;
		g_ubMapStartPointCrossSide = 0;

		--s_ubEditorStep;
		drawEditorStep();
	}

	/* Accept buttons handling */
	if (keyUse(KEY_RETURN) || keyUse(KEY_SPACE)) {
		if ((g_ubMapStartPointX != MAP_WIDTH) && (g_ubMapStartPointY != MAP_HEIGHT)) {
			++s_ubEditorStep;
			drawEditorStep();
		}
	}
}

void handleEditorStepDestinationPointActions() {
	/* Cross sides toggling */
	for (UBYTE ubKeyIndex = 0; ubKeyIndex < CROSS_SIDE_COUNT; ++ubKeyIndex) {
		if (keyUse(s_pKeysForCrossSide[ubKeyIndex])) {
			UBYTE ubCrossData = g_pMapData[s_ubMapCursorX][s_ubMapCursorY];
			if (ubCrossData && !(ubKeyIndex & 1) && !getCrossSideState(ubCrossData, ubKeyIndex) && !((s_ubMapCursorX == g_ubMapStartPointX) && (s_ubMapCursorY == g_ubMapStartPointY) && (g_ubMapStartPointCrossSide == ubKeyIndex))) {
				undrawMapDestinationPoint();

				if ((g_ubMapDestinationPointX == g_ubMapStartPointX) && (g_ubMapDestinationPointY == g_ubMapStartPointY)) {
					drawMapStartPoint();
				}

				g_ubMapDestinationPointX = s_ubMapCursorX;
				g_ubMapDestinationPointY = s_ubMapCursorY;
				g_ubMapDestinationPointCrossSide = ubKeyIndex;

				drawMapDestinationPoint();
				drawCursor();
			}
		}
	}

	/* Cancel buttons handling */
	if (keyUse(KEY_ESCAPE) || keyUse(KEY_BACKSPACE)) {
		undrawMapDestinationPoint();

		if ((g_ubMapDestinationPointX == g_ubMapStartPointX) && (g_ubMapDestinationPointY == g_ubMapStartPointX)) {
			drawMapStartPoint();
		}

		if ((g_ubMapDestinationPointX == s_ubMapCursorX) && (g_ubMapDestinationPointY == s_ubMapCursorY)) {
			drawCursor();
		}

		g_ubMapDestinationPointX = MAP_WIDTH;
		g_ubMapDestinationPointY = MAP_HEIGHT;
		g_ubMapDestinationPointCrossSide = 0;

		--s_ubEditorStep;
		drawEditorStep();
	}

	/* Accept buttons handling */
	if (keyUse(KEY_RETURN) || keyUse(KEY_SPACE)) {
		if ((g_ubMapDestinationPointX != MAP_WIDTH) && (g_ubMapDestinationPointY != MAP_HEIGHT)) {
			undrawCursor();
			loadCubePositionsFromMap();

			++s_ubEditorStep;
			drawEditorStep();
		}
	}
}

void handleEditorStepPlayTestActions() {
	for (UBYTE ubKeyIndex = 0; ubKeyIndex < CROSS_SIDE_COUNT; ++ubKeyIndex) {
		if (keyUse(s_pKeysForCrossSide[ubKeyIndex]) || keyUse(s_pKeysForMapCursor[ubKeyIndex])) {
			if (s_ubCubeCrossSide == CROSS_SIDE_COUNT) {
			 	if (getCrossSideState(g_pMapData[s_ubCubeMapStartPointX][s_ubCubeMapStartPointY], ubKeyIndex) && (getOppositeCrossSide(ubKeyIndex) != s_ubCubeMapStartPointCrossSide)) {
					setNewCubeDestination(ubKeyIndex);
			 	}
			}
			else if (getOppositeCrossSide(s_ubCubeCrossSide) == ubKeyIndex) {
				s_ubCubeCrossSide = ubKeyIndex;

				swapCubePositions();
			}
		}
	}

	/* Cancel buttons handling */
	if (keyUse(KEY_ESCAPE) || keyUse(KEY_BACKSPACE)) {
		--s_ubEditorStep;
		drawEditorStep();
		drawCursor();

		undrawCube(s_uwCubeX, s_uwCubeY);

		loadCubePositionsFromMap();

		drawMapStartPoint();
	}

	/* Accept buttons handling */
	if (keyUse(KEY_RETURN) || keyUse(KEY_SPACE)) {
		++s_ubEditorStep;
		drawEditorStep();
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
			else if (loadMapFromFile(szMapFilePath)) {
				loadCubePositionsFromMap();

				undrawMap();
				drawMap();

				s_ubEditorStep = EDITOR_STEP_PLAY_TEST;
				drawEditorStep();
			}
		}
	}
}

void loadCubePositionsFromMap() {
	s_ubCubeMapStartPointX = g_ubMapStartPointX;
	s_ubCubeMapStartPointY = g_ubMapStartPointY;
	s_ubCubeMapStartPointCrossSide = g_ubMapStartPointCrossSide;

	s_uwCubeX = getMapCrossX(s_ubCubeMapStartPointX) + g_pCubeCrossSideAdjust[s_ubCubeMapStartPointCrossSide][0][0];
	s_uwCubeY = getMapCrossY(s_ubCubeMapStartPointX, s_ubCubeMapStartPointY) + g_pCubeCrossSideAdjust[s_ubCubeMapStartPointCrossSide][0][1];

	s_uwCubeStartPointX = s_uwCubeX;
	s_uwCubeStartPointY = s_uwCubeY;

	s_ubCubeCrossSide = CROSS_SIDE_COUNT;
}

void setNewCubeDestination(UBYTE ubCrossSide) {
	s_ubCubeMapDestinationPointX = getMapNeighborX(s_ubCubeMapStartPointX, ubCrossSide);
	s_ubCubeMapDestinationPointY = getMapNeighborY(s_ubCubeMapStartPointX, s_ubCubeMapStartPointY, ubCrossSide);

	s_uwCubeDestinationPointX = getMapCrossX(s_ubCubeMapDestinationPointX) + g_pCubeCrossSideAdjust[s_ubCubeMapStartPointCrossSide][0][0];
	s_uwCubeDestinationPointY = getMapCrossY(s_ubCubeMapDestinationPointX, s_ubCubeMapDestinationPointY) + g_pCubeCrossSideAdjust[s_ubCubeMapStartPointCrossSide][0][1];

	s_ubCubeCrossSide = ubCrossSide;
}

void swapCubePositions() {
	UBYTE ubCubeMapStartPointX = s_ubCubeMapStartPointX;
	UBYTE ubCubeMapStartPointY = s_ubCubeMapStartPointY;

	s_ubCubeMapStartPointX = s_ubCubeMapDestinationPointX;
	s_ubCubeMapStartPointY = s_ubCubeMapDestinationPointY;

	s_ubCubeMapDestinationPointX = ubCubeMapStartPointX;
	s_ubCubeMapDestinationPointY = ubCubeMapStartPointY;

	UWORD uwCubeStartPointX = s_uwCubeStartPointX;
	UWORD uwCubeStartPointY = s_uwCubeStartPointY;

	s_uwCubeStartPointX = s_uwCubeDestinationPointX;
	s_uwCubeStartPointY = s_uwCubeDestinationPointY;

	s_uwCubeDestinationPointX = uwCubeStartPointX;
	s_uwCubeDestinationPointY = uwCubeStartPointY;
}

void moveCube() {
	if (s_ubCubeCrossSide != CROSS_SIDE_COUNT) {
		if ((s_uwCubeX == s_uwCubeDestinationPointX) && (s_uwCubeY == s_uwCubeDestinationPointY)) {
			UBYTE ubNoOtherDirections = 1;
			for (UBYTE ubCrossSide = 0; ubCrossSide < CROSS_SIDE_COUNT; ++ubCrossSide) {
				if (getCrossSideState(g_pMapData[s_ubCubeMapDestinationPointX][s_ubCubeMapDestinationPointY], ubCrossSide) && (ubCrossSide != s_ubCubeCrossSide) && (ubCrossSide != getOppositeCrossSide(s_ubCubeCrossSide))) {
					ubNoOtherDirections = 0;
					break;
				}
			}

			s_ubCubeMapStartPointX = s_ubCubeMapDestinationPointX;
			s_ubCubeMapStartPointY = s_ubCubeMapDestinationPointY;

			s_uwCubeStartPointX = s_uwCubeDestinationPointX;
			s_uwCubeStartPointY = s_uwCubeDestinationPointY;

			if (ubNoOtherDirections && getCrossSideState(g_pMapData[s_ubCubeMapDestinationPointX][s_ubCubeMapDestinationPointY], s_ubCubeCrossSide)) {
				setNewCubeDestination(s_ubCubeCrossSide);
			}
			else {
				s_ubCubeCrossSide = CROSS_SIDE_COUNT;
			}
		}
		else {
			undrawCube(s_uwCubeX, s_uwCubeY);

			addAndClampCubeCoordValue(&s_uwCubeX, s_pCubeStep[s_ubCubeCrossSide][0], s_uwCubeDestinationPointX);
			addAndClampCubeCoordValue(&s_uwCubeY, s_pCubeStep[s_ubCubeCrossSide][1], s_uwCubeDestinationPointY);

			drawCube(s_uwCubeX, s_uwCubeY);
		}
	}
}

void addAndClampCubeCoordValue(UWORD *pCubeCoordValue, BYTE bCubeStep, UWORD uwMaxValue) {
	if (bCubeStep < 0) {
		*pCubeCoordValue += bCubeStep;

		if (*pCubeCoordValue < uwMaxValue) {
			*pCubeCoordValue = uwMaxValue;
		}
	}
	else if (0 < bCubeStep) {
		*pCubeCoordValue += bCubeStep;

		if (uwMaxValue < *pCubeCoordValue) {
			*pCubeCoordValue = uwMaxValue;
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

	/* Cut off horizontal sides of cursor at map edges */
	if (!s_ubMapCursorX) {
		uwX += CROSS_CURSOR_LEFT;
		ubWidth -= CROSS_CURSOR_LEFT;
		ubStartX = CROSS_CURSOR_LEFT;
	}
	else if (s_ubMapCursorX == (MAP_WIDTH - 1)) {
		ubWidth -= CROSS_CURSOR_RIGHT;
	}

	/* Cut off vertical sides of cursor at map edges */
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