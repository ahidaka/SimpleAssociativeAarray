#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//
// Simple Associative Aarray
//

#define AA_DEFAULT_SIZE (128)

typedef struct _aa_type {
	int Index;
	char *Key;
	char *StrValue;
	double DataValue;
} AA_TYPE;

int AAInit(int Size);
int AASetStr(char *Key, char *Value);
int AASetData(char *Key, double Value);
char *AAGetStr(char *Key);
double AAGetData(char *Key);
void AACleanUp(void);
int AASplit(char *Input, char **Key, char **Value);

static AA_TYPE *AaTable;
static int AaMaxSize;
static int AaCurrent;

int AAInit(int Size)
{
	if (Size == 0) {
		Size = AA_DEFAULT_SIZE;
	}
	AaCurrent = 0;
	AaMaxSize = Size;
	AaTable = calloc(sizeof(AA_TYPE), AaMaxSize);
	if (AaTable == NULL) {
		Size = 0;
	}
	return Size;
}

int AASetStr(char *Key, char *Value)
{
	int i = 0;
	char *existing = AAGetStr(Key);
	if (!existing) {
		i = AaCurrent++;
	}
	else {
		//printf("existing! i=%d\n", i);
		for(i = 0; i < AaCurrent; i++) {
			//printf("%d:%d<%s:%s>=<%s>\n", i, AaTable[i].Index, Key,
			//       AaTable[i].Key, AaTable[i].Value);
			if (!strcmp(Key, AaTable[i].Key)) {
				break;
			}
		}
	}
	AaTable[i].Index = i;
	AaTable[i].Key = Key;
	AaTable[i].StrValue = Value;

	return(i);
}

int AASetData(char *Key, double Value)
{
	int i = 0;
	char *existing = AAGetStr(Key);
	if (!existing) {
		i = AaCurrent++;
	}
	else {
		//printf("existing! i=%d\n", i);
		for(i = 0; i < AaCurrent; i++) {
			//printf("%d:%d<%s:%s>=<%s>\n", i, AaTable[i].Index, Key,
			//       AaTable[i].Key, AaTable[i].Value);
			if (!strcmp(Key, AaTable[i].Key)) {
				break;
			}
		}
	}
	AaTable[i].Index = i;
	AaTable[i].Key = Key;
	if (AaTable[i].StrValue == NULL)
		AaTable[i].StrValue = Key;
	AaTable[i].DataValue = Value;


	return(i);
}

char *AAGetStr(char *Key)
{
	int i;
	for(i = 0; i < AaCurrent; i++) {
		if (AaTable[i].Key == NULL) {
			return NULL;
		}
		if (!strcmp(Key, AaTable[i].Key))
			break;
	}
	return AaTable[i].StrValue;
}

double AAGetData(char *Key)
{
	int i;
	for(i = 0; i < AaCurrent; i++) {
		if (AaTable[i].Key == NULL) {
			return 0.00D;
		}
		if (!strcmp(Key, AaTable[i].Key))
			break;
	}
	return AaTable[i].DataValue;
}

void AACleanUp(void)
{
	int i;
	for(i = 0; i < AaCurrent; i++) {
		free(AaTable[i].Key);
		free(AaTable[i].StrValue);
		AaTable[i].DataValue = 0.00D;
	}
	AaCurrent = 0;
}

int AASplit(char *Input, char **Key, char **Value)
{
	char buffer[128];
	char *separeter;

	strcpy(buffer, Input);
	separeter = index(buffer, '=');
	if (separeter == NULL) {
		return 0;
	}
	*separeter++ = '\0';
	*Key = strdup(buffer);
	*Value = strdup(separeter);
	return(separeter - buffer);
}

#if 1
/*
 *
 */
char *Samples[] = {
	"AA=aa",
	"BBB=bbbbb",
	"CCC=ccc",
	"DD=dd",
	"EEEE=eeeeeee",
	"FF=fff",
	"GGGG=gg",
	"HHH=hhh",
	"AA=aaaaaa",
	"FF=ffffff",
	"HHH=hh",
	NULL
};

char *Tests[] = {
	"AA",
	"BBB",
	"CC",
	"DD",
	"EEEE",
	"FF",
	"GGGG",
	"HHH",
	"III",
	NULL
};

int main(int ac, char** av)
{
	int i;
	char *key;
	char *value;
	int rtn;

	AAInit(0);
	
	for(i = 0; Samples[i] != NULL; i++) {
		rtn = AASplit(Samples[i], &key, &value);
		if (rtn > 0) {
			rtn = AASetStr(key, value);
			printf("AASet=%d %s %s\n", rtn, key, value);
		}
	}

	printf("\n");
	for(i = 0; AaTable[i].Key != NULL; i++) {
		printf("%d:%d<%s>=<%s><%.2f>\n", i, AaTable[i].Index,
		       AaTable[i].Key, AaTable[i].StrValue), AaTable[i].DataValue;
	}
	
	printf("\n");
	for(i = 0; Tests[i] != NULL; i++) {
		char *out;
			
		out = AAGetStr(Tests[i]);
		printf("AAGet-%d: %s=%s\n", i, Tests[i], out);
	}
	
	return 0;
}
#endif
