#ifndef _SHEET_H_
#define _SHEET_H_

#include "..\memory\memory.h"
#include "..\task\task.h"

#define SHEET_USE		1
#define MAX_SHEETS		256

#define mouse_updown	64

struct Window{
	char title[20];
	int width,height;
	struct SHEET *sht;
};

struct SHEET {
	unsigned char *buf;
	struct SHTCTL *shtctl;
	int bxsize, bysize, vx0, vy0, col_inv, height, flags;
	struct TASK *task;
	char title[20];
};

struct SHTCTL {
	unsigned char *vram,*map;
	int xsize, ysize, top;
	struct SHEET *sheets[MAX_SHEETS];
	struct SHEET sheets0[MAX_SHEETS];
};

struct BUF_SHT{
	char name[20];
	struct SHEET *sht;
	unsigned char *buf;
};

struct SHEET_INFO{
    struct SHTCTL *shtctl;
	struct BUF_SHT buf_sht[128];
	int size;
};



void sheet(struct SHEET_INFO *sheet_info,struct MEMMAN *memman, unsigned char *vram, int xsize, int ysize);
struct SHTCTL *shtctl_init(struct MEMMAN *memman, unsigned char *vram, int xsize, int ysize);
struct SHEET *sheet_alloc(struct SHTCTL *ctl);
void sheet_setbuf(struct SHEET *sht, unsigned char *buf, int xsize, int ysize, int col_inv);
void sheet_updown(struct SHTCTL *ctl, struct SHEET *sht, int height);
void sheet_refreshsub(struct SHTCTL *ctl,int vx0,int vy0,int vx1,int vy1,int,int);
void sheet_refreshmap(struct SHTCTL *ctl, int vx0, int vy0, int vx1, int vy1, int h0);
void sheet_refresh(struct SHTCTL *ctl, struct SHEET *sht, int bx0, int by0, int bx1, int by1);
void sheet_slide(struct SHTCTL *ctl, struct SHEET *sht, int vx0, int vy0);
void sheet_free(struct SHTCTL *ctl, struct SHEET *sht);
void updown_mouse(struct SHEET_INFO *sheet_info);

void Make_Window_Base(struct SHEET_INFO *sheet_info,struct MEMMAN *memman,struct Window *window);
void Make_Window(struct SHEET_INFO *sheet_info,struct MEMMAN *memman,struct Window *window,int x0,int  y0);
void Make_Win1(struct SHEET_INFO *sheet_info,struct MEMMAN *memman,struct Window *window,int x0,int  y0);
void Make_Console(struct SHEET_INFO *sheet_info,struct MEMMAN *memman,struct Window *window,int x0,int  y0);
#endif
