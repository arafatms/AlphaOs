#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include "..\sheet\sheet.h"

#ifndef _COLOR_DEFINE
#define _COLOR_DEFINE

#define COL8_FAFAFA		0
#define COL8_7C7C7C		1
#define COL8_C4C4C4		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_000000		7
#define COL8_C6C6C6		8   
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_FFFFFF		14
#define COL8_Transparent		15
#define COL8_848484		16
#define COL8_008484		17
#define COL_Cons_Head_BC		18    //console window head bc
#define COL_Cons_BC		19
//100001001000
#endif

enum FlagOfBg{
	OnBg,NoBg
};

enum Flag_Select{
	OnSel,OffSel
};

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void boxfill_circle(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1,int radius,enum FlagOfBg flag);
void boxfill_circle_forWindow(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1,int radius,enum FlagOfBg flag);
void Circlefill8(unsigned char *vram, int xsize, unsigned char c, int centre_x, int centre_y, int radius);
void Circlefill8_No_BG(unsigned char *vram, int xsize, unsigned char c, int centre_x, int centre_y, int radius);
void FixPosition(int *x0,int *y0,int *x1,int *y1);
void init_screen(char *vram, int x, int y);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize);
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void make_Console8(unsigned char *buf, int xsize, int ysize,char *title);
void make_window8(unsigned char *buf, int xsize, int ysize,char *title);
void make_textbox8(struct SHEET *sht, int x0, int y0, int sx, int sy);
void Sel_window(struct SHEET *sht,enum Flag_Select flag);
void Sel_Console(struct Window *window,enum Flag_Select flag);

void putfonts8_asc_sht(struct SHTCTL *shtctl,struct SHEET *sht, int x, int y, int c, int b, char *s, int l);

#endif
