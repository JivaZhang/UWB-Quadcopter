

#include "quad_buttons.h"


void quad_buttons_init() {
	// Configure the Right and Left Buttons
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	// Configure the E Stop Button (at least I believe it is the E-Stop :P )
	//HWREG(GPIO_PORTE_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	//HWREG(GPIO_PORTE_BASE + GPIO_O_CR) |= 0x01;
	//HWREG(GPIO_PORTE_BASE + GPIO_O_LOCK) = 0;
	//GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_DIR_MODE_IN);
	//GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	
}


// Currently button presses don't use a "state", though will eventually be 
// configured to.  This state will essentially change what a particular button
// press, or combination of button presses does.  For example, if we are in the
// calibration state, we want the buttons to set max and min values to the speed
// controllers. 
// 
// A button_event happens whenever a button is pressed and released.  Note that
// holding down a button will not trigger multiple events (at this time anyway,
// support for this may be added later).
void quad_buttons_handle_button_events(int button_events) {
	// We should handle the E-stop case separately, and immediately.
	if ((button_events & E_STOP_BUTTON) == E_STOP_BUTTON) {
		quad_motors_set_value(MOTOR_1, 0);
		quad_motors_set_value(MOTOR_2, 0);
		quad_motors_set_value(MOTOR_3, 0);
		quad_motors_set_value(MOTOR_4, 0);
		return;
	}
	
	// For the previous values, the quad_motors_set_value() function will
	// automatically cap the values at 
	int motor_1_prev = quad_motors_get_last_val(MOTOR_1);
	int motor_2_prev = quad_motors_get_last_val(MOTOR_2);
	int motor_3_prev = quad_motors_get_last_val(MOTOR_3);
	int motor_4_prev = quad_motors_get_last_val(MOTOR_4);
	
	switch (button_events) {
		case RIGHT_BUTTON:
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2); // Red for right button
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
			// set max value / Increase Amount
			quad_motors_set_value(MOTOR_1, motor_1_prev + 1);
			quad_motors_set_value(MOTOR_2, motor_2_prev + 1);
			quad_motors_set_value(MOTOR_3, motor_3_prev + 1);
			quad_motors_set_value(MOTOR_4, motor_4_prev + 1);
			break;
		case LEFT_BUTTON:
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); 
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8); // Green for left button
			// set min value / Decrease Amount
			quad_motors_set_value(MOTOR_1, motor_1_prev - 1);
			quad_motors_set_value(MOTOR_2, motor_2_prev - 1);
			quad_motors_set_value(MOTOR_3, motor_3_prev - 1);
			quad_motors_set_value(MOTOR_4, motor_4_prev - 1);
			break;
		case (RIGHT_BUTTON | LEFT_BUTTON):
			// Toggle between max and min values:
			
			// We are not at MAX_VALUE for all motors
			if (motor_1_prev != MAX_VALUE || motor_2_prev != MAX_VALUE ||
				motor_3_prev != MAX_VALUE || motor_4_prev != MAX_VALUE) {	
				
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2); 
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4); // White for max
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);			
				
				quad_motors_set_value(MOTOR_1, MAX_VALUE);
				quad_motors_set_value(MOTOR_2, MAX_VALUE);
				quad_motors_set_value(MOTOR_3, MAX_VALUE);
				quad_motors_set_value(MOTOR_4, MAX_VALUE);
				
			// We are at MAX_VALUE for all motors
			} else {				
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); 
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); // off for min
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);			
				
				quad_motors_set_value(MOTOR_1, 0);
				quad_motors_set_value(MOTOR_2, 0);
				quad_motors_set_value(MOTOR_3, 0);
				quad_motors_set_value(MOTOR_4, 0);
			}

			break;
		default:
			break;		
	}
}

int quad_buttons_get_button_events() {

	int button_events = 0;
	//	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2);
	//	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
	//	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);
	
	// Right button pressed or released
	if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0x00) {
		button_pressed_right = true;
		button_released_right = false;
	} else {
		button_released_right = true;;
	}
	
	
	
	// Left button pressed or released
	if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0x00) {
		button_pressed_left = true;
		button_released_left = false;
	} else {
		button_released_left = true;
	}
	
	
	
	// Check if both buttons are pressed, and neither is released.  Checking 
	// that neither is released is necessary because we only mark a button press
	// as false when both buttons have been released.  Okay...I spent way too
	// long figuring out why this wasn't working the way I wanted :P
	if ((button_pressed_left && button_pressed_right) && 
		(!button_released_right && !button_released_left)) {
		
		button_pressed_left_and_right = true;
	}
	
	
	// Set the events based on press and release combinations.
	if (button_pressed_left_and_right && (button_released_left || button_released_right)) {
		button_events |= LEFT_BUTTON | RIGHT_BUTTON;
		button_pressed_left_and_right = false;
	}
	
	if (button_pressed_right && button_released_right && !button_pressed_left) {
		button_events |= RIGHT_BUTTON;
		button_pressed_right = false;
	}
	
	if (button_pressed_left && button_released_left && !button_pressed_right) {
		button_events |= LEFT_BUTTON;
		button_pressed_left = false;
	}
	
	
	
	// Once both buttons have been released, then we can mark the button_pressed
	// variable as false.  
	if (button_released_left && button_released_right) {
		button_pressed_left = false;
		button_pressed_right = false;
	}
	
	//if (button_released_right && button_released_left) {
	//	button_pressed_left = false;
	//	button_pressed_right = false;
	//}
	
	/*
	// E-stop button
	if (ROM_GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3) == 0x00) {
		button_pressed_left = true;
		button_released_left = false;
	}
	
	if (ROM_GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3) == 0x10) {
		button_released_left = true;
	}
	
	if (button_pressed_right && button_released_right) {
		button_events |= E_STOP_BUTTON;
		button_pressed_left = false;
	}*/
	
	return button_events;
}



