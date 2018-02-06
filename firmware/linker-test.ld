MEMORY
{
	rom (rx) : ORIGIN =  0x20000000, LENGTH = 9K
	ram (rwx) : ORIGIN = 0x20002400, LENGTH = 3K
}

/* Include the common ld script. */
INCLUDE libopencm3_stm32f3.ld
