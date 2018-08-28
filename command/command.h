#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "..\memory\memory.h"
#include "..\sheet\sheet.h"
#include "..\timer\timer.h"

#define Console_Line_dis	20
#define Cons_addr	0x0fec
#define App_addr	0x0fe8

struct CONSOLE {
	struct SHEET *sht;
	int cur_x, cur_y, cur_c;
	struct TIMER *timer;
};

typedef void (*COMMAND_FUNC)(struct CONSOLE *cons,char *cmdline);

struct Command{
	int size;
	char Name[20][20];
	COMMAND_FUNC command_func[20];
};






void add_command(char *name,COMMAND_FUNC fanc);
void Command_int(struct MEMMAN *mem);
COMMAND_FUNC Find_Command(char *name);
void Command_mem(struct CONSOLE *cons,char *cmdline);
void Command_clear(struct CONSOLE *cons,char *cmdline);
void Command_ls(struct CONSOLE *cons,char *cmdline);
void Command_cat(struct CONSOLE *cons,char *cmdline);
void cmd_hlt(struct CONSOLE *cons,char *cmdline);
void file_readfat(int *fat, unsigned char *img);
void file_loadfile(int clustno, int size, char *buf, int *fat, char *img);


void console_task(struct SHEET *sheet,unsigned int memtotal);
void cons_newline(struct CONSOLE *cons);
void cons_putchar(struct CONSOLE *cons, int chr, char move);
void cons_putstr(struct CONSOLE *cons, char *s);
void cons_putnstr(struct CONSOLE *cons, char *s, int n);
int cmd_app(struct CONSOLE *cons, int *fat, char *cmdline);
int *hrb_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax);
void hrb_api_linewin(struct SHEET *sht, int x0, int y0, int x1, int y1, int col);
#endif
