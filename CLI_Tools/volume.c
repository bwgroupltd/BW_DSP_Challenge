/***********************************************************************************
** Project      : DSP Engineering Test - Raspberry Pi
** File         : volume.c
** Purpose      : Sets/Reads the MasterVolume level in the HifiBerry DSP.
************************************************************************************
** This program is the confidential and proprietary product of: Bowers & Wilkins.
** Unauthorised use, reproduction or transfer of this program is strictly prohibited
************************************************************************************
** Copyright 2021 Bowers & Wilkins.
** (Subject to limited distribution and restricted disclosure only)
** All rights reserved.
************************************************************************************
** Note:
** 	File 		contains THREE deliberate syntax errors.
**	setVolume() contains ONE   deliberate logic error.
***********************************************************************************/

/***********************************************************************************
** Includes
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***********************************************************************************
** Function Prototypes
*/

void setVolume(int volume);
uint32_t getVolume(void);

/***********************************************************************************
** Global Variables
*/

int masterVolAddr 	= 0x2f;
float lsbVal 		= pow(2,-24);

/***********************************************************************************
** Functions
*/

/***********************************************************************************
** \brief   Main Function (Program Entry Point)
**
** \inputs  Text String - Either a 'get' command or percentage value to set the volume
**
** \return  Nothing
*/
void
main (
	int argc,
	char** argv
) {
	if (argc != 2) {
		printf("Enter one argument, Volume Percentage (0-100)\n");
	}
	else if (!strcmp(argv[1], "get")) {
		getVolume();
	}
	else {
		/* Convert String to Integer */
		char *endptr;
		int value = strtol(argv[1], &endptr, 10);
		
		if (endptr == argv[2]){
			printf("Invalid Number\n");
		}
		else if (*endptr){
			printf("Trailing characters after number\n");
		}
		setVolume(value);
	}
}

/***********************************************************************************
** \brief   Sets DSP Volume Fader
**
** \inputs  int - Volume (%)
**
** \return  Nothing
**
** \notes	HifiBerry 'dsptoolkit' MUST be installed for this to work
**			Converts % to dB value (0% = -80.0dB, 50% = -40dB, 100% = 0.0dB)
**			Converts dB to suitable format for the DSP
** 			Calls DSP write function
*/
void
setVolume(
	int volume
) {
	char s[50];
	float dB;
	float linear,
	uint32_t binary;
	
	/* Convert % to dB to linear */
	dB = ((float) volume * 0.8f) - 80.0f;
	
	linaer = powf(10, dB);
	binary = (linear / lsbVal);
	
	/* Print Volume */
	printf("Set volume to %d = %.2fdB\n", volume, dB);
	printf("Hex value of volume to DSP: 0x%.8x\n", binary);
	
	/* Transmit To DSP I2C Driver (as ASCII String) */
	sprintf(s, "dsptoolkit write-mem 0x%x 0x%.8x", masterVolAddr, binary); 
	system(s);
}

/***********************************************************************************
** \brief   Gets DSP Volume Fader Level
**
** \inputs  N/A
**
** \return  DSP Volume (%)
**
** \notes	HifiBerry 'dsptoolkit' MUST be installed for this to work
**			Sends read command to DSP
**			Pipes output into buffer to extract value
** 			Removes whitespace from returned value
** 			Convert from Hex to Percentage
*/
uint32_t
getVolume(
	void
) {
	FILE *fp;
	char s[50];
	char data[16];
	char c = ' ';
	uint32_t vol;
	float linear;
	float dB;
	int i;
	int j;

	/* Read Volume from DSP I2C Driver (As ASCII String) */
	sprintf(s, "dsptoolkit read-hex 0x%x", masterVolAddr); 	
	fp = popen(s, "r");
	
	if (fp == NULL) {
		printf("Failed to run command\n");
		exit(1);
	}

	while (fgets(data, sizeof(data), fp) != NULL) {
		printf("%s", data);
	}
	pclose(fp);
	
	/* Remove Whitespace */
	for (i = j = 0; i < strlen(data); ++i){
		if (data[i] != c) {
			data[j++] = data[i];
		}
	}
	data[j] = '\0';

	/* Data Conversion */
	vol = strtol(data, NULL, 16);	
	printf("Hex value = %x\n", vol);
	linear = ((float) vol * lsbVal);
	dB = (20.0f * log10f(linear));
	vol = ((dB + 80.0f) / 0.8f);
	printf("Volume = %d%% = %fdB\n", vol, dB);
	
	return vol;
}
