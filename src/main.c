#include <stdio.h>
#include <stdlib.h>
#include <psx.h>
#include "../lib/fight.h"

#define gravity 1
#define ANCHO 320
#define ALTO 240

#define irandom_range(a, b) ((rand()%((absf(a-b))+1))+a)
#define room_goto(a) room = a

#define instanceCreate(c, d) \
	if (d.Ini == NULL) { \
		d.Ini = (struct c *) malloc(sizeof(struct c)); \
		d.Ini->Ant = NULL; \
		d.Fin = d.Ini; \
	} \
	else { \
		d.Fin->Sig = (struct c *) malloc(sizeof(struct c)); \
		d.Fin->Sig->Ant = d.Fin; \
		d.Fin = d.Fin->Sig; \
	} \
	d.Fin->Sig = NULL;

#define instanceDestroy(a, b) \
	if (b.Ini->Sig == NULL) { \
		free(b.Ini); \
		b.Ini = NULL; \
		b.Fin = NULL; \
	} \
	else if (a->Sig == b.Fin) { \
		b.Fin->Ant = a->Ant; \
		*a = *b.Fin; \
		free(b.Fin); \
		b.Fin = a; \
	} \
	else if (a->Sig != NULL) { \
		a->Sig->Ant = a->Ant; \
		*a = *a->Sig; \
		free(a->Sig->Ant); \
		a->Sig->Ant = a;\
	} \
	else { \
		b.Fin = b.Fin->Ant; \
		free(b.Fin->Sig); \
		b.Fin->Sig = NULL; \
	}

#define crearBala(_a, _b) \
	instanceCreate(bala, globalBalas); \
	globalBalas.Fin->mascara.x = _a; \
	globalBalas.Fin->mascara.y = _b; \
	globalBalas.Fin->mascara.h = 5; \
	globalBalas.Fin->mascara.w = 5; \
	globalBalas.Fin->mascara.r = 0; \
	globalBalas.Fin->mascara.g = 255;\
	globalBalas.Fin->mascara.b = 0;\
	globalBalas.Fin->mascara.attribute = 80;\
	globalBalas.Fin->hspeed = (Player->xscale * Player->potencia);\
	globalBalas.Fin->vspeed = -Player->potencia;\

#define reset() \
	player[0].jumps = 0; \
	player[0].mascara.x = 50; \
	player[0].mascara.y = 160; \
	player[0].mascara.h = 25; \
	player[0].mascara.w = 10; \
	player[0].mascara.r = 0; \
	player[0].mascara.g = 0; \
	player[0].mascara.b = 255; \
	player[0].mascara.attribute = 0; \
	player[0].xscale = 1; \
	 \
	player[0].barraSangre.x = 10; \
	player[0].barraSangre.y = 10; \
	player[0].barraSangre.w = 100; \
	player[0].barraSangre.h = 8; \
	player[0].barraSangre.r = 0; \
	player[0].barraSangre.g = 0; \
	player[0].barraSangre.b = 255; \
	player[0].barraSangre.attribute = 0; \
	 \
	player[1].jumps = 0; \
	player[1].mascara.x = 270; \
	player[1].mascara.y = 160; \
	player[1].mascara.h = 25; \
	player[1].mascara.w = 10; \
	player[1].mascara.r = 255; \
	player[1].mascara.g = 0; \
	player[1].mascara.b = 0; \
	player[1].mascara.attribute = 0; \
	player[1].xscale = 1; \
	 \
	player[1].barraSangre.x = 180; \
	player[1].barraSangre.y = 10; \
	player[1].barraSangre.w = 100; \
	player[1].barraSangre.h = 8; \
	player[1].barraSangre.r = 255; \
	player[1].barraSangre.g = 0; \
	player[1].barraSangre.b = 0; \
	player[1].barraSangre.attribute = 0; \
	 \
	while(globalBalas.Ini != NULL) { \
		instanceDestroy(globalBalas.Fin, globalBalas); \
	} \
	player[1].xscale = -1; \
	player[0].potencia = 5; \
	player[1].potencia = 5; \
	player[0].barraEnergia.x = 10; \
	player[0].barraEnergia.y = 20; \
	player[0].barraEnergia.w = 100; \
	player[0].barraEnergia.h = 8; \
	player[0].barraEnergia.r = 255; \
	player[0].barraEnergia.g = 0; \
	player[0].barraEnergia.b = 255; \
	player[1].barraEnergia.x = 180; \
	player[1].barraEnergia.y = 20; \
	player[1].barraEnergia.w = 100; \
	player[1].barraEnergia.h = 8; \
	player[1].barraEnergia.r = 255; \
	player[1].barraEnergia.g = 0; \
	player[1].barraEnergia.b = 255;

#define crearBlock(_a, _b, _c, _d) \
	instanceCreate(block, globalBLocks); \
	globalBLocks.Fin->mascara.x = _a; \
	globalBLocks.Fin->mascara.y = _b; \
	globalBLocks.Fin->mascara.w = _c; \
	globalBLocks.Fin->mascara.h = _d; \
	globalBLocks.Fin->mascara.r = 0; \
	globalBLocks.Fin->mascara.g = 255; \
	globalBLocks.Fin->mascara.b = 0; \
	globalBLocks.Fin->mascara.attribute = 0;

#define controlPSelect(_a) \
	if (padCheck(_a, PAD_UP)) { \
		if (!PJ[_a].press_up) { \
			PJ[_a].press_up = true; \
			PJ[_a].select = clamp(PJ[_a].select - 1, 0, 5); \
		} \
	} \
	else { \
		PJ[_a].press_up = false; \
	} \
	if (padCheck(_a, PAD_DOWN)) { \
		if (!PJ[_a].press_down) { \
			PJ[_a].press_down = true; \
			PJ[_a].select = clamp(PJ[_a].select + 1, 0, 5); \
		} \
	} \
	else { \
		PJ[_a].press_down = false; \
	} \
	if (padCheck(_a, PAD_CROSS)) { \
		if (!PJ[_a].press_cross) { \
			PJ[_a].press_cross = true; \
			player[_a].name = PJ[_a].select; \
		} \
	} \
	else { \
		PJ[_a].press_cross = false; \
	}

