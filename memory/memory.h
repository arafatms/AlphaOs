#ifndef _MEMORY_H_
#define _MEMORY_H_

#define EFLAGS_AC_BIT		0x00040000
#define CR0_CACHE_DISABLE	0x60000000
#define MEMMAN_FREES		4090	//32kb
#define MEMMAN_ADDR			0x003c0000  

struct FREEINFO {	/* ������� */
	unsigned int addr, size;
};

struct MEMMAN {		/* �������Ǘ� */
	int frees, maxfrees, lostsize, losts;
	struct FREEINFO free[MEMMAN_FREES];
};

int load_cr0(void);
void store_cr0(int cr0);
unsigned int memtest(unsigned int start, unsigned int end);
unsigned int memtest_sub(unsigned int start, unsigned int end);
void memman_init(struct MEMMAN *man);
unsigned int memman_total(struct MEMMAN *man);
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size);
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);
unsigned int memman_alloc_4K(struct MEMMAN *man,unsigned int size);
int memman_free_4K(struct MEMMAN *man,unsigned int addr,unsigned int size);

#endif
