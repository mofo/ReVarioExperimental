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

int32_t TEMP2;
int64_t OFF2, SENS2;

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
	for (int i = 1; i <= 7; i ++) {

		command = PROM_READ | (i << 1);

		error = CI2C0_SendBlockSynch(&command, 1, &sent);
		error = CI2C0_RecvBlockSynch(receivedData, 2, &sent);

		epromData.dataRead[i] = receivedData[0] << 8 | receivedData[1];
		//temp = epromData.dataRead[i] << 8;
		//epromData.dataRead[i] = temp | receivedData[1];

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
	uint8_t command;
	byte  error;
	uint32_t outTemp = 0;

	const TickType_t xDelay = 10 / portTICK_PERIOD_MS;

	command = CONVERT_D2_4096;
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

	command = CONVERT_D1_4096;
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

double ms5611ReadTemperature()
{
    uint32_t D2 = ms5611ReadRawTemperature();

    int32_t dT = D2 - (uint32_t)epromData.dataRead[5] * 256;

    int32_t TEMP = 2000 + ((int64_t) dT * epromData.dataRead[6]) / 8388608;

    TEMP2 = 0;

    if (TEMP < 2000)
    {
    	TEMP2 = (dT * dT) / (2 << 30);
    }

    TEMP = TEMP - TEMP2;

    return ((double)TEMP/100);
}

int32_t ms5611ReadPressure()
{
    uint32_t D1 = ms5611ReadRawPressure();

    uint32_t D2 = ms5611ReadRawTemperature();
    int32_t dT = D2 - (uint32_t)epromData.dataRead[5] * 256;

    int64_t OFF = (int64_t)epromData.dataRead[2] * 65536 + (int64_t)epromData.dataRead[4] * dT / 128;
    int64_t SENS = (int64_t)epromData.dataRead[1] * 32768 + (int64_t)epromData.dataRead[3] * dT / 256;

	int32_t TEMP = 2000 + ((int64_t) dT * epromData.dataRead[6]) / 8388608;

	OFF2 = 0;
	SENS2 = 0;

	if (TEMP < 2000)
	{
	    OFF2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / 2;
	    SENS2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / 4;
	}

	if (TEMP < -1500)
	{
	    OFF2 = OFF2 + 7 * ((TEMP + 1500) * (TEMP + 1500));
	    SENS2 = SENS2 + 11 * ((TEMP + 1500) * (TEMP + 1500)) / 2;
	}

	OFF = OFF - OFF2;
	SENS = SENS - SENS2;

    uint32_t P = (D1 * SENS / 2097152 - OFF) / 32768;

    return P;
}

// Calculate altitude from Pressure & Sea level pressure
double ms5611GetAltitude(double pressure, double seaLevelPressure)
{
    return (44330.0f * (1.0f - pow((double)pressure / (double)seaLevelPressure, 0.1902949f)));
}

// Calculate sea level from Pressure given on specific altitude
double ms5611GetSeaLevel(double pressure, double altitude)
{
    return ((double)pressure / pow(1.0f - ((double)altitude / 44330.0f), 5.255f));
}
