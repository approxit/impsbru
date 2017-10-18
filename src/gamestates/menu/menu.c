#include "gamestates/menu/menu.h"

#include <ace/managers/log.h>
#include <ace/managers/key.h>
#include <ace/managers/game.h>
#include <ace/utils/bitmap.h>

#include "game.h"

#include "gamestates/editor/editor.h"

void gsMenuCreate() {
	logWrite("gsMenuCreate\n");

	g_sGameManager.pStateFirst->pView = g_pView;

	bitmapLoadFromFile(g_pBufferManager->pBuffer, "/data/menu_frames/logo.bm", 0, 0);
}

void gsMenuLoop() {
	if (keyUse(KEY_RETURN) || keyUse(KEY_SPACE)) {
		gamePushState(gsEditorCreate, gsEditorLoop, gsEditorDestroy);
	}

	if (keyUse(KEY_ESCAPE) || keyUse(KEY_BACKSPACE)) {
		gameClose();
	}
}

void gsMenuDestroy() {
	logWrite("gsMenuDestroy\n");
}

void drawLogo() {
	bitmapLoadFromFile(g_pBufferManager->pBuffer, "/data/menu_frames/logo.bm", 0, 0);
}