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
	int Valid;
	char *StrValue;
	double DataValue;
} AA_TYPE;

int AAInit(int Size);
int AASetMark(char *Key, int Mark);
int AAGetMark(char *Key);
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

int AASetMark(char *Key, int Mark)
{
	int i = 0;
	char *existing = AAGetStr(Key);
	if (!existing) {
		return -1;
	}
	else {
		for(i = 0; i < AaCurrent; i++) {
			if (!strcmp(Key, AaTable[i].Key)) {
				break;
			}
		}
	}
	AaTable[i].Valid = Mark;
	return(i);
}

int AAGetMark(char *Key)
{
	int i = 0;
	char *existing = AAGetStr(Key);
	if (!existing) {
		return -1;
	}
	else {
		for(i = 0; i < AaCurrent; i++) {
			if (!strcmp(Key, AaTable[i].Key)) {
				break;
			}
		}
	}
	return (AaTable[i].Valid);
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
	AaTable[i].Valid = 0;

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
	if (AaTable[i].StrValue == NULL) {
		AaTable[i].StrValue = Key;
	}
	AaTable[i].DataValue = Value;
	AaTable[i].Valid = 1;

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

int AAGetValidData(char *Key, double *pd)
{
	int i;
	for(i = 0; i < AaCurrent; i++) {
		if (AaTable[i].Key == NULL) {
			return -2;
		}
		if (!strcmp(Key, AaTable[i].Key))
			break;
	}
	if (AaTable[i].Valid) {
	 	if (pd != NULL) {
			*pd = AaTable[i].DataValue;
		 }
		AaTable[i].Valid = 0;
	}
	else {
		return -1;
	}
	return i;
}

void AACleanUp(void)
{
	int i;
	for(i = 0; i < AaCurrent; i++) {
		free(AaTable[i].Key);
		AaTable[i].Key = NULL;
		free(AaTable[i].StrValue);
		AaTable[i].StrValue = NULL;
		AaTable[i].DataValue = 0.00D;
		AaTable[i].Valid = 0;		
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
	double data;
	double data2;

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
	
	AASetData("AA", 1.12);
	rtn = AAGetValidData("AA", &data);
	i = AAGetValidData("AA", &data2);
	printf("AA: %.3f:%d %.3f:%d\n", data, rtn, data2, i);

	AASetData("CC", 3.45);
	rtn = AAGetValidData("CC", &data);
	i = AAGetValidData("CC", &data2);
	printf("CC: %.3f:%d %.3f:%d\n", data, rtn, data2, i);

	AASetData("DD", 3.45);
	rtn = AAGetValidData("DD", &data);
	i = AAGetValidData("DD", &data2);
	printf("DD: %.3f:%d %.3f:%d\n", data, rtn, data2, i);

	rtn = AAGetValidData("EE", &data);
	i = AAGetValidData("EE", &data2);
	printf("EE: %.3f:%d %.3f:%d\n", data, rtn, data2, i);

	return 0;
}
#endif
