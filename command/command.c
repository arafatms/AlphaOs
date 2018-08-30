#include "command.h"
#include "stdio.h"
#include "string.h"
#include "..\graphic\graphic.h"
#include "..\bootpack.h"
#include "..\task\task.h"
#include "..\file\file.h"
#include "..\memory\memory.h"
#include "..\dsctbl\dsctbl.h"
struct Command *command_list;
extern struct MEMMAN *memman;
extern unsigned int mem_total;

void Command_int(struct MEMMAN *mem){
    command_list=(struct Command *)memman_alloc_4K(mem,sizeof(struct Command));
    command_list->size=0;

    add_command("mem",(COMMAND_FUNC)&Command_mem);
    add_command("clear",(COMMAND_FUNC)&Command_clear);
    add_command("ls",(COMMAND_FUNC)&Command_ls);
    add_command("cat",(COMMAND_FUNC)&Command_cat);
	//add_command("hlt",(COMMAND_FUNC)&cmd_hlt);
}


void add_command(char *name,COMMAND_FUNC fanc){
    strcpy(command_list->Name[command_list->size],name);
    command_list->command_func[command_list->size]=fanc;
    command_list->size++;
}

COMMAND_FUNC Find_Command(char *name){

    int i,j=0;
    while((name[j]!=' ')){
        j++;
    }
	
    for(i=0;i<command_list->size;i++){
		// while((name[j]!=' ')&&(name[j]!=0)){

		// }
        if(strncmp(name,command_list->Name[i],j)==0)
            return command_list->command_func[i];
    }
    return NULL;
    
}


void Command_mem(struct CONSOLE *cons,char *cmdline){
    char s[60];
    struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
    sprintf(s, "total   %dMB\nfree %dKB\n\n", mem_total / (1024 * 1024), memman_total(memman) / 1024);
	cons_putstr(cons, s);
	return;
}


void Command_clear(struct CONSOLE *cons,char *cmdline){
    int x,y;
	struct SHEET *sheet=cons->sht;
    for (y = 28; y < sheet->bysize-10; y++) {
							for (x = 8; x < sheet->bxsize-10; x++) {
								sheet->buf[x + y * sheet->bxsize] = COL_Cons_BC;
							}
						}
						sheet_refresh(sheet->shtctl,sheet, 8, 28, sheet->bysize-10, sheet->bxsize-10);
						cons->cur_y = 28;
}

void Command_ls(struct CONSOLE *cons,char *cmdline){
    struct FILEINFO *finfo = (struct FILEINFO *) (ADR_DISKIMG + 0x002600);
    char s[40];
    int x,y;
	struct SHEET *sheet=cons->sht;
    for (x = 0; x < 224; x++) {
		if (finfo[x].name[0] == 0x00) {
			break;
		}
		if (finfo[x].name[0] != 0xe5) {
			if ((finfo[x].type & 0x18) == 0) {
				sprintf(s, "filename.ext   %7d\n", finfo[x].size);
				for (y = 0; y < 8; y++) {
					s[y] = finfo[x].name[y];
				}
				s[ 9] = finfo[x].ext[0];
				s[10] = finfo[x].ext[1];
				s[11] = finfo[x].ext[2];
				cons_putstr(cons, s);
			}
		}
	}
	cons_newline(cons);
}

void Command_cat(struct CONSOLE *cons,char *cmdline){
    char *p;
	struct SHEET *sheet=cons->sht;
    struct FILEINFO *finfo = file_search(cmdline+4,(struct FILEINFO *) (ADR_DISKIMG + 0x002600),224);

    int *fat = (int *) memman_alloc_4K(memman, 4 * 2880);
    file_readfat(fat, (unsigned char *) (ADR_DISKIMG + 0x000200));

    int x;
						if (finfo!=0) {//找到文件
                            p = (char *) memman_alloc_4K(memman, finfo->size);
							file_loadfile(finfo->clustno, finfo->size, p, fat, (char *) (ADR_DISKIMG + 0x003e00));
							cons_putnstr(cons,p,finfo->size);
							
                            memman_free_4K(memman, (int) p, finfo->size);
						} else {
							//没有找到文件
							cons_putstr(cons, "File not found.\n");
						}
						cons_newline(cons);
                        cons_newline(cons);
}

