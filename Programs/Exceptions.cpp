// Exceptions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std;

void Example1()
{
    printf("\n");
    printf("Example 1");
    printf("\n");

    try
    {
        try
        {
            throw "My Error";
        }
        catch (const char* c)
        {
            printf("const char* has been catched\n");
        }
    }
    catch (int x)
    {
        printf("Int has been catched\n");
    }
};

/*
With RTTI:

0, int has been catched: 0
1, const char* has been catched: Error
2, exception has been catched: Message 1, class std::exception
3, exception has been catched: Message 2, class std::runtime_error
4, Unknown exception has been catched

Without RTTI:

0, int has been catched: 0
1, const char* has been catched: Error
2, exception has been catched: Message 1
3, exception has been catched: Message 2
4, Unknown exception has been catched

*/

void Example2()
{
    printf("\n");
    printf("Example 2");
    printf("\n");

    for (size_t i = 0; i < 5; i++)
    {
        try
        {
            if (i == 0)
                throw 0;
            if (i == 1)
                throw "Error";
            if (i == 2)
                throw exception("Message 1");
            if (i == 3)
                throw runtime_error("Message 2");
            if (i == 4)
                throw make_pair(0, 0);
        }
        catch (int IntVal)
        {
            printf("%d, int has been catched: %d\n", (int)i, IntVal);
        }
        catch(const char* StrVal)
        {
            printf("%d, const char* has been catched: %s\n", (int)i, StrVal);
        }
        catch (const exception& Ex)
        {
            try
            {
                //RTTI is required
                printf("%d, exception has been catched: %s, %s\n", (int)i, Ex.what(), typeid(Ex).name());
            }
            catch (const bad_typeid& Ex1)
            {
                printf("%d, exception has been catched: %s\n", (int)i, Ex.what());
            }
        }
        catch (...)
        {
            printf("%d, Unknown exception has been catched\n", (int)i);
        }
    }
}

void Example3()
{
    printf("\n");
    printf("Example 3");
    printf("\n");

    auto F = []() {
        throw exception("Error 1");
    };

    try
    {
        F();
    }
    catch (exception Ex)
    {
        printf("Exception has been catched %s\n", Ex.what());
    }

}

#include <windows.h>

/*
    // Defined values for the exception filter expression
    #define EXCEPTION_EXECUTE_HANDLER      1
    #define EXCEPTION_CONTINUE_SEARCH      0
    #define EXCEPTION_CONTINUE_EXECUTION (-1)

    An element of SEH handlers list (x86).

    EXCEPTION_REGISTRATION {
        struc prev dd ?
        handler dd ?
    }_EXCEPTION_REGISTRATION ends

    or

    struct EXCEPTION_REGISTRATION
    {
        EXCEPTION_REGISTRATION *prev;
        DWORD handler;
    };

    See also:
        How a C++ compiler implements exception handling - CodeProject

        Vectored Exception Handling
        (AddVectoredExceptionHandler, RemoveVectoredExceptionHandler)
*/

/*
Example output:

SEH exception has been catched

*/


void Example4()
{
    printf("\n");
    printf("Example 4");
    printf("\n");

    int i = 0;

    auto F = [i]() {
        try
        {
            int i1 = 10 / i;
            printf("%d\n", i1);
        }
        catch (...)
        {
            printf("Unknown exception has been catched\n");
        }
    };

    __try
    {        
        F();
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        auto ExceptionCode = _exception_code();
        if (ExceptionCode == EXCEPTION_INT_DIVIDE_BY_ZERO)
        {
            printf("SEH exception has been catched: int divide by zero\n");
        }
        else
        {
            printf("SEH exception has been catched %x\n", (int)_exception_code());
        }
    }
}

static int Filter(unsigned int code, struct _EXCEPTION_POINTER* ep)
{
    if (code == EXCEPTION_INT_DIVIDE_BY_ZERO)
        return EXCEPTION_EXECUTE_HANDLER;
    return EXCEPTION_CONTINUE_SEARCH;
}

void Example5()
{
    printf("\n");
    printf("Example 5");
    printf("\n");

    struct CFilter
    {
        static int Filter(unsigned int code, struct _EXCEPTION_POINTERS* ep)
        {
            if (code == EXCEPTION_INT_DIVIDE_BY_ZERO)
                return EXCEPTION_EXECUTE_HANDLER;
            return EXCEPTION_CONTINUE_SEARCH;
        }
    };

    int i = 0;

    auto F = [i](int val) {
        try
        {
            if (val == 0)
            {
                int i1 = 10 / i;
                printf("%d\n", i1);
            }
            if (val == 1)
            {
                int* p = 0;
                int i1 = *p;
                printf("%d\n", i1);
            }
        }
        catch (...)
        {
            printf("Unknown exception has been catched\n");
        }
    };

    __try
    {
        for (int j = 0; j < 5; j++)
        {
            __try
            {

                F(j);
            }
            __except (CFilter::Filter(GetExceptionCode(), GetExceptionInformation()))
            {
                printf("SEH exception has been catched, division by zero %x\n", (int)_exception_code());
            }
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        printf("SEH exception has been catched %x\n", (int)_exception_code());
    }

}

static LONG NTAPI VectoredExceptionHandler(_EXCEPTION_POINTERS* ExceptionInfo)
{
    printf("Vectored exception handler: %x\n", ExceptionInfo->ExceptionRecord->ExceptionCode);
    return EXCEPTION_CONTINUE_SEARCH;
}


/*
Example output:

Vectored exception handler: c0000094
SEH exception has been catched c0000094
*/

void Example6()
{
    printf("\n");
    printf("Example 6");
    printf("\n");

    int i = 0;
    auto F = [i]() {
        try
        {
            int i1 = 10 / i;
            printf("%d\n", i1);
        }
        catch (...)
        {
            printf("Unknown exception has been catched\n");
        }
    };

    void* H = AddVectoredExceptionHandler(0, VectoredExceptionHandler);
    if (!H)
    {
        printf("Cannot register vectored exception handler\n");
    }

    __try
    {
        F();
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        printf("SEH exception has been catched %x\n", (int)_exception_code());
    }

    if (H)
        RemoveVectoredExceptionHandler(H);
}


int main()
{
    Example1();
    Example2();
    Example3();
    Example4(); 
    Example5();
    Example6();

    printf("Completed\n");
}
