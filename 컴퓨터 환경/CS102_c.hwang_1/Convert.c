/*******************************************************************************
filename    Convert.c
author      c.hwang == Chan Il. Hwang
email       c.hwang@digipen.edu / tktlssla803@gmail.com
course      CS102
assignment  1


Brief Description:
  This has the function definitions for the Assignment 1 in CS102.

*******************************************************************************/
#include <stdio.h> /*printf for BinaryToHex Function*/

/*Set up Some defines that might help*/
/*Number of max hex digits*/
#define HEX_DIGITS  2
/*Number of binary digits per hex value*/
#define BIN_PER_HEX 4
/*Max number of bits*/
#define BIT_COUNT   8 

/*******************************************************************************
   Function: DecimalToBinary

Description: Given a Decimal Value, convert the value to Binary and print the
             result.

     Inputs: decimalNumber - The decimal number to convert.

    Outputs: The converted binary number.
*******************************************************************************/
int DecimalToBinary(int decimalNumber)
{
	/*Shift decimal number and then use modulus operator to check whether first
	  digit number is one or not*/
	/*Number converted from decimal to binary*/
	int convertedBinaryNumber = 0;
	/*How many times I shift decimalNumber*/
	int shiftingNumber;
	/*to show involution of ten*/
	int involution;
	/*to store involution of ten*/
	int digitOfBinary;
	
	/* iterate BIT_COUNT times */
	for(shiftingNumber = 0 ; shiftingNumber < BIT_COUNT ; decimalNumber >>= 1, shiftingNumber++)
	{
		/* to show binary number's digit with decimal number's digit*/
		for(involution = 0 ; involution <= shiftingNumber ; involution++)
		{
			if(involution == 0)
				digitOfBinary = 1;
			else
				digitOfBinary *= 10;
		}
		/*calculate BinaryNumber with decimal number's first digit*/
		convertedBinaryNumber += ((decimalNumber % 2 == 1)? 1 : 0) * digitOfBinary;
	}
	
	return convertedBinaryNumber;
}
/*******************************************************************************
   Function: BinaryToDecimal

Description: Given a binary Value, convert the value to decimal and print the
             result.

     Inputs: binaryNumber - The binary number to convert.

    Outputs: The converted decimal number.
*******************************************************************************/
int BinaryToDecimal(int binaryNumber)
{
	/*Number converted from binary to decimal */
	int convertedDecimalNumber = 0;
	/*How many times I interate*/
	int iteration;
	/*to show involution of two*/
	int involution = 0;
	/*to store involution of two*/
	int digitOfDecimal = 1;
	
	/*iterate BIT_COUNT times*/
	for(iteration = 0 ; iteration < BIT_COUNT ; iteration++)
	{
		/*Check a last digit of binaryNumber and then change binary number to decimal number(multiply the involution of 2) to add it */
		convertedDecimalNumber += ((binaryNumber % 10 == 1)? 1 : 0) * digitOfDecimal;
		
		/*this while loop is for maintaining involution of 2 correctly*/
		while(involution <= iteration)
		{
			digitOfDecimal *= 2;
			involution++;
			binaryNumber /= 10;
		}
	}
	
	return convertedDecimalNumber;
}
/*******************************************************************************
   Function: BinaryToHex

Description: Given a binary Value, convert the value to hex and print the
             result.

     Inputs: binaryNumber - The binary number to convert.

    Outputs: None.
*******************************************************************************/
void BinaryToHex(int binaryNumber)
{
	/*How many times I interate*/
	int iteration;
	/*to show the number of shifting obviously*/
	int shiftingNumber = 0;
	/*to store one digit of hex value*/
	int oneHexValue[2] = {0};
	/*to check last digit of binary*/
	int lastDigitOfBinary;
	
	for(iteration = 0; iteration < HEX_DIGITS ; iteration++)
	{
		shiftingNumber = 0;
		
		while(shiftingNumber < BIN_PER_HEX)
		{
			/*Check the first digit of binary number*/
			lastDigitOfBinary = binaryNumber & 1;
			/*make that number to correct decimal value*/
			lastDigitOfBinary <<= shiftingNumber;
			/*Add the correct decimal value before being 16 to make hex value*/
			oneHexValue[iteration] += lastDigitOfBinary;
			
			shiftingNumber++;
			/*for checking next digit of binary*/
			binaryNumber /= 10;
		}
	}
	/*this switch state is for the numbers that only exist in hex*/
	/*it initialized by 1 because oneHexValue[1] is second digit*/
	for(iteration= 1; iteration >= 0 ; iteration--)
		{
			switch(oneHexValue[iteration])
			{
				case 10:
					printf("A");
					break;
				case 11:
					printf("B");
					break;
				case 12:
					printf("C");
					break;
				case 13:
					printf("D");
					break;
				case 14:
					printf("E");
					break;
				case 15:
					printf("F");
					break;
				default:
					printf("%d",oneHexValue[iteration]);
			}
		}
	printf("\n");
}
