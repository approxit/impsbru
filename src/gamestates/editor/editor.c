#include "gamestates/editor/editor.h"

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

#include "gamestates/menu/menu.h"

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
static UBYTE s_ubCubeMapStartPointX = 0;
static UBYTE s_ubCubeMapStartPointY = 0;
static UBYTE s_ubCubeMapStartPointCrossSide = 0;
static UBYTE s_ubCubeMapStartPointCrossSideAdjustRotation = 0;
static UBYTE s_ubCubeMapDestinationPointX = 0;
static UBYTE s_ubCubeMapDestinationPointY = 0;
static UBYTE s_ubCubeMapDestinationPointCrossSide = 0;
static UBYTE s_ubCubeMapDestinationPointCrossSideAdjustRotation = 0;
static UWORD s_uwCubeX = 0;
static UWORD s_uwCubeY = 0;
static UBYTE s_ubCubeMoveCrossSide = CROSS_SIDE_COUNT;
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

	g_sGameManager.pStateFirst->pView = g_pView;

	createCrossAtlas();
	createCubeAtlas();
	createEditorStepAtlas();

	blitRect(
		g_pBufferManager->pBuffer, 0, 0,
		WINDOW_SCREEN_WIDTH, WINDOW_SCREEN_HEIGHT,
		0
	);

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
		case EDITOR_STEP_SHARE:
			handleEditorStepShareActions();
			if (!handleMapSaveActions()) {
				handleMapLoadActions();
			}
			break;
		default:
			handleEditorStepActions();
	}

	handleMapLoadActions();
}

void gsEditorDestroy() {
	logWrite("gsEditorDestroy\n");

	blitRect(
		g_pBufferManager->pBuffer, 0, 0,
		WINDOW_SCREEN_WIDTH, WINDOW_SCREEN_HEIGHT,
		0
	);

	drawLogo();

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
	createAtlasFile(s_pEditorStepBitMapAtlas, EDITOR_STEP_SHARE_TEXT, "/data/editor_frames/share1.bm");
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
			gamePopState();
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

		undrawCursor();
		s_ubMapCursorX = g_ubMapStartPointX;
		s_ubMapCursorY = g_ubMapStartPointY;
		drawCursor();

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
		if (keyCheck(s_pKeysForCrossSide[ubKeyIndex]) || keyCheck(s_pKeysForMapCursor[ubKeyIndex])) {
			if (s_ubCubeMoveCrossSide == CROSS_SIDE_COUNT) {
			 	if (isCubeMovePossible(ubKeyIndex)) {
					setNewCubeDestination(ubKeyIndex);
					s_ubCubeMoveCrossSide = ubKeyIndex;
			 	}
			}
			else if (getOppositeCrossSide(s_ubCubeMoveCrossSide) == ubKeyIndex) {
				s_ubCubeMoveCrossSide = ubKeyIndex;

				swapCubePositions();
			}
		}
	}

	/* Cancel buttons handling */
	if (keyUse(KEY_ESCAPE) || keyUse(KEY_BACKSPACE)) {
		s_ubMapCursorX = g_ubMapDestinationPointX;
		s_ubMapCursorY = g_ubMapDestinationPointY;
		drawCursor();

		undrawCube(s_uwCubeX, s_uwCubeY);

		loadCubePositionsFromMap();

		drawMapStartPoint();

		--s_ubEditorStep;
		drawEditorStep();
	}
}

