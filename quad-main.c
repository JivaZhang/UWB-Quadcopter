

#include "quad_common.h"
#include "quad_pwm.h"
#include "quad_buttons.h"
#include "quad_motor.h"
#include "quad_9_axis.h"
#include "quad_rgb_led.h"

// Motor Driver~
	// set_max_duty()
	// set_min_duty()
	// write value (uint16_t value, unsigned long pin)

// 9Axis
	// Read values
	// calibrate
	
// How do we abstract away the pins? Massive switch statement? Clever #defines?

// Potential Interrupts?
	// E-stop
	// Calibrate orientation button
	// Calibrate motors button
	
// How do we calibrate the 9-axis (either the booster pack or the Arduino)?

// How do we calibrate the motors?  Buttons?  Timing (hard coded)?  Other remote?


struct NineAxisReading {
	float gyro_x;
	float gyro_y;
	float gyro_z;
	float accl_x;
	float accl_y;
	float accl_z;
	float magn_x;
	float magn_y;
	float magn_z;
};

struct NineAxisReading orientation_history[10];

struct PWMSettings {
	int period_usec;
	int max_duty_usec;
	int min_duty_usec;
};


void setup() {
	// We need to set the clock before anything else!
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	quad_pwm_init();
	quad_motors_init();
	quad_buttons_init();
	quad_rgb_led_init();
	
}

void loop() {
	int button_events = quad_buttons_get_button_events();
	quad_buttons_handle_button_events(button_events);
}


int main() {
	setup();
	
	quad_rgb_led_set_color(GREEN); // Green light indicates that we have
								   // successfully finished initializing the 
								   // quadcopter.
	while(1) {
		loop();
	}

}
