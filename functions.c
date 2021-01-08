#include "defines.h"
#include "data.h"

extern int ERROR;
/** list of the all operand by order with the how many operands need to use **/
struct opcode {
	char *name;
	int group;
		} opr[] = {
		{ "mov", TWO_OPERANDS },
		{ "cmp", TWO_OPERANDS },
		{ "add", TWO_OPERANDS },
		{ "sub", TWO_OPERANDS },
		{ "not", ONE_OPERAND },
		{ "clr", ONE_OPERAND },
		{ "lea", TWO_OPERANDS },
		{ "inc", ONE_OPERAND },
		{ "dec", ONE_OPERAND },
		{ "jmp", ONE_OPERAND },
		{ "bne", ONE_OPERAND },
		{ "red", ONE_OPERAND },
		{ "prn", ONE_OPERAND },
		{ "jsr", ONE_OPERAND },
		{ "rts", ZERO_OPERANDS },
		{ "stop", ZERO_OPERANDS } 
		};


/* check if there is a label that ends with ':' and return without ':' */
int isLabel(char **line)
{
	int i;
	for (i = 0; line[0][i] != '\0'; i++);
	return line[0][--i] == ':';
}

/* remove the ':' from label */
char *cleanLabel(char *label)
{
	int i;
	char *new = (char *)malloc(LINE_WORD * sizeof(char));

	for (i = 0; label[i] != '\0'; i++)
		new[i] = label[i];
	/** if there is ':' so --i delets it **/
	if (new[--i] == ':')
		new[i] = 0;
 
	if (!isSymbol(new))
	{
		fprintf(stderr, "error : %s is not a legal label. \n", new);
		ERROR = TRUE;
	}

	return new;
}
/** checks if there is '(' from left side **/
char *getCleanJumpLeft(char *label)
{
	int i=0,k;
	char *new = (char *)malloc(LINE_WORD * sizeof(char));

	while(label[i] != '(')
	i++;
	k=i;
	for(i=0;i < strlen(label);i++,k++)
		new[i]=label[k+1];
	return new;
}
/** checks if there is ')' from the right side **/
char *getCleanJumpOther(char *label)
{
	int i=0;
	char *new = (char *)malloc(LINE_WORD * sizeof(char));
	for(i=0;label[i] != ')' ;i++)
		new[i]=label[i];
 
	return new;
}
/** getting a clear jump function without ')' or '(' **/
char *getCleanJump(char *label)
{
	int i;
	char *new = (char *)malloc(LINE_WORD * sizeof(char));

	for (i = 0; label[i] != '('; i++)
		new[i] = label[i];

	if (new[--i] == '(')
		new[i] = 0;
 
	if (!isSymbol(new))
	{
		fprintf(stderr, "Error - %s is not a legal label. \n", new);
		ERROR = TRUE;
	}

	return new;

}
/** makes all scaned line to array **/
char **getLineArray(char *cmd)
{
	int i, j, k;
	int inWord = TRUE;

	char **arr = malloc(MAX_WORDS * sizeof(char *));
	for (j = 0; j < MAX_WORDS; j++)
		arr[j] = malloc(LINE_WORD * sizeof(char));
	i = j = k = 0;
	/** separte to array box's **/
	while (cmd[i] != '\n')
	{
		if (cmd[i] != ' ' && cmd[i] != '\t')
		{
			if (k >= LINE_WORD)
				arr[j][--k] = '\0';

			if (j >= MAX_WORDS)
			{
				arr[--j][0] = '\n';
				arr[j][1] = '\0';
			}

			if (inWord && cmd[i] == ',')
			{
				arr[j][k] = '\0';
				j++;

				k = 0;
				arr[j][k] = cmd[i];
				/** we do not set  arr[j][1] = '\0'  since it will do so next iteration **/

				inWord = FALSE;
			}
			/** if it not inWord **/
			else if (!inWord) 
			{
				if (k != 0)
				{
					arr[j][k] = '\0';
					j++;
					k = 0;
				}
				inWord = TRUE;
			}

			arr[j][k] = cmd[i];
			k++;
		}

		else
			inWord = FALSE;

		i++;
	}

	/** Set the last word to be "\n" **/
	j++;
	if (j >= 60)
		j--;
	arr[j][0] = '\n';
	arr[j][1] = '\0';
	return arr;
}
/** array size function **/
int arraySize(char **array)
{
	int i;
	for (i = 0; strcmp(array[i], "\n") != 0; i++);
	return i;
}