unsigned int primeList[0x4000];
volatile int displayOld = 1;
volatile int timeCounter = 0;
int dbuf = 0;
unsigned short padbuf;
unsigned short padbuf2;
void blankHandler();

unsigned char room = MENU; // Rom de inicio
unsigned char state = 0;
unsigned char alarm = 0; // Alarma global

_Bool padCheck(int, const int);
void controlPlayer(struct jugador *, const int);
void controlIA(struct jugador *, const int);
void movePlayer(struct jugador *, const int);
void collisionBlock(struct jugador *, struct block *);
void collisionBala(struct jugador *, struct bala *Bala);
void logicaEscenario();
void logicaBala();

struct listaDeBlocks globalBLocks;
struct jugador player[2];
struct listaBalas globalBalas;
struct PJMenus PJ[2];
struct nombres nom[7] = {"RAMBO", "ROBOT", "CYBOR", "NINJA", "MARCO", "ALDO", "DIOS"};

struct imgbuf {
	unsigned char imagenBuffer[0x40000];
};

struct spr {
	GsSprite my_sprite;
	GsImage my_image;
	int ro;
	int sc_x;
	int sc_y;
};

#define addSprite(_a, _b) \
	Sprite[_a]->ro = 0; \
	Sprite[_a]->sc_x = SCALE_ONE; \
	Sprite[_a]->sc_y = SCALE_ONE; \
	f = fopen(_b, "rb"); \
	fseek(f, 0, SEEK_END); \
	y = ftell(f); \
	Menu = (struct imgbuf *) malloc(sizeof(struct imgbuf)); \
	fseek(f, 0, SEEK_SET); \
	fread(Menu->imagenBuffer, sizeof(char), y, f); \
	fclose(f); \
	 \
	Sprite[_a] = (struct spr *) malloc(sizeof(struct spr)); \
	GsImageFromTim(&Sprite[_a]->my_image, Menu->imagenBuffer); \
	GsSortCls(0, 0, 0); \
	GsSpriteFromImage(&Sprite[_a]->my_sprite, &Sprite[_a]->my_image, 1); \
	free(Menu); \


	unsigned char mode;


