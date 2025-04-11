# Краткий план курса ФИЛП (Функциональные структуры данных) на С++

Читается в УрФУ с 2017 г, 2-й курс, весна.

# Лекции

## Типы данных

В том числе 

int, short, long, signed *, unsigned *

uint8_t, uint16_t, uint64_t

int8_t, int16_t, int64_t

uint_least64_t, int_least64_t

uint_max_t, int_max_t

size_t, ptrdiff_t, uintptr_t

## Указатели, Структуры, Классы

https://github.com/VeretennikovAlexander/CPP-Public/blob/master/Examples/Structures.cpp

https://github.com/VeretennikovAlexander/CPP-Public/blob/master/Examples/FunctionPointers.cpp

https://github.com/VeretennikovAlexander/CPP-Public/tree/master/Tutorial

https://github.com/VeretennikovAlexander/CPP-Public/blob/master/Programs/FunctionPointers.cpp

## Стек и вызов процедуры, соглашения о вызовах

https://github.com/VeretennikovAlexander/CPP-Public/tree/master/Examples/Stack

https://vk.com/wall-227225018_4

https://vk.com/wall-227225018_10

## Аллокаторы

### Аллокатор на списке свободных блоков

См. Дональд Кнут. Искусство программирования. Том 1. Основные алгоритмы. Раздел: Динамическая память.

### Аллокатор на списке свободных блоков + дескрипторы границ

### Система двойников (Buddy memory allocation)

## Правило трех, конструктор, деструктор, оператор присваивания, конструктор копирования

https://github.com/VeretennikovAlexander/CPP-Public/blob/master/SmartPtr/SimplePtr.h

## Умные указатели

https://github.com/VeretennikovAlexander/CPP-Public/tree/master/SmartPtr

Операторы ->, *, explicit и прочее, см. примеры

## Умные указатели, реализация со стратегиями

Стратегия с помощью подсчета ссылок.

Стратегия с использованием двусвязного циклический список.

Стратегия с внедренным счетчиком.

Стратегия копирования объекта.

Стратегия с поддержкой слабой ссылки.

См. Андрей Александреску. Современное проектирование на C++. 2015 г (Andrei Alexandrescu. Modern C++ Design: Generic Programming and Design Patterns Applied.) + исходники shared_ptr в Visual Studio

## Стандартые умные указатели

https://github.com/VeretennikovAlexander/CPP-Public/tree/master/SmartPtr/Standard

## Виртуальные функции и ООП

https://github.com/VeretennikovAlexander/CPP-Public/blob/master/Programs/VirtualFunctions.cpp

## Функциональные структуры данных

Понятие функциональных структур данных (Persistent Data Structures)

Функции высшего порядка (High Order Functions)

### Список

Теория и пример реализации.

В примере рассматриваем function, лямбда функции в C++, списки инициализации

https://github.com/VeretennikovAlexander/CPP-Public/blob/master/FunctionalDataStructures/List.h

### Очередь и амортизационная вычислительная сложность

См. Крис Окасаки. Чисто функциональные структуры данных.

Chris Okasaki. Purely Functional Data Structures.

https://github.com/VeretennikovAlexander/CPP-Public/blob/master/FunctionalDataStructures/Queue.h

### Binominal Heap

См. Крис Окасаки. Чисто функциональные структуры данных. 

Chris Okasaki. Purely Functional Data Structures.

https://github.com/VeretennikovAlexander/CPP-Public/blob/master/FunctionalDataStructures/BinominalHeap.h

### Random Access List

См. Крис Окасаки. Чисто функциональные структуры данных.

https://github.com/VeretennikovAlexander/CPP-Public/blob/master/FunctionalDataStructures/RandomAccessList.h

# Сборка мусора, основные понятия и пример реализации в C++

## Консервативный сборщик мусора Mark & Sweep (однопоточная реализация)

https://github.com/VeretennikovAlexander/CPP-Public/tree/master/FunctionalDataStructuresGC

## Копирующий сборщик мусора  (однопоточная реализация)

https://github.com/VeretennikovAlexander/CPP-Public/tree/master/FunctionalDataStructuresGCCopying

# Шаблоны и метапрограммирование

https://github.com/VeretennikovAlexander/CPP-Public/tree/master/Metaprogramming

# Исключения и SEH

https://github.com/VeretennikovAlexander/CPP-Public/blob/master/Programs/Exceptions.cpp

# Практики

## Простой аллокатор

https://github.com/VeretennikovAlexander/CPP-Public/tree/master/Examples/TextMap

Реализация кастомного аллокатора, который только выделяет память "вперед" в большом буфере,
при необходимости выделяется новый большой буфер и так далее.

### Аллокатор на списке свободных блоков

### Аллокатор на списке свободных блоков + дескрипторы границ

### Система двойников (Buddy memory allocation)

### Система двойников (Buddy memory allocation) + оптимизация с помощью BSF, BSR

Оптимизация с помощью Bit Scan Forward, Bit Scan Reverse при выделении памяти.

Битовые операции <<, >>, ~, &, |

См. также

https://github.com/VeretennikovAlexander/CPP-Public/tree/master/Notes%20for%20clang%20%26%20gcc

Готовые реализации не выкладываются, т. к. делаются студентами на паре

### Реализация умного указателя

### Функциональные структуры данных. Бинарное дерево

### Функциональные структуры данных. AVL

см. также

https://github.com/VeretennikovAlexander/CPP-Public/blob/master/Tasks.md



