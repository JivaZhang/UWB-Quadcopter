

#include "quad_9_axis.h"
#include <math.h>


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


void quad_9_axis_get_euler_angles(NineAxisFloat *naf_cur, NineAxisFloat *naf_prev, float *yaw, float *pitch, float *roll) {

	float gyrox = naf_cur->gyro_x; // Right
	float gyroy = naf_cur->gyro_y; // Forward
	float gyroz = naf_cur->gyro_z; // Up


	float gyrox_prev = naf_prev->gyro_x; // Right
	float gyroy_prev = naf_prev->gyro_y; // Forward
	float gyroz_prev = naf_prev->gyro_z; // Up

	float accelx = naf_cur->accle_x; //
	float accely = naf_cur->accle_y; //
	float accelz = naf_cur->accle_z; //

	//float temp_yaw = arctanf();
	float accel_pitch = arctanf(accelz / accely);
	float accel_roll = arctanf(accelz / accelx);

	float gyro_pitch_prev = naf_prev->gyro_picth;
	float gyro_roll_prev = naf_prev->gyro_roll;

	float gyro_pitch = gyro_pitch_prev + (gyrox - gyrox_prev) * GYRO_SCALE * TIMESTEP;
	float gyro_roll = gyro_roll_prev + (gyroy - gyroy_prev) * GYRO_SCALE * TIMESTEP;

	(*roll) = gyro_roll * 0.95 + accel_roll * 0.05;
	(*pitch) = gyro_pitch * 0.95 + accel_pitch * 0.05;

	naf_prev->gyro_roll = gyro_roll;
	naf_prev->gyro_pitch = gyro_pitch;


}