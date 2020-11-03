#include <stdio.h>
#include <stdlib.h>
#include <psx.h>
#include "fight.h"

unsigned int distancia(int x1, int y1, int x2, int y2) {
	unsigned int DisX, DisY;
	DisX = absf(x1-x2);
	DisY = absf(y1-y2);
	return DisX + DisY;
}

_Bool collisionRectangle(int hspeed, int vspeed, GsRectangle *Temp, GsRectangle *Temp2) {
	return (Temp->x+hspeed < Temp2->x+Temp2->w 
	&& Temp->x+hspeed+Temp->w > Temp2->x 
	&& Temp->y+vspeed < Temp2->y+Temp2->h 
	&& Temp->y+vspeed+Temp->h > Temp2->y);
}

int absf(int a) {
	return (a < 0) ? (-(a)) : a;
}

int clamp(int x, int a, int b) {
	return (x < a) ? a : (x > b) ? b : x;
}

char dir(int a) {
	return (a > 0) ? 1 : (a < 0) ? -1 : 0;
}
