#include "input.h"

/* Globals */

/* Functions */
void inputOpen() {
	joyOpen();
}

void inputProcess() {
	keyProcess();
	joyProcess();
}

void inputClose() {
	joyClose();
}