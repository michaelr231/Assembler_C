#include <stdio.h>
#include <string.h>
#include <stdlib.h>
enum BOOL {FALSE , TRUE};
enum op {ZERO_OPERANDS, ONE_OPERAND, TWO_OPERANDS};
enum encode {NUMBER, ADDRESS, JUMP, REGISTER, TWO_REGISTERS, MAIN_COMMAND};
enum encodeType {A, E, R};
enum operandType {SRC, DEST};
#define OPERATIONS_NUMBER 16 /** operands numbers **/
#define MAX_LINE 100 /** maximum line size **/
#define IC 100 /** start address **/
#define MAX_BITS 14 /** maximum bits in strange word **/
#define LINE_WORD 30 /** maximum word line size **/
#define MAX_WORDS 60 /** maximum word size **/
#define REG_VALUE_INDEX 1 /* register index */
#define MAX_ARRAY 256	/** maximum array size **/
#define DATA_INDEX 0	/** index starts in 0 **/
#define NUM_OPS1 1 /** one operands **/
#define NUM_OPS2 2 /** two operands **/
#define NUM_OPS4 4 /** four operands **/
#define STRING_NUM 2 /** .string command words number **/
#define OPERTATION_NAME_INDEX hasSymbol + 0 /** index after label\symbol **/
#define FIRST_INDEX hasSymbol + 1 /** first paramter index after label\symbol **/
#define SECOND_INDEX hasSymbol + 3/** second parameter index after label\symbol **/
#define BETWEEN_INDEX hasSymbol + 2 /** ',' index after label\symbol **/
#define CONTENT_INDEX hasSymbol + 1 /** content index after label\symbol **/
#define FIRST_DATA_INDEX 1 /** index start in 1 , skip ,.data **/
#define FIRST_CHAR_INDEX 1 /**index start in 1 ,skip the ' " ' in .string **/
#define LAST_CHAR_INDEX slength - 2 /** defents on slength **/
#define FIRST_QUOTE_INDEX 0 /** first index ' " ' **/
#define LAST_QUOTE_INDEX slength - 1 /** last end index of ' " ' **/


