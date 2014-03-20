

#include "quad_9_axis.h"
#include <math.h>


void quad_9_axis_init() {

	// Configure the Gyroscope sensitivity
	I2CMasterSlaveAddrSet(I2C2_BASE, SLAVE_ADDR_9_AXIS, false);
	I2CMasterDataPut(I2C2_BASE, WHO_AM_I);
	I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	while(I2CMasterBusy(I2C2_BASE)) {
		;
	}
	
	I2CMasterSlaveAddrSet(I2C2_BASE, SLAVE_ADDR_9_AXIS, true);
	I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
	while(I2CMasterBusy(I2C2_BASE)) {
		;
	}

	int j = 0;
	int i = 0;
	while (i < 100000) {
		i++;
		j++;
	}


	I2CMasterSlaveAddrSet(I2C2_BASE, SLAVE_ADDR_9_AXIS, false);
	I2CMasterDataPut(I2C2_BASE, POWER_SETTING);
	I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy(I2C2_BASE)) {
		;
	}
	I2CMasterDataPut(I2C2_BASE, 0x00);
	I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
	while(I2CMasterBusy(I2C2_BASE)) {
		;
	}


	i = 0;
	while (i < 100000) {
		i++;
		j++;
	}


	/*
	// The code here configures the Gyroscope and  Accelerometer's 
	// sensitivity.
	// Configure the Gyroscope sensitivity
	I2CMasterSlaveAddrSet(I2C2_BASE, SLAVE_ADDR_9_AXIS, false);

	//I2CMasterDataPut(I2C2_BASE, 0x00);
	I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy(I2C2_BASE)) {
		;
	}

	I2CMasterDataPut(I2C2_BASE, GYRO_CONFIG);
	I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
	while(I2CMasterBusy(I2C2_BASE)) {
		;
	}

	I2CMasterDataPut(I2C2_BASE, FS_SEL << 3);
	I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
	while(I2CMasterBusy(I2C2_BASE)) {
		;
	}
	


	// Configure the Accelerometer sensitivity
	I2CMasterSlaveAddrSet(I2C2_BASE, SLAVE_ADDR_9_AXIS, false);

	//I2CMasterDataPut(I2C2_BASE, 0x00);
	I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy(I2C2_BASE)) {
		;
	}

	I2CMasterDataPut(I2C2_BASE, ACCEL_CONFIG);
	I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
	while(I2CMasterBusy(I2C2_BASE)) {
		;
	}

	I2CMasterDataPut(I2C2_BASE, FS_SEL << 3);
	I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
	while(I2CMasterBusy(I2C2_BASE)) {
		;
	}*/
}


uint16_t quad_9_axis_read_register(uint8_t reg) {
	I2CMasterSlaveAddrSet(I2C2_BASE, SLAVE_ADDR_9_AXIS, false);
	I2CMasterDataPut(I2C2_BASE, reg);
	I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	while(I2CMasterBusy(I2C2_BASE)) {
		;
	}
	
	I2CMasterSlaveAddrSet(I2C2_BASE, SLAVE_ADDR_9_AXIS, true);
	I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
	while(I2CMasterBusy(I2C2_BASE)) {
		;
	}
	uint16_t temp = (uint16_t)((I2CMasterDataGet(I2C2_BASE)) & 0x000000FF);
	return temp;
}

void quad_9_axis_read_raw_data() {
	nar_cur.gyro_x = quad_9_axis_read_register(GYRO_XOUT_H) << 8;
	nar_cur.gyro_x |= quad_9_axis_read_register(GYRO_XOUT_L);
	
	nar_cur.gyro_y = quad_9_axis_read_register(GYRO_YOUT_H) << 8;
	nar_cur.gyro_y |= quad_9_axis_read_register(GYRO_YOUT_L);
	
	nar_cur.gyro_z = quad_9_axis_read_register(GYRO_ZOUT_H) << 8;
	nar_cur.gyro_z |= quad_9_axis_read_register(GYRO_ZOUT_L);



	nar_cur.accel_x = quad_9_axis_read_register(ACCEL_XOUT_H) << 8;
	nar_cur.accel_x |= quad_9_axis_read_register(ACCEL_XOUT_L);
	
	nar_cur.accel_y = quad_9_axis_read_register(ACCEL_YOUT_H) << 8;
	nar_cur.accel_y |= quad_9_axis_read_register(ACCEL_YOUT_L);
	
	nar_cur.accel_z = quad_9_axis_read_register(ACCEL_ZOUT_H) << 8;
	nar_cur.accel_z |= quad_9_axis_read_register(ACCEL_ZOUT_L);
}


/*
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

	naf_prev->gyro_roll = *roll;
	naf_prev->gyro_pitch = *pitch;


}*/