int main() {
	PSX_Init();
	GsInit();
	GsSetList(primeList);
	GsClearMem();
	GsSetVideoMode(ANCHO, ALTO, VMODE_PAL);
	GsLoadFont(768, 0, 768, 256);
	SetVBlankHandler(blankHandler);

	// 
	FILE *f;
	int y = 0;
	struct spr *Sprite[20];
	struct imgbuf *Menu;
	addSprite(0, "cdrom:\\LOGO.TIM;1");
	
	reset();
	
	// Escenario temporal
	crearBlock(0, 0, 56, 16);
	crearBlock(104, 0, 112, 16);
	crearBlock(264, 0, 56, 16);

	crearBlock(0, 224, 56, 16);
	crearBlock(104, 224, 112, 16);
	crearBlock(264, 224, 56, 16);
	
	crearBlock(0, 16, 16, 80);
	crearBlock(0, 144, 16, 80);
	
	crearBlock(304, 16, 16, 80);
	crearBlock(304, 144, 16, 80);
	
	crearBlock(16, 80, 16, 16);
	crearBlock(16, 144, 16, 16);
	
	crearBlock(288, 80, 16, 16);
	crearBlock(288, 144, 16, 16);
	
	crearBlock(64, 120, 64, 16);
	crearBlock(192, 120, 64, 16);
	
	PJ[0].opcion = 0;
	PJ[1].opcion = 0;

	player[0].name = NOONE;
	player[1].name = NOONE;
	PJ[1].select = 1;
	player[1].alarm = 0;
	unsigned char i;


	GsRectangle INTRO[2];
	INTRO[0].x = 120;
	INTRO[0].y = 100;
	INTRO[0].w = 10;
	INTRO[0].h = 10;
	INTRO[0].r = 0;
	INTRO[0].g = 0;
	INTRO[0].b = 255;
	INTRO[0].attribute = 0;

	INTRO[1].x = 160;
	INTRO[1].y = 100;
	INTRO[1].w = 10;
	INTRO[1].h = 10;
	INTRO[1].r = 255;
	INTRO[1].g = 0;
	INTRO[1].b = 0;
	INTRO[1].attribute = 0;

	char hspeed[2] = {3, -3};
	char vspeed[2] = {3, -3};

	Sprite[0]->my_sprite.x = 50;
	Sprite[0]->my_sprite.y = 10;
	Sprite[0]->my_sprite.r = Sprite[0]->my_sprite.g = Sprite[0]->my_sprite.b = NORMAL_LUMINOSITY;

	while(1) {		
		if(displayOld) {
			dbuf = !dbuf;
			GsSetDispEnvSimple(0, dbuf ? 0: 256);
			GsSetDrawEnvSimple(0, dbuf ? 256 : 0, 320, 240);
			GsSortCls(0, 0, 0);
			PSX_ReadPad(&padbuf, &padbuf2);
			switch(room) {
				case MENU:
					// logo 1
					/*
					// logo 2
					Sprite[1]->my_sprite.x = 30;
					Sprite[1]->my_sprite.y = 30;
					Sprite[1]->my_sprite.r = Sprite[0]->my_sprite.g = Sprite[0]->my_sprite.b = NORMAL_LUMINOSITY;*/
					/*Sprite->my_sprite.rotate = ROTATE_ONE*ro;
					Sprite->my_sprite.scalex = sc_x;
					Sprite->my_sprite.scaley = sc_y;
					Sprite->my_sprite.mx = (my_sprite.w/2) * (my_sprite.scalex / 4096);
					Sprite->my_sprite.my = (my_sprite.h/2) * (my_sprite.scaley / 4096);*/
					/*GsSortSprite(&Sprite[1]->my_sprite);*/

					GsSortRectangle(&INTRO[0]);
					GsSortRectangle(&INTRO[1]);

					if (collisionRectangle(hspeed[0], vspeed[0], &INTRO[0], &INTRO[1])) {
						hspeed[0] *= -1;
						vspeed[0] *= -1;
					}
					if (collisionRectangle(hspeed[1], vspeed[1], &INTRO[1], &INTRO[0])) {
						hspeed[1] *= -1;
						vspeed[1] *= -1;
					}
					if (INTRO[0].x > 310 || INTRO[0].x < 0) {
						hspeed[0] *= -1;
					}
					if (INTRO[1].x > 310 || INTRO[1].x < 0) {
						hspeed[1] *= -1;
					}

					if (INTRO[0].y > 230 || INTRO[0].y < 0) {
						vspeed[0] *= -1;
					}
					if (INTRO[1].y > 230 || INTRO[1].y < 0) {
						vspeed[1] *= -1;
					}

					INTRO[0].x += hspeed[0];
					INTRO[0].y += vspeed[0];

					INTRO[1].x += hspeed[1];
					INTRO[1].y += vspeed[1];


					// Menu level
					//GsPrintFont(10, 10, "MENU");
					//GsPrintFont(20, 30, "Un jugador");
					if (player[1].alarm++ < 20) {
					GsPrintFont(70, 110, "PRESIONE X PARA COMENZAR");
					}else {
						if (player[1].alarm >= 40) {
							player[1].alarm = 0;
						}
					}

					GsSortSprite(&Sprite[0]->my_sprite);
					/*
					GsPrintFont(10, 30 + (PJ[0].opcion*10), ">");
				
					if (padCheck(0, PAD_UP)) {
						if (!PJ[0].press_up) {
							PJ[0].press_up = true;
							PJ[0].opcion = clamp(PJ[0].opcion - 1, 0, 1);
						}
					}
					else {
						PJ[0].press_up = false;
					}
					if (padCheck(0, PAD_DOWN)) {
						if (!PJ[0].press_down) {
							PJ[0].press_down = true;
							PJ[0].opcion = clamp(PJ[0].opcion + 1, 0, 1);
						}
					}
					else {
						PJ[0].press_down = false;
					}*/
					if (padCheck(0, PAD_CROSS)) {
						if (!PJ[0].press_cross) {
							PJ[0].press_cross = true;
							mode = PJVSPJ;
							room_goto(PLAYER_SELECT);
						}
					}
					else {
						PJ[0].press_cross = false;
					}
				break;

				case PLAYER_SELECT:
					GsPrintFont(10, 10, "Seleccione Personaje");
					for(i = 0; i < 6; i++) {
						GsPrintFont(20, 30+(i*10), "%s", nom[i].n);
					}
					switch(mode) {
						case PJVSCOM:
							GsPrintFont(10, 30+(PJ[0].select*10), ">");
							if (player[0].name != NOONE) {
								player[1].name = irandom_range(0, 6);
								room_goto(ROM1);
							}
							else {	
								controlPSelect(0);
							}
						break;
						case PJVSPJ:
							if (player[0].name != NOONE && player[1].name != NOONE) {
								room_goto(ROM1);
							} else {
								if (player[0].name == NOONE) {
									GsPrintFont(10, 30+(PJ[0].select*10), "1");
									controlPSelect(0);
								}
								if (player[1].name == NOONE) {
									GsPrintFont(60, 30+(PJ[1].select*10), "2");
									controlPSelect(1);
								}
							}
						break;
					}
				break;


				case ROM1:
					logicaEscenario();
					if (padCheck(0, PAD_SELECT) || padCheck(1, PAD_SELECT)) {
						room_goto(PLAYER_SELECT);
						reset();	
						player[0].name = NOONE;
						player[1].name = NOONE;
						PJ[1].select = 1;
					}
				break;

				case GAMEOVER:
					GsPrintFont(100, 60, "%s %d WINS", (player[0].barraSangre.w > player[1].barraSangre.w) ? nom[player[0].name].n : nom[player[1].name].n, (player[0].barraSangre.w > player[1].barraSangre.w) ? 1 : 2);
					if (padCheck(0, PAD_START)) {
						reset();
						room_goto(ROM1);
					}
				break;
			}
			GsDrawList();
			while(GsIsDrawing());
			displayOld = 0;
		}
	}
	return 0;
}

void blankHandler() {
	displayOld = 1;
	timeCounter++;
}

_Bool padCheck(int a, const int b) {
	switch(a) {
		case 0: return (padbuf & b); break;
		case 1: return (padbuf2 & b); break;
	}
}

