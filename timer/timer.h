#ifndef _TIMER_H_
#define _TIMER_H_

#include "../FIFO/FIFO.h"


#define PIT_CTRL	0x0043
#define PIT_CNT0	0x0040
#define MAX_TIMER   500

#define TIMER_FLAGS_ALLOC		1	//已配置状态
#define TIMER_FLAGS_USING		2	//定时器运行中

struct TIMER {
	struct TIMER *next;
	unsigned int timeout, flags;
	struct FIFO32 *fifo;
	int data;
};
struct TIMERCTL {
	unsigned int count, next, using;
	struct TIMER *t0;
	struct TIMER timers0[MAX_TIMER];
};

void init_pit(void);
void inthandler20(int *esp);
struct TIMER *timer_alloc(void);
void timer_free(struct TIMER *timer);
void timer_init(struct TIMER *timer, struct FIFO32 *fifo, int data);
void timer_settime(struct TIMER *timer, unsigned int timeout);

#endif
