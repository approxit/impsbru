#ifndef GUARD_IMPSBRU_GAMESTATES_EDITOR_H
#define GUARD_IMPSBRU_GAMESTATES_EDITOR_H

#include <ace/types.h>

void gsEditorCreate();
void gsEditorLoop();
void gsEditorDestroy();

void handleMapEditActions();
void handleMapCursorActions();
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

void drawCursor();
void undrawCursor();

#endif