/** check if there is same operand name **/
int checkOp(char *op)
{
	int i;
	for (i = 0; i < OPERATIONS_NUMBER; i++)
		if (strcmp(op, opr[i].name) == 0)
			return TRUE;
	return FALSE;
}

int getOp(char *op)
{
	int i;
	for (i = 0; i < OPERATIONS_NUMBER; i++)
		if (strncmp(op, opr[i].name, strlen(opr[i].name)) == 0)
			return i;
	/* opcode wasnt found */
	fprintf(stderr, "Error - Illegal opcode. \n");
	ERROR = TRUE;
	return ERROR;
}

int getGroup(char *op)
{
	int i;
	for (i = 0; i < OPERATIONS_NUMBER; i++)
		if (strncmp(op, opr[i].name, strlen(opr[i].name)) == 0)
			return opr[i].group;
	/* opcode wasnt found, error already printed */
	ERROR = TRUE;
	return ERROR;
}
/** return addressing type or if it is not legal symbol\label **/
int isOperand(char *operand)
{
	int addressingType = checkAddress(operand);
	if (addressingType != ADDRESS)
		return TRUE; 
	return isSymbol(operand); 
}

int isNumber(char *str)
{
	/** ski '+' and '-' signs **/
	if (*str == '+' || *str == '-') 
		str++;

	while (*str != '\0')
	{
		if (!(*str >= '0' && *str <= '9'))
			return FALSE;
		str++;
	}
	return TRUE;
}
/** checks if this cmd is a symbol **/
int isSymbol(char *cmd)
{
	int i = 0;
	int length = strlen(cmd);
	if (!length)
		return FALSE;
	/**checks if the first character not a letter **/
	if (cmd[i] < 'A' || cmd[i] > 'z' || (cmd[i] < 'a' && cmd[i] > 'Z')) 
		return FALSE;
	/** this checks if the frst name not a register **/
	if (cmd[i] == 'r' && length == 2)
		for (i = 0; i < 8; i++)
			if ((cmd[1] - '0') == i)
				return FALSE;
	/** isnt a character or letter **/
	for (i = 1; i < length; i++)
		if (cmd[i] < '0' || (cmd[i] > '9' && cmd[i] < 'A') || ((cmd[i] < 'a' && cmd[i] > 'Z')) || cmd[i] > 'z') 
			return FALSE;
	/** check if it not as same name as operands list **/
	for (i = 0; i < OPERATIONS_NUMBER; i++)
		if (strcmp(opr[i].name, cmd) == 0)
			return FALSE;
	return TRUE;
}
int checkJumpAddress(char *operand)
{
	/** if the opearnd start with #NUMBER **/
	if (operand[0] == '#')
	{
		if (strlen(operand) == 2 && operand[1] == '0');

		else if (atoi(operand + 1) == 0)
		{
			fprintf(stderr, "Error - The operand is not a number. \n");
			ERROR = TRUE;
			return ADDRESS;
		}

		return NUMBER;
	}
	
	/**checks if there is a register **/
	if (strlen(operand) == 2 && operand[0] == 'r')
	{
		if (operand[1] >= '0' && operand[1] <= '7')
			return REGISTER;
	}



	return ADDRESS;
}
int checkAddress(char *operand)
{	
	/** if the opearnd start with #NUMBER **/
	if (operand[0] == '#')
	{
		if (strlen(operand) == 2 && operand[1] == '0');

		else if (atoi(operand + 1) == 0)
		{
			fprintf(stderr, "error : The operand is not a number. \n");
			return ADDRESS;
		}

		return NUMBER;
	}
	/**checks if there is a register **/
	if (strlen(operand) == 2 && operand[0] == 'r')
	{
		if (operand[1] >= '0' && operand[1] <= '7')
			return REGISTER;
	}



	return ADDRESS;
	
}
