#ifndef _INT_H_
#define _INT_H_
#define PORT_KEYDAT 	0x0060

#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

/* int.c */
void init_pic(void);
int *inthandler0c(int *esp);
int *inthandler0d(int *esp);
void inthandler21(int *esp);
void inthandler27(int *esp);
void inthandler2c(int *esp);

void Allow_InterruptMask(void);
#endif
