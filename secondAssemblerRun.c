#include "defines.h"
#include "data.h"
#include "binary.h"
struct code cmdArray[MAX_ARRAY];
struct data dataArray[MAX_ARRAY];
struct binarycode binArray[MAX_ARRAY];
extern int commandSize;
extern int dataSize;
extern int binarySize;
extern int ERROR;


void binaryCommand(int dec, char *str)
{
	binArray[binarySize].address = dec;
	fromBinaryToDot(binArray[binarySize++].binary, str);
}
/** binary code that transforms the code from : 010111010101
 to ././//./././ for example**/
void fromBinaryToDot(char *dest, char *src)
{
	int i;
	for (i = 0; i < MAX_BITS; i++)
		if(src[i] == '1')
			dest[i] = '/';
		else
			dest[i] = '.';
}
/** start second pass **/
void secondAssemblerRun()
{
	int i;
	char *ch;
	/**for loop that go over and over again and fits the address in the right place that
	already was created **/
	for (i = 0; i < commandSize; i++)
	{
		if (cmdArray[i].encode == MAIN_COMMAND)
		{
			ch = encode(cmdArray[i]);
			binaryCommand(cmdArray[i].address, ch);

			if (cmdArray[i].opNum > 0 && cmdArray[i + 1].encode == TWO_REGISTERS)
			{
				ch = encode(cmdArray[i + 1]);
				binaryCommand(cmdArray[i].address + 1, ch);
			}
			/** if there is one paramters **/
			else if (cmdArray[i].opNum == 1)
			{
				cmdArray[i + 1] = OperandChose(cmdArray[i].destOperand, cmdArray[i + 1]);
				ch = encode(cmdArray[i + 1]);
				binaryCommand(cmdArray[i].address + 1, ch);
			}
			/** if there is two paramters **/
			else if (cmdArray[i].opNum == 2)
			{
				cmdArray[i + 1] = OperandChose(cmdArray[i].srcOperand, cmdArray[i + 1]);
				ch = encode(cmdArray[i + 1]);
				binaryCommand(cmdArray[i].address + 1, ch);

				cmdArray[i + 2] = OperandChose(cmdArray[i].destOperand, cmdArray[i + 2]);
				ch = encode(cmdArray[i + 2]);
				binaryCommand(cmdArray[i].address + 2, ch);
			
			}
			/**if there is three parameters for example : LOOP(W,K) **/
			else if(cmdArray[i].opNum == 3)
			{
				cmdArray[i + 1] = OperandChose(cmdArray[i].srcOperand, cmdArray[i + 1]);
				ch = encode(cmdArray[i + 1]);
				binaryCommand(cmdArray[i].address + 1, ch);

				cmdArray[i + 2] = OperandChose(cmdArray[i].destOperand, cmdArray[i + 2]);
				ch = encode(cmdArray[i + 2]);
				binaryCommand(cmdArray[i].address + 2, ch);
				
				cmdArray[i + 3] = OperandChose(cmdArray[i].destOperand, cmdArray[i + 3]);
				ch = encode(cmdArray[i + 3]);
				binaryCommand(cmdArray[i].address + 3, ch);


			}
		}
	}
	/** converting all data to binary string **/
	for (i = 0; i < dataSize; i++)
	{
		ch = ConvertToBinaryStr(dataArray[i].value, MAX_BITS);
		binaryCommand(dataArray[i].address, ch);
	}
}

/** building the operands for each address and symbol and function **/
struct code OperandChose(char *operand, struct code command)
{
	struct code c;
	c.encode = command.encode;
	c.operandRole = command.operandRole;
	switch (c.encode)
	{
	/** #-4 **/
	case NUMBER: 
		c.encodeType = A;
		/**number without # **/
		c.numberVal = atoi(++operand);
		c.address = getAddress(operand);
		break;
	/** LABEL **/
	case ADDRESS: 
		c.addressVal = getAddress(operand);
		c.address = getAddress(operand);
		c.encodeType = (c.addressVal > 0) ? R : E;
		break;
	/** (r1,K) **/
	case JUMP:
		c.encodeType = A;
		c.address = getAddress(operand);
		c.opcode = getOp(operand);
		break;
	/** r7 **/
	case REGISTER: 
		c.encodeType = A;
		break;
	/** r4,r3 two registers but we already make this function in first run **/
	case TWO_REGISTERS:
		break;
	}
	return c;
}
