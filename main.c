/** Assembler project @beta for MAMAN14
 the program is an assembler that read an input and transforms
 to binary code : 01001101010 with 14 bits to ./..//././. also with 14 bits
 and make 3 file outputs, object,extren,entry
 Author: Michael Riversky **/
#include "main.h"
struct code cmdArray[MAX_ARRAY];
struct data dataArray[MAX_ARRAY];
struct binarycode binArray[MAX_ARRAY];
char *entryArray[MAX_ARRAY];
char *externArray[MAX_ARRAY];
int ERROR;
int binarySize;
int externSize;
int entrySize;
int commandSize;
int dataSize;

int main(int argc, char *argv[])
{
	int i,length;
	char *fp;
	FILE *f;
	binarySize=0;
	externSize=0;
	entrySize=0;
	commandSize=0;
	dataSize=0;
	/** checks if the user enter any file name **/
	if (argc == 1)
	{
		fprintf(stderr, "error : enter file again please\n");
		return 0;
	}
	/** a loop for mount of file names like: ./assembler p1 p2 p3 p4 **/
	for (i = 1; i < argc; i++)
	{
		ERROR = FALSE;
		fp= argv[i];
		length = strlen(fp);
		f = fopen(strcat(fp, ".as"), "r");
		fp[length] = 0;

		if (!f)
		{
			fprintf(stderr, "error : %s there is no file that name\n", fp);
			continue;
		}
		/** starting first and second pass **/
		firstAssemblerRun(f);
		secondAssemblerRun();

		if (!ERROR)
		{
			/** create object file if no emptry **/
			f = fopen(strcat(fp, ".ob"), "w");
			fp[length] = 0;
			if(!f)	
			{								  					          
        			fprintf(stderr, "error : %s cannot open the fike ", fp);
				continue;
			}
			createObjectFile(f);

			/** create entry file if no emptry **/
			if (entrySize > 0)
			{
				f = fopen(strcat(fp, ".ent"), "w");
				fp[length] = 0;
				if(!f)	
				{								  					    
        				fprintf(stderr, "error : %s cannot open the fike ", fp);
					continue;
				}
				createEntryFile(f);
			}
			/** create extern file if no emptry **/
			if (externSize > 0)
			{
				f = fopen(strcat(fp, ".ext"), "w");
				fp[length] = 0;
				if(!f)
				{								  					       
        				fprintf(stderr, "error : %s cannot open the fike ", fp);
					continue;
				}
					createExternFile(f);
			}
		}

		fclose(f);
	}
	return 0;
}

void firstAssemblerRun(FILE *f)
{
	char fullLine[MAX_LINE + 1];
	char **line;
	/** read line by line into fullLine char **/
	while (fgets(fullLine, MAX_LINE ,f))
	{
		char *fWord;
		int arrSize, hasSymbol;
		line = getLineArray(fullLine);
		arrSize = arraySize(line);
		/** checks if there is label that ends with ':' **/
		hasSymbol = isLabel(line);

		/** if there is file but he empty **/
		if (arrSize<0)
		{
			fprintf(stderr, "error : empty document \n");
			ERROR = TRUE;
		}
		/** first word after the label if there is **/
		fWord = line[hasSymbol]; 
		/** if there is any 'space' ';' 'tab' the program will ignore this signs **/
		if (line[0][0] == '\0' || line[0][0] ==';' || line [0][0] == '\t') 
			continue;

		else if ( (strcmp(fWord,"jsr")==0) || (strcmp(fWord,"bne")==0) || (strcmp(fWord,"jmp")==0) )
			addJump(line);

		else if (checkOp(fWord))
			addCommand(line);

		else if (strcmp(fWord, ".extern") == 0)
			addExtern(line);

		else if (strcmp(fWord, ".entry") == 0)
			addEntry(line);

		else if (strcmp(fWord, ".data") == 0)
			addData(line);

		else if (strcmp(fWord, ".string") == 0)
			addString(line);

	

		else
		{
			fprintf(stderr, "error : %s not recognaized command\n", fullLine);
			ERROR = TRUE;
		}
	}
	/** call to decimal address to make the addres IC and DC start **/
	DecimalAddress();
	free(line);
}

/** creating 3 files and printing in the file ,object,entry,extren **/
void createObjectFile(FILE *f)
{
	int i;
	fprintf(f, "%d  %d \n", commandSize, dataSize);
	for (i = 0; i < binarySize; i++)
		fprintf(f, "0%d   %s \n", binArray[i].address, binArray[i].binary);
}

void createEntryFile(FILE *f)
{
	int i;
	for (i = 0; i < entrySize; i++)
		fprintf(f, "%s   %d\n", entryArray[i],getAddress(entryArray[i]));
}

void createExternFile(FILE *f)
{
int i;
	for (i=0; i < externSize; i++)
			fprintf(f,"%s   %d\n",externArray[i],getAddress(externArray[i]));		

}
