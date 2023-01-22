/*
 * ads1015.h
 *
 *  Created on: 03.04.2022
 *      Author: markus
 */

#ifndef INC_ADS1015_H_
#define INC_ADS1015_H_
#include "stm32l4xx_hal_def.h" //uint16 definition

#define ADS1015_ADDRESS_GND 0x48 //7-bit unshifted default I2C Address
#define ADS1015_ADDRESS_VDD 0x49
#define ADS1015_ADDRESS_SDA 0x4A
#define ADS1015_ADDRESS_SCL 0x4B

//Register addresses
#define ADS1015_ADR_CONVERS      0x00
#define ADS1015_ADR_CONFIG       0x01
#define ADS1015_ADR_LOWTHRESH    0x02
#define ADS1015_ADR_HITHRESH     0x03

//Operational status
#define ADS1015_CONFIG_OS_NOEFF      0x0000
#define ADS1015_CONFIG_OS_SINGLE     0x8000
#define ADS1015_CONFIG_OS_READY      0x8000
#define ADS1015_CONFIG_OS_NOTREADY   0x0000

#define ADS1015_CONFIG_MODE_CONT     0x0000
#define ADS1015_CONFIG_MODE_SINGLE   0x0100

#define ADS1015_CONFIG_MUX_SING_AIN0    		0x4000
#define ADS1015_CONFIG_MUX_SING_AIN1    		0x5000
#define ADS1015_CONFIG_MUX_SING_AIN2    		0x6000
#define ADS1015_CONFIG_MUX_SING_AIN3    		0x7000
#define ADS1015_CONFIG_MUX_DIFF_AINP0_AINN1  	0x0000
#define ADS1015_CONFIG_MUX_DIFF_AINP0_AINN3  	0x1000
#define ADS1015_CONFIG_MUX_DIFF_AINP1_AINN3  	0x2000
#define ADS1015_CONFIG_MUX_DIFF_AINP2_AINN3  	0x3000


#define ADS1015_CONFIG_SRATE_128HZ    0x0000
#define ADS1015_CONFIG_SRATE_250HZ    0x0020
#define ADS1015_CONFIG_SRATE_490HZ    0x0040
#define ADS1015_CONFIG_SRATE_920HZ    0x0060
#define ADS1015_CONFIG_SRATE_1600HZ   0x0080
#define ADS1015_CONFIG_SRATE_2400HZ   0x00A0
#define ADS1015_CONFIG_SRATE_3300HZ   0x00C0

// #define ADS1015_CONFIG_PGA_MASK      (0X0E00)
#define ADS1015_CONFIG_PGA_6144     	0X0000  // +/- 6.144v
#define ADS1015_CONFIG_PGA_4096         0X0200  // +/- 4.096v
#define ADS1015_CONFIG_PGA_2048        	0X0400  // +/- 2.048v
#define ADS1015_CONFIG_PGA_1024       	0X0600  // +/- 1.024v
#define ADS1015_CONFIG_PGA_0512        	0X0800  // +/- 0.512v
#define ADS1015_CONFIG_PGA_0256        	0X0A00  // +/- 0.256v

#define ADS1015_CONFIG_CMODE_TRAD   (0x0000)  // Traditional comparator with hysteresis (default)
#define ADS1015_CONFIG_CMODE_WINDOW (0x0010)  // Window comparator
#define ADS1015_CONFIG_CPOL_ACTVLOW (0x0000)  // ALERT/RDY pin is low when active (default)
#define ADS1015_CONFIG_CPOL_ACTVHI  (0x0008)  // ALERT/RDY pin is high when active
#define ADS1015_CONFIG_CLAT_NONLAT  (0x0000)  // Non-latching comparator (default)
#define ADS1015_CONFIG_CLAT_LATCH   (0x0004)  // Latching comparator
#define ADS1015_CONFIG_CQUE_1CONV   (0x0000)  // Assert ALERT/RDY after one conversions
#define ADS1015_CONFIG_CQUE_2CONV   (0x0001)  // Assert ALERT/RDY after two conversions
#define ADS1015_CONFIG_CQUE_4CONV   (0x0002)  // Assert ALERT/RDY after four conversions
#define ADS1015_CONFIG_CQUE_NONE    (0x0003)  // Disable the comparator and put ALERT/RDY in high state (default)



#define ADCTXBUFFERSIZE 10

typedef struct adc {
	uint8_t address;
	uint16_t mode;   //sampling mode
	uint16_t status;
	uint16_t pga;
	uint16_t mux;
	uint16_t srate;
}ads1015;

int ADS1015_isConnected(ads1015 *adc, I2C_HandleTypeDef *hi2c);
int ADS1015_Setup(ads1015 *adc, I2C_HandleTypeDef *hi2c);
uint16_t ADS1015_GetSample(ads1015 *adc, I2C_HandleTypeDef *hi2c);
int16_t ADS1015_GetVoltage(ads1015 *adc, I2C_HandleTypeDef *hi2c);

uint16_t ADS1015_Read(ads1015 *adc, I2C_HandleTypeDef *hi2c, uint8_t reg_addr);
int ADS1015_Write(ads1015 *adc, I2C_HandleTypeDef *hi2c, uint8_t reg_addr, uint16_t reg_value);

#endif /* INC_ADS1015_H_ */