void controlPlayer(struct jugador *Player, const int control) {
	// Draw Player
	GsSortRectangle(&Player->mascara);

	if(padCheck(control, PAD_R1) && Player->barraEnergia.w < 100 && Player->jumps == 0) {
		Player->hspeed = 0;
		Player->barraEnergia.w++;
		if (control == 1) {
			Player->barraEnergia.x--;
		}
	} 
	else {

		// Controles Player
		if (padCheck(control, PAD_LEFT)) {
			Player->hspeed = -2;
			Player->xscale = -1;
		}
		else if (padCheck(control, PAD_RIGHT)) {
			Player->hspeed = 2;
			Player->xscale = 1;
		}
		else {
			Player->hspeed = 0;
		}

		if (padCheck(control, PAD_CROSS)) {
			if (!PJ[control].press_cross) {
				PJ[control].press_cross = true;
				if (Player->jumps++ < 2) {
					Player->vspeed = -12;
				}
			}
		}
		else {
			if (PJ[control].press_cross) {
				PJ[control].press_cross = false;
				if (Player->vspeed < -4) {
					Player->vspeed = -4;
				}
			}
		}
		
		switch(Player->name) {
			case ROBOT:
				//Crear bala
				if(padCheck(control, PAD_SQUARE)){
					if (!PJ[control].press_square) {
						PJ[control].press_square = true;
						crearBala(Player->mascara.x+(10*Player->xscale), Player->mascara.y);
						globalBalas.Fin->estado = DISPARO;
						globalBalas.Fin->hspeed = Player->xscale*9;
						globalBalas.Fin->creador = control;
						globalBalas.Fin->mascara.w = 15;
						globalBalas.Fin->mascara.h = 15;

					}
				}else{
					PJ[control].press_square = false;
				}

				if(padCheck(control, PAD_CIRCLE)) {
					if (!PJ[control].press_circle) {
						PJ[control].press_circle = true;
						if (Player->potencia < 10 && Player->barraEnergia.w >= 25) {
							crearBala(Player->mascara.x+(10*Player->xscale), Player->mascara.y);
							globalBalas.Fin->estado = GRANADA;
							globalBalas.Fin->hspeed = Player->xscale*10;
							globalBalas.Fin->creador = control;
							Player->barraEnergia.w-=25;
							if (control == 1) {
								Player->barraEnergia.x+=25;
							}
						}
					}
				}else{
					PJ[control].press_circle = false;
				}
	
				if(padCheck(control, PAD_TRIANGLE) && Player->potencia < 15 && Player->barraEnergia.w >= 50) {
					PJ[control].press_triangle = true;
					GsPrintFont(10,10,"%d",Player->potencia);
					GsRectangle carga = {Player->mascara.x,Player->mascara.y-10,Player->potencia,5,0,0,255,0};
					GsSortRectangle(&carga);
					if(globalBalas.Fin->alarm++ == 15 ){
						globalBalas.Fin->alarm = 0;
						Player->potencia++;
					}
				}else{
					if (PJ[control].press_triangle) {
						PJ[control].press_triangle = false;
						crearBala(Player->mascara.x+10, Player->mascara.y);
						globalBalas.Fin->estado = BASUCA;
						globalBalas.Fin->creador = control;
			
						Player->barraEnergia.w -= 50;
						if (control == 1) {
							Player->barraEnergia.x+=50;
						}
						globalBalas.Fin->alarm = 0;
						Player->potencia = 5;
					}
				}
			break;
			case RAMBO:
				//Crear bala
				if(padCheck(control, PAD_SQUARE)){
					if (!PJ[control].press_square) {
						PJ[control].press_square = true;
						crearBala(Player->mascara.x+(10*Player->xscale), Player->mascara.y);
						globalBalas.Fin->estado = DISPARO;
						globalBalas.Fin->hspeed = Player->xscale*9;
						globalBalas.Fin->creador = control;
					}
				}else{
					PJ[control].press_square = false;
				}

				if(padCheck(control, PAD_CIRCLE) && Player->potencia < 15 && Player->barraEnergia.w >= 25) {
					PJ[control].press_circle = true;
					GsPrintFont(10,10,"%d",Player->potencia);
					GsRectangle carga = {Player->mascara.x,Player->mascara.y-10,Player->potencia,5,0,0,255,0};
					GsSortRectangle(&carga);
					if(globalBalas.Fin->alarm++ == 15 ){
						globalBalas.Fin->alarm = 0;
						Player->potencia++;
					}
				}else{
					if (PJ[control].press_circle) {
						PJ[control].press_circle = false;
						crearBala(Player->mascara.x+10, Player->mascara.y);
						globalBalas.Fin->estado = GRANADA;
						globalBalas.Fin->creador = control;
	
						Player->barraEnergia.w -= 25;
						if (control == 1) {
							Player->barraEnergia.x += 25;
						}
						globalBalas.Fin->alarm = 0;
						Player->potencia = 5;
					}
				}

				if(padCheck(control, PAD_TRIANGLE) && Player->potencia < 15 && Player->barraEnergia.w >= 50) {
					PJ[control].press_triangle = true;
					GsPrintFont(10,10,"%d",Player->potencia);
					GsRectangle carga = {Player->mascara.x,Player->mascara.y-10,Player->potencia,5,0,0,255,0};
					GsSortRectangle(&carga);
					if(globalBalas.Fin->alarm++ == 15 ){
						globalBalas.Fin->alarm = 0;
						Player->potencia++;
					}
				}else{
					if (PJ[control].press_triangle) {
						PJ[control].press_triangle = false;
						crearBala(Player->mascara.x+10, Player->mascara.y);
						globalBalas.Fin->estado = BASUCA;
						globalBalas.Fin->creador = control;
	
						Player->barraEnergia.w -= 50;
						if (control == 1) {
							Player->barraEnergia.x+=50;
						}
						globalBalas.Fin->alarm = 0;
						Player->potencia = 5;
					}
				}
			break;
			case ALDO:
				//Crear bala
				if(padCheck(control, PAD_SQUARE)){
					if (!PJ[control].press_square) {
						PJ[control].press_square = true;
						switch(Player->xscale) {
							case 1: crearBala(Player->mascara.x+10, Player->mascara.y+15); break;
							case -1: crearBala(Player->mascara.x-25, Player->mascara.y+15); break;
						}
						globalBalas.Fin->estado = DISPARO;
						globalBalas.Fin->creador = control;
						globalBalas.Fin->hspeed = Player->xscale*9;
						globalBalas.Fin->mascara.h = 10;
						globalBalas.Fin->mascara.w = 25;
					}
				}else{
					PJ[control].press_square = false;
				}

				if(padCheck(control, PAD_CIRCLE) && Player->potencia < 15 && Player->barraEnergia.w >= 25) {
					if (!PJ[control].press_circle) {
						PJ[control].press_circle = true;
						crearBala(Player->mascara.x+((18+Player->hspeed)*Player->xscale), Player->mascara.y);
						globalBalas.Fin->estado = GRANADA;
						globalBalas.Fin->creador = control;
	
						Player->barraEnergia.w -= 25;
						if (control == 1) {
							Player->barraEnergia.x += 25;
						}

						globalBalas.Fin->alarm = 0;
						Player->potencia = 5;
						globalBalas.Fin->mascara.h = 10;
						globalBalas.Fin->mascara.w = 10;

						globalBalas.Fin->hspeed = Player->hspeed;
						globalBalas.Fin->xscale = Player->xscale;
					}
				}else{
					PJ[control].press_circle = false;
				}

				if(padCheck(control, PAD_TRIANGLE) && Player->potencia < 15 && Player->barraEnergia.w >= 50) {
					if (!PJ[control].press_triangle) {
						PJ[control].press_triangle = true;
						switch(Player->xscale) {
							case 1: crearBala(Player->mascara.x+10, Player->mascara.y-20); break;
							case -1: crearBala(Player->mascara.x-15, Player->mascara.y-20); break;
						}
						globalBalas.Fin->estado = BASUCA;
						globalBalas.Fin->creador = control;
	
						Player->barraEnergia.w -= 50;
						if (control == 1) {
							Player->barraEnergia.x+=50;
						}
						globalBalas.Fin->mascara.h = 45;
						globalBalas.Fin->mascara.w = 15;

						globalBalas.Fin->hspeed = Player->xscale*7;
					}
				}else{
					PJ[control].press_triangle = false;
				}
			break;
			case CYBOR:
				//Crear bala
				if(padCheck(control, PAD_SQUARE)){
					if (!PJ[control].press_square) {
						PJ[control].press_square = true;
						switch(Player->xscale) {
							case 1: crearBala(Player->mascara.x + 10, Player->mascara.y); break;
							case -1: crearBala(Player->mascara.x - 15, Player->mascara.y); break;
						}
						globalBalas.Fin->estado = DISPARO;
						globalBalas.Fin->hspeed = Player->xscale*9;
						globalBalas.Fin->creador = control;
						globalBalas.Fin->mascara.w = 15;
					}
				}else{
					PJ[control].press_square = false;
				}

				if(padCheck(control, PAD_CIRCLE) && Player->potencia < 15 && Player->barraEnergia.w >= 25) {
					if (!PJ[control].press_circle) {
						PJ[control].press_circle = true;
						switch(Player->xscale) {
							case 1: crearBala(Player->mascara.x+10, Player->mascara.y); break;
							case -1: crearBala(Player->mascara.x-15, Player->mascara.y); break;
						}
						globalBalas.Fin->estado = GRANADA;
						globalBalas.Fin->creador = control;
						globalBalas.Fin->mascara.h = 15;
						globalBalas.Fin->creador = control;
						Player->barraEnergia.w -= 25;
						if (control == 1) {
							Player->barraEnergia.x += 25;
						}
					}
				}else{
						PJ[control].press_circle = false;
				}

				if(padCheck(control, PAD_TRIANGLE) && Player->potencia < 15 && Player->barraEnergia.w >= 50) {
					PJ[control].press_triangle = true;
					GsPrintFont(10,10,"%d",Player->potencia);
					GsRectangle carga = {Player->mascara.x,Player->mascara.y-10,Player->potencia,5,0,0,255,0};
					GsSortRectangle(&carga);
					if(globalBalas.Fin->alarm++ == 15 ){
						globalBalas.Fin->alarm = 0;
						Player->potencia++;
					}
				}else{
					if (PJ[control].press_triangle) {
						PJ[control].press_triangle = false;
						crearBala(Player->mascara.x+10, Player->mascara.y);
						globalBalas.Fin->estado = BASUCA;
						globalBalas.Fin->creador = control;
						Player->barraEnergia.w -= 50;
						if (control == 1) {
							Player->barraEnergia.x+=50;
						}
						globalBalas.Fin->alarm = 0;
						Player->potencia = 5;
					}
				}
			break;
			case NINJA:
				//Crear bala
				if(padCheck(control, PAD_SQUARE)){
					if (!PJ[control].press_square) {
						PJ[control].press_square = true;
						switch(Player->xscale) {
							case 1: crearBala(Player->mascara.x+10, Player->mascara.y); break;
							case -1: crearBala(Player->mascara.x-20, Player->mascara.y); break;
						}
						globalBalas.Fin->estado = DISPARO;
						globalBalas.Fin->creador = control;
						globalBalas.Fin->mascara.w = 20;
						globalBalas.Fin->mascara.h = 20;
					}
				}else{
					PJ[control].press_square = false;
				}

				if(padCheck(control, PAD_CIRCLE) && Player->potencia < 15 && Player->barraEnergia.w >= 25) {
					if (!PJ[control].press_circle) {
						PJ[control].press_circle = true;
						Player->alarm = 15;
						Player->barraEnergia.w -= 25;
						if (control == 1) {
							Player->barraEnergia.x += 25;
						}
					}
				}else{
						PJ[control].press_circle = false;
				}
				// Generar balas
				if (Player->alarm == 5 || Player->alarm == 10 || Player->alarm == 15) {
						crearBala(Player->mascara.x+(10*Player->xscale), Player->mascara.y+5);
						globalBalas.Fin->hspeed = 9*Player->xscale;
						if (absf(globalBalas.Fin->mascara.y-player[(control == 0) ? 1 : 0].mascara.y) > 27) {
							globalBalas.Fin->vspeed = -dir(globalBalas.Fin->mascara.y-player[(control == 0) ? 1 : 0].mascara.y)*4;
						}
						else {
							globalBalas.Fin->vspeed = 0;
						}
						globalBalas.Fin->estado = GRANADA;
						globalBalas.Fin->creador = control;
				}
				if (Player->alarm > 0) {Player->alarm--;}

				if(padCheck(control, PAD_TRIANGLE) && Player->potencia < 15 && Player->barraEnergia.w >= 50) {
					PJ[control].press_triangle = true;
					GsPrintFont(10,10,"%d",Player->potencia);
					GsRectangle carga = {Player->mascara.x,Player->mascara.y-10,Player->potencia,5,0,0,255,0};
					GsSortRectangle(&carga);
					if(globalBalas.Fin->alarm++ == 15 ){
						globalBalas.Fin->alarm = 0;
						Player->potencia++;
					}
				}else{
					if (PJ[control].press_triangle) {
						PJ[control].press_triangle = false;
						crearBala(Player->mascara.x+10, Player->mascara.y);
						globalBalas.Fin->estado = BASUCA;
						globalBalas.Fin->creador = control;
						Player->barraEnergia.w -= 50;
						if (control == 1) {
							Player->barraEnergia.x+=50;
						}
						globalBalas.Fin->alarm = 0;
						Player->potencia = 5;
					}
				}
			break;
			case MARCO:
				//Crear bala
				if(padCheck(control, PAD_SQUARE)){
					if (!PJ[control].press_square) {
						PJ[control].press_square = true;
						switch(Player->xscale) {
							case 1: crearBala(Player->mascara.x+10, Player->mascara.y); break;
							case -1: crearBala(Player->mascara.x-20, Player->mascara.y); break;
						}
						globalBalas.Fin->estado = DISPARO;
						globalBalas.Fin->creador = control;
						globalBalas.Fin->mascara.w = 20;
						globalBalas.Fin->mascara.h = 20;
					}
				}else{
					PJ[control].press_square = false;
				}

				if(padCheck(control, PAD_CIRCLE) && Player->potencia < 15 && Player->barraEnergia.w >= 25) {
					if (!PJ[control].press_circle) {
						PJ[control].press_circle = true;
						Player->alarm = 20;
						Player->barraEnergia.w -= 25;
						if (control == 1) {
							Player->barraEnergia.x += 25;
						}
					}
				}else{
						PJ[control].press_circle = false;
				}
				// Generar balas
				if (Player->alarm == 5 || Player->alarm == 10 || Player->alarm == 15 || Player->alarm == 20) {
						crearBala(Player->mascara.x+(10*Player->xscale), Player->mascara.y+irandom_range(-10, 10));
						globalBalas.Fin->hspeed = 9*Player->xscale;
						globalBalas.Fin->vspeed = 0;
						globalBalas.Fin->estado = GRANADA;
						globalBalas.Fin->creador = control;
				}
				if (Player->alarm > 0) {Player->alarm--;}

				if(padCheck(control, PAD_TRIANGLE) && Player->potencia < 15 && Player->barraEnergia.w >= 50) {
					PJ[control].press_triangle = true;
					GsPrintFont(10,10,"%d",Player->potencia);
					GsRectangle carga = {Player->mascara.x,Player->mascara.y-10,Player->potencia,5,0,0,255,0};
					GsSortRectangle(&carga);
					if(globalBalas.Fin->alarm++ == 15 ){
						globalBalas.Fin->alarm = 0;
						Player->potencia++;
					}
				}else{
					if (PJ[control].press_triangle) {
						PJ[control].press_triangle = false;
						crearBala(Player->mascara.x+10, Player->mascara.y);
						globalBalas.Fin->estado = BASUCA;
						globalBalas.Fin->creador = control;
						Player->barraEnergia.w -= 50;
						if (control == 1) {
							Player->barraEnergia.x+=50;
						}
						globalBalas.Fin->alarm = 0;
						Player->potencia = 5;
					}
				}
			break;
		}
	}

	
	GsRectangle cara = {Player->mascara.x+2,Player->mascara.y-5,6,5,Player->mascara.r,Player->mascara.g,Player->mascara.b,0};
	GsSortRectangle(&cara);
	GsRectangle pistola = {Player->mascara.x+(5*Player->xscale),Player->mascara.y+2,10,4,230,230,230,0};
	GsSortRectangle(&pistola);

	// Limitar Velocidad Vertical
	if (Player->vspeed > 14) {
		Player->vspeed = 14;
	}

}

