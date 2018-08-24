#include "..\bootpack.h"
#include "..\FIFO\FIFO.h"
#include "int.h"
#include "..\Graphic\graphic.h"
#include "..\command\command.h"
//key
extern struct FIFO32 *keyfifo;
extern int keydata0;
//mouse
extern struct FIFO32 *mousefifo;
extern int mousedata0;
void init_pic(void)
//PIC初始化
{
	
	io_out8(PIC0_IMR,  0xff  );
	io_out8(PIC1_IMR,  0xff  ); 

	io_out8(PIC0_ICW1, 0x11  ); 
	io_out8(PIC0_ICW2, 0x20  ); 
	io_out8(PIC0_ICW3, 1 << 2); 
	io_out8(PIC0_ICW4, 0x01  );

	io_out8(PIC1_ICW1, 0x11  );
	io_out8(PIC1_ICW2, 0x28  ); 
	io_out8(PIC1_ICW3, 2     );
	io_out8(PIC1_ICW4, 0x01  );

	io_out8(PIC0_IMR,  0xfb  ); 
	io_out8(PIC1_IMR,  0xff  ); 

	return;
}

//键盘驱动程序
void inthandler21(int *esp)
{
	int data;
	io_out8(PIC0_OCW2, 0x61);	
	data = io_in8(PORT_KEYDAT);
	fifo32_put(keyfifo, data + keydata0);
	return;
	
}

//鼠标驱动程序
void inthandler2c(int *esp)
{
	int data;
	io_out8(PIC1_OCW2, 0x64);	
	io_out8(PIC0_OCW2, 0x62);	
	data = io_in8(PORT_KEYDAT);
	fifo32_put(mousefifo, data + mousedata0);
	return;
}

void inthandler27(int *esp)
{
	io_out8(PIC0_OCW2, 0x67); /* IRQ-07��t������PIC�ɒʒm(7-1�Q��) */
	return;
}

int *inthandler0c(int *esp)
{
	struct CONSOLE *cons = (struct CONSOLE *) *((int *) Cons_addr);
	struct TASK *task = task_now();
	char s[30];
	cons_putstr(cons, "\nINT 0C :\n Stack Exception.\n");
	sprintf(s, "EIP = %08X\n", esp[11]);
	cons_putstr(cons, s);
	return &(task->tss.esp0);	/* �ُ�I�������� */
}

int *inthandler0d(int *esp)
{
	struct CONSOLE *cons = (struct CONSOLE *) *((int *) Cons_addr);
	struct TASK *task = task_now();
	char s[30];
	cons_putstr(cons, "\nINT 0D :\n General Protected Exception.\n");
	sprintf(s, "EIP = %08X\n", esp[11]);
	cons_putstr(cons, s);
	return &(task->tss.esp0);
}


void Allow_InterruptMask(void){
	io_out8(PIC0_IMR, 0xf8); /* PIC1(11111001) 终端屏蔽允许*/
	io_out8(PIC1_IMR, 0xef); /* (11101111) */
}

