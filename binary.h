void secondAssemblerRun();
/** binary code struct with address , char, and next pointer **/
struct binarycode
{
        int address;
        char binary[MAX_BITS];
        struct binarycode *next;
};
struct code OperandChose(char *operand, struct code command);
char *encode(struct code code);
char *ConvertToBinaryStr(int num, int size);
void ConvertToString(char *bin, char *num, int start, int size);
void reverse(char *s);
void binaryCommand(int address, char *bin);
void fromBinaryToDot(char *dest, char *src);

