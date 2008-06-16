#include "MX21regs.h"
#include "vcmx21_GPIO.h"

void GPIO_DisableIRQ(int mode)
{
	unsigned int pin = mode & GPIO_PIN_MASK;
	unsigned int port = (mode & GPIO_PORT_MASK) >> GPIO_PORT_POS;

	IMR(port) &= ~(1 << pin);
}
void GPIO_Mode(int gpio_mode)
{
	unsigned int pin = gpio_mode & GPIO_PIN_MASK;
	unsigned int port = (gpio_mode & GPIO_PORT_MASK) >> GPIO_PORT_POS;
	unsigned int ocr = (gpio_mode & GPIO_OCR_MASK) >> GPIO_OCR_POS;
	unsigned int tmp;

	if (port != GPIO_PORTKP)
	{
		/* Pullup enable */
		if (gpio_mode & GPIO_PUEN)
			PUEN(port) |= (1<<pin);
		else
			PUEN(port) &= ~(1<<pin);

		/* Data direction */
		if (gpio_mode & GPIO_OUT)
			DDIR(port) |= 1<<pin;
		else
			DDIR(port) &= ~(1<<pin);

		/* Primary / alternate function */
		if (gpio_mode & GPIO_AF)
			GPR(port) |= (1<<pin);
		else
			GPR(port) &= ~(1<<pin);

		/* use as gpio? */
		if (ocr == 3)
			GIUS(port) |= (1<<pin);
		else
			GIUS(port) &= ~(1<<pin);

		/* Output / input configuration */
		/* FIXME: I'm not very sure about OCR and ICONF, someone
		* should have a look over it
		*/
		if (pin<16)
		{
			tmp = OCR1(port);
			tmp &= ~( 3<<(pin*2));
			tmp |= (ocr << (pin*2));
			OCR1(port) = tmp;

			if (gpio_mode &	GPIO_AOUT)
				ICONFA1(port) &= ~( 3<<(pin*2));
			if (gpio_mode &	GPIO_BOUT)
				ICONFB1(port) &= ~( 3<<(pin*2));
		}
		else
		{
			tmp = OCR2(port);
			tmp &= ~( 3<<((pin-16)*2));
			tmp |= (ocr << ((pin-16)*2));
			OCR2(port) = tmp;

			if (gpio_mode &	GPIO_AOUT)
				ICONFA2(port) &= ~( 3<<((pin-16)*2));
			if (gpio_mode &	GPIO_BOUT)
				ICONFB2(port) &= ~( 3<<((pin-16)*2));
		}

		if (gpio_mode & GPIO_IRQ_MASK)
		{
			int cfg;
			cfg = (gpio_mode & GPIO_IRQ_CFG_MASK) >> GPIO_IRQ_CFG_POS;

			/* Mask the interrupt */
			GPIO_DisableIRQ(gpio_mode);

			if (pin < 16)
			{
				tmp = ICR1(port);
				tmp &= ~(3 << (pin * 2));
				tmp |= cfg << (pin * 2);
				ICR1(port) = tmp;
			}
			else
			{
				tmp = ICR2(port);
				tmp &= ~(3 << ((pin - 16) * 2));
				tmp |= cfg << ((pin - 16) * 2);
				ICR1(port) = tmp;
			}
		}
	}
	else
	{
		/* PORT KP */
		if (gpio_mode & GPIO_OCEN)
		{
			if (pin < 8)
				printf("Pin %d of GPIO_PORTKP doesn't support open collector", pin);
			else
				KPP_KPCR |= (1 << pin);
		}
		if (gpio_mode & GPIO_OUT)
			KPP_KDDR |= (1 << pin);
		else
			KPP_KDDR &= ~(1 << pin);
	}
}
