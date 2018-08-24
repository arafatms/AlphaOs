#include "API.h"

char buf[150 * 50];

void HariMain(void)
{
	int win;
	win = api_openwin(buf, 150, 50, 15, "hello");
	api_end();
}
