#ifndef _TASK_H_
#define _TASK_H_

#include "..\memory\memory.h"
#include "..\FIFO\FIFO.h"

#define MAX_TASKS		1000     
#define TASK_GDT0		3
#define MAX_TASKS_LV	100
#define MAX_TASKLEVELS	10
#define Task_running_flag			2


struct TSS32 {
	int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
	int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	int es, cs, ss, ds, fs, gs;
	int ldtr, iomap;
};

struct TASK {
	int sel, flags; /* sel��GDT�̔ԍ��̂��� */
	int level, priority;
	
	struct TSS32 tss;
	struct FIFO32 *fifo;
};

struct TASKLEVEL {
	int running; //正在运行的任务数量
	int now; //正在运行的是哪个任务
	struct TASK *tasks[MAX_TASKS_LV];
};
struct TASKCTL {
	int now_lv; //正在活动的LV
	char lv_change; //在下次任务切换时是否需要切换
	struct TASKLEVEL level[MAX_TASKLEVELS];
	struct TASK tasks0[MAX_TASKS];
};

extern struct TIMER *task_timer;
struct TASK *task_init(struct MEMMAN *memman);
struct TASK *task_alloc(void);
void task_run(struct TASK *task, int level, int priority);
void task_switch(void);
void task_sleep(struct TASK *task);
struct TASK *task_now(void);
void task_add(struct TASK *task);
void task_remove(struct TASK *task);
void task_switchsub(void);
void task_idle(void);
void load_tr(int tr);
#endif
