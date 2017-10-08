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
#define EDITOR_STEP_HEIGHT 32

void gsEditorCreate();
void gsEditorLoop();
void gsEditorDestroy();

void createEditorStepAtlas();
void destroyEditorStepAtlas();

void handleMapEditActions();
void handleMapCursorActions();
void handleEditorStepActions();
void handleMapLoadSaveActions();

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