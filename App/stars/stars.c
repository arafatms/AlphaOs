#include "API.h"
int rand(void);		//产生随机数

void HariMain(void)
{
	char *buf;
	int win, i, x, y;
	api_initmalloc();
	buf = api_malloc(150 * 100);
	win = api_openwin(buf, 150, 100, COL8_Transparent, "stars");
	api_boxfilwin(win+1,  6, 26, 143, 93, COL8_000000 );
	for (i = 0; i < 50; i++) {
		x = (rand() % 137) +  6;
		y = (rand() %  67) + 26;
		api_point(win+1, x, y, COL8_Blue );
	}
    api_refreshwin(win,6,26,143,93);
	for (;;) {
        //api_putchar(api_getkey(1));
		if (api_getkey(1) == 0x0a) {
			break; /* Enter to break; */
		}
	}
    api_closewin(win);
	api_end();
}