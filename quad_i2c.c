

#include "quad_i2c.h"

void quad_i2c_init() {
	// We will be using I2C pins:
	//		PE4 -- I2C SCL
	//		PE5 -- I2C SDA
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2); // We will be jumping the pins
												// for I2C2 to PB7 and PB6 for
												// the Sensor Hub BoosterPack.
			
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);// Enable the port (if it 
    											// hasn't already been done).
	GPIOPinConfigure(GPIO_PE4_I2C2SCL);
	GPIOPinConfigure(GPIO_PE5_I2C2SDA);
	
	GPIOPinTypeI2C(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	
	I2CMasterInitExpClk(I2C2_BASE, SysCtlClockGet(), false); // set to true to
															 // enable 400kbps
															 // transmissions
	
	// If we want this device to act as a slave, we would need to run the
	// following commands.  This could be useful if we want more than one board
	// communicating together.  Change 'x' to be the I2C module number in use.
	// I2CSlaveEnable(I2Cx_BASE);
	// I2CSlaveInit(I2Cx_BASE, SLAVE_ADDRESS)
	
	
	
	// To write to the I2C data line:
	// -------------------------------------------------------------------------
	// I2CMasterDataPut(I2C2_BASE, data); // send data
	// I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND); // send data
	// I2CSlaveStatus(I2C2_BASE); // check if slave acknowledged data trans.
	
	
	// To read from the I2C data line:
	// -------------------------------------------------------------------------
	// I2CSlaveDataGet(I2C2_BASE);
	
	
	// To set the slave address:
	// -------------------------------------------------------------------------
	// I2CMasterSlaveAddrSet(I2C2_BASE, SLAVE_ADDRESS, true);
	
	// To do a dummy read (to avoid getting stale data):
	// -------------------------------------------------------------------------
	// I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
}
