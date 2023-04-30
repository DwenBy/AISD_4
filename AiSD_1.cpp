#include <iostream>
#include <random>
#include <vector>
#include <math.h>
#include <string>
#include <map>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <algorithm>
using namespace std;




struct TreeElement {
	double Value;
	TreeElement* Left = NULL;
	TreeElement* Right = NULL;
};
int BST_Depth(TreeElement* node) {
	if (node == NULL)
		return 0;
	else {
		/* compute the depth of each subtree */
		int lDepth = BST_Depth(node->Left);
		int rDepth = BST_Depth(node->Right);

		/* use the larger one */
		if (lDepth > rDepth)
			return (lDepth + 1);
		else
			return (rDepth + 1);
	}
}
TreeElement* Insert(TreeElement *Root, double Value) {
	if (Root == NULL) {
		Root = new TreeElement;
		Root->Value = Value;
	}
	else {
		TreeElement* Current=Root;
		while (true) {
			
			if (Current->Value > Value) {
				if (Current->Left == NULL) { Current->Left = new TreeElement; Current = Current->Left; break; }
				else {Current = Current->Left;}
			}
			else { 
				if (Current->Right == NULL) { Current->Right = new TreeElement; Current = Current->Right; break; }
				else { Current = Current->Right; }
			}

		}

		Current->Value = Value;
	}
	return Root;
}
void Tree_Out(TreeElement*Root) {
	if (Root == NULL) { cout << "Tree is empty.\n"; return; }
	cout << Root->Value << endl;
	char LorR;
	TreeElement* Cur=Root;
	TreeElement* Prev=Cur;
	while (true) {
	cin >> LorR;
	if (LorR == 'L') {
		Prev = Cur;
		Cur = Cur->Left;
	}
	else if (LorR == 'R') {
		Prev = Cur;
		Cur = Cur->Right;
	}
	else if (LorR == '-') {
		Cur = Prev;
	}
	if (Cur == NULL) { cout << "NULL\n"; return; }
	else { cout << Cur->Value << endl; }
	}

}
struct AVL_Node {
	double Value;
	AVL_Node* Left = NULL;
	AVL_Node* Right = NULL;
	unsigned int Level=1;

};
struct AVL_Tree {
	int Height(AVL_Node* C) {
		if (C == NULL) { return 0; }
		else { return C->Level; }
	}
	int getBalance(AVL_Node* N)
	{
		if (N == NULL)
			return 0;
		return Height(N->Left) - Height(N->Right);
	}
	void Out(AVL_Node* Root) {
		if (Root == NULL) { cout << "Tree is empty.\n"; return; }
		cout << Root->Value << " | " << (Height(Root)) << endl;
		char LorR;
		AVL_Node* Cur = Root;
		AVL_Node* Prev = Cur;
		while (true) {
			cin >> LorR;
			if (LorR == 'L') {
				Prev = Cur;
				Cur = Cur->Left;
			}
			else if (LorR == 'R') {
				Prev = Cur;
				Cur = Cur->Right;
			}
			else if (LorR == '-') {
				Cur = Prev;
			}
			if (Cur == NULL) { cout << "NULL\n"; return; }
			else { cout << Cur->Value << " | " << (Height(Cur)) << endl; }
		}

	}
	AVL_Node *RightRotate(AVL_Node* Parent) {
		AVL_Node* Child = Parent->Left;
		AVL_Node* Temp = Child->Right;

		// Perform rotation
		Child->Right = Parent;
		Parent->Left = Temp;

		
		Parent->Level = max(Height(Parent->Left), Height(Parent->Right)) + 1;
		Child->Level = max(Height(Child->Left), Height(Child->Right)) + 1;

		
		return Child;
		
	}

	AVL_Node* LeftRotate(AVL_Node* Parent) {
		AVL_Node* Child = Parent->Right;
		AVL_Node* Temp = Child->Left;

		// Perform rotation
		Child->Left = Parent;
		Parent->Right = Temp;

		
		Parent->Level = max(Height(Parent->Left), Height(Parent->Right)) + 1;
		Child->Level = max(Height(Child->Left), Height(Child->Right)) + 1;


		return Child;
	}
	
	AVL_Node* Add(AVL_Node* node,double Value) {
			/* 1. Perform the normal BST insertion */
		if (node == NULL) 
		{
			AVL_Node* node = new AVL_Node;
			node->Value = Value;
			return node;
		}

		if (Value <= node->Value)
			node->Left = Add(node->Left, Value);
		else if (Value > node->Value)
			node->Right = Add(node->Right, Value);

			
		node->Level = 1 + max(Height(node->Left),Height(node->Right));
		int balance = getBalance(node);

			// If this node becomes unbalanced, then
			// there are 4 cases

			// Left Left Case
		if (balance > 1 && Value < node->Left->Value)
			return RightRotate(node);

			// Right Right Case
		if (balance < -1 && Value > node->Right->Value)
			return LeftRotate(node);

			// Left Right Case
		if (balance > 1 && Value > node->Left->Value)
		{
			node->Left = LeftRotate(node->Left);
			return RightRotate(node);
		}

			// Right Left Case
		if (balance < -1 && Value < node->Right->Value)
		{
			node->Right = RightRotate(node->Right);
			return LeftRotate(node);
		}

			
		return node;
	}
};

struct RnB_Node {
	double data; 
	RnB_Node* parent; 
	RnB_Node* left; 
	RnB_Node* right; 
	int color; // 1 -> Red, 0 -> Black
};


