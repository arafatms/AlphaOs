#ifndef _API_H_
#define _API_H_

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

void api_putchar(int c);
void api_end(void);
void api_putstr(char *s);
int api_openwin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_putstrwin(int win, int x, int y, int col, int len, char *str);
void api_boxfilwin(int win, int x0, int y0, int x1, int y1, int col);
#endif
