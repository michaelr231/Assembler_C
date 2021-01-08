#include "defines.h"
#include "data.h"

extern struct code cmdArray[MAX_ARRAY];
extern struct data dataArray[MAX_ARRAY];
extern char *entryArray[MAX_ARRAY];
extern char *externArray[MAX_ARRAY];
extern int commandSize;
extern int dataSize;
extern int externSize;
extern int entrySize;
extern int ERROR;
/** function for jump use for bne,jmp,jsr operands **/
void addJump(char **line)
{
	int i = commandSize;
	int hasSymbol = isLabel(line);
	int NumberWords = arraySize(line) - hasSymbol;
	int wordsNum; 
		cmdArray[i].encode = MAIN_COMMAND;
		cmdArray[i].encodeType = A;
		cmdArray[i].address = i;
		cmdArray[i].hasSymbol = hasSymbol;
		/** if this is symbol\label so put into the cmdArray[i].symbol **/
		if (cmdArray[i].hasSymbol)
			cmdArray[i].symbol = cleanLabel(line[DATA_INDEX]);


		cmdArray[i].opcode = getOp(line[OPERTATION_NAME_INDEX]);
		cmdArray[i].group = getGroup(line[OPERTATION_NAME_INDEX]);
		/**if there one word **/
		if (NumberWords == NUM_OPS1)
		{
			wordsNum = 0;
			cmdArray[i].opNum = 0;
		}
		/** if there 2 words **/
		else if (NumberWords == NUM_OPS2)
		{
		/** check if this is operand **/
		if (!isOperand(line[FIRST_INDEX]))
		{
			fprintf(stderr, "error : %s not a operand \n", line[FIRST_INDEX]);
			ERROR = TRUE;
			return;
		}
		
		wordsNum = 1;
		cmdArray[i].opNum = 1;
		cmdArray[i].destOperand = line[FIRST_INDEX];
		cmdArray[i].destAddressingType = checkJumpAddress(cmdArray[i].destOperand);
		cmdArray[i + 1].encode = cmdArray[i].destAddressingType;
		cmdArray[i + 1].operandRole = DEST;
		cmdArray[i + 1].address = i + 1;
		cmdArray[i + 1].hasSymbol = FALSE;

		}

		else if (NumberWords == NUM_OPS4)
		{
		cmdArray[i].opNum= 3;
		cmdArray[i].srcOperand = line[FIRST_INDEX];
		cmdArray[i].srcAddressingType = checkAddress(cmdArray[i].srcOperand);

		cmdArray[i].destOperand = line[SECOND_INDEX];
		cmdArray[i].destAddressingType = checkAddress(cmdArray[i].destOperand);



		cmdArray[i+1].srcOperand = getCleanJumpLeft(line[FIRST_INDEX]);
		cmdArray[i+1].destOperand = getCleanJumpOther(line[SECOND_INDEX]);
		cmdArray[i + 1].srcAddressingType = checkAddress(getCleanJumpLeft(line[FIRST_INDEX]));
		cmdArray[i + 1].destAddressingType = checkAddress(getCleanJumpOther(line[SECOND_INDEX]));
		if(cmdArray[i + 1].srcAddressingType == REGISTER && cmdArray[i + 1].srcAddressingType ==REGISTER)
		{
			wordsNum = 2;
			cmdArray[i].opNum = 2;
			cmdArray[i + 1].encode = TWO_REGISTERS;
			cmdArray[i + 1].srcReg = cmdArray[i].srcOperand[REG_VALUE_INDEX] - '0';
			cmdArray[i + 1].destReg = cmdArray[i].destOperand[REG_VALUE_INDEX] - '0';
			cmdArray[i + 1].address = i + 1;
			cmdArray[i + 1].hasSymbol = FALSE;
		}
		
		else
		{
			wordsNum=3;
		cmdArray[i].opNum = 3;
		cmdArray[i + 1].srcAddressingType = checkAddress(getCleanJumpLeft(line[FIRST_INDEX]));
		cmdArray[i + 1].destAddressingType = checkAddress(getCleanJumpOther(line[SECOND_INDEX]));
		cmdArray[i + 1].encode = cmdArray[i + 1].srcAddressingType;
		cmdArray[i + 1].operandRole = SRC;
		cmdArray[i + 1].address = i + 1;
		cmdArray[i + 1].hasSymbol = FALSE;

		cmdArray[i + 2].encode = cmdArray[i + 2].destAddressingType;
		cmdArray[i + 2].srcAddressingType = checkAddress(getCleanJumpLeft(line[FIRST_INDEX]));
		cmdArray[i + 2].destAddressingType = checkAddress(getCleanJumpOther(line[SECOND_INDEX]));
		cmdArray[i + 2].operandRole = DEST;
		cmdArray[i + 2].address = i + 2;
		cmdArray[i + 2].hasSymbol = FALSE;
		}
	
	}
	/** forword commandSize int + 1 ready for the next word **/
	commandSize = i + wordsNum + 1; 
}

