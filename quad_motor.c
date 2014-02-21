

#include "quad_motor.h"

// Initializes the PWM's for the motors, sets the upper and lower bounds, and 
// starts a default PWM signal.
void quad_motors_init() {
	// Motor 1: Port D Pin 0 (PD0)
	// Motor 2: Port D Pin 1 (PD1)
	// Motor 3: Port A Pin 6 (PA6)
	// Motor 4: Port A Pin 7 (PA7)

	// Set the clock for the PWM
	SysCtlPWMClockSet(SYSCTL_PWMDIV_32);
	pwm_clock_freq_hz = SysCtlClockGet() / 32;
	
	// Enable the peripheral devices / ports
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	// Set the pins on the aformentioned ports
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	
	GPIOPinConfigure(GPIO_PD0_M1PWM0);
	GPIOPinConfigure(GPIO_PD1_M1PWM1);
	GPIOPinConfigure(GPIO_PA6_M1PWM2);
	GPIOPinConfigure(GPIO_PA7_M1PWM3);
	
	// Set the PWM Generators and periods, and calculate numbers of ticks
	
	period_num_ticks = (pwm_clock_freq_hz * PWM_PERIOD_us) / PULSE_TIME_RES_Hz;
	min_pulse_ticks = (pwm_clock_freq_hz * MIN_PULSE_WIDTH_us) / PULSE_TIME_RES_Hz;
	max_pulse_ticks = (pwm_clock_freq_hz * MAX_PULSE_WIDTH_us) / PULSE_TIME_RES_Hz;
	
	PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN);
	
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, period_num_ticks);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, period_num_ticks);
	
	// Enable PWM output
	PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true); // motor 1
	PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, true); // motor 2
	PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT, true); // motor 3
	PWMOutputState(PWM1_BASE, PWM_OUT_3_BIT, true); // motor 4
	
	PWMGenEnable(PWM1_BASE, PWM_GEN_0);
	PWMGenEnable(PWM1_BASE, PWM_GEN_1);
	
	// Set the initial pulse widths to be the minimum pulse width.
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, min_pulse_ticks); // motor 1
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, min_pulse_ticks); // motor 2
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, min_pulse_ticks); // motor 3
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, min_pulse_ticks); // motor 4
}



// This should run through the calibration routine as soon as the quad is 
// powered on.  There should be a slight delay so that the "operator" can 
// get their hands away :P.

// Unfortunately, the ESC's were designed to be calibrated with direct human
// contact, and not by a machine.  Thus, we must use timers to make sure we
// are in the proper step of the calibration process.  
// 
// We could probably do this with 
void quad_motors_calibrate() {
	
}




void quad_motors_set_value(int motor, int value) {
	// Cap our values.
	if (value < 0) {
		value = 0;
	} else if (value >= MAX_VALUE) {
		value = MAX_VALUE;
	}

	uint32_t set_ticks = calculate_num_ticks(value);

	switch(motor) {
		case MOTOR_1:
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, set_ticks); // motor 1
			prev_val_motor_1 = value;
			break;
		case MOTOR_2:
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, set_ticks); // motor 2
			prev_val_motor_2 = value;
			break;
		case MOTOR_3:
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, set_ticks); // motor 3
			prev_val_motor_3 = value;
			break;
		case MOTOR_4:
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, set_ticks); // motor 4
			prev_val_motor_4 = value;
			break;
	}
}



int quad_motors_get_last_val(int motor) {
	switch(motor) {
		case MOTOR_1:
			return prev_val_motor_1;
			break;
		case MOTOR_2:
			return prev_val_motor_2;
			break;
		case MOTOR_3:
			return prev_val_motor_3;
			break;
		case MOTOR_4:
			return prev_val_motor_4;
			break;
	}
}


// calculate_num_ticks() calculates the desired pulse width in ticks based on
// the value provided, the MAX_VALUE, and the min_pulse_ticks and
// max_pulse_ticks values.
// 
// The routine does use a rudimentary form of rounding. Note that this rounding
// is still not perfect as it will be off if MAX_VALUE is an odd number.  This
// is easily mitigated by setting MAX_VALUE to be an even number.
uint32_t calculate_num_ticks(uint32_t value) {
	// We are assuming that value has already been truncated to be in the range
	// [0, MAX_VALUE].  Bad things could happen otherwise!
	
	uint32_t return_ticks;
	uint32_t div_remaining;
	uint32_t pulse_range_ticks = max_pulse_ticks - min_pulse_ticks;
	
	return_ticks = (pulse_range_ticks * value) / MAX_VALUE;
	div_remaining = (pulse_range_ticks * value) % MAX_VALUE;
	
	if (div_remaining >= MAX_VALUE / 2) {
		return_ticks++;
	}
	
	return_ticks += min_pulse_ticks;
	return return_ticks;
}


