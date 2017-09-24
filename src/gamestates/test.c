#include "gamestates/test.h"

#include <ace/managers/log.h>
#include <ace/managers/game.h>
#include <ace/managers/key.h>
#include <ace/managers/viewport/simplebuffer.h>

#include <ace/utils/extview.h>
#include <ace/utils/palette.h>

static tView *s_pView;
static tVPort *s_pVPort;
static tSimpleBufferManager *s_pBuffer;

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

	s_pBuffer = simpleBufferCreate(0,
		TAG_SIMPLEBUFFER_VPORT, s_pVPort,
		TAG_DONE
	);

	paletteLoad("../data/impsbru.plt", s_pVPort->pPalette, 1 << WINDOW_SCREEN_BPP);

	viewLoad(s_pView);
}

void gsTestLoop() {
	if (keyUse(KEY_ESCAPE)) {
		gameClose();
	}
}

void gsTestDestroy() {
	logWrite("gsTestDestroy\n");

	viewDestroy(s_pView);
}