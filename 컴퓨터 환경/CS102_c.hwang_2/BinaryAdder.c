/*******************************************************************************
filename    BinaryAdder.c
author      Chan Il Hwang
email       c.hwang@digipen.edu
course      CS102
assignment  2


Brief Description:
  This has the function definitions for the Assignment 2 in CS102.

*******************************************************************************/

/*Find First Digit Of something*/
#define FDO(a) a%10
/*Calculate rounding value of that digit with last rounding value*/
#define CRV(a,b,r) (a+b+r)>=2?1:0
/*Calculate decimal values like binary values*/
#define CB(a,b,r) ((a+b+r)==0||(a+b+r)==2)?0:1
#define Bit_Count 8

int BinaryAdder(int first, int second)
{
  /*Use it to put calculated values to right digit*/
  int powerOfTen = 1;
  /*rounding value that is calculated with next digit's values*/
  int rounding = 0;
  int i;
  /*sum of first and second*/
  int sum = 0;
  for(i=0;i<Bit_Count;i++)
  {
    /*plus calculated value of that digit multiplied by powerOfTen*/
    sum += CB(FDO(first),FDO(second),rounding) * powerOfTen;
    /*calculate next rounding value*/
    rounding = CRV(FDO(first),FDO(second),rounding);
    /*adjusting values to next digit*/
    powerOfTen *= 10;
    first /= 10;
    second /= 10;
  }
  /*return the final sum*/
  return sum;
}

