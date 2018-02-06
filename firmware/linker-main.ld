MEMORY
{
	rom (rx) : ORIGIN = 0x08000800, LENGTH = 12K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 12K
}

/* Include the common ld script. */
INCLUDE libopencm3_stm32f3.ld
