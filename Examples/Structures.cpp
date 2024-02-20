
#include <stdio.h>

struct CExampleStruct
{
	char C;
	int I;
};

#pragma pack(push,1)

struct CExampleStruct1
{
	char C;
	int I;
};

#pragma pack(pop)

#include <stddef.h>

/*
Example output:

CExampleStruct sizeof 8
CExampleStruct1 sizeof 5
offsetof(CExampleStruct, I) 4
offsetof(CExampleStruct1, I) 1
CExampleStruct alignof 4
CExampleStruct1 alignof 1

*/
void structuresTest()
{
	printf("CExampleStruct sizeof %d\n", (int)sizeof(CExampleStruct));
	printf("CExampleStruct1 sizeof %d\n", (int)sizeof(CExampleStruct1));
	printf("offsetof(CExampleStruct, I) %d\n", (int)offsetof(CExampleStruct, I));
	printf("offsetof(CExampleStruct1, I) %d\n", (int)offsetof(CExampleStruct1, I));
	printf("CExampleStruct alignof %d\n", (int)alignof(CExampleStruct));
	printf("CExampleStruct1 alignof %d\n", (int)alignof(CExampleStruct1));
};