void cmd_hlt(struct CONSOLE *cons,char *cmdline)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct FILEINFO *finfo = file_search("HLT.EXE", (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;
	char *p;

	int *fat = (int *) memman_alloc_4K(memman, 4 * 2880);
    file_readfat(fat, (unsigned char *) (ADR_DISKIMG + 0x000200));

	if (finfo != 0) {
		//找到
		p = (char *) memman_alloc_4K(memman, finfo->size);
		file_loadfile(finfo->clustno, finfo->size, p, fat, (char *) (ADR_DISKIMG + 0x003e00));
		set_segmdesc(gdt + 1003, finfo->size - 1, (int) p, AR_CODE32_ER);
		farcall(0, 1003 * 8);
		memman_free_4K(memman, (int) p, finfo->size);
	} else {
		
		putfonts8_asc_sht(cons->sht->shtctl,cons->sht, 8, cons->cur_y, COL8_FFFFFF, COL8_000000, "File not found.", 15);
		cons_newline(cons);
	}
	memman_free_4K(memman,(int)fat,4 * 2880);
	cons_newline(cons);
	return;
}



void console_task(struct SHEET *sheet,unsigned int memtotal)
{
	//struct TIMER *timer;
	struct TASK *task = task_now();
	int i, fifobuf[128],cursor0=65;
	char s[30],cmdline[30];
	fifo32_init(task->fifo, 128, fifobuf, task);


	int *fat = (int *) memman_alloc_4K(memman, 4 * 2880);
    file_readfat(fat, (unsigned char *) (ADR_DISKIMG + 0x000200));

	struct CONSOLE cons;
	cons.sht=sheet;
	cons.cur_x=cursor0;
	cons.cur_y=Win_Title_height;
	cons.cur_c=-1;

	*((int *)Cons_addr)=(int)&cons;

	COMMAND_FUNC func;
	cons.timer = timer_alloc();
	timer_init(cons.timer, task->fifo, 1);
	timer_settime(cons.timer, 50);

	putfonts8_asc_sht(sheet->shtctl,sheet, 8, 28, COL8_FAFAFA, COL_Cons_BC, "User:$", 6);

	for (;;) {
		io_cli();
		if (fifo32_status(task->fifo) == 0) {
			task_sleep(task);
			io_sti();
		} else {
			i = fifo32_get(task->fifo);
			io_sti();
			if (i <= 1) { //光标用定时器
				if (i != 0) {
					timer_init(cons.timer, task->fifo, 0); //下次置0
					if (cons.cur_c >= 0)
						cons.cur_c = COL8_FAFAFA;
				} else {
					timer_init(cons.timer, task->fifo, 1); //下次置1
					if (cons.cur_c >= 0)
						cons.cur_c = COL_Cons_BC;
				}
				timer_settime(cons.timer, 50);
			}
			if (i == 2) {	//光标ON
				cons.cur_c = COL8_FAFAFA;
			}
			if (i == 3) {	//光标OFF
				boxfill8(sheet->buf, sheet->bxsize, COL_Cons_BC, cons.cur_x, 28, cons.cur_x + 7, 43);
				cons.cur_c = -1;
			}
			if (256 <= i && i <= 511) {   //键盘数据（通过A）传递
				if (i == 8 + 256) {
					//退格
					if (cons.cur_x > cursor0) {
						
						cons_putchar(&cons,' ',0);
						cons.cur_x -= 8;
					}
				} else if (i == 10 + 256) {
					/* Enter */
					cons_putchar(&cons,' ',0);
					cmdline[(cons.cur_x-cursor0)/8] = 0;
					cons_newline(&cons);
					func=Find_Command(cmdline);    //从命令列表中找到命名并返回命令函数指针
					if (func!=NULL) {
						func(&cons,cmdline);

					}else{
						if(cmd_app(&cons,fat,cmdline)==0){
							cons_putstr(&cons,"Bad Command.\n\n");
						}
					}
					putfonts8_asc_sht(sheet->shtctl,sheet, 8, cons.cur_y, COL8_FAFAFA, COL_Cons_BC, "User:$", 6);
					cons.cur_x = cursor0;
				} else {
					//一般字符
					if (cons.cur_x < sheet->bxsize-16) {
						cmdline[(cons.cur_x-cursor0)/8]=i-256;
						cons_putchar(&cons,i-256,1);
					}
				}
			}
			//重新获取光标
			if (cons.cur_c >= 0)
				boxfill8(sheet->buf, sheet->bxsize, cons.cur_c, cons.cur_x, cons.cur_y, cons.cur_x + 7, cons.cur_y+15);
			sheet_refresh(sheet->shtctl,sheet, cons.cur_x, cons.cur_y, cons.cur_x + 8, cons.cur_y+16);
		}
	}
}

void cons_newline(struct CONSOLE *cons)
{
	int x, y;
	struct SHEET *sheet = cons->sht;
	
	if (cons->cur_y < sheet->bysize-38) {
		cons->cur_y += Console_Line_dis; 
	} else {
		//滚动
		for (y = 28; y < sheet->bysize-28; y++) {
			for (x = 8; x < sheet->bxsize; x++) {
				sheet->buf[x + y * sheet->bxsize] = sheet->buf[x + (y + Console_Line_dis) * sheet->bxsize];
			}
		}
		for (y = sheet->bysize-28; y < sheet->bysize-8; y++) {
			for (x = 8; x < sheet->bxsize; x++) {
				sheet->buf[x + y * sheet->bxsize] = COL_Cons_BC;
			}
		}
		sheet_refresh(sheet->shtctl,sheet, 8, 28, sheet->bxsize,sheet->bysize);
	}
	cons->cur_x = 8;
}

void cons_putchar(struct CONSOLE *cons, int chr, char move)
{
	char s[2];
	s[0] = chr;
	s[1] = 0;
	if (s[0] == 0x09) {	//制表符
		for (;;) {
			putfonts8_asc_sht(cons->sht->shtctl,cons->sht, cons->cur_x, cons->cur_y, COL8_FAFAFA, COL_Cons_BC, " ", 1);
			cons->cur_x += 8;
			if (cons->cur_x >=cons->sht->bxsize-10) {
				cons_newline(cons);
			}
			if (((cons->cur_x - 8) & 0x1f) == 0) {
				break;	
			}
		}
	} else if (s[0] == 0x0a) {	//换行
		cons_newline(cons);
	} else if (s[0] == 0x0d) {	//回车/
		//先没有操作
	} else {	//一般字符
		putfonts8_asc_sht(cons->sht->shtctl,cons->sht, cons->cur_x, cons->cur_y, COL8_FAFAFA, COL_Cons_BC, s, 1);
		if (move != 0) {
			//move为0时光标不后移
			cons->cur_x += 8;
			if (cons->cur_x >=cons->sht->bxsize-10) {
				cons_newline(cons);
			}
		}
	}
	return;
}

void cons_putstr(struct CONSOLE *cons, char *s)
{
	for (; *s != 0; s++) {
		cons_putchar(cons, *s, 1);
	}
	return;
}

void cons_putnstr(struct CONSOLE *cons, char *s, int n)
{
	int i;
	for (i = 0; i < n; i++) {
		cons_putchar(cons, s[i], 1);
	}
	return;
}

int cmd_app(struct CONSOLE *cons, int *fat, char *cmdline)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct FILEINFO *finfo;
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;
	char name[18], *p,*q;
	struct TASK *task = task_now();
	int i, segsiz, datsiz, esp, dathrb;
	struct SHTCTL *shtctl;
	struct SHEET *sht;

	//生成文件名
	for (i = 0; i < 13; i++) {
		if (cmdline[i] <= ' ') {
			break;
		}
		name[i] = cmdline[i];
	}
	name[i] = 0; 

	//查找文件
	finfo = file_search(name, (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	if (finfo == 0 && name[i - 1] != '.') {
		//找不到后面加EXE
		name[i    ] = '.';
		name[i + 1] = 'E';
		name[i + 2] = 'X';
		name[i + 3] = 'E';
		name[i + 4] = 0;
		finfo = file_search(name, (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	}

	if (finfo != 0) {
		//找到文件
		p = (char *) memman_alloc_4K(memman, finfo->size);
		file_loadfile(finfo->clustno, finfo->size, p, fat, (char *) (ADR_DISKIMG + 0x003e00));
		if (finfo->size >= 36 && strncmp(p + 4, "Hari", 4) == 0 && *p == 0x00) {
			segsiz = *((int *) (p + 0x0000));
			esp    = *((int *) (p + 0x000c));
			datsiz = *((int *) (p + 0x0010));
			dathrb = *((int *) (p + 0x0014));
			q = (char *) memman_alloc_4K(memman, segsiz);
			*((int *) App_addr) = (int) q;
			set_segmdesc(gdt + 1003, finfo->size - 1, (int) p, AR_CODE32_ER + 0x60);
			set_segmdesc(gdt + 1004, segsiz - 1,      (int) q, AR_DATA32_RW + 0x60);
			for (i = 0; i < datsiz; i++) {
				q[esp + i] = p[dathrb + i];
			}
			start_app(0x1b, 1003 * 8, esp, 1004 * 8, &(task->tss.esp0));
			shtctl = (struct SHTCTL *) *((int *) ADR_shtctl);
			for (i = 0; i < MAX_SHEETS; i++) {
				sht = &(shtctl->sheets0[i]);
				
				if (sht->task == task &&(sht->flags & 0x11)==0x11) {
				
					//找到被应用程序遗留的窗口
					sheet_free(shtctl,sht);	//关闭
				}
			}
			memman_free_4K(memman, (int) q, segsiz);
		} else {
			cons_putstr(cons, ".hrb file format error.\n");
		}
		memman_free_4K(memman, (int) p, finfo->size);
		cons_newline(cons);
		return 1;
	}
	//没有找到
	return 0;
}


int *hrb_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax)
{
	struct TASK *task = task_now();
	struct SHTCTL *shtctl=(struct SHTCTL *) *((int *)ADR_shtctl);
	struct CONSOLE *cons = (struct CONSOLE *) *((int *) 0x0fec);
	struct SHEET *sht;
	int i;
	int *reg = &eax + 1;	
		/* reg[0] : EDI,   reg[1] : ESI,   reg[2] : EBP,   reg[3] : ESP */
		/* reg[4] : EBX,   reg[5] : EDX,   reg[6] : ECX,   reg[7] : EAX */
	int ds_base=*((int *)App_addr);

	if (edx == 1) {
		cons_putchar(cons, eax & 0xff, 1);
	} else if (edx == 2) {
		cons_putstr(cons, (char *) ebx+ds_base);
	} else if (edx == 3) {
		cons_putnstr(cons, (char *) ebx+ds_base, ecx);
	}else if (edx == 4) {
		return &(task->tss.esp0);
	}else if(edx==5){	//创建
		sht = sheet_alloc(shtctl);
		sht->task=task;
		sht->flags |= 0x10;
		sheet_setbuf(sht, (char *) ebx + ds_base, esi, edi, eax);
		make_window8((char *) ebx + ds_base, esi, edi, (char *) ecx + ds_base);
		sheet_slide(shtctl,sht, 100, 50);
		sheet_updown(shtctl,sht, 3);	
		reg[7] = (int) sht;
	} else if (edx == 6) {
		sht = (struct SHEET *) (ebx&0xfffffffe);
		putfonts8_asc(sht->buf, sht->bxsize, esi, edi, eax, (char *) ebp + ds_base);
		if ((ebx & 1) == 0)
			sheet_refresh(shtctl,sht, esi, edi, esi + ecx * 8, edi + 16);
	} else if (edx == 7) {
		sht = (struct SHEET *) (ebx&0xfffffffe);
		boxfill8(sht->buf, sht->bxsize, ebp, eax, ecx, esi, edi);
		if ((ebx & 1) == 0)
			sheet_refresh(shtctl,sht, eax, ecx, esi + 1, edi + 1);
	} else if (edx == 8) {
		memman_init((struct MEMMAN *) (ebx + ds_base));
		ecx &= 0xfffffff0;	//16字节为单位
		memman_free((struct MEMMAN *) (ebx + ds_base), eax, ecx);
	} else if (edx == 9) {
		ecx = (ecx + 0x0f) & 0xfffffff0; 
		reg[7] = memman_alloc((struct MEMMAN *) (ebx + ds_base), ecx);
	} else if (edx == 10) {
		ecx = (ecx + 0x0f) & 0xfffffff0; 
		memman_free((struct MEMMAN *) (ebx + ds_base), eax, ecx);
	} else if (edx == 11) {
		sht = (struct SHEET *) (ebx&0xfffffffe);
		sht->buf[sht->bxsize * edi + esi] = eax;
		if ((ebx & 1) == 0)
			sheet_refresh(sht->shtctl,sht, esi, edi, esi + 1, edi + 1);
	} else if (edx == 12) {
		sht = (struct SHEET *) ebx;
		sheet_refresh(sht->shtctl,sht, eax, ecx, esi, edi);
	} else if (edx == 13) {
		sht = (struct SHEET *) (ebx & 0xfffffffe);
		hrb_api_linewin(sht, eax, ecx, esi, edi, ebp);
		if ((ebx & 1) == 0) {
			sheet_refresh(sht->shtctl,sht, eax, ecx, esi + 1, edi + 1);
		}
	} else if (edx == 14) {
		sht = (struct SHEET *) ebx;
		sheet_free(sht->shtctl,sht);
	} else if (edx == 15) {
		for (;;) {
			io_cli();
			if (fifo32_status(task->fifo) == 0) {
				if (eax != 0) {
					task_sleep(task);	/* FIFO空时休眠 */
				} else {   //退出
					io_sti();
					reg[7] = -1;
					return 0;
				}
			}
			i = fifo32_get(task->fifo);
			io_sti();
			if (i <= 1) { //光标用定时器
				
				timer_init(cons->timer, task->fifo, 1);
				timer_settime(cons->timer, 50);
			}
			if (i == 2) {	/* 光标ON */
				cons->cur_c = COL8_FAFAFA;
			}
			if (i == 3) {	/* 光标OFF */
				cons->cur_c = -1;
			}
			if (256 <= i && i <= 511) { //键盘输入
				reg[7] = i - 256;
				return 0;
			}
		}
	}
	return 0;
}

void hrb_api_linewin(struct SHEET *sht, int x0, int y0, int x1, int y1, int col)
{
	int i, x, y, len, dx, dy;

	dx = x1 - x0;
	dy = y1 - y0;
	x = x0 << 10;
	y = y0 << 10;
	if (dx < 0) {
		dx = - dx;
	}
	if (dy < 0) {
		dy = - dy;
	}
	if (dx >= dy) {
		len = dx + 1;
		if (x0 > x1) {
			dx = -1024;
		} else {
			dx =  1024;
		}
		if (y0 <= y1) {
			dy = ((y1 - y0 + 1) << 10) / len;
		} else {
			dy = ((y1 - y0 - 1) << 10) / len;
		}
	} else {
		len = dy + 1;
		if (y0 > y1) {
			dy = -1024;
		} else {
			dy =  1024;
		}
		if (x0 <= x1) {
			dx = ((x1 - x0 + 1) << 10) / len;
		} else {
			dx = ((x1 - x0 - 1) << 10) / len;
		}
	}

	for (i = 0; i < len; i++) {
		sht->buf[(y >> 10) * sht->bxsize + (x >> 10)] = col;
		x += dx;
		y += dy;
	}

	return;
}
