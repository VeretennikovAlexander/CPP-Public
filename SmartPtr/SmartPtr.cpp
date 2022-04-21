// SmartPtr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "SmartPtrTestObjectCollection.h"

namespace SimpleSmartPtr {

	#include "SimpleSmartPtr.h"

}

namespace SimpleSmartPtr2 {

	#include "SimpleSmartPtr2.h"

}

namespace SimpleSmartPtr3 {

#include "SimpleSmartPtr3.h"

}


namespace SimpleSmartPtrWithStrategy {

	#include "SimpleSmartPtrWithStrategy.h"

}

namespace SimplePtr
{
	#include "SimplePtr.h"
}

#include "SmartPtr.h"

#include "Standard/StandardSmartPtr.h"

#include "Standard/StandardSmartPtr2.h"

#include "Standard/StandardSmartPtr3.h"

#include "SmartPtrVirtual.h"

#include "Standard/UniquePtr.h"

#include "Standard/AutoPtr.h"

#include "Standard/CustomDeleter.h"

int main()
{
	
	CSmartPtrDetailedTester SmartPtrTester;
	SmartPtrTester.Test();
		
	printf("\r\nSimple Smart Pointer Test\r\n\r\n");
	SimpleSmartPtr::CSimpleSmartPtrTester SimpleSmartPtrTester;
	SimpleSmartPtrTester.Test();
	printf("\r\n");
	
	printf("\r\nSimple Smart Pointer/Strategy Test\r\n\r\n");
	SimpleSmartPtrWithStrategy::CSimpleSmartPtrTester SimpleSmartPtrTester1;
	SimpleSmartPtrTester1.Test();
	SimpleSmartPtrTester1.TestDefaultConstructor();
	printf("\r\n");
	
	CStandardSmartPtrTester StandardSmartPtrTester;
	StandardSmartPtrTester.Test();

	CSmartPtrVirtualTester SmartPtrVirtualTester;
	SmartPtrVirtualTester.Test();

	CStandardSmartPtrVirtualTest StandardSmartPtrVirtualTest;
	StandardSmartPtrVirtualTest.Test();

	CWeakPtrTester3 WeakPtrTester3;
	WeakPtrTester3.Test();

	{
		CUniquePtrTester UniquePtrTester;
		UniquePtrTester.Test();
	}

	CAutoPtrTester AutoPtrTester;
	AutoPtrTester.Test();

	{
		CCustomDeleterTester1 CustomDeleterTester;
		CustomDeleterTester.Test();
	}

	printf("\r\nSimple Smart Pointer Test (3)\r\n\r\n");
	SimpleSmartPtr3::CSimpleSmartPtrTester SimpleSmartPtrTester3;
	SimpleSmartPtrTester3.Test();
	printf("\r\n");
	
    return 0;
}

