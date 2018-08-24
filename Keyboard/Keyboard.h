#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "..\sheet\sheet.h"
#include "..\FIFO\FIFO.h"

#define PORT_KEYDAT				0x0060
#define PORT_KEYSTA				0x0064
#define PORT_KEYCMD				0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE		0x60
#define KBC_MODE				0x47
#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4
#define KEYCMD_LED		0xed

struct MOUSE_DEC {
	unsigned char buf[3], phase;
	int x, y, btn;
};

void keyboard_manage(struct SHEET_INFO *sheet_info);
void wait_KBC_sendready(void);
void init_keyboard(struct FIFO32 *fifo, int data0);
void enable_mouse(struct FIFO32 *fifo, int data0, struct MOUSE_DEC *mdec);
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat);

#endif
