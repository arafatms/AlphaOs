#ifndef _BOOTPACK_H_
#define _BOOTPACK_H_

//告诉编译器有一个函数在别的文件里
#include <stdio.h>
#include "sheet\sheet.h"
//显卡信息
struct BootInfo{
	char cyls,leds,vmode,reserve;
	short scrnX,scrnY;
	char *vram;
};




#define ADR_BOOTINFO	0x00000ff0
#define ADR_DISKIMG		0x00100000
#define ADR_shtctl		0x0fe4





#define Win_Title_height	28



void io_hlt(void);
void write_mem8(int addr,int data);
void io_cli(void);
void io_out8(int port, int data);
int io_in8(int port);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void io_stihlt(void);
void io_sti(void);
void farjmp(int eip, int cs);
void farcall(int eip, int cs);
void asm_cons_putchar(void);
void asm_hrb_api(void);
void start_app(int eip, int cs, int esp, int ds, int *tss_esp0);
void asm_inthandler0d(void);
void asm_inthandler0c(void);
void asm_end_app(void);


#define resolution_x    1024
#define resolution_y    768


#define sht_back_addr	0x0fec
#define sht_ctl_addr	0x0fe8







#endif
