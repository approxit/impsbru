#include "game.h"

#include <stdio.h>

#include <ace/types.h>
#include <ace/generic/main.h>
#include <ace/managers/game.h>
#include <ace/managers/joy.h>

#include "gamestates/test.h"

const UWORD WINDOW_SCREEN_WIDTH = 320;
const UWORD WINDOW_SCREEN_HEIGHT = 256;
const UWORD WINDOW_SCREEN_BPP = 4;

void genericCreate(void) {
	joyOpen();

	gamePushState(gsTestCreate, gsTestLoop, gsTestDestroy);
}

void genericProcess(void) {
	keyProcess();
	joyProcess();
}

void genericDestroy(void) {
	joyClose();
}