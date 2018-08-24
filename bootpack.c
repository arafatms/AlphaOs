#include "bootpack.h"
#include "Keyboard\Keyboard.h"
#include "Int\int.h"
#include "Graphic\graphic.h"
#include "Dsctbl\dsctbl.h"
#include "memory\memory.h"

#include "timer\timer.h"
#include "task\task.h"
#include "string.h"
#include "command\command.h"

void task_b_main(struct SHEET *sht_win_b,struct SHTCTL *shtctl);


struct MEMMAN *memman;
extern struct TIMERCTL timerctl;
extern struct Command *command_list;

unsigned int mem_total;
void HariMain(void)
{
	struct BootInfo *binfo;
	struct SHEET_INFO *sheet_info;
	struct CONSOLE *cons;
	// int mx, my;

	
	
	init_gdtidt();
	init_pic();
	io_sti();
	
	binfo=(struct BootInfo*) ADR_BOOTINFO;
	init_pit();

	Allow_InterruptMask();

	unsigned int memtotal;
	memman = (struct MEMMAN *) MEMMAN_ADDR;
	memtotal = memtest(0x00400000, 0xbfffffff);
	mem_total=memtotal;
	memman_init(memman);
	memman_free(memman, 0x00001000, 0x0009e000); /* 0x00001000 - 0x0009efff */
	memman_free(memman, 0x00400000, memtotal - 0x00400000);

	//屏幕图层
	sheet_info=(struct SHEET_INFO *)memman_alloc_4K(memman,sizeof(struct SHEET_INFO));
	init_palette();   //设定调色板
	sheet(sheet_info,memman,binfo->vram,binfo->scrnX,binfo->scrnY);
	//boxfill_circle(binfo->vram,binfo->scrnX,0,50,50,250,250,10);
	
	
	*((int *)ADR_shtctl)=(int)sheet_info->shtctl;

	struct FIFO32 fifo, keycmd;
	int fifobuf[128], keycmd_buf[32];
	struct MOUSE_DEC mdec;

	fifo32_init(&fifo, 128, fifobuf,0);
	fifo32_init(&keycmd,32,keycmd_buf,0);
	init_keyboard(&fifo, 256);
	enable_mouse(&fifo, 512, &mdec);

	
	char s[40],cmdline[30];
	int i,count=0;;
	

	int mx, my;
	mx = (sheet_info->shtctl->xsize - 16) / 2; /* ��ʒ����ɂȂ�悤�ɍ��W�v�Z */
	my = (sheet_info->shtctl->ysize - 28 - 16) / 2;
	

	struct BUF_SHT *back=&sheet_info->buf_sht[0];
	struct BUF_SHT *mouse=&sheet_info->buf_sht[1];



	static char keytable[0x54] = {
		0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0,   0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@', '[', 0,   0,   'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0,   0,   ']', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.'
	};
	static char keytable0[0x80] = {
		0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0,   0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@', '[', 0,   0,   'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0,   0,   ']', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   0x5c, 0,  0,   0,   0,   0,   0,   0,   0,   0,   0x5c, 0,  0
	};
	static char keytable1[0x80] = {
		0,   0,   '!', 0x22, '#', '$', '%', '&', 0x27, '(', ')', '~', '=', '~', 0,   0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '`', '{', 0,   0,   'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', '+', '*', 0,   0,   '}', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   '_', 0,   0,   0,   0,   0,   0,   0,   0,   0,   '|', 0,   0
	};
	int key_to = 0, key_shift = 0, key_leds = (binfo->leds >> 4) & 7, keycmd_wait = -1,key_ctrl=0;
	fifo32_put(&keycmd, KEYCMD_LED);
	fifo32_put(&keycmd, key_leds);

	// //任务
	// struct TIMER *timer_ts;
	// timer_ts = timer_alloc();
	// timer_init(timer_ts, &fifo, 2);
	// timer_settime(timer_ts, 2);



	//Command
	Command_int(memman);
	




	//Timer
	struct TIMER *timer;

	timer = timer_alloc();
	timer_init(timer, &fifo, 1);
	timer_settime(timer, 50);

	//win1
	struct TASK *task_a,*task_cons;
	task_a=task_init(memman);
	fifo.task=task_a;
	task_run(task_a,1,0);
	struct Window win1;
	win1.height=52;
	win1.width=144;
	strcpy(win1.title,"window1");
	Make_Win1(sheet_info,memman,&win1,8,56);
	Sel_window(&win1,OnSel);


	//console
	struct Window Console_Window;
	Console_Window.height=512;
	Console_Window.width=612;
	strcpy(Console_Window.title,"Console");
	Make_Console(sheet_info,memman,&Console_Window,100,100);

	task_cons = task_alloc();
	task_cons->tss.esp = memman_alloc_4K(memman, 64 * 1024) + 64 * 1024 - 12;
	task_cons->tss.eip = (int) &console_task;
	task_cons->tss.es = 1 * 8;
	task_cons->tss.cs = 2 * 8;
	task_cons->tss.ss = 1 * 8;
	task_cons->tss.ds = 1 * 8;
	task_cons->tss.fs = 1 * 8;
	task_cons->tss.gs = 1 * 8;
	*((int *) (task_cons->tss.esp + 4)) = (int) Console_Window.sht;
	*((int *) (task_cons->tss.esp + 8)) = memtotal;
	task_run(task_cons, 2, 2); /* level=2, priority=2 */


	int textBoxX0=8,textBoxY0=28,texBoxWidth=100;
	int cursor_x=textBoxX0+8,cursor0=textBoxX0+8,cursor_c;  //光标
	cursor_c = COL8_FAFAFA;
	
	updown_mouse(sheet_info);

	// sheet_slide(sheet_info->shtctl,sheet_info->buf_sht[2].sht, 168,  56);
	// sheet_slide(sheet_info->shtctl,sheet_info->buf_sht[3].sht,   8, 116);
	// sheet_slide(sheet_info->shtctl,sheet_info->buf_sht[4].sht, 168, 116);
	
    while(1){
		if (fifo32_status(&keycmd) > 0 && keycmd_wait < 0) {
			//如果存在向键盘控制器发送的数据，则发送它
			keycmd_wait = fifo32_get(&keycmd);
			wait_KBC_sendready();
			io_out8(PORT_KEYDAT, keycmd_wait);
		}
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			task_sleep(task_a);
			io_sti();
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (256 <= i && i <= 511) {     //键盘
				
				if (i < 0x80 + 256 ) { /* �ʏ핶�� */
					if (key_shift == 0) {
						s[0] = keytable0[i - 256];
					} else {
						s[0] = keytable1[i - 256];
					}
				} else {
					s[0] = 0;
				}

				if ('A' <= s[0] && s[0] <= 'Z') {	/* ���͕������A���t�@�x�b�g */
					if (((key_leds & 4) == 0 && key_shift == 0) ||
							((key_leds & 4) != 0 && key_shift != 0)) {
						s[0] += 0x20;	/* �啶��������ɕϊ� */
					}
				}
				if (s[0] != 0) {
					if (key_to == 0) {	//发送给任务A
						if (cursor_x < 128) {
							
							//s[0] = keytable[i - 256];
							s[1] = 0;
							putfonts8_asc_sht(sheet_info->shtctl,win1.sht, cursor_x, textBoxY0+3, COL8_000000, COL8_FAFAFA, s, 1);
							cursor_x += 8;
						}
					} else {	/* �R���\�[���� */
						fifo32_put(task_cons->fifo, s[0] + 256);
					}
				}

				if (i == 256 + 0x0e) {	//退格
					if (key_to == 0) {	//发送给a
						if (cursor_x > cursor0) {
							putfonts8_asc_sht(sheet_info->shtctl,win1.sht, cursor_x, textBoxY0+3, COL8_000000, COL8_FAFAFA, " ", 1);
							cursor_x -= 8;
						}
					} else {	//发送给b
						fifo32_put(task_cons->fifo, 8 + 256);
					}
				}
				if (i == 256 + 0x1c) {	/* Enter */
					if (key_to != 0) {	//发送给console
						fifo32_put(task_cons->fifo, 10 + 256);
					}
				}
				if (i == 256 + 0x0f) { /* Tab */
					if (key_to == 0) {
						key_to = 1;
						Sel_Console(&Console_Window,OnSel);
						Sel_window(&win1,OffSel);
						cursor_c=-1;
						boxfill8(win1.sht->buf, win1.width, COL8_FAFAFA, cursor_x, textBoxY0+3, cursor_x + 7, textBoxY0+18);
						fifo32_put(task_cons->fifo, 2);   //命令行窗口光标ON
					} else {
						key_to = 0;
						Sel_Console(&Console_Window,OffSel);
						Sel_window(&win1,OnSel);
						cursor_c = COL8_000000;
						fifo32_put(task_cons->fifo, 3); //命令行窗口OFF
						
					}
				}
				if (i == 256 + 0x2a) {	//左shiftOn
					key_shift |= 1;
				}
				if (i == 256 + 0x36) {	//右shiftOn
					key_shift |= 2;
				}
				if (i == 256 + 0xaa) {	//左shiftOff
					key_shift &= ~1;
				}
				if (i == 256 + 0xb6) {	//右shiftOff
					key_shift &= ~2;
				}

				if (i == 256 + 0x1d) {	//ctrlOn
					key_ctrl |= 1;
				}
				if (i == 256 + 0x9d) {	//ctrlOff
					key_ctrl &= ~1;
				}
				
				if (i == 256 + 0x3a) {	/* CapsLock */
					key_leds ^= 4;
					fifo32_put(&keycmd, KEYCMD_LED);
					fifo32_put(&keycmd, key_leds);
				}
				if (i == 256 + 0x45) {	/* NumLock */
					key_leds ^= 2;
					fifo32_put(&keycmd, KEYCMD_LED);
					fifo32_put(&keycmd, key_leds);
				}
				if (i == 256 + 0x46) {	/* ScrollLock */
					key_leds ^= 1;
					fifo32_put(&keycmd, KEYCMD_LED);
					fifo32_put(&keycmd, key_leds);
				}
				if (i == 256 + 0x2e && key_ctrl != 0 && task_cons->tss.ss0 != 0) {	/* ctrl+c */
					cons = (struct CONSOLE *) *((int *) Cons_addr);
					cons_putstr(cons, "\nBreak(key) :\n");
					io_cli();	
					task_cons->tss.eax = (int) &(task_cons->tss.esp0);
					task_cons->tss.eip = (int) asm_end_app;
					io_sti();
				}
				if (i == 256 + 0xfa) {	//键盘成功接收到数据
					keycmd_wait = -1;
				}
				if (i == 256 + 0xfe) {	//键盘没有成功接收到数据
					wait_KBC_sendready();
					io_out8(PORT_KEYDAT, keycmd_wait);
				}

				/* �J�[�\���̍ĕ\�� */
				if (cursor_c >= 0) {
					boxfill8(win1.sht->buf, win1.sht->bxsize, 15, cursor_x, textBoxY0+3, cursor_x + 7, textBoxY0+17);
				}

				sheet_refresh(sheet_info->shtctl,win1.sht, cursor_x, textBoxY0+3, cursor_x + 8, textBoxY0+18);
			} else if (512 <= i && i <= 767) { /* �}�E�X�f�[�^ */
				if (mouse_decode(&mdec, i - 512) != 0) {
					
					mx += mdec.x;
					my += mdec.y;
					if (mx < 0) {
						mx = 0;
					}
					if (my < 0) {
						my = 0;
					}
					if (mx > sheet_info->shtctl->xsize- 1) {
						mx = sheet_info->shtctl->xsize - 1;
					}
					if (my > sheet_info->shtctl->ysize - 1) {
						my = sheet_info->shtctl->ysize - 1;
					}
					
					sheet_slide(sheet_info->shtctl, mouse->sht, mx, my); /* sheet_refresh��܂� */
					if((mdec.btn & 0x01)!=0){
						sheet_slide(sheet_info->shtctl,win1.sht,mx-80,my-8);
					}
				}
			} else if (i <= 1) { //控制光标
			if (i != 0) {
					timer_init(timer, &fifo, 0); /* ����0�� */
					if (cursor_c >= 0) {
						cursor_c = COL8_000000;
					}
				} else {
					timer_init(timer, &fifo, 1); /* ����1�� */
					if (cursor_c >= 0) {
						cursor_c = COL8_FAFAFA;
					}
				}
				timer_settime(timer, 50);
				if (cursor_c >= 0) {
					boxfill8(win1.sht->buf, win1.sht->bxsize, cursor_c, cursor_x, textBoxY0+3, cursor_x + 7, textBoxY0+17);
					sheet_refresh(sheet_info->shtctl,win1.sht, cursor_x, textBoxY0+3, cursor_x + 8, textBoxY0+18);
				}
			}
		}
	}
	


fin:
	io_hlt(); //执行naskfanc.nas里的_io_hlt
	goto fin;

}

void task_b_main(struct SHEET *sht_win_b,struct SHTCTL *shtctl)
{
	// struct SHEET *sht_back=(struct SHEET *) *((int *)sht_back_addr);
	// struct SHTCTL *shtctl=(struct SHTCTL *) *((int *)sht_ctl_addr);
	struct FIFO32 fifo;
	struct TIMER *timer_ts;
	int i, fifobuf[128], count = 0, count0 = 0;
	char s[12];


	fifo32_init(&fifo, 128, fifobuf,0);
	timer_ts = timer_alloc();
	timer_init(timer_ts, &fifo, 100);
	timer_settime(timer_ts, 100);

	for (;;) {
		count++;
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			io_sti();
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (i == 100) {
				sprintf(s, "%11d", count);
				putfonts8_asc_sht(shtctl,sht_win_b, 24, 28, COL8_FFFFFF, COL8_008484, s, 11);
				count0 = count;
				timer_settime(timer_ts, 100);
			}
		}
	}
}


