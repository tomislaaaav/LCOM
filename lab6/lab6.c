#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>

#include "test6.h"

int main(int argc, char **argv) {

	 /* Initialize service */

	  sef_startup();

	  /* Enable IO-sensitive operations for ourselves */

	  sys_enable_iop(SELF);

	  rtc_test_int();
	  //rtc_test_date();
	  return 0;

	/* To be completed */

}

