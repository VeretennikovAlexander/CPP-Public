

class CWeakPtrTester3
{

	class CNode
	{
		weak_ptr<CNode> Parent;
		vector<shared_ptr<CNode> > Children;
		size_t Value = 0;

	public:

		CNode(size_t AValue): Value(AValue)
		{

		}

		CNode(size_t AValue, shared_ptr<CNode> AParent): Value(AValue)
		{
			Parent = AParent;
		}

		void Add(shared_ptr<CNode> Node)
		{
			Children.push_back(Node);
		}

		shared_ptr<CNode> Get(size_t i)
		{
			return Children[i];
		}

		shared_ptr<CNode> GetParent()
		{
			return Parent.lock();
		}

	};

public:


	void Test()
	{
		printf("Standard smart ptr 3 test\n");

		shared_ptr<CNode> Item;

		{
			shared_ptr<CNode> Parent(make_shared<CNode>(1));
			Parent->Add(make_shared<CNode>(2, Parent));
			Parent->Add(make_shared<CNode>(3, Parent));

			Item = Parent->Get(0);
			printf("Has parent %d\n", Item->GetParent() != 0);

			Item->Add(make_shared<CNode>(4, Item));
		}

		printf("Has parent %d\n", Item->GetParent() != 0);

		printf("Has parent %d\n", Item->Get(0)->GetParent() != 0);
	}
};
