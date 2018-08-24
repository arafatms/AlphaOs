#ifndef _FIFO_H_
#define _FIFO_H_

#include "..\task\task.h"
struct FIFO8{
	unsigned char *buf;
	int p,q,size,free,flags;
};

struct FIFO32{
	int *buf;
	int p,q,size,free,flags;
	struct TASK *task;
};

#define FLAGS_OVERRUN		0x0001
//8
void fifo8_init(struct FIFO8 *fifo, int size, unsigned char *buf);
int fifo8_put(struct FIFO8 *fifo, unsigned char data);
int fifo8_get(struct FIFO8 *fifo);
int fifo8_status(struct FIFO8 *fifo);
//32
void fifo32_init(struct FIFO32 *fifo, int size, int *buf, struct TASK *task);
int fifo32_put(struct FIFO32 *fifo, int data);
int fifo32_get(struct FIFO32 *fifo);
int fifo32_status(struct FIFO32 *fifo);

#endif
