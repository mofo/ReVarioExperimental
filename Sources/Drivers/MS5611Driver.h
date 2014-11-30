/*
 * MS5611Driver.h
 *
 *  Created on: Nov 8, 2014
 *      Author: gian
 */

#ifndef MS5611DRIVER_H_
#define MS5611DRIVER_H_


#include "PE_Types.h"

// Commands definitions

#define 	RESET    		0x1E
#define 	CONVERT_D1_256	0x40
#define 	CONVERT_D1_512	0x42
#define 	CONVERT_D1_1024	0x44
#define 	CONVERT_D1_2048	0x46
#define 	CONVERT_D1_4096	0x48
#define 	CONVERT_D2_256	0x50
#define 	CONVERT_D2_512	0x52
#define 	CONVERT_D2_1024	0x54
#define 	CONVERT_D2_2048	0x56
#define 	CONVERT_D2_4096	0x58
#define 	ADC_READ		0x00
#define 	PROM_READ		0xA0 // this goes with the address that needs to be read

typedef union {

	struct {
		uint16 factoryData;
		uint16 pressSens;
		uint16 pressOffset;
		uint16 tempCoeffPressSens;
		uint16 tempCoeffPressOff;
		uint16 refTemperature;
		uint16 tempCoeffofTemp;
		uint16 SerialAndCRC;
	} factoryData;

	uint16 dataRead[8];

} EPROM_5611;

// List of the API

bool ms5611Init();
bool ms5611Reset();
EPROM_5611* ms5611GetEprom ();
uint32_t ms5611ReadRawTemperature();
int32_t  ms5611ReadTemperature();
uint32_t ms5611ReadRawPressure();
uint32_t  ms5611ReadPressure();

#endif /* MS5611DRIVER_H_ */
