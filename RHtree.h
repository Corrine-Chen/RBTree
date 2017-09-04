

#include<iostream>
using namespace std;

enum COLOR{RED, BLACK};

template<class K, class V>
struct RBTreeNode
{
	RBTreeNode(const K& key, const V& value, const COLOR& color = RED)
		: _pLeft(NULL)
		, _pRight(NULL)
		, _pParent(NULL)
		, _key(key)
		, _value(value)
		, _color(color)
	{}

	RBTreeNode<K, V>* _pLeft;
	RBTreeNode<K, V>* _pRight;
	RBTreeNode<K, V>* _pParent;
	K _key;
	V _value;
	COLOR _color;
};


template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	RBTree()
		: _pRoot(NULL)
	{}

	bool Insert(const K& key, const V& value)
	{
		if(_pRoot == NULL)//根节点为空  直接插入
		{
			_pRoot = new Node(key,value,BLACK);
			return true;
		}


		Node* pCur = _pRoot;
		Node* parent = pCur;

		while(pCur)
		{
			if(key < pCur->_key)//往节点的 左边走
			{
				parent = pCur;
				pCur = pCur->_pLeft;
			}
			else if(key > pCur->_key)//往节点的右边走
			{
				parent = pCur;
				pCur = pCur->_pRight;
			}
			else//找到key值，则不需要插入
			{
				return false;
			}
		}

		//找到插入位置,插入元素
		pCur = new Node(key,value);
		if(key < parent->_key)
		{
			parent->_pLeft = pCur;
			pCur->_pParent = parent;
		}
		else
		{
			parent->_pRight = pCur;
			pCur->_pParent = parent;
		}

		while(pCur->_color == RED && parent->_color == RED && parent )
		{
			Node* grandfather = parent->_pParent;
			Node* uncle = NULL;

			if(grandfather->_pLeft == parent)//节点插在左侧
			{
				uncle = grandfather->_pRight;
				if(uncle && uncle->_color == RED)//情况1
				{
					parent->_color = BLACK;
					uncle->_color = BLACK;
					grandfather->_color = RED;

					pCur = grandfather;

				}
				else//情况2,3
				{
					if(uncle && uncle->_color == BLACK &&pCur == parent->_pRight)
					{
                        _RotateL(parent);
                        swap(pCur,parent);
                    }
					_RotateR(grandfather);
                    grandfather->_color = RED;
                    parent->_color = BLACK;

					if(parent->_pParent && parent->_pParent->_color == BLACK && parent->_pParent->_pParent && parent->_pParent->_pParent ->_color==BLACK)
					{
						parent->_pParent->_color = RED;
					}
				}
			}
			else//节点插在右侧
			{
				uncle = grandfather->_pLeft;
				if(uncle && uncle->_color == RED)//情况1
				{
					parent->_color = BLACK;
					uncle->_color = BLACK;
					grandfather->_color = RED;

					pCur = grandfather;
				}
				else//情况2，3
				{
					if(uncle && uncle->_color == BLACK && parent->_pLeft == pCur)
					{
                        _RotateR(parent);
                        swap(pCur,parent);
                    }
					_RotateL(grandfather);
                    grandfather->_color = RED;
                    parent->_color = BLACK;


					

					if(parent->_pParent && parent->_pParent->_color == BLACK && parent->_pParent->_pParent && parent->_pParent->_pParent ->_color==BLACK)
					{
						parent->_pParent->_color = RED;
					}
				}
			}
		}

		_pRoot->_color = BLACK;

	}

	void InOrder()
	{
		cout<<" InOrder: ";
		_InOrder(_pRoot);
		cout<<endl;
	}

	bool CheckRBTree()
	{
		if(_pRoot == NULL)
			return true;

		if(_pRoot->_color == RED)
		{
			return false;
		}
		int blackCount=0 ;
		Node* pCur = _pRoot;
		while(pCur)
		{
			if(pCur->_color == BLACK)
				blackCount++;
			pCur = pCur->_pLeft;
		}

		_CheckRBTree(_pRoot, blackCount, 0);
		return true;
	}

protected:
	void _RotateL(Node* pParent)
	{
		Node* subR = pParent->_pRight;
		Node* subRL = subR->_pLeft;
		pParent->_pRight = subRL;

		if(subRL)
		{
			subRL->_pParent = pParent ;
		}

		Node* pPParent = pParent->_pParent;
		subR->_pLeft = pParent;
		pParent->_pParent = subR;
		if(pPParent == NULL)
		{
			_pRoot = subR;
			subR->_pParent = NULL;
		}
		else
		{
			if(pParent == pPParent->_pLeft)
			{
				pPParent->_pLeft = subR;
				subR->_pParent = pPParent;
			}
			else
			{
				pPParent->_pRight = subR;
				subR->_pParent = pPParent;
			}
		}

	}

	void _RotateR(Node* pParent)
	{
		
		Node* subL = pParent->_pLeft;
    	        Node* subLR = subL->_pRight;
		pParent->_pLeft = subLR;

		if(subLR)
		{
			subLR->_pParent = pParent;
		}

		Node* pPParent = pParent->_pParent;
		subL->_pRight = pParent;
		pParent->_pParent = subL;
		if(pPParent == NULL)
		{
			_pRoot = subL;
			subL->_pParent = NULL;
		}
		else
		{
			if(pParent == pPParent->_pLeft)
			{
				pPParent->_pLeft = subL;
				subL->_pParent = pPParent;
			}
			else
			{
				pPParent->_pRight = subL;
				subL->_pParent = pPParent;
			}
		}

	
	}
	void _InOrder(Node* pRoot)
	{
		if(pRoot == NULL)
			return ;
		_InOrder(pRoot->_pLeft);
		cout<<pRoot->_key<<" ";
		_InOrder(pRoot->_pRight);
	}
	bool _CheckRBTree(Node* pRoot, const size_t blackCount, size_t k)
	{
		if(pRoot == NULL)
			return true;
		if(pRoot->_color == BLACK)
			++k;
		Node* parent = pRoot->_pParent;

		if(parent && parent->_color == RED && pRoot->_color == RED)
		{
			cout<<"违反了规则3，不是红黑树"<<endl;
			return false;
		}
		if(parent && parent->_pLeft== NULL && parent->_pRight ==NULL)
		{
			if(k != blackCount)
			{
				cout<<"违反了规则4，不是红黑树"<<endl;
				return false;
			}
		}
		return (_CheckRBTree(pRoot->_pLeft ,blackCount,k)&&_CheckRBTree(pRoot->_pRight,blackCount,k));
	}

protected:
	Node* _pRoot;
};

void TestRBTree()
{
	int a[] = {10, 7, 8, 15, 5, 6, 11, 13, 12};
	RBTree<int, int> t;
	for(int idx = 0; idx < sizeof(a)/sizeof(a[0]); ++idx)
		t.Insert(a[idx], idx);

	t.InOrder();
	
	
	t.CheckRBTree();


}
