#ifndef GUARD_IMPSBRU_GAMESTATES_EDITOR_H
#define GUARD_IMPSBRU_GAMESTATES_EDITOR_H

#include <ace/types.h>

#define EDITOR_STEP_CREATE 0
#define EDITOR_STEP_START_POINT 1
#define EDITOR_STEP_DESTINATION_POINT 2
#define EDITOR_STEP_PLAY_TEST 3
#define EDITOR_STEP_SHARE 4
#define EDITOR_STEP_COUNT 5
#define EDITOR_STEP_ATLAS_SIZE 5

#define EDITOR_STEP_WIDTH WINDOW_SCREEN_WIDTH
#define EDITOR_STEP_HEIGHT 22

void gsEditorCreate();
void gsEditorLoop();
void gsEditorDestroy();

void createEditorStepAtlas();
void destroyEditorStepAtlas();

void handleEditorStepCreateActions();
void handleEditorStepStartPointActions();
void handleEditorStepDestinationPointActions();
void handleEditorStepPlayTestActions();
void handleMapCursorActions();
void handleEditorStepActions();
void handleMapLoadSaveActions();

void loadCubePositionsFromMap();
void setNewCubeDestination(UBYTE ubCrossSide);
void swapCubePositions();

void makeCubeStepToDestination(
	INOUT UWORD *pCubeCoordValue,
	IN UBYTE ubCubeStep,
	IN UWORD ubDestinationValue
);

//void drawCubeDebugCrossSide(
//	IN UWORD uwX,
//	IN UWORD uwY,
//	IN UBYTE ubCrossSide,
//	IN UBYTE ubCrossSideAdjustRotation
//);

UBYTE isCubeMovePossible(
	IN UBYTE ubCrossSide
);

void moveCube();

void toggleCrossSideState(
	IN UBYTE ubCrossXIndex,
	IN UBYTE ubCrossYIndex,
	IN UBYTE ubCrossSide
);

void toggleNeighborCrossSideState(
	IN UBYTE ubCrossXIndex,
	IN UBYTE ubCrossYIndex,
	IN UBYTE ubCrossSide
);

void drawEditorStep();
void drawCursor();
void undrawCursor();

#endif