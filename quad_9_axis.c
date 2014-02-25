

#include "quad_9_axis.h"


void quad_9_axis_init() {
	// Configure the Gyroscope sensitivity
	I2CMasterSlaveAddrSet(I2C2_BASE, SLAVE_ADDR_9_AXIS, false);
	I2CMasterDataPut(I2C2_BASE, GYRO_CONFIG);
	I2CMasterDataPut(I2C2_BASE, FS_SEL << 3);
	
	// Configure the Accelerometer sensitivity
	I2CMasterSlaveAddrSet(I2C2_BASE, SLAVE_ADDR_9_AXIS, false);
	I2CMasterDataPut(I2C2_BASE, GYRO_CONFIG);
	I2CMasterDataPut(I2C2_BASE, FS_SEL << 3);

	// If an error occured here, hop into an infinite loop and set the led RED.
	if (I2CMasterErr(I2C2_BASE) != I2C_MASTER_ERR_NONE) {
		quad_rgb_led_set_color(RED);
		while(1) {
			;
		}
	}
	
}