void movePlayer(struct jugador *Player, const int control) {
	// Fisica
	Player->vspeed += gravity;
	Player->mascara.x += Player->hspeed;
	Player->mascara.y += Player->vspeed;

	// Reaparecer
	if (Player->mascara.y+(Player->mascara.h/2) > GsScreenH) {
		Player->mascara.y -= GsScreenH-(Player->mascara.h/2);
	}

	if (Player->mascara.y+(Player->mascara.h/2) < 0) {
		Player->mascara.y += GsScreenH-(Player->mascara.h/2);
	}

	if (Player->mascara.x+(Player->mascara.w/2) > GsScreenW) {
		Player->mascara.x -= GsScreenW-(Player->mascara.w/2);
	}

	if (Player->mascara.x+(Player->mascara.w/2) < 0) {
		Player->mascara.x += GsScreenW-(Player->mascara.w/2);
	}
}

void collisionBlock(struct jugador *Player, struct block *Block) {
	// Collision Horizontal
	if (collisionRectangle(Player->hspeed, 0, &Player->mascara, &Block->mascara)) {
		switch(dir(Player->hspeed)) {
			case 1:
				Player->mascara.x = Block->mascara.x - Player->mascara.w;
				Player->hspeed = 0;
			break;

			case -1:
				Player->mascara.x = Block->mascara.x + Block->mascara.w;
				Player->hspeed = 0;
			break;
		}
	}
	// Collision Vertical
	if (collisionRectangle(0, Player->vspeed + gravity, &Player->mascara, &Block->mascara)) {
		switch(dir(Player->vspeed + gravity)) {
			case 1:
				Player->mascara.y = Block->mascara.y - Player->mascara.h;
				Player->vspeed = -gravity;
				Player->jumps = 0;
			break;
			case -1:
				Player->mascara.y = Block->mascara.y + Block->mascara.h;
				Player->vspeed = gravity;
			break;
		}
	}
	// Correccion de collision
	if (collisionRectangle(Player->hspeed, Player->vspeed, &Player->mascara, &Block->mascara)) {
		Player->hspeed = 0;
	}
}

