#include "API.h"
void HariMain(void)
{
	char *buf;
	int win, i;
	api_initmalloc();
	buf = api_malloc(160 * 100);
	win = api_openwin(buf, 160, 100, COL8_Transparent, "lines");
	for (i = 0; i < 8; i++) {
		api_linewin(win + 1,  8, 26, 77, i * 9 + 26, i);
		api_linewin(win + 1, 88, 26, i * 9 + 88, 89, i);
	}
	api_refreshwin(win,  6, 26, 154, 90);
    for (;;) {
        //api_putchar(api_getkey(1));
		if (api_getkey(1) == 0x0a) {
			break; /* Enter to break; */
		}
	}
	api_closewin(win);
	api_end();
}
