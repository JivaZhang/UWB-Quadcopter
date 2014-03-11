#include "quad_common.h"
#include "quad_ranger.h"

void quad_ranger_init() {

	timerValue = 0;
	distance = 0;
	uint32_t ui32Period,timer1Hz,timer2Hz;
	timer1Hz = 17241;
	timer2Hz = 20;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_1);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

	TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT_UP);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

	GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_BOTH_EDGES); // set interupt to both edges

	ui32Period = (SysCtlClockGet() / timer1Hz ); // to triger every 58 usec or ever cm of the range finder.
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
	ui32Period = (SysCtlClockGet()/timer2Hz); // set the frequency for how often we want to check
	TimerLoadSet(TIMER1_BASE, TIMER_A, ui32Period -1);

	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	IntEnable(INT_TIMER1A);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

	IntEnable(INT_GPIOD);
	GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_1);

	IntMasterEnable();





}

void pinIntHandler(void)
{

	GPIOIntClear(GPIO_PORTB_BASE, TIMER_TIMA_TIMEOUT);
	if (GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_1) == 0x40 && timerValue == 0){



			TimerEnable(TIMER0_BASE, TIMER_A); // start counting
		}

		if (GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_1) == 0x00 && timerValue != 0 )
		{

			//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2|GPIO_PIN_3, 0);
			IntDisable(INT_GPIOD);

			if (timerValue != 1)
			{
			if ( distance-timerValue < 5 && distance-timerValue > -5)
			{
			distance = timerValue;
			}
			else
			{
				if (timerValue > distance)
				distance += 5;
				else
				distance -= 5;

			}
			}
			timerValue = 0;
			sendData(distance);
			TimerDisable(TIMER0_BASE,TIMER_A);
			TimerEnable(TIMER1_BASE, TIMER_A);

			//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
		}

}

void Timer0IntHandler(void)
{
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	// Read the current state of the GPIO pin and
	// write back the opposite state

	timerValue+=1;
}

void ResetListener(void)
{
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT); //Clear the timer interrupt


	TimerDisable(TIMER1_BASE,TIMER_A);

	//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);

	IntEnable(INT_GPIOD);

}
