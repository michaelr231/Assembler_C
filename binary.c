#include "defines.h"
#include "data.h"
#include "binary.h"

extern int ERROR;

char *encode(struct code code)
{
	int i;
	char *str;
	int encode = code.encode;
	str = (char *)malloc((MAX_BITS)*sizeof(char));
	for (i = 0; i < MAX_BITS; i++)
		str[i] = '0';
	str[MAX_BITS] = 0;

	switch (encode)
	{
	case NUMBER:
		ConvertToString(str, ConvertToBinaryStr(code.encodeType, 2), 0, 2);
		ConvertToString(str, ConvertToBinaryStr(code.destAddressingType, 2), 2, 2);
		ConvertToString(str, ConvertToBinaryStr(code.srcAddressingType, 2), 4, 2);
		ConvertToString(str, ConvertToBinaryStr(code.opcode, 4), 6, 4);
		ConvertToString(str, ConvertToBinaryStr(code.numberVal, 13), 2, 13);
		break;

	case ADDRESS:
		ConvertToString(str, ConvertToBinaryStr(code.encodeType, 2), 0, 2);
		ConvertToString(str, ConvertToBinaryStr(code.destAddressingType, 2), 2, 2);
		ConvertToString(str, ConvertToBinaryStr(code.srcAddressingType, 2), 4, 2);
		ConvertToString(str, ConvertToBinaryStr(code.opcode, 4), 6, 4);
		ConvertToString(str, ConvertToBinaryStr(code.addressVal, 13), 2, 13);
		break;

	case JUMP:

		ConvertToString(str, ConvertToBinaryStr(code.destAddressingType, 2), 2, 2);
		ConvertToString(str, ConvertToBinaryStr(code.srcAddressingType, 2), 4, 2);
		ConvertToString(str, ConvertToBinaryStr(code.opcode, 4), 6, 4);
		if (code.opNum == 1)
			ConvertToString(str, ConvertToBinaryStr(code.destAddressingType, 2), 2, 2);
		if(code.opNum != 2)
			ConvertToString(str, "00", 10, 2);
			ConvertToString(str,"00",12,2);

		if (code.opNum == 2)
		{
			ConvertToString(str, ConvertToBinaryStr(code.destAddressingType, 2), 2, 2);
			ConvertToString(str, ConvertToBinaryStr(code.srcAddressingType, 2), 4, 2);
			ConvertToString(str, "11", 10, 2);
		}
		

		break;

	case REGISTER:
		ConvertToString(str, ConvertToBinaryStr(code.encodeType, 2), 0, 2);
		if (code.operandRole == DEST)
			ConvertToString(str, ConvertToBinaryStr(code.reg1, 6), 2, 6);
		if (code.operandRole == SRC)
		ConvertToString(str, ConvertToBinaryStr(code.opcode, 4), 6, 4);
			ConvertToString(str, ConvertToBinaryStr(code.reg1, 6), 8, 6);
		if(code.opNum == 2)
			ConvertToString(str, "11", 10, 2);
			
		break;

	case TWO_REGISTERS:
		ConvertToString(str, ConvertToBinaryStr(code.encodeType, 2), 0, 2);
		ConvertToString(str, ConvertToBinaryStr(code.destReg, 6), 2, 6);
		
		ConvertToString(str, ConvertToBinaryStr(code.srcReg, 6), 8, 6);
		if(code.opNum == 2)
			{
			ConvertToString(str, "11", 10, 2);
			ConvertToString(str,"11",12,2);}
		break;

	case MAIN_COMMAND:
			ConvertToString(str, ConvertToBinaryStr(code.encodeType, 2), 0, 2);

		if (code.opNum == 1)
			ConvertToString(str, ConvertToBinaryStr(code.destAddressingType, 2), 2, 2);
		if(code.opNum != 2){
			ConvertToString(str, "00", 10, 2);
			ConvertToString(str,"00",12,2);}

		if (code.opNum == 2)
		{
			ConvertToString(str, ConvertToBinaryStr(code.destAddressingType, 2), 2, 2);
			ConvertToString(str, ConvertToBinaryStr(code.srcAddressingType, 2), 4, 2);
		}

		ConvertToString(str, ConvertToBinaryStr(code.opcode, 4), 6, 4);
		
		break;
	}
	reverse(str);
	return str;
}
/** returns binary code **/
char *ConvertToBinaryStr(int num, int size) 
{
	int arr[MAX_BITS];
	char *bin = (char *)malloc((MAX_BITS)*sizeof(char));
	int i, j, sign;

	for (i = 0; i < size; i++)
		bin[i] = '0';

	if (num == 0)
		return bin;

	sign = (num >= 0) ? 1 : -1;
	num = sign * num; 

	for (i = 0; num > 0 && i < size; i++)
	{
		arr[i] = num % 2;
		num = num / 2;
	}

	j = i - 1;

	for (i = 0; i < j + 1; i++)
	{
		if (arr[j - i])
			bin[i + size - 1 - j] = '1';
		else
			bin[i + size - 1 - j] = '0';
	}

	if (sign < 0)
	{
		i = size - 1;
		while (i >= 0 && bin[i] != '1')
			i--;
		i--;
		while (i >= 0)
		{
			if (bin[i] == '1')
				bin[i] = '0';

			else if (bin[i] == '0')
				bin[i] = '1';
			i--;
		}
	}

	return bin;
}

/** represent binary code to a string for example 1001000010 **/
void ConvertToString(char *bin, char *num, int start, int size)
{
	int i = start + size - 1;
	int j = 0;

	while (j < size)
	{
		if (num[j] == '1')
			bin[i] = '1';
		i--;
		j++;
	}
}


void reverse(char *s)
{
	int c, i, j;
	for (i = 0, j = MAX_BITS - 1; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}