struct RnB_Tree {

	RnB_Node* root;
	RnB_Node* TNULL;

	void initializeNULLNode(RnB_Node* node, RnB_Node* parent) {
		node->data = 0;
		node->parent = parent;
		node->left = nullptr;
		node->right = nullptr;
		node->color = 0;
	}

	// fix the red-black tree
	void fixInsert(RnB_Node* k) {
		RnB_Node* u;
		while (k->parent->color == 1) {
			if (k->parent == k->parent->parent->right) {
				u = k->parent->parent->left; // uncle
				if (u->color == 1) {
					
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->left) {
						
						k = k->parent;
						rightRotate(k);
					}
					
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			}
			else {
				u = k->parent->parent->right; // uncle

				if (u->color == 1) {
					
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->right) {
						
						k = k->parent;
						leftRotate(k);
					}
					
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
	}

	RnB_Tree() {
		TNULL = new RnB_Node;
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		root = TNULL;
	}
	void Out() {
		if (root ==TNULL) { cout << "Tree is empty.\n"; return; }
		cout << root->data<< " | " << root->color << endl;
		char LorR;
		RnB_Node* Cur = root;
		RnB_Node* Prev = Cur;
		while (true) {
			cin >> LorR;
			if (LorR == 'L') {
				Prev = Cur;
				Cur = Cur->left;
			}
			else if (LorR == 'R') {
				Prev = Cur;
				Cur = Cur->right;
			}
			else if (LorR == '-') {
				Cur = Prev;
			}
			if (Cur == TNULL) { cout << "NULL\n"; return; }
			else { cout << Cur->data << " | " << Cur->color << endl; }
		}

	}
	int RnB_Depth(RnB_Node* node) {
		if (node == TNULL)
			return 0;
		else {
			/* compute the depth of each subtree */
			int lDepth = RnB_Depth(node->left);
			int rDepth = RnB_Depth(node->right);

			/* use the larger one */
			if (lDepth > rDepth)
				return (lDepth + 1);
			else
				return (rDepth + 1);
		}
	}
	
	// rotate left at node x
	void leftRotate(RnB_Node* x) {
		RnB_Node* y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// rotate right at node x
	void rightRotate(RnB_Node* x) {
		RnB_Node* y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	// insert the key to the tree in its appropriate position
	// and fix the tree
	void insert(double key) {
		RnB_Node* node = new RnB_Node;
		node->parent = nullptr;
		node->data = key;
		node->left = TNULL;
		node->right = TNULL;
		node->color = 1; // new node is red

		RnB_Node* y = nullptr;
		RnB_Node* x = this->root;

		while (x != TNULL) {
			y = x;
			if (node->data < x->data) {
				x = x->left;
			}
			else {
				x = x->right;
			}
		}

		// y is parent of x
		node->parent = y;
		if (y == nullptr) {
			root = node;
		}
		else if (node->data < y->data) {
			y->left = node;
		}
		else {
			y->right = node;
		}

		// if new node is a root node
		
		if (node->parent == nullptr) {
			node->color = 0;
			
			return;
		}

		// if the grandparent is null
		if (node->parent->parent == nullptr) {
			
			return;
		}

		// Balancing
		
		fixInsert(node);
	}


};


int main()
{
	random_device rd;
	mt19937 gen1(rd());
	mt19937 gen2(rd());
	std::exponential_distribution<> d1(1);
	std::normal_distribution <> d2;
	//double exp_arr[50000];
	float exp_arr[10000];

	/*for (int n = 0; n < 50000; n++)
		exp_arr[n] = d1(gen1);*/
	for (int n = 0; n < 10000; n++)
		exp_arr[n] = d2(gen2);
	TreeElement* BST_Root = NULL;
	//Root=Insert(Root, 10);
	//Root = Insert(Root, 15);
	AVL_Node* AVL_Root = NULL;
	AVL_Tree AVLTree;
	/*AVL_Root = AVLTree.Add(AVL_Root, 10);
	AVL_Root = AVLTree.Add(AVL_Root, 15);
	AVL_Root = AVLTree.Add(AVL_Root, 17);
	AVL_Root = AVLTree.Add(AVL_Root, 5);
	AVL_Root = AVLTree.Add(AVL_Root, 7);*/
	//cout << AVLTree.Height(AVL_Root) << endl;
	RnB_Tree RnBTree;
	/*RnBTree.Out();
	cout << AVLTree.Height(AVL_Root) << endl;*/
	//RnBTree.Out();
	ofstream f1, f2, f3;
	sort(exp_arr, exp_arr + 10000);
	//auto start = chrono::high_resolution_clock::now();
	//auto stop = chrono::high_resolution_clock::now();
	//float duration = chrono::duration_cast<chrono::nanoseconds>(stop - start).count();
	f1.open("BST_Normal_Height.txt", ios::out);
	f2.open("AVL_Normal_Height.txt", ios::out);
	f3.open("RnB_Normal_Height.txt", ios::out);
	for (int i = 0; i < 500; i++) {
		BST_Root = Insert(BST_Root, exp_arr[i]);
		f1 << BST_Depth(BST_Root) << endl;
		AVL_Root = AVLTree.Add(AVL_Root,exp_arr[i]);
		f2 << AVLTree.Height(AVL_Root) << endl;
		RnBTree.insert(exp_arr[i]);
		f3 << RnBTree.RnB_Depth(RnBTree.root) << endl;

	}
	RnBTree.Out();

}


