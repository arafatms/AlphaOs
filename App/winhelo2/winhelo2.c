#include "API.h"

char buf[150 * 50];

void HariMain(void)
{
	int win;
	win = api_openwin(buf, 150, 50, COL8_Transparent, "hello");
	api_boxfilwin(win,  8, 36, 141, 43, 3 );
	api_putstrwin(win, 28, 28, COL8_000000 , 12, "hello, world");
	api_end();
}