void collisionBala(struct jugador *Player, struct bala *Bala) {
	unsigned char dano;
	// Collision con Player
	if (collisionRectangle(Bala->hspeed, Bala->vspeed, &Bala->mascara, &Player->mascara)
	|| collisionRectangle(Bala->hspeed/2, Bala->vspeed/2, &Bala->mascara, &Player->mascara)) {
		switch(Bala->estado) {
			case DISPARO: dano = 6; break;
			case GRANADA: 
				switch(player[Bala->creador].name) {
					case NINJA: MARCO: dano = 5; break;
					default: dano = 25; break;
				}
			break;
			case BASUCA: dano = 50; break;
		}
		if (Player->barraSangre.r == 255) {
			Player->barraSangre.x = clamp(Player->barraSangre.x + dano, 180, 280);
		}
		Player->barraSangre.w = clamp(Player->barraSangre.w - dano, 0, 100);
		if (Player->barraSangre.w <= 0) {
			room_goto(GAMEOVER);
		}
		instanceDestroy(Bala, globalBalas);
	}
}

void logicaEscenario() {
	controlPlayer(&player[0], 0);
	
		controlPlayer(&player[1], 1);
	/*if (mode == PJVSPJ) {
		controlPlayer(&player[1], 1);
	}
	else {
		controlIA(&player[1], 0);
	}*/

	struct block *TempBlock;
	TempBlock = globalBLocks.Ini;
	while(TempBlock != NULL) {
		// Draw Block
		GsSortRectangle(&TempBlock->mascara);

		// Collision
		collisionBlock(&player[0], TempBlock);
		collisionBlock(&player[1], TempBlock);

		TempBlock = TempBlock->Sig;
	}

	logicaBala();
	struct bala *Bala;
	Bala = globalBalas.Ini;
	while(Bala != NULL) {
		collisionBala(&player[0], Bala);
		collisionBala(&player[1], Bala);

		struct block *TempBlock;
		TempBlock = globalBLocks.Ini;
		while(TempBlock != NULL) {
			// Collision
			if (collisionRectangle(Bala->hspeed, Bala->vspeed, &Bala->mascara, &TempBlock->mascara)
			|| collisionRectangle(Bala->hspeed/2, Bala->vspeed/2, &Bala->mascara, &TempBlock->mascara)
			|| collisionRectangle(Bala->hspeed/4, Bala->vspeed/4, &Bala->mascara, &TempBlock->mascara)) {
				switch(player[Bala->creador].name) {
					case RAMBO:
						switch(Bala->estado) {
							case DISPARO: case BASUCA: instanceDestroy(Bala, globalBalas); break;
						}
					break;
					case ROBOT:
						switch(Bala->estado) {
							case DISPARO: case BASUCA: case GRANADA: instanceDestroy(Bala, globalBalas); break;
						}
					break;
					case ALDO:
						switch(Bala->estado) {
							case DISPARO: case BASUCA: case GRANADA: instanceDestroy(Bala, globalBalas); break;
						}
					break;
					case CYBOR:
						switch(Bala->estado) {
							case DISPARO: case BASUCA: case GRANADA: instanceDestroy(Bala, globalBalas); break;
						}
					break;
					case NINJA:
						switch(Bala->estado) {
							case BASUCA: case GRANADA: instanceDestroy(Bala, globalBalas); break;
						}
					break;
					case MARCO:
						switch(Bala->estado) {
							case BASUCA: case GRANADA: instanceDestroy(Bala, globalBalas); break;
						}
					break;
				}
			}

			// Rebote de balas
			if (player[Bala->creador].name == RAMBO && Bala->estado == GRANADA) {
				if (collisionRectangle(0, Bala->vspeed+gravity, &Bala->mascara, &TempBlock->mascara)) {
					Bala->vspeed *= -1;
					Bala->vspeed -= gravity;
				}

				if (collisionRectangle(Bala->hspeed, 0, &Bala->mascara, &TempBlock->mascara)) {
					Bala->hspeed *= -1;
				}
			}
			TempBlock = TempBlock->Sig;
		}
		Bala = Bala->Sig;
	}
	
	movePlayer(&player[0], 0);
	movePlayer(&player[1], 1);

	
	// Barra Sangre
	GsSortRectangle(&player[0].barraSangre);
	GsSortRectangle(&player[1].barraSangre);


	player[0].barraEnergia.w = clamp(player[0].barraEnergia.w, 0, 100);
	player[1].barraEnergia.w = clamp(player[1].barraEnergia.w, 0, 100);
	// Barra de Energia
	GsSortRectangle(&player[0].barraEnergia);
	GsSortRectangle(&player[1].barraEnergia);

	// Nombre del player
	GsPrintFont(10, 32, "%s", nom[player[0].name].n);
	GsPrintFont(180, 32, "%s", nom[player[1].name].n);
}

