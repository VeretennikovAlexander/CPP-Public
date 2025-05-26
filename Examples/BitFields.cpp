
#include <stdio.h>
#include <cstdlib>

#pragma pack(push,1)
struct COneByte
{
    int b0:1;
    int b1:1;
    int b2:1;
    int b3:1;
    int b4:1;
    int b5:1;
    int b6:1;
    int b7:1;
};
#pragma pack(pop)

#pragma pack(push,1)
struct CStruct1
{
    int b0:1;
    int b12:2;
};
#pragma pack(pop)

/*
 * Example output:
 * 
 * 1
 * 5
 * 1
 * 6
 * 
 */ 
int main(int argc, char** argv)
{
    /*
    * Структура определена как набор полей, каждое из которых
    * занимает 1 бит.
    * Это позволяет менять каждый бит по отдельности.
    */ 
    printf("%d\n", (int)sizeof(COneByte));
    COneByte OneByte = {0};
    OneByte.b0 = 1;
    OneByte.b2 = 1;
    printf("%d\n", (int)(*(unsigned char*)&OneByte));
    
    /*
	 * Структура определена как поле из одного бита и поле из двух бит.
	 * Если мы меняем второе поле, то меняем биты 1 и 2 (нумерация с 0), 
	 * байта, в котором размещена структура.
	 * Если b12 = 3, то в байте будет размещено 110, где 11 это 3 
	 * закодированное в двоичной системе исчисления,
	 * размещенное в младших битах 1 и 2 (при архитектуре Little Endian), а 110 это число 6,
	 * закодированное в двоичной системе исчисления.
	 */ 
    printf("%d\n", (int)sizeof(CStruct1));
    CStruct1 Struct1 = {0};
    Struct1.b12 = 3;
    printf("%d\n", (int)(*(unsigned char*)&Struct1));
    
    return 0;
}

