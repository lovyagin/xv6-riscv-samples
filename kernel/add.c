#include "types.h"
#include "riscv.h"
#include "defs.h"

uint64 sys_add(void)
{
	int x, y;

	argint(0, &x);
	argint(1, &y);
	printf("%d\n", x + y);
	return 0;
}