/** addCommand function for regular command **/
void addCommand(char **line)
{
	int i = commandSize;
	int hasSymbol = isLabel(line);
	int NumberWords = arraySize(line) - hasSymbol;
	int wordsNum; /* Real words; only extra words (not including this one) */
	
	cmdArray[i].encode = MAIN_COMMAND;
	cmdArray[i].encodeType = A;
	cmdArray[i].address = i;

	cmdArray[i].hasSymbol = hasSymbol;
	/** if there is a symbol\label **/
	if (cmdArray[i].hasSymbol)
		cmdArray[i].symbol = cleanLabel(line[DATA_INDEX]);

	cmdArray[i].opcode = getOp(line[OPERTATION_NAME_INDEX]);
	cmdArray[i].group = getGroup(line[OPERTATION_NAME_INDEX]);

	if (NumberWords == NUM_OPS1)
	{
		wordsNum = 0;
		cmdArray[i].opNum = 0;
	}

	else if (NumberWords == NUM_OPS2)
	{
		
		if (!isOperand(line[FIRST_INDEX]))
		{
			fprintf(stderr, "error : %s is not a operand \n", line[FIRST_INDEX]);
			ERROR = TRUE;
			return;
		}
		
		
		wordsNum = 1;
		cmdArray[i].opNum = 1;
		cmdArray[i].destOperand = line[FIRST_INDEX];
		cmdArray[i].destAddressingType = checkAddress(cmdArray[i].destOperand);
		cmdArray[i + 1].encode = cmdArray[i].destAddressingType;
		cmdArray[i + 1].operandRole = DEST;
		cmdArray[i + 1].address = i + 1;
		cmdArray[i + 1].hasSymbol = FALSE;
	}

	else if (NumberWords == NUM_OPS4)
	{
		/** if the first paramter is regular operand **/
		if (!isOperand(line[FIRST_INDEX]))
		{
			fprintf(stderr, "error : %s is not a operand \n", line[FIRST_INDEX]);
			ERROR = TRUE;
			return;
		}
		/** if there is ',' between two operands **/
		if (strcmp(line[BETWEEN_INDEX], ",") != 0)
		{
			fprintf(stderr, "error : , %s is missing between the parameters \n",line[BETWEEN_INDEX]);
			ERROR = TRUE;
			return;
		}
		/** if the second paramter is regular operand **/
		if (!isOperand(line[SECOND_INDEX]))
		{
			fprintf(stderr, "error : %s is not an operand \n", line[SECOND_INDEX]);
			ERROR = TRUE;
			return;
		}

		cmdArray[i].opNum = 2;
		cmdArray[i].srcOperand = line[FIRST_INDEX];
		cmdArray[i].srcAddressingType = checkAddress(cmdArray[i].srcOperand);

		cmdArray[i].destOperand = line[SECOND_INDEX];
		cmdArray[i].destAddressingType = checkAddress(cmdArray[i].destOperand);
		/** this is when the two parameters is register example r7,r1 **/
		if (cmdArray[i].srcAddressingType == REGISTER && cmdArray[i].destAddressingType == REGISTER) 
		{
			wordsNum = 1;
			cmdArray[i + 1].encode = TWO_REGISTERS;
			cmdArray[i + 1].srcReg = cmdArray[i].srcOperand[REG_VALUE_INDEX] - '0';
			cmdArray[i + 1].destReg = cmdArray[i].destOperand[REG_VALUE_INDEX] - '0';
			cmdArray[i + 1].address = i + 1;
			cmdArray[i + 1].hasSymbol = FALSE;
		}

		else
		{
		
			wordsNum = 2;
			cmdArray[i + 1].encode = cmdArray[i].srcAddressingType;
			cmdArray[i + 1].operandRole = SRC;
			cmdArray[i + 1].address = i + 1;
			cmdArray[i + 1].hasSymbol = FALSE;

			cmdArray[i + 2].encode = cmdArray[i].destAddressingType;
			cmdArray[i + 2].operandRole = DEST;
			cmdArray[i + 2].address = i + 2;
			cmdArray[i + 2].hasSymbol = FALSE;
			
		}
	}
	/** forword commandSize int + 1 ready for the next word **/
	commandSize = i + wordsNum + 1; 
}

