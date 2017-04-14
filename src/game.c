#include "game.h"

const UWORD WINDOW_SCREEN_WIDTH = 320;
const UWORD WINDOW_SCREEN_HEIGHT = 256;
const UWORD WINDOW_SCREEN_BPP = 5;

void gameRunBefore() {
	joyOpen();

	gamePushState(gsTestCreate, gsTestLoop, gsTestDestroy);
}

void gameRunProcess() {
	keyProcess();
	joyProcess();
}

void gameRunAfter() {
	joyClose();
}