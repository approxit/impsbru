#include "main.h"

#include <ace/managers/memory.h>
#include <ace/managers/log.h>
#include <ace/managers/timer.h>
#include <ace/managers/window.h>
#include <ace/managers/blit.h>
#include <ace/managers/game.h>

#include "config.h"
#include "input.h"

int main(void) {
	memCreate();
	logOpen();
	timerCreate();
	
	windowCreate();
	blitManagerCreate(0, 0);
	
	inputOpen();
	
	gameCreate(gsMenuCreate, gsMenuLoop, gsMenuDestroy);
	while (gameIsRunning()) {
		timerProcess();
		inputProcess();
		gameProcess();
	}
	gameDestroy();
	
	inputClose();
	
	blitManagerDestroy();
	windowDestroy();

	timerDestroy();
	logClose();
	memDestroy();
	return EXIT_SUCCESS;
}