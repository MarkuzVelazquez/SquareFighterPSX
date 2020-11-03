#ifndef _FIGHT_H
#define _FIGHT_H

enum {
	MENU, ROM1, PLAYER_SELECT, GAMEOVER,
	DISPARO, BASUCA, GRANADA,
	PJVSCOM, PJVSPJ,
	RAMBO = 0, ROBOT = 1, CYBOR = 2, NINJA = 3,
	MARCO = 4, ALDO = 5, DIOS = 6, NOONE = 7
};

struct block {
	GsRectangle mascara;
	struct block *Sig;
	struct block *Ant;
};

struct listaDeBlocks {
	struct block *Ini;
	struct block *Fin;
};

struct jugador {
	char vspeed;
	char hspeed;
	char xscale;
	unsigned char jumps;
	GsRectangle mascara;
	//Sprite
	unsigned char alarm;
	unsigned char potencia;
	unsigned char name;
	GsRectangle barraSangre;
	GsRectangle barraEnergia;
};

struct bala {
	GsRectangle mascara;
	short int x;
	short int y;
	char vspeed;
	char hspeed;
	char xscale;
	unsigned char alarm;
	unsigned char estado;
	unsigned char creador;
	struct bala *Sig;
	struct bala *Ant;
};

struct listaBalas {
	struct bala *Ini;
	struct bala *Fin;
};

struct PJMenus {
	unsigned char opcion;
	unsigned char select;
	_Bool press_up;
	_Bool press_down;
	_Bool press_right;
	_Bool press_left;
	_Bool press_cross;
	_Bool press_circle;
	_Bool press_square;
	_Bool press_triangle;
	_Bool press_start;
};

// Lista de nombres
struct nombres {
	char n[6];
};

unsigned int distancia(int, int, int, int);
_Bool collisionRectangle(int, int, GsRectangle *, GsRectangle *);
int clamp(int, int, int);
int absf(int);
char dir(int);

#endif