/** adding data function **/
void addData(char **line)
{
	int i = dataSize;
	int j = FIRST_DATA_INDEX;
	int loopFlag = TRUE;
	int hasSymbol = isLabel(line);

	dataArray[i].address = i;
	dataArray[i].hasSymbol = hasSymbol;
	/** if this is a label\symbol **/
	if (!hasSymbol)
		fprintf(stderr, "error : you must enter a label \n");
	/** if there is dataArray[i] is a sybol **/
	if (dataArray[i].hasSymbol)
		dataArray[i].symbol = cleanLabel(line[DATA_INDEX]);

	while (loopFlag)
	{
		/** check if no data numbers **/
		if (strcmp(line[hasSymbol + j], "") == 0 || strcmp(line[hasSymbol + j], "\n") == 0)
		{
			if (j == 1)
				fprintf(stderr, "error : no number is enterd \n"); 
			else
				fprintf(stderr, "error : you need to fix your input \n");
			loopFlag = FALSE;
			break;
		}
		/** checks if there is no number and ',' enterd **/
		else if (strcmp(line[hasSymbol + j], ",") == 0)
		{
			fprintf(stderr, "error : a number was expected \n");
			ERROR = TRUE;
			loopFlag = FALSE;
			break;
		}
		/** checks if there is a number data **/
		if (isNumber(line[hasSymbol + j]))
		{
			dataArray[i].address = i;
			dataArray[i].value = atoi(line[hasSymbol + j]);
			if (j != FIRST_INDEX)
				dataArray[i].hasSymbol = FALSE;
		}

		else
		{	/** not a number but i ignore this , and go back to forword the dataArray[i] **/
			fprintf(stderr, "error : %s in not a number \n", line[hasSymbol + j]);
			i--; 
		}

		if (strcmp(line[hasSymbol + j + 1], "\n") == 0 || line[hasSymbol + j + 1] == '\0')
		{
			loopFlag = FALSE;
			break;
		}
		/**checks if there is only ',' missing **/
		else if (strcmp(line[hasSymbol + j + 1], ",") != 0)
		{ 
			fprintf(stderr, "error : , is missing\n");
			ERROR = TRUE;
			loopFlag = FALSE;
			break;
		}

		j += 2;
		i++;
	}

	dataSize = ++i;
}

/** adding string function **/
void addString(char **line)
{
	int i = dataSize, j;
	int hasSymbol = isLabel(line);
	int NumberWords = arraySize(line) - hasSymbol;
	int slength;

	if (NumberWords != STRING_NUM)
	{
		fprintf(stderr, "error : not correct .command\n");
		ERROR = TRUE;
		return;
	}

	slength = strlen(line[CONTENT_INDEX]);
	/** checks if the string not starts and ends with ' "" ' **/
	if (line[CONTENT_INDEX][FIRST_QUOTE_INDEX] != '"' || line[CONTENT_INDEX][LAST_QUOTE_INDEX] != '"')
	{
		fprintf(stderr, "error : not correct inpuit \n");
		ERROR = TRUE;
		return;
	}

	dataArray[i].address = i;
	dataArray[i].hasSymbol = hasSymbol;
	if (!hasSymbol)
		fprintf(stderr, "Warning - No label was given to the data. It won't be accessible. \n");
	if (dataArray[i].hasSymbol)
		dataArray[i].symbol = cleanLabel(line[DATA_INDEX]);

	for (j = FIRST_CHAR_INDEX; j <= LAST_CHAR_INDEX; i++, j++)
	{
		dataArray[i].address = i;
		dataArray[i].value = line[CONTENT_INDEX][j];
		if (j != FIRST_CHAR_INDEX)
			dataArray[i].hasSymbol = FALSE;
	}

	/* adds zero to the end of the string */
	dataArray[i].address = i;
	dataArray[i].value = 0;
	dataArray[i].hasSymbol = FALSE;


	dataSize = ++i;
}

/** adding extern function .extern example **/
void addExtern(char **line)
{
	int hasSymbol = isLabel(line);
	
	if (externSize < MAX_ARRAY) 
		externArray[externSize] = cleanLabel(line[CONTENT_INDEX]);
	externSize++;

}
/** adding entry function .entry example **/
void addEntry(char **line)
{
	int hasSymbol = isLabel(line);
	if (entrySize < MAX_ARRAY) 
		entryArray[entrySize] = cleanLabel(line[CONTENT_INDEX]);
	entrySize++;
}

/** count the data address that will fit right after the cmd address **/
void DecimalAddress(int add) 
{
	int i;
	for (i = 0; i < commandSize; i++)
		cmdArray[i].address += IC;

	for (i = 0; i < dataSize; i++)
		dataArray[i].address += IC + commandSize;
}

/** getting address for symbol\label **/
int getAddress(char *symbol)
{
	int i,k=130;
	
	for (i = 0; i < commandSize; i++)
		if (cmdArray[i].hasSymbol)
			if (strcmp(cmdArray[i].symbol, symbol) == 0)
				return cmdArray[i].address;

	for (i = 0; i < dataSize; i++)
		if (dataArray[i].hasSymbol)
			if (strcmp(dataArray[i].symbol, symbol) == 0)
				return dataArray[i].address;

	for (i = 0; i < externSize; i++)
		if (strcmp(externArray[i], symbol) == 0)
			return cmdArray[i].address;

	return k;
}