void logicaBala() {
	struct bala *temp;
	temp = globalBalas.Ini;
	while(temp != NULL) {
		//movimiento pistola
		switch(player[temp->creador].name) {
			case RAMBO:
				switch(temp->estado) {
					case DISPARO:
						temp->mascara.x += temp->hspeed;
					break;
					//movimiento granada
					case GRANADA:
						if (temp->alarm++ == 49){
							temp->alarm = 0;
							temp->hspeed -= dir(temp->hspeed);
							if (temp->hspeed == 0) {
								instanceDestroy(temp, globalBalas);
							}
						}

						temp->mascara.x += temp->hspeed;
						temp->vspeed += gravity;
						temp->mascara.y += temp->vspeed;
					break;
					//movimiento basuca
					case BASUCA:
						temp->mascara.x += temp->hspeed;
						temp->vspeed += gravity;
						temp->mascara.y += temp->vspeed;
					break;
				}
			break;

			case ROBOT:
				switch(temp->estado) {
					case DISPARO:
						temp->mascara.x += temp->hspeed;
					break;
					//movimiento granada
					case GRANADA:
						temp->mascara.y = player[temp->creador].mascara.y+10;
						switch(dir(temp->hspeed)) {
							case 1:
								temp->mascara.w += temp->hspeed;
								temp->mascara.x = player[temp->creador].mascara.x+(10*player[temp->creador].xscale);
							break;
							case -1:
								temp->mascara.w = absf(temp->mascara.x-player[temp->creador].mascara.x+(10*player[temp->creador].xscale));
								temp->mascara.x += temp->hspeed;
							break;
						}
						// Salio de la pantalla
						if (temp->mascara.w >= 150 || dir(temp->hspeed) != dir(player[temp->creador].xscale)) {
							instanceDestroy(temp,globalBalas);
						}
					break;
					//movimiento basuca
					case BASUCA:
						temp->mascara.x += temp->hspeed;
						temp->vspeed += gravity;
						temp->mascara.y += temp->vspeed;
					break;
				}
			break;

			case ALDO:
				switch(temp->estado) {
					case DISPARO: case BASUCA:
						temp->mascara.x += temp->hspeed;
					break;
					//movimiento granada
					case GRANADA:
						player[temp->creador].alarm++;
						if(player[temp->creador].alarm <= 5){
							temp->mascara.x += (8+absf(temp->hspeed))*temp->xscale;
							temp->mascara.y += (4)*temp->xscale;
						}
						else if (player[temp->creador].alarm <= 10){
							temp->mascara.x += (8+absf(temp->hspeed))*temp->xscale;
							temp->mascara.y -= (4)*temp->xscale;
						}
						else if (player[temp->creador].alarm <= 15){
							temp->mascara.x -= (2+absf(temp->hspeed))*temp->xscale;
							temp->mascara.y -= (4)*temp->xscale;
						}
						else if (player[temp->creador].alarm <= 20){
							temp->mascara.x -= (2+absf(temp->hspeed))*temp->xscale;
							temp->mascara.y += (4)*temp->xscale;
						}
						else {
							player[temp->creador].alarm = 0;
						}
					break;
				}
			break;

			case CYBOR:
				switch(temp->estado) {
					case DISPARO:
						temp->mascara.x += temp->hspeed;
					break;
					//movimiento granada
					case GRANADA:
						if (temp->mascara.w >= 150) {
							temp->mascara.x += temp->hspeed;
						}
						else {
							temp->mascara.y = player[temp->creador].mascara.y;
							switch(dir(temp->hspeed)) {
								case 1:
									temp->mascara.w += temp->hspeed;
									temp->mascara.x = player[temp->creador].mascara.x+10;
								break;
								case -1:
									temp->mascara.w = absf(temp->mascara.x-player[temp->creador].mascara.x);
									temp->mascara.x += temp->hspeed;
								break;
							}
						}
						// Salio de la pantalla
						if (dir(temp->hspeed) != dir(player[temp->creador].xscale)) {
							instanceDestroy(temp,globalBalas);
						}
					break;
					//movimiento basuca
					case BASUCA:
						temp->mascara.x += temp->hspeed;
						temp->vspeed += gravity;
						temp->mascara.y += temp->vspeed;
					break;
				}
			break;
			case NINJA:
				switch(temp->estado) {
					case DISPARO:
						switch(player[temp->creador].xscale) {
							case 1: temp->mascara.x = player[temp->creador].mascara.x+14; break;
							case -1: temp->mascara.x = player[temp->creador].mascara.x-24; break;
						}
						temp->mascara.y = player[temp->creador].mascara.y;
						if (temp->alarm++ == 10){
							temp->alarm = 0;
							instanceDestroy(temp,globalBalas);
						}
						if (dir(temp->hspeed) != dir(player[temp->creador].xscale)) {
							instanceDestroy(temp,globalBalas);
						}
					break;
					case GRANADA:
						temp->mascara.x += temp->hspeed;
						temp->mascara.y += temp->vspeed;
					break;
					case BASUCA:
						temp->mascara.x += temp->hspeed;
						temp->vspeed += gravity;
						temp->mascara.y += temp->vspeed;
					break;
				}
			break;
			case MARCO:
				switch(temp->estado) {
					case DISPARO:
						switch(player[temp->creador].xscale) {
							case 1: temp->mascara.x = player[temp->creador].mascara.x+14; break;
							case -1: temp->mascara.x = player[temp->creador].mascara.x-24; break;
						}
						temp->mascara.y = player[temp->creador].mascara.y;
						if (temp->alarm++ == 10){
							temp->alarm = 0;
							instanceDestroy(temp,globalBalas);
						}
						if (dir(temp->hspeed) != dir(player[temp->creador].xscale)) {
							instanceDestroy(temp,globalBalas);
						}
					break;
					case GRANADA:
						temp->mascara.x += temp->hspeed;
						temp->mascara.y += temp->vspeed;
					break;
					case BASUCA:
						temp->mascara.x += temp->hspeed;
						temp->vspeed += gravity;
						temp->mascara.y += temp->vspeed;
					break;
				}
			break;
		}
		GsSortRectangle(&temp->mascara);
		//destruccion balas
		if (temp->mascara.x+temp->mascara.w >= GsScreenW || temp->mascara.x <= 0 || temp->mascara.y+temp->mascara.h >= GsScreenH || temp->mascara.y <= 0) {
			instanceDestroy(temp,globalBalas);
		}
		temp = temp->Sig;
	}
}
