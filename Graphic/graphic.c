#include "..\bootpack.h"
#include "graphic.h"
#include "..\sheet\sheet.h"
//初始化显示
void init_screen(char *vram, int x, int y)
{
	boxfill8(vram, x, 1,  0,     0,      x -  1, y - 29);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 28, x -  1, y - 28);
	boxfill8(vram, x, COL8_FFFFFF,  0,     y - 27, x -  1, y - 27);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 26, x -  1, y -  1);

	boxfill8(vram, x, COL8_FFFFFF,  3,     y - 24, 59,     y - 24);
	boxfill8(vram, x, COL8_FFFFFF,  2,     y - 24,  2,     y -  4);
	boxfill8(vram, x, COL8_848484,  3,     y -  4, 59,     y -  4);
	boxfill8(vram, x, COL8_848484, 59,     y - 23, 59,     y -  5);
	boxfill8(vram, x, COL8_000000,  2,     y -  3, 59,     y -  3);
	boxfill8(vram, x, COL8_000000, 60,     y - 24, 60,     y -  3);

	boxfill8(vram, x, COL8_848484, x - 47, y - 24, x -  4, y - 24);
	boxfill8(vram, x, COL8_848484, x - 47, y - 23, x - 47, y -  4);
	boxfill8(vram, x, COL8_FFFFFF, x - 47, y -  3, x -  4, y -  3);
	boxfill8(vram, x, COL8_FFFFFF, x -  3, y - 24, x -  3, y -  3);
	return;
}

void init_palette()
{
	static unsigned char table_rgb[32 * 3] = {
		//p81 有具体颜色
		0xfa, 0xfa, 0xfa,	/*  0:白*/
		0x7c, 0x7c, 0x7c,	/*  1:window Title色 */
		0xc4, 0xc4, 0xc4,	/*  2:window背景色 */
		0xcd, 0x29, 0x32,	/*  3:红色*/
		0xfe, 0xbe, 0x50,	/*  4:黄色 */
		0xff, 0x00, 0xff,	/*  5:浅亮蓝 */
		0x5a, 0xa6, 0xd9,	/*  6:浅蓝色 */
		0x00, 0x00, 0x00,	/*  7:�� */
		0xc6, 0xc6, 0xc6,	/*  8:���邢�D�F */
		0x84, 0x00, 0x00,	/*  9:�Â��� */
		0x00, 0x84, 0x00,	/* 10:�Â��� */
		0x84, 0x84, 0x00,	/* 11:�Â����F */
		0x00, 0x00, 0x84,	/* 12:�Â��� */
		0x84, 0x00, 0x84,	/* 13:�Â��� */
		0xff, 0xff, 0xff,	/* 14:白色 */
		0xfe, 0xfe, 0xfe,	/* 15:透明色 */
		0x84, 0x84, 0x84,   //16
		0x00, 0x84, 0x84,	//17
		0x45, 0x44, 0x3f,	//18 console window head bc
		0x30, 0x09, 0x24	//19
	};
	set_palette(0, 32, table_rgb);
	return;
}

void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();	//记录中断许可标志
	io_cli(); 					//将中断许可标志置为0
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);	//复原中断许可标志
	return;
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
			vram[y * xsize + x] = c;
	}
	return;
}

void boxfill_circle_forWindow(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1,int radius,enum FlagOfBg flag){
	if(flag==OnBg){
		Circlefill8(vram,xsize,c,x0+radius,y0+radius,radius);
		Circlefill8(vram,xsize,c,x1-radius,y0+radius,radius);
	}
	else{
		Circlefill8_No_BG(vram,xsize,c,x0+radius,y0+radius,radius);
		Circlefill8_No_BG(vram,xsize,c,x1-radius,y0+radius,radius);
	}
	boxfill8(vram,xsize,c,x0+radius,y0,x1-radius,y0+radius);
	boxfill8(vram,xsize,c,x0,y0+radius,x1,y1);
}

void boxfill_circle(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1,int radius,enum FlagOfBg flag){
	if(flag==OnBg){
		Circlefill8(vram,xsize,c,x0+radius,y0+radius,radius);
		Circlefill8(vram,xsize,c,x1-radius,y0+radius,radius);
		Circlefill8(vram,xsize,c,x0+radius,y1-radius,radius);
		Circlefill8(vram,xsize,c,x1-radius,y1-radius,radius);
	}
	else{
		Circlefill8_No_BG(vram,xsize,c,x0+radius,y0+radius,radius);
		Circlefill8_No_BG(vram,xsize,c,x1-radius,y0+radius,radius);
		Circlefill8_No_BG(vram,xsize,c,x0+radius,y1-radius,radius);
		Circlefill8_No_BG(vram,xsize,c,x1-radius,y1-radius,radius);
	}
	boxfill8(vram,xsize,c,x0+radius,y0,x1-radius,y0+radius);
	boxfill8(vram,xsize,c,x0,y0+radius,x1,y1-radius);
	boxfill8(vram,xsize,c,x0+radius,y1-radius,x1-radius,y1);
}


