* TINY Compilation to TM Code
* File: testfiles/simple.tm
* Standard prelude:
  0:     LD  6,0(0) 	load maxaddress from location 0
  1:     ST  0,0(0) 	clear location 0
* End of standard prelude.
* -> Id
  2:     LD  0,0(5) 	load id value
* <- Id
* -> Id
  3:     LD  0,2(5) 	load id value
* <- Id
* End of execution.
  4:   HALT  0,0,0 	
