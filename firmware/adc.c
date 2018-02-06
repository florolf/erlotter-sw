#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/dma.h>

#include "adc.h"
#include "constants.h"
#include "systick.h"
#include "usart.h"
#include "pid.h"
#include "lamp.h"

/*
 * Pin mapping
 * -----------
 *
 * Channel 1 (Warm):
 *  - Isense: PA0 / ADC1_IN1
 *  - Vsense: PB2 / ADC2_IN12
 *
 * Channel 2 (Cold):
 *  - Isense: PA5 / ADC2_IN2
 *  - Vsense: PA1 / ADC1_IN2
 */

#define OVERSAMPLING 20

volatile uint16_t buf[2][OVERSAMPLING] __attribute__((aligned(2)));
volatile uint16_t offset[NUM_CHANNELS] = {0};

#ifdef MODE_test
volatile uint32_t adc_last_sample[NUM_CHANNELS];
#endif

static void dma_handler(enum channel channel)
{
	uint32_t avg = 0;

	for (int i = 0; i < OVERSAMPLING; i++)
		avg += buf[channel][i];

	avg /= OVERSAMPLING;

	if (lamp_off)
		offset[channel] = avg;

	if (avg < offset[channel])
		avg = 0;
	else
		avg -= offset[channel];

#ifdef MODE_test
	adc_last_sample[channel] = avg;
#endif

	pid_run(channel, avg);
}

void dma1_channel1_isr(void)
{
	DMA1_IFCR = DMA_IFCR_CTCIF1;
	DMA1_CCR1 &= ~DMA_CCR_EN;

	dma_handler(CHANNEL_WARM);

	DMA1_CNDTR1 = OVERSAMPLING;
	DMA1_CCR1 |= DMA_CCR_EN;

	ADC1_CR |= ADC_CR_ADSTART;
}

void dma1_channel2_isr(void)
{
	DMA1_IFCR = DMA_IFCR_CTCIF2;
	DMA1_CCR2 &= ~DMA_CCR_EN;

	dma_handler(CHANNEL_COLD);

	DMA1_CNDTR2 = OVERSAMPLING;
	DMA1_CCR2 |= DMA_CCR_EN;

	ADC2_CR |= ADC_CR_ADSTART;
}

static void init_dma(void)
{
	rcc_periph_clock_enable(RCC_DMA1);

	DMA1_CNDTR1 = OVERSAMPLING;
	DMA1_CPAR1 = ADC1 + 0x40;
	DMA1_CMAR1 = (uint32_t)buf[0];
	DMA1_CCR1 = DMA_CCR_MSIZE_16BIT | DMA_CCR_PSIZE_16BIT | DMA_CCR_MINC | DMA_CCR_TCIE | DMA_CCR_EN;

	DMA1_CNDTR2 = OVERSAMPLING;
	DMA1_CPAR2 = ADC2 + 0x40;
	DMA1_CMAR2 = (uint32_t)buf[1];
	DMA1_CCR2 = DMA_CCR_MSIZE_16BIT | DMA_CCR_PSIZE_16BIT | DMA_CCR_MINC | DMA_CCR_TCIE | DMA_CCR_EN;

	nvic_enable_irq(NVIC_DMA1_CHANNEL1_IRQ);
	nvic_enable_irq(NVIC_DMA1_CHANNEL2_IRQ);
	nvic_set_priority(NVIC_DMA1_CHANNEL1_IRQ, 5 << 4);
	nvic_set_priority(NVIC_DMA1_CHANNEL2_IRQ, 5 << 4);
}

static void init_one_adc(uint32_t adc)
{
	adc_enable_regulator(adc);
	delay_ms(1);

	ADC_CR(adc) |= ADC_CR_ADCAL;
	while (ADC_CR(adc) & ADC_CR_ADCAL)
		;

	adc_set_sample_time_on_all_channels(adc, ADC_SMPR_SMP_19DOT5CYC);
}

void adc_init(void)
{
	init_dma();

	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0 | GPIO1 | GPIO5);
	gpio_mode_setup(GPIOB, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO2);

	rcc_periph_clock_enable(RCC_ADC12);
	rcc_periph_reset_pulse(RST_ADC12);

	ADC12_CCR |= ADC_CCR_CKMODE_DIV1;

	init_one_adc(ADC1);
	init_one_adc(ADC2);

	ADC1_SQR1 =  1 << ADC_SQR1_SQ1_SHIFT |
	             0 << ADC_SQR1_L_SHIFT;
	ADC2_SQR1 =  2 << ADC_SQR1_SQ1_SHIFT |
	             0 << ADC_SQR1_L_SHIFT;

	ADC1_CFGR1 |= ADC_CFGR1_EXTSEL_VAL(7) |
	              ADC_CFGR1_EXTEN_RISING_EDGE;

	ADC2_CFGR1 |= ADC_CFGR1_EXTSEL_VAL(8) |
	              ADC_CFGR1_EXTEN_RISING_EDGE;

	adc_power_on(ADC1);
	adc_power_on(ADC2);
	delay_ms(1);

	ADC1_CFGR |= ADC_CFGR1_DMAEN;
	ADC2_CFGR |= ADC_CFGR1_DMAEN;

	ADC1_CR |= ADC_CR_ADSTART;
	ADC2_CR |= ADC_CR_ADSTART;
}
