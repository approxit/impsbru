#include "game.h"

#include <stdio.h>

#include <ace/types.h>
#include <ace/generic/main.h>
#include <ace/managers/game.h>
#include <ace/managers/viewport/simplebuffer.h>
#include <ace/utils/extview.h>
#include <ace/utils/palette.h>

#include "gamestates/menu/menu.h"

const UWORD WINDOW_SCREEN_WIDTH = 320;
const UWORD WINDOW_SCREEN_HEIGHT = 256;
const UWORD WINDOW_SCREEN_BPP = 4;

tView *g_pView;
tVPort *g_pVPort;
tSimpleBufferManager *g_pBufferManager;

void genericCreate() {
	g_pView = (tView *) viewCreate(0,
		TAG_VIEW_GLOBAL_CLUT, 1,
		TAG_DONE
	);

	copBlockDisableSprites(g_pView->pCopList, 0xFF);

	g_pVPort = vPortCreate(0,
		TAG_VPORT_VIEW, g_pView,
		TAG_DONE
	);

	g_pBufferManager = simpleBufferCreate(0,
		TAG_SIMPLEBUFFER_VPORT, g_pVPort,
		TAG_DONE
	);

	paletteLoad("/data/impsbru.plt", g_pVPort->pPalette, 1 << WINDOW_SCREEN_BPP);

	viewLoad(g_pView);

	gamePushState(gsMenuCreate, gsMenuLoop, gsMenuDestroy);
}

void genericProcess() {
	keyProcess();
	gameProcess();
	WaitTOF();
}

void genericDestroy() {
	viewDestroy(g_pView);
}