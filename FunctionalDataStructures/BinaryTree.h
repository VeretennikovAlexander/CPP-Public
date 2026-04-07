

template <class T>
class CBinaryTree
{
	struct CNode
	{
		T Value;
		shared_ptr<CNode> Left;
		shared_ptr<CNode> Right;
		
		CNode()
		{
		}
		
		CNode(T AValue): Value(AValue)
		{
		}
		
		CNode(T AValue, shared_ptr<CNode> ALeft, shared_ptr<CNode> ARight): Value(AValue), Left(ALeft), Right(ARight)
		{
		}
	};
	
	shared_ptr<CNode> Root;
	
	shared_ptr<CNode> InsertTree(shared_ptr<CNode> Node, T Value)
	{
		if (!Node)
		{
			return make_shared<CNode>(Value);
		}
		if (Node->Value == Value)
		{
			return Node;
		}
		if (Node->Value < Value)
		{
			return make_shared<CNode>(Node->Value, InsertTree(Node->Left, Value), Node->Right);
		} else
		{
			return make_shared<CNode>(Node->Value, Node->Left, InsertTree(Node->Right, Value));
		}
	}
		
	size_t GetHeight(shared_ptr<CNode> Node) const
	{
		if (!Node)		
			return 0;
		return 1 + max(GetHeight(Node->Left), GetHeight(Node->Right));
	}
	
	CBinaryTree(shared_ptr<CNode> ARoot): Root(ARoot)
	{
	};
	
public:	

	CBinaryTree()
	{
	}
	
	CBinaryTree(T Value)
	{
		Root = make_shared<CNode>(Value);
	}
	
	CBinaryTree Insert(T Value)
	{
		return CBinaryTree(InsertTree(Root, Value));
	}
	
	size_t Height() const
	{
		return GetHeight(Root);
	}
};

class CBinaryTreeTest
{
public:

	void Test()
	{
		printf("\n");
		printf("Binary Tree Test\n");
		
		CBinaryTree<int> Tree(10);
		for (int i = 0; i<50; i++)
		{
			Tree = Tree.Insert(rand());
		}
		printf("Binary tree height %d", (int)Tree.Height());
	}
};