void Circlefill8(unsigned char *vram, int xsize, unsigned char c, int centre_x, int centre_y, int radius)
{
	int x, y;
	int y0=centre_y-radius;
	int x0=centre_x-radius;
	int x1=centre_x+radius;
	int y1=centre_y+radius;
	FixPosition(&x0,&y0,&x1,&y1);
	for (y=y0; y <= y1; y++) {
		for (x=x0; x <= x1; x++)

		if(radius*radius>=(x-centre_x)*(x-centre_x)+(y-centre_y)*(y-centre_y))
			vram[y * xsize + x] = c;
		else vram[y * xsize + x] = 15;
	}

	return;
}

void Circlefill8_No_BG(unsigned char *vram, int xsize, unsigned char c, int centre_x, int centre_y, int radius)
{
	int x, y;
	int y0=centre_y-radius;
	int x0=centre_x-radius;
	int x1=centre_x+radius;
	int y1=centre_y+radius;
	FixPosition(&x0,&y0,&x1,&y1);
	for (y=y0; y <= y1; y++) {
		for (x=x0; x <= x1; x++)

		if(radius*radius>=(x-centre_x)*(x-centre_x)+(y-centre_y)*(y-centre_y))
			vram[y * xsize + x] = c;
	}

	return;
}


//超出屏幕修正
void FixPosition(int *x0,int *y0,int *x1,int *y1){
	if((*x0)<0) (*x0)=0;
	if((*y0)<0) (*y0)=0;
	if((*x1)>resolution_x) (*x1)=resolution_x;
	if((*y1)>resolution_y) (*y1)=resolution_y;
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
{
	int i;
	char *p, d /* data */;
	for (i = 0; i < 16; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}

void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s)
{
	extern char hankaku[4096];
	for (; *s != 0x00; s++) {
		putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
		x += 8;
	}
	return;
}

//显示鼠标
void init_mouse_cursor8(char *mouse,char bc){
	static char cursor[16][16] = {
		"*...............",
		"*O*.............",
		"*OO*............",
		"*OOOO*..........",
		"*OOOOO*.........",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOOOOOOOO*....",
		"*OOOOOOOOOOO*...",
		"*OOOOOOOOOOOOO*.",
		"*OOOOOOOOOOOOOO*",
		"*OOOO*OOOO*.....",
		"*OO*..*OOOO*....",
		"*......*OOOO*...",
		"........*OOOO*..",
		"..........****.."
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == '*') {
				mouse[y * 16 + x] = 0;
			}
			if (cursor[y][x] == 'O') {
				mouse[y * 16 + x] = 99;
			}
			if (cursor[y][x] == '.') {
				mouse[y * 16 + x] = bc;
			}
		}
	}
	return;
}

// void entry_screen(char *buff){
// 	static char cursor[16][16] = {
// 		"........................................................................................................................",
// 		"........................................................................................................................",
// 		"........................................................................................................................",
// 		"........................................................................................................................",
// 		"........................................................................................................................",
// 		"........................................................................................................................",
// 		"........................................................................................................................",
// 		"........................................................................................................................",
// 		"........................................................................................................................",
// 		"........................................................................................................................",
// 		"........................................................................................................................",
// 		"........................................................................................................................",
// 		"........................................................................................................................",
// 		"........................................................................................................................",
// 		"........................................................................................................................",
// 		"........................................................................................................................"
// 	};
// }

void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize)
{
	int x, y;
	for (y = 0; y < pysize; y++) {
		for (x = 0; x < pxsize; x++) {
			vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
		}
	}
	return;
}
void make_window8(unsigned char *buf, int xsize, int ysize,char *title)
{
	static char closebtn[7][8] = {
		"@@QQQQ@@",
		"Q@@QQ@@Q",
		"QQ@@@@QQ",
		"QQQ@@QQQ",
		"QQ@@@@QQ",
		"Q@@QQ@@Q",
		"@@QQQQ@@"
	};
	int x, y;
	char c;
	boxfill_circle_forWindow(buf,xsize,0,0,0,xsize,ysize,10,OnBg);
	boxfill8(buf,xsize,2,0,25,xsize,ysize-15);
	Circlefill8_No_BG(buf,xsize,3,18,12,7);   //close
	Circlefill8_No_BG(buf,xsize,4,35,12,7);   //minimize
	boxfill8(buf,xsize,0,33,12,38,13);
	putfonts8_asc(buf, xsize, 53, 5, COL8_7C7C7C, title);
	
	//绘制X
	for (y = 0; y < 7; y++) {
		for (x = 0; x < 8; x++) {
			c = closebtn[y][x];
			if (c == '@') {
				c = 0;
				buf[(9 + y) * xsize + x+14] = c;
			}
			
		}
	}
	return;
}

