struct code {
	int encodeType; /**A R E**/
	int encode; /**Tye paramters **/
	int opcode; /** operand code example : mov **/
	int group; /** TWO_OPERANDS ,ONE_OPERANDS, ZERO_OPERANDS **/
	int address;
	char *srcOperand;
	char *destOperand;
	int srcAddressingType;
	int destAddressingType;
	int hasSymbol;
	char *symbol;
	int opNum; 
	int param1;
	int param2;
	int operandRole; 
	int numberVal; 
	int addressVal; 
	int srcReg; 
	int destReg; 
	int reg1; 
	int reg2; 

};

struct data {	/* DATA = data OR string*/
	int address;
	int value;
	int hasSymbol;
	char *symbol;
};

struct ext {
	char *symbol;
};

struct entry {
	char *symbol;
};
int isSymbol(char *cmd);
int isLabel(char **line);
int isNumber(char *);
int isOperand(char *);

int getAddress(char *label);
void addJump(char **line);
void addCommand(char **line);
void addData(char **line);
void addString(char **line);
void addExtern(char **line);
void addEntry(char **line);
char *getCleanJump(char *label);
char *getCleanJumpLeft(char *label);
char *getCleanJumpOther(char *label);
char **getLineArray(char *cmd);
char *cleanLabel(char *label);
int getOp(char *op);
int getGroup(char *op);

int checkJumpAddress(char *operand);
int checkOp(char *op);
int checkOperand(char *operand);
int checkAddress(char *operand);

int arraySize(char **array);
void DecimalAddress();

