
#include <stdio.h>
#include <cstdlib>
#include <memory>

struct CExampleStruct
{
	char C;
	int I;
};

/*
  Задаем режим выравнивания полей 1. См.
  https://vk.com/wall-227225018_64
*/
#pragma pack(push,1)

struct CExampleStruct1
{
	/*
	 * Для полей структуры можно задавать начальное значение.
	 */
	char C = 10;
	int I;
	char Data[100];
	
	CExampleStruct1()
	{
	}
	
};

#pragma pack(pop)

#include <stddef.h>

/*
Example output:
10
3
0
0
10
10
*/
void StructuresTest()
{
	/*
	 * Создаем структуру, объект размещается в стеке.
	 */
	 
	CExampleStruct1 C1;
	/*
	 * Изменение значения в поле структуры.
	 */
	C1.Data[0] = 1;
	
	/*
	 * Получаем адрес объекта в памяти с использованием оператора &.
	 * Адрес объекта C1 сохраняем в переменной CExampleStruct1*.
	 */ 
	CExampleStruct1* P1 = &C1;
	
	/*
	 * Изменение значения в поле структуры. То же самое, что и выше, но обращаемся к указателю на структуру.
	 * Используем оператор ->, если переменная является указателем на структуру.
	 */
	P1->Data[0] = 2;
	
	/*
	 * Объявляем переменную типа "ссылка". Это особый вид указателя. Для доступа к полям структуры используем снова . 
	 */
	CExampleStruct1& R1 = C1;	
	R1.Data[0] = 3;
	
	/*
	 * В данном примере мы используем указатель на структуру, меняем его тип на (char*) и смещаемся вперед на offsetof(CExampleStruct1, Data).
	 * В результате новая переменная DataPtr ссылается на поле Data структуры C1.
	 * 
	 * Метод offsetof имеет два параметра, имя типа CExampleStruct1 и имя поля в этом типе, то есть, Data.
	 * */
	char* DataPtr = (char*)P1 + offsetof(CExampleStruct1, Data);
	printf("%d\n", (int)(C1.C));
	printf("%d\n", (int)(*DataPtr));
	
	/*
	 * Создаем экземпляр структуры, размещенный в куче.
	 */ 
	CExampleStruct1* P2 = new CExampleStruct1();
	
	/*
	 * Освобождение памяти.
	 */
	delete P2;
	
	/*
	 * Пример с выделением блока памяти произвольным аллокатором 
	 * и размещение в нем двух экземпляров структур.
	 */
	char* Ptr = (char*)malloc(sizeof(CExampleStruct1)*2);
	CExampleStruct1* P3 = (CExampleStruct1*)Ptr;
	CExampleStruct1* P4 = (CExampleStruct1*)(Ptr + sizeof(CExampleStruct1));
	
	/*
	 * Будет выведено непредсказуемое значение, т. к. поле C
	 *  структуры CExampleStruct1 не было проинициализировано (не был вызван конструктор).
	 */
	
	printf("%d\n", (int)(P3->C));
	printf("%d\n", (int)(P4->C));
	/*
	 * Вызов конструктора структуры на блоке памяти (in-place constructor).
	 * Требует #include memory in GCC. После этого точно будет выведено 10 и 10, т. к. поле C 
	 * структуры CExampleStruct1 будет проинициализировано.
	 */
	new ((void*)P3) CExampleStruct1();
	new ((char*)P4) CExampleStruct1();
	
	printf("%d\n", (int)(P3->C));
	printf("%d\n", (int)(P4->C));
	
	free(Ptr);
};

int main(int argc, char** argv)
{
	StructuresTest();
}
