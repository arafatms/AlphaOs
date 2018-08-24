#include "..\bootpack.h"
#include "FIFO.h"
#include "..\Graphic\graphic.h"
#include "..\task\task.h"
//初始化FIFO
void fifo8_init(struct FIFO8 *fifo, int size, unsigned char *buf)
{
	fifo->size = size;
	fifo->buf = buf;
	fifo->free = size; 
	fifo->flags = 0;
	fifo->p = 0;     //下一个数据写入位置
	fifo->q = 0;     //下一个数据读取位置
	return;
}

//进队列
int fifo8_put(struct FIFO8 *fifo, unsigned char data)
{
	
	if (fifo->free == 0) {
		fifo->flags |= FLAGS_OVERRUN;
		return -1;
	}
	fifo->buf[fifo->p] = data;
	fifo->p++;
	if (fifo->p == fifo->size) {
		fifo->p = 0;
	}
	fifo->free--;
	
	return 0;
}

//出队列
int fifo8_get(struct FIFO8 *fifo)
{
	int data;
	if (fifo->free == fifo->size) {
		return -1;
	}
	data = fifo->buf[fifo->q];
	fifo->q++;
	if (fifo->q == fifo->size) {
		fifo->q = 0;
	}
	fifo->free++;
	return data;
}

//返回队列中数据的数量
int fifo8_status(struct FIFO8 *fifo)
{
	
	return fifo->size - fifo->free;
}


//初始化FIFO
void fifo32_init(struct FIFO32 *fifo, int size, int *buf, struct TASK *task)
{
	fifo->size = size;
	fifo->buf = buf;
	fifo->free = size; 
	fifo->flags = 0;
	fifo->p = 0;     //下一个数据写入位置
	fifo->q = 0;     //下一个数据读取位置
	fifo->task=task;  //有数据写入时唤醒的任务
	return;
}

//进队列
int fifo32_put(struct FIFO32 *fifo, int data)
{
	
	if (fifo->free == 0) {
		fifo->flags |= FLAGS_OVERRUN;
		return -1;
	}
	fifo->buf[fifo->p] = data;
	fifo->p++;
	if (fifo->p == fifo->size) {
		fifo->p = 0;
	}
	fifo->free--;
	if (fifo->task != 0) {
		
		if (fifo->task->flags != 2) { //如果任务休眠中
			task_run(fifo->task,-1,0); //唤醒任务
		}
	}
	return 0;
}

//出队列
int fifo32_get(struct FIFO32 *fifo)
{
	int data;
	if (fifo->free == fifo->size) {
		return -1;
	}
	data = fifo->buf[fifo->q];
	fifo->q++;
	if (fifo->q == fifo->size) {
		fifo->q = 0;
	}
	fifo->free++;
	return data;
}

//返回队列中数据的数量
int fifo32_status(struct FIFO32 *fifo)
{
	
	return fifo->size - fifo->free;
}