void handleEditorStepShareActions() {
	/* Cancel buttons handling */
	if (keyUse(KEY_ESCAPE) || keyUse(KEY_BACKSPACE)) {
		blitRect(
			g_pBufferManager->pBuffer, 0, 0,
			WINDOW_SCREEN_WIDTH, WINDOW_SCREEN_HEIGHT,
			0
		);

		drawMap();

		--s_ubEditorStep;
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

UBYTE handleMapLoadActions() {
	for (UBYTE ubKey = KEY_1; ubKey <= KEY_0; ++ubKey) {
		if (keyUse(ubKey)) {
			char szMapFilePath[255];
			sprintf(szMapFilePath, "/data/maps/%u.map", ubKey);

			if (loadMapFromFile(szMapFilePath)) {
				loadCubePositionsFromMap();

				undrawMap();
				drawMap();

				s_ubEditorStep = EDITOR_STEP_PLAY_TEST;
				drawEditorStep();

				return 1;
			}
		}
	}

	return 0;
}

UBYTE handleMapSaveActions() {
	for (UBYTE ubKey = KEY_1; ubKey <= KEY_0; ++ubKey) {
		if (keyCheck(ubKey)) {
			char szMapFilePath[255];
			sprintf(szMapFilePath, "/data/maps/%u.map", ubKey);

			if (keyCheck(KEY_LSHIFT) || keyCheck(KEY_RSHIFT)) {
				saveMapToFile(szMapFilePath);
				return 1;
			}
		}
	}

	return 0;
}

void loadCubePositionsFromMap() {
	s_ubCubeMapStartPointX = g_ubMapStartPointX;
	s_ubCubeMapStartPointY = g_ubMapStartPointY;
	s_ubCubeMapStartPointCrossSide = g_ubMapStartPointCrossSide;
	s_ubCubeMapStartPointCrossSideAdjustRotation = g_ubMapStartPointCrossSide;

	s_uwCubeX = getMapCrossX(s_ubCubeMapStartPointX) + g_pCubeCrossSideAdjust[s_ubCubeMapStartPointCrossSide][s_ubCubeMapStartPointCrossSide][0];
	s_uwCubeY = getMapCrossY(s_ubCubeMapStartPointX, s_ubCubeMapStartPointY) + g_pCubeCrossSideAdjust[s_ubCubeMapStartPointCrossSide][s_ubCubeMapStartPointCrossSide][1];

	s_uwCubeStartPointX = s_uwCubeX;
	s_uwCubeStartPointY = s_uwCubeY;

	s_ubCubeMoveCrossSide = CROSS_SIDE_COUNT;

	s_ubCubeMapDestinationPointX = 0;
	s_ubCubeMapDestinationPointX = 0;
	s_ubCubeMapDestinationPointCrossSide = 0;
	s_ubCubeMapDestinationPointCrossSideAdjustRotation = 0;

	s_uwCubeDestinationPointX = 0;
	s_uwCubeDestinationPointY = 0;
}

void setNewCubeDestination(UBYTE ubCrossSide) {
	s_ubCubeMapDestinationPointX = getMapNeighborX(s_ubCubeMapStartPointX, ubCrossSide);
	s_ubCubeMapDestinationPointY = getMapNeighborY(s_ubCubeMapStartPointX, s_ubCubeMapStartPointY, ubCrossSide);

	if (s_ubCubeMapStartPointCrossSideAdjustRotation == ubCrossSide) {
		UBYTE ubCrossCrossSideAdjustRotation = s_ubCubeMapStartPointCrossSideAdjustRotation;
		s_ubCubeMapStartPointCrossSideAdjustRotation = s_ubCubeMapStartPointCrossSide;
		s_ubCubeMapStartPointCrossSide = ubCrossCrossSideAdjustRotation;
	}

	if (getCrossSideState(g_pMapData[s_ubCubeMapDestinationPointX][s_ubCubeMapDestinationPointY], s_ubCubeMapStartPointCrossSideAdjustRotation)) { /* Cube will be next to the cross side */
		s_ubCubeMapDestinationPointCrossSide = getOppositeCrossSide(ubCrossSide);
	}
	else { /* Cube will be directly in cross side */
		s_ubCubeMapDestinationPointCrossSide = s_ubCubeMapStartPointCrossSideAdjustRotation;
	}

	s_ubCubeMapDestinationPointCrossSideAdjustRotation = s_ubCubeMapStartPointCrossSideAdjustRotation;

	s_uwCubeDestinationPointX = getMapCrossX(s_ubCubeMapDestinationPointX) + g_pCubeCrossSideAdjust[s_ubCubeMapDestinationPointCrossSide][s_ubCubeMapDestinationPointCrossSideAdjustRotation][0];
	s_uwCubeDestinationPointY = getMapCrossY(s_ubCubeMapDestinationPointX, s_ubCubeMapDestinationPointY) + g_pCubeCrossSideAdjust[s_ubCubeMapDestinationPointCrossSide][s_ubCubeMapDestinationPointCrossSideAdjustRotation][1];

	s_ubCubeMoveCrossSide = ubCrossSide;

//	drawCubeDebugCrossSide(
//		0,
//		WINDOW_SCREEN_HEIGHT - CROSS_HEIGHT,
//		s_ubCubeMapStartPointCrossSide,
//		s_ubCubeMapStartPointCrossSideAdjustRotation
//	);
//
//	drawCubeDebugCrossSide(
//		WINDOW_SCREEN_WIDTH - CROSS_WIDTH,
//		WINDOW_SCREEN_HEIGHT - CROSS_HEIGHT,
//		s_ubCubeMapDestinationPointCrossSide,
//		s_ubCubeMapDestinationPointCrossSideAdjustRotation
//	);
}

//void drawCubeDebugCrossSide(UWORD uwX, UWORD uwY, UBYTE ubCrossSide, UBYTE ubCrossSideAdjustRotation) {
//	undrawCross(uwX, uwY);
//	drawCrossAtlasIndex(uwX, uwY, CROSS_CENTER);
//
//	drawCubeAtlasIndex(
//		uwX + g_pCubeCrossSideAdjust[ubCrossSide][ubCrossSideAdjustRotation][0],
//		uwY + g_pCubeCrossSideAdjust[ubCrossSide][ubCrossSideAdjustRotation][1],
//		0
//	);
//
//	if (ubCrossSide & 1) {
//		drawCrossAtlasIndex(uwX, uwY, CROSS_CENTER);
//	}
//}

void swapCubePositions() {
	UBYTE ubCubeMapStartPointX = s_ubCubeMapStartPointX;
	UBYTE ubCubeMapStartPointY = s_ubCubeMapStartPointY;
	UBYTE ubCubeMapStartPointCrossSide = s_ubCubeMapStartPointCrossSide;
	UBYTE ubCubeMapStartPointCrossSideAdjustRotation = s_ubCubeMapStartPointCrossSideAdjustRotation;

	s_ubCubeMapStartPointX = s_ubCubeMapDestinationPointX;
	s_ubCubeMapStartPointY = s_ubCubeMapDestinationPointY;
	s_ubCubeMapStartPointCrossSide = s_ubCubeMapDestinationPointCrossSide;
	s_ubCubeMapStartPointCrossSideAdjustRotation = s_ubCubeMapDestinationPointCrossSideAdjustRotation;

	s_ubCubeMapDestinationPointX = ubCubeMapStartPointX;
	s_ubCubeMapDestinationPointY = ubCubeMapStartPointY;
	s_ubCubeMapDestinationPointCrossSide = ubCubeMapStartPointCrossSide;
	s_ubCubeMapDestinationPointCrossSideAdjustRotation = ubCubeMapStartPointCrossSideAdjustRotation;

	UWORD uwCubeStartPointX = s_uwCubeStartPointX;
	UWORD uwCubeStartPointY = s_uwCubeStartPointY;

	s_uwCubeStartPointX = s_uwCubeDestinationPointX;
	s_uwCubeStartPointY = s_uwCubeDestinationPointY;

	s_uwCubeDestinationPointX = uwCubeStartPointX;
	s_uwCubeDestinationPointY = uwCubeStartPointY;
}

UBYTE isCubeMovePossible(UBYTE ubCrossSide) {
	if (s_ubCubeMapStartPointCrossSide == s_ubCubeMapStartPointCrossSideAdjustRotation) { /* Cube is directly in cross side */
		return getCrossSideState(g_pMapData[s_ubCubeMapStartPointX][s_ubCubeMapStartPointY], ubCrossSide) && (getOppositeCrossSide(ubCrossSide) != s_ubCubeMapStartPointCrossSide);
	}
	else { /* Cube is next to the cross side */
		return (ubCrossSide == s_ubCubeMapStartPointCrossSide) || (ubCrossSide == s_ubCubeMapStartPointCrossSideAdjustRotation);
	}
}

void moveCube() {
	if (s_ubCubeMoveCrossSide == CROSS_SIDE_COUNT) {
		return;
	}

	// TODO: move continuation should make step in single function call
	if (isCubeMoveFinished()) {
		finishCubeMove();

		if ((s_ubCubeMapStartPointX == g_ubMapDestinationPointX) && (s_ubCubeMapStartPointY == g_ubMapDestinationPointY) && (s_ubCubeMapStartPointCrossSide == g_ubMapDestinationPointCrossSide)) {
			finishMap();
			return;
		}
	}
	else {
		moveCubeStep();
	}
}

UBYTE isCubeMoveFinished() {
	return (s_uwCubeX == s_uwCubeDestinationPointX) && (s_uwCubeY == s_uwCubeDestinationPointY);
}

void finishCubeMove() {
	s_ubCubeMapStartPointX = s_ubCubeMapDestinationPointX;
	s_ubCubeMapStartPointY = s_ubCubeMapDestinationPointY;
	s_ubCubeMapStartPointCrossSide = s_ubCubeMapDestinationPointCrossSide;
	s_ubCubeMapStartPointCrossSideAdjustRotation = s_ubCubeMapDestinationPointCrossSideAdjustRotation;

	s_uwCubeStartPointX = s_uwCubeDestinationPointX;
	s_uwCubeStartPointY = s_uwCubeDestinationPointY;

	UBYTE ubNoOtherDirections = 1;
	for (UBYTE ubCrossSide = 0; ubCrossSide < CROSS_SIDE_COUNT; ++ubCrossSide) {
		if (isCubeMovePossible(ubCrossSide) && (ubCrossSide != s_ubCubeMoveCrossSide) && (ubCrossSide != getOppositeCrossSide(s_ubCubeMoveCrossSide))) {
			ubNoOtherDirections = 0;
			break;
		}
	}

	if (ubNoOtherDirections && getCrossSideState(g_pMapData[s_ubCubeMapStartPointX][s_ubCubeMapStartPointY], s_ubCubeMoveCrossSide) && (s_ubCubeMapStartPointCrossSide == s_ubCubeMapStartPointCrossSideAdjustRotation)) {
		setNewCubeDestination(s_ubCubeMoveCrossSide);
	}
	else {
		s_ubCubeMoveCrossSide = CROSS_SIDE_COUNT;
	}
}

void moveCubeStep() {
	undrawCube(s_uwCubeX, s_uwCubeY);

	makeCubeStepToDestination(&s_uwCubeX, CUBE_STEP_X, s_uwCubeDestinationPointX);
	makeCubeStepToDestination(&s_uwCubeY, s_uwCubeX == s_uwCubeDestinationPointX ? CUBE_STEP_Y_FAST : CUBE_STEP_Y_SLOW, s_uwCubeDestinationPointY);

	drawCube(s_uwCubeX, s_uwCubeY);
//	redrawCrossDepth(
//		s_ubCubeMapStartPointX,
//		s_ubCubeMapStartPointY,
//		s_ubCubeMapStartPointCrossSide,
//		s_ubCubeMapStartPointCrossSideAdjustRotation
//	);
//
//	redrawCrossDepth(
//		s_ubCubeMapDestinationPointX,
//		s_ubCubeMapDestinationPointY,
//		s_ubCubeMapDestinationPointCrossSide,
//		s_ubCubeMapDestinationPointCrossSideAdjustRotation
//	);
}

void finishMap() {
	undrawMap();

	blitCopyAligned(
		s_pEditorStepBitMapAtlas[EDITOR_STEP_SHARE_TEXT], 0, 0,
		g_pBufferManager->pBuffer, 0, 115,
		EDITOR_STEP_WIDTH, 32
	);

	++s_ubEditorStep;
	drawEditorStep();
}

void redrawCrossDepth(UBYTE ubCrossMapX, UBYTE ubCrossMapY, UBYTE ubCubeCrossSideAdjustRotation, UBYTE ubCubeCrossSideAdjustRotation) {
	UBYTE ubCubeDepth = getCubeDepth(ubCubeCrossSideAdjustRotation, ubCubeCrossSideAdjustRotation);

	UWORD uwCubeStartPointX = getMapCrossX(ubCrossMapX);
	UWORD uwCubeStartPointY = getMapCrossY(ubCrossMapX, ubCrossMapY);
	UBYTE ubCrossData = g_pMapData[ubCrossMapX][ubCrossMapY];

	if (ubCubeDepth <= CUBE_DEPTH_BOTTOM) {
		drawCrossCenter(uwCubeStartPointX, uwCubeStartPointY, ubCrossData);
		drawCrossDepthMiddle(uwCubeStartPointX, uwCubeStartPointY, ubCrossData);
	}

	if (ubCubeDepth <= CUBE_DEPTH_MIDDLE) {
		drawCrossDepthTop(uwCubeStartPointX, uwCubeStartPointY, ubCrossData);
	}

	if ((ubCrossMapX == g_ubMapDestinationPointX) && (ubCrossMapY == g_ubMapDestinationPointY)) {
		drawMapDestinationPoint();
	}
}

void makeCubeStepToDestination(UWORD *pCubeCoordValue, UBYTE ubCubeStep, UWORD ubDestinationValue) {
	if (*pCubeCoordValue < ubDestinationValue) {
		*pCubeCoordValue += ubCubeStep;

		if (ubDestinationValue < *pCubeCoordValue) {
			*pCubeCoordValue = ubDestinationValue;
		}
	}
	else if (ubDestinationValue < *pCubeCoordValue) {
		*pCubeCoordValue -= ubCubeStep;

		if (*pCubeCoordValue < ubDestinationValue) {
			*pCubeCoordValue = ubDestinationValue;
		}
	}
}

void toggleCrossSideState(UBYTE ubCrossXIndex, UBYTE ubCrossYIndex, UBYTE ubCrossSide) {
	g_pMapData[ubCrossXIndex][ubCrossYIndex] ^= 1 << ubCrossSide;

	UWORD uwX = getMapCrossX(s_ubMapCursorX);
	UWORD uwY = getMapCrossY(s_ubMapCursorX, s_ubMapCursorY);

	undrawCross(uwX, uwY);
	drawCross(uwX, uwY, g_pMapData[s_ubMapCursorX][s_ubMapCursorY]);
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