/*
 * MS5611Driver.c
 *
 *  Created on: Nov 8, 2014
 *      Author: gian
 */

#include "MS5611Driver.h"
#include "CI2C0.h"
#include "FreeRTOS.h"
#include "task.h"

uint8 lastPressureRead[3];
uint8 lastTemperatureRead[3];
EPROM_5611 epromData;

int32_t dT = 0;
int32_t TEMP = 0;
int32_t T2 = 0;

// List of the API these are all synchronous

bool ms5611Init()
{
	word sent;
	uint8_t command;
	uint8_t receivedData[2];
	byte  error;
	int i = 0;
	uint16_t temp = 0;

	CI2C0_SelectSlave(0x77);

	//ms5611Reset();

	const TickType_t xDelay = 5 / portTICK_PERIOD_MS;

	// Read the EPROM data once at the beginning
	for (int i = 0; i <= 7; i ++) {
		command = PROM_READ | (i << 1);
		error = CI2C0_SendBlockSynch(&command, 1, &sent);
		error = CI2C0_RecvBlockSynch(receivedData, 2, &sent);

		epromData.dataRead[i] = receivedData[0];
		temp = epromData.dataRead[i] << 8;
		epromData.dataRead[i] = temp | receivedData[1];

		// Wait 5ms for the sampling to complete
		vTaskDelay(xDelay);

	}

#if 0

	for (int i = 7; i >= 0; i --) {
		command = PROM_READ | (i << 1);
		error = CI2C0_SendBlockSynch(&command, 1, &sent);
		error = CI2C0_RecvBlockSynch(receivedData, 2, &sent);

		epromData.dataRead[i] = receivedData[0] << 8 | receivedData[1];

		// Wait 5ms for the sampling to complete
		vTaskDelay(xDelay);

	}

#endif

	//epromData.factoryData.refTemperature = 33464;
	//epromData.factoryData.tempCoeffofTemp = 28312;

	return TRUE;
}

bool ms5611Reset()
{
	word sent;
	char command;

	// Send Reset command
	command = RESET;
	CI2C0_SendBlockSynch(&command, 1, &sent);

	const TickType_t xDelay = 10 / portTICK_PERIOD_MS;
	// Wait 5ms for the sampling to complete
	vTaskDelay(xDelay);

	return TRUE;
}

EPROM_5611* ms5611GetEprom ()
{
	return &epromData;
}


uint32_t ms5611ReadRawTemperature()
{
	word sent;
	char command;
	byte  error;
	uint32_t outTemp = 0;

	const TickType_t xDelay = 10 / portTICK_PERIOD_MS;

	command = CONVERT_D2_1024;
	error = CI2C0_SendBlockSynch(&command, 1, &sent);

	// Wait 10ms for the sampling to complete
	vTaskDelay(xDelay);

	// No error check at the moment this is just a test
	command = ADC_READ;
	error = CI2C0_SendBlockSynch(&command, 1, &sent);
	error = CI2C0_RecvBlockSynch(lastTemperatureRead, 3, &sent);

	outTemp = lastTemperatureRead[0];
	outTemp = outTemp << 8;
	outTemp |= lastTemperatureRead[1];
	outTemp = outTemp << 8;
	outTemp	|= lastTemperatureRead[2];

	return outTemp;
}

uint32_t ms5611ReadRawPressure()
{
	word sent;
	char command;
	byte  error;
	uint32_t outPress = 0;

	const TickType_t xDelay = 10 / portTICK_PERIOD_MS;

	command = CONVERT_D1_1024;
	error = CI2C0_SendBlockSynch(&command, 1, &sent);

	// Wait 10ms for the sampling to complete
	vTaskDelay(xDelay);

	// No error check at the moment this is just a test
	command = ADC_READ;
	error = CI2C0_SendBlockSynch(&command, 1, &sent);
	error = CI2C0_RecvBlockSynch(lastPressureRead, 3, &sent);

	outPress = lastPressureRead[0] << 16 | lastPressureRead[1] << 8 | lastPressureRead[2];

	return outPress;
}

int32_t  ms5611ReadTemperature()
{

    uint32_t D2 = ms5611ReadRawTemperature();

    dT = 0;
    TEMP = 0;
    T2 = 0;

    dT = D2 - ((uint32_t)epromData.factoryData.refTemperature << 8);

    int64_t result = (int64_t)dT * (int64_t)epromData.factoryData.tempCoeffofTemp;

    TEMP = 2000 + (result >> 23);

	if (TEMP < 2000)
	{
		T2 = (dT * dT) >> 31;
	}

	return TEMP - T2;
}

uint32_t  ms5611ReadPressure()
{

	ms5611ReadTemperature();

    uint32_t D1 = ms5611ReadRawPressure();

    int64_t OFF = ((uint64_t)epromData.factoryData.pressOffset << 16) + (((uint64_t)epromData.factoryData.tempCoeffPressOff * dT) >> 7);
    int64_t SENS = ((uint64_t)epromData.factoryData.pressSens << 15) + (((uint64_t)epromData.factoryData.tempCoeffPressSens * dT) >> 8);

    int64_t OFF2 = 0;
    int64_t SENS2 = 0;

	if (TEMP < 2000)
	{
	    OFF2 = 5 * (((TEMP - 2000) * (TEMP - 2000)) >> 1);
	    SENS2 = 5 * (((TEMP - 2000) * (TEMP - 2000)) >> 2);

		if (TEMP < -1500)
		{
		    OFF2 = OFF2 + 7 * ((TEMP + 1500) * (TEMP + 1500));
		    SENS2 = SENS2 + 11 * (((TEMP + 1500) * (TEMP + 1500)) >> 1);
		}
	}

	OFF = OFF - OFF2;
	SENS = SENS - SENS2;

    int32_t P = (((D1 * SENS) >> 21) - OFF) >> 15;

    return P;
}
