

#include "timeline.h"
int main(int argc, char **argv)
{
	(void)argc;

	timeline *tl = timeline_open(argv[1], argv[2]);

	timeline_close(tl);
}