void make_Console8(unsigned char *buf, int xsize, int ysize,char *title)
{
	static char closebtn[7][8] = {
		"@@QQQQ@@",
		"Q@@QQ@@Q",
		"QQ@@@@QQ",
		"QQQ@@QQQ",
		"QQ@@@@QQ",
		"Q@@QQ@@Q",
		"@@QQQQ@@"
	};
	int x, y;
	char c;
	boxfill_circle_forWindow(buf,xsize,COL_Cons_Head_BC,0,0,xsize,28,10,OnBg);
	boxfill8(buf,xsize,COL_Cons_BC,0,25,xsize,ysize);
	Circlefill8_No_BG(buf,xsize,3,18,12,7);   //close
	Circlefill8_No_BG(buf,xsize,4,35,12,7);   //minimize
	boxfill8(buf,xsize,0,33,12,38,13);
	putfonts8_asc(buf, xsize, 53, 5, COL8_FFFFFF, title);
	
	//绘制X
	for (y = 0; y < 7; y++) {
		for (x = 0; x < 8; x++) {
			c = closebtn[y][x];
			if (c == '@') {
				c = 0;
				buf[(9 + y) * xsize + x+14] = c;
			}
			
		}
	}
	return;
}

void Sel_window(struct SHEET *sht,enum Flag_Select flag){
	//boxfill8(window->sht->buf,window->width,COL8_FFFFFF,53,5,75,15);
	int x, y, xsize = sht->bxsize;
	char c, Window_new, Console_new, Window_old, Console_old, *buf = sht->buf;
	if (flag==OnSel) {
		Window_new  = COL_Cons_BC;
		Window_old  = COL8_7C7C7C;

		Console_new  = COL8_FFFFFF;
		Console_old  = COL_Cons_BC;

	} else {
		Window_new  = COL8_7C7C7C;
		Window_old  = COL_Cons_BC;

		Console_new  = COL_Cons_BC;
		Console_old  = COL8_FFFFFF;
	}
	for (y = 5; y <= 21; y++) {
		for (x = 53; x <= xsize - 4; x++) {
			c = buf[y * xsize + x];
			if(c == Window_old){
				c=Window_new;
				buf[y * xsize + x] = c;
			}
			else if(c == Console_old){
				c= Console_new;
				buf[y * xsize + x] = c;
			}
			
			
		}
	}
	sheet_refresh(sht->shtctl,sht,53,5,sht->bxsize-20,21);
	//sheet_slide(window->sht->shtctl,window->sht,0,0);
}

void Sel_Console(struct Window *window,enum Flag_Select flag){
	//boxfill8(window->sht->buf,window->width,COL_Cons_Head_BC,53,5,75,15);
	if(flag==OnSel){
		putfonts8_asc(window->sht->buf, window->width,53, 5, COL8_FAFAFA, window->title);
	}
	else{
		putfonts8_asc(window->sht->buf, window->width,53, 5, COL8_7C7C7C, window->title);
	}
	sheet_refresh(window->sht->shtctl,window->sht,53,5,window->width-20,21);
	
}

void make_textbox8(struct SHEET *sht, int x0, int y0, int sx, int sy)
{
	int x1 = x0 + sx, y1 = y0 + sy;
	boxfill_circle(sht->buf, sht->bxsize, 6, x0, y0, x1, y1,3,NoBg);
	boxfill_circle(sht->buf, sht->bxsize, 0,x0 +1, y0 + 1, x1 - 1, y1 - 1,3,NoBg);
	return;
}

void putfonts8_asc_sht(struct SHTCTL *shtctl,struct SHEET *sht, int x, int y, int c, int b, char *s, int l)
{
	boxfill8(sht->buf, sht->bxsize, b, x, y, x + l * 8 - 1, y + 15);
	putfonts8_asc(sht->buf, sht->bxsize, x, y, c, s);
	sheet_refresh(shtctl,sht, x, y, x + l * 8, y + 16);
	return;
}
