#pragma once

class CStandardSmartPtrVirtualTest
{
	class CNode
	{
	public:

		CNode()
		{

		}

		virtual ~CNode()
		{

		}

		virtual int Type()
		{
			return 0;
		}

	};

	class CNode1 : public CNode
	{
	public:

		CNode1()
		{
			printf("CNode1 created\r\n");
		}

		~CNode1()
		{
			printf("CNode1 destroy\r\n");
		}

		int Type() override
		{
			return 1;
		}
	};
	
	typedef shared_ptr<CNode> PNode;

public:

	PNode F1()
	{
		return make_shared<CNode1>();
	}
	
	void Test()
	{
		PNode N = F1();
		printf("%d\r\n", N->Type());
	}
};