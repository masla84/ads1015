/*
 * ads1015.c
 *
 *  Created on: 04.04.2022
 *      Author: markus
 */

#include "main.h"
//#include "ads1015.h"

/* Buffer used for transmission */
uint8_t ADC_TxBuffer[ADCTXBUFFERSIZE];

/* Buffer used for reception */
uint8_t ADC_RxBuffer[ADCTXBUFFERSIZE];

int ADS1015_isConnected(ads1015 *adc, I2C_HandleTypeDef *hi2c) {

	uint16_t address = adc->address << 1;
	ADC_TxBuffer[0] = 1; //Address of config register

	/*##-2- Start the transmission process #####################################*/
	    /* While the I2C in reception process, user can transmit data through
	       "aTxBuffer" buffer */

	      if(HAL_I2C_Master_Transmit_IT(hi2c, address, (uint8_t*)ADC_TxBuffer, 1)!= HAL_OK)
	      {
	        /* Error_Handler() function is called when error occurs. */
	        Error_Handler();
	      }

	      /*##-3- Wait for the end of the transfer #################################*/
	      /*  Before starting a new communication transfer, you need to check the current
	          state of the peripheral; if it's busy you need to wait for the end of current
	          transfer before starting a new one.
	          For simplicity reasons, this example is just waiting till the end of the
	          transfer, but application may perform other tasks while transfer operation
	          is ongoing. */
	      while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY)
	      {
	      }

	      /* When Acknowledge failure occurs (Slave don't acknowledge it's address) */
	    if(HAL_I2C_GetError(hi2c) == HAL_I2C_ERROR_AF){
	    	return -1;
	    }

	return 0;
}

//read 16bit
uint16_t ADS1015_Read(ads1015 *adc, I2C_HandleTypeDef *hi2c, uint8_t reg_addr) {

	uint16_t address = adc->address << 1;
	ADC_TxBuffer[0] = reg_addr; //Address of config register
	uint16_t rtn = 0;

	      if(HAL_I2C_Master_Transmit_IT(hi2c, address, (uint8_t*)ADC_TxBuffer, 1)!= HAL_OK)
	      {
	        Error_Handler();
	      }

	      while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY)
	      {
	      }

	      /* When Acknowledge failure occurs (Slave don't acknowledge it's address) */
	    if(HAL_I2C_GetError(hi2c) == HAL_I2C_ERROR_AF){
	    	return -1;
	    }
	    /*##-4- Put I2C peripheral in reception process ###########################*/
	      if(HAL_I2C_Master_Receive_IT(hi2c, address, (uint8_t *)ADC_RxBuffer, 2) != HAL_OK)
	      {
	        Error_Handler();
	      }

	      while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY)
	      {
	      }

	      /* When Acknowledge failure occurs (Slave don't acknowledge it's address)*/
	    if(HAL_I2C_GetError(hi2c) == HAL_I2C_ERROR_AF) {
	    	return -1;
	    }
	    rtn = (ADC_RxBuffer[0]<<8) | ADC_RxBuffer[1];
	return rtn;
}

//write 8bit header + 16bit register
int ADS1015_Write(ads1015 *adc, I2C_HandleTypeDef *hi2c, uint8_t reg_addr, uint16_t reg_value) {

	uint16_t address = adc->address << 1; //HAL_I2C.. expects 7bit address left-shifted by 1
	ADC_TxBuffer[0] = reg_addr; //Address of register
	ADC_TxBuffer[1] = (reg_value & 0xFF00)>>8;
	ADC_TxBuffer[2] = (reg_value & 0xFF);

	      if(HAL_I2C_Master_Transmit_IT(hi2c, address, (uint8_t*)ADC_TxBuffer, 3)!= HAL_OK)
	      {
	        Error_Handler();
	      }

	      while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY)
	      {
	      }

	      /* When Acknowledge failure occurs (Slave don't acknowledge it's address) */
	    if(HAL_I2C_GetError(hi2c) == HAL_I2C_ERROR_AF){
	    	return -1;
	    }
	return 0;
}

int ADS1015_Setup(ads1015 *adc, I2C_HandleTypeDef *hi2c) {

	uint16_t config =
			ADS1015_CONFIG_OS_NOEFF |
			adc->mux |
			adc->pga |
			adc->mode |
			adc->srate;

	return ADS1015_Write(adc,hi2c,ADS1015_ADR_CONFIG,config);
}

uint16_t ADS1015_GetSample(ads1015 *adc, I2C_HandleTypeDef *hi2c) {

	return ADS1015_Read(adc,hi2c,ADS1015_ADR_CONVERS)>>4;

}

//return voltage in mV
int16_t ADS1015_GetVoltage(ads1015 *adc, I2C_HandleTypeDef *hi2c) {

	uint16_t sample = ADS1015_GetSample(adc,hi2c);
//	float samplef = (float)sample;
	//check for negative value
	if(sample > 0x7FF) { sample |= 0x8000;} //add sign

	switch(adc->pga) {
	case(ADS1015_CONFIG_PGA_6144):
		return sample*3; //LSB=3mV
	case(ADS1015_CONFIG_PGA_4096):
		return sample*2; //LSB=2mV
	case(ADS1015_CONFIG_PGA_2048):
		return sample*1; //LSB=1mV
	default:
		return sample*1; //LSB=1mV
	}

	return 0;
}


