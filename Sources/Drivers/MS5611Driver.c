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

// List of the API these are all synchronous

bool ms5611Init()
{
	word sent;
	char command;
	char receivedData[2];
	byte  error;
	int i = 0;

	CI2C0_SelectSlave(0x77);

	//ms5611Reset();

	// Read the EPROM data once at the beginning
	for (int i = 0; i <= 7; i ++) {
		command = PROM_READ | (i << 1);
		error = CI2C0_SendBlockSynch(&command, 1, &sent);
		error = CI2C0_RecvBlockSynch(receivedData, 2, &sent);
		epromData.dataRead[i] = receivedData[0] << 8 | receivedData[1];
	}

	return TRUE;
}

bool ms5611Reset()
{
	word sent;
	char command;

	// Send Reset command
	command = RESET;
	CI2C0_SendBlockSynch(&command, 1, &sent);

	return TRUE;
}

EPROM_5611* ms5611GetEprom ()
{
	return &epromData;
}


uint32 ms5611ReadRawTemperature()
{
	word sent;
	char command;
	byte  error;
	uint32 outTemp = 0;

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

int32 dT = 0;

uint32  ms5611ReadTemperature()
{
	int32 Temp = 0;
	uint32 tempRead = 0;
	const int32 multiplier = 1 << 8;
	const int32 multiplier1 = 1 << 23;

	tempRead = ms5611ReadRawTemperature();

	dT = tempRead - (epromData.factoryData.refTemperature * multiplier);
	Temp = 2000 + (dT * epromData.factoryData.tempCoeffofTemp / multiplier1);
	return Temp;
}


uint32 ms5611ReadRawPressure()
{
	word sent;
	char command;
	byte  error;
	uint32 outPress = 0;

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

uint32  ms5611ReadPressure()
{
	long long OFF = 0;
	long long SENS = 0;
	uint32 P = 0;
	uint32 outPress = ms5611ReadRawPressure();

	const int32 pow2_16 = 1 << 16;
	const int32 pow2_7 = 1 << 7;
	const int32 pow2_8 = 1 << 8;
	const int32 pow2_15 = 1 << 15;
	const int32 pow2_21 = 1 << 21;

	OFF = (epromData.factoryData.pressOffset * pow2_16) + (epromData.factoryData.tempCoeffPressOff * dT) / pow2_7;
	SENS = (epromData.factoryData.pressSens * pow2_15) + (epromData.factoryData.tempCoeffPressSens * dT) / pow2_8;
	P = ((outPress * SENS / pow2_21) - OFF) / pow2_15;

	return P;
}
