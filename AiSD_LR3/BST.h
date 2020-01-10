#pragma once
#include <iostream>
#include <list>
#include "node.h"
using namespace std;
class ArrayException : public std::exception
{
private:
	std::string m_error;

public:
	ArrayException(std::string error) : m_error(error) {}

	const char* what() const noexcept { return m_error.c_str(); }
};

template <typename T1, typename T2>
class BST
{
protected:
	node<T1, T2> *root; //корень
	int size, numOperation;
	void print(node<T1, T2> *n, int level);
	void L_R_t(node<T1, T2>* n);
	node<T1, T2>* Max(node<T1, T2>* n)
	{
		numOperation++;
		if (n == NULL)
			return NULL;
		while (n->right != NULL)
		{
			numOperation++;
			n = n->right;
		}
		return n;
	}
	node<T1, T2>* Min(node<T1, T2>* n)
	{
		numOperation++;
		if (n == NULL)
			return NULL;
		while (n->left != NULL)
		{
			numOperation++;
			n = n->left;
		}
		return n;
	}

	node<T1, T2>* R_Parent(node<T1, T2>* t, node<T1, T2>* x)
	{
		numOperation++;
		node<T1, T2> * rparent = NULL;
		if (t == x)
			return NULL;
		if (x->key > t->key)
		{
			rparent = R_Parent(t->right, x);
			if (rparent != NULL)
				return rparent;
			else return t;
		}
		else return R_Parent(t->left, x);
	}
	node<T1, T2>* L_Parent(node<T1, T2>* t, node<T1, T2>* x)
	{
		numOperation++;
		node<T1, T2>* lparent = NULL;
		if (t == x)
			return NULL;
		if (x->key < t->key)
		{
			lparent = L_Parent(t->left, x);
			if (lparent != NULL)
				return lparent;
			else return t;
		}
		else return L_Parent(t->right, x);
	}

	node<T1, T2>* Predecessor(node<T1, T2>* t, node<T1, T2> * x)
	{
		if (x == NULL)
		{
			return NULL;
		}
		else if (x->left != NULL)
		{
			x = Max(x->left);
			return x;
		}
		else
		{
			x = R_Parent(t, x);
			return x;
		}
	}
	node<T1, T2>* Successor(node<T1, T2>* t, node<T1, T2>* x)
	{
		if (x == NULL)
		{
			return NULL;
		}
		else if (x->right != NULL)
		{
			x = Min(x->right);
			return x;
		}
		else
		{
			x = L_Parent(t, x);
			return x;
		}
	}

public:
	BST(); //конструктор
	BST(const BST<T1, T2> &tree); // конструктор копирования
	~BST(); //  деструктор

	int numOp();
	int Size(); // размер дерева
	void clear(); //очистка дерева
	bool is_empty(); //проверка на пустоту
	virtual T2 getData(T1 k);  //доступ по чтению
	bool setData(T1 k, T2 v); //доступ по записи
	virtual bool push(T1 k, T2 v); //вставка по ключу
	virtual bool erase(T1 k); //удаление по ключу
	T1* keyList(); //список ключей
	int keyNumber(T1 k); //количество узлов с ключами, больше заданного
	void show(); //вывод дерева на консоль

	class Iterator {
		BST<T1, T2>* tree; //указатель на дерево
		node<T1, T2>* cur; //Указатель на текущий элемент дерева
		void prev()
		{
			cur = tree->Predecessor(tree->root, cur);
		}
		void next()
		{
			cur = tree->Successor(tree->root, cur);
		}
	public:
		Iterator(); 
		Iterator(BST<T1, T2>* t, node<T1, T2>* n); 
		Iterator(const Iterator&iter);

		T2& operator *(); //доступ к данным текущего элемента 
		Iterator& operator++()
		{
			tree->numOperation = 0;
			next();
			return *this;
		}
		Iterator& operator--()
		{
			tree->numOperation = 0;
			prev();
			return *this;
		}
		bool operator ==(const Iterator& iter)
		{
			if (tree == iter.tree && cur == iter.cur)
				return true;
			else return false;
		}
		bool operator !=(const Iterator& iter)
		{
			if (tree != iter.tree || cur != iter.cur)
				return true;
			else return false;
		}
	};
	friend class Iterator;

	Iterator begin()		//получение итератора begin( )
	{
		node<T1, T2>* temp = root;
		if (temp != NULL)
		{
			while (temp->left != NULL)
			{
				temp = temp->left;
			}
		}
		Iterator iter(this, temp);
		return iter;
	}
	Iterator end()		//получение итератора end( )
	{
		node<T1, T2>* temp = root;
		while (temp != NULL)
		{
			temp = temp->right;
		}
		Iterator iter(this, temp);
		return iter;
	}
	Iterator rbegin()		//получение итератора rbegin( )
	{
		node<T1, T2>* temp = root;
		if (temp != NULL)
		{
			while (temp->right != NULL)
			{
				temp = temp->right;
			}
		}
		Iterator iter(this, temp);
		return iter;
	}
};
template <typename T1, typename T2>
BST <T1, T2>::BST()
{
	root = NULL;
	size = 0;
	numOperation = 0;
}

template <typename T1, typename T2>
BST <T1, T2>::BST(const BST<T1, T2> &tree)
{
	root = tree.root;
	size = tree.size;
	numOperation = tree.numOperation;
}

template <typename T1, typename T2>
BST <T1, T2>::~BST()
{
	clear();
}

template <typename T1, typename T2>
int BST <T1, T2>::numOp()
{
	return numOperation;
}

template <typename T1, typename T2>
int BST <T1, T2>::Size()
{
	return size;
}

template <typename T1, typename T2>
bool BST <T1, T2>::is_empty()
{
	if (size == 0)
		return true;
	else return false;
}

template <typename T1, typename T2>
bool BST <T1, T2>::push(T1 k, T2 v)
{
	numOperation = 1;
	if (root == NULL) //вставка в корень если дерево пусто
	{
		root = new node<T1, T2>(k, v);
		size++;
		return true;
	}

	node<T1, T2> *t = root;
	node<T1, T2> *pred = t;

	while (t != NULL) // поиск места вставки
	{
		if (k == t->key) // совпадение ключей
		{
			t = root;
			while (t->key != k)
			{
				numOperation++;
				t->num_descendants--;
				if (k < t->key)
					t = t->left;
				else t = t->right;
			}
			return false;
		}

		numOperation++;
		t->num_descendants++;
		pred = t;

		if (k < t->key)
			t = t->left;
		else t = t->right;
	}

	if (k < pred->key)
	{
		pred->left = new node<T1, T2>(k, v);
		//pred->left->parent = pred;
	}
	else
	{
		pred->right = new node<T1, T2>(k, v);
		//pred->right->parent = pred;
	}

	size++;
	return true;
}

template <typename T1, typename T2>
bool BST <T1, T2>::erase(T1 k)
{
	numOperation = 1;
	node<T1, T2> *temp = root;
	node<T1, T2> *parent1 = NULL;
	node<T1, T2> *savedSubtree; //то, что идет после удаляемого элемента
	node<T1, T2> *minRight;

	while (temp != NULL && temp->key != k) // поиск удаляемого элемента
	{
		parent1 = temp;
		numOperation++;
		temp->num_descendants--;
		if (k < temp->key)
			temp = temp->left;
		else temp = temp->right;
	}

	if (temp == NULL) //  если не нашли ключ
	{
		temp = root;
		while (temp != NULL)
		{
			numOperation++;
			temp->num_descendants++;
			if (k < temp->key)
				temp = temp->left;
			else temp = temp->right;
		}
		return false;
	}

	if (temp == root)
	{
		temp->num_descendants--;
	}

	if (temp->left == NULL and temp->right == NULL) //если это лист
		savedSubtree = NULL;

	else if (temp->left == NULL) //если есть только правое поддерево
		savedSubtree = temp->right;

	else if (temp->right == NULL) //если есть только левое поддерево
		savedSubtree = temp->left;

	else //если есть оба сына
	{
		parent1 = temp;
		minRight = temp->right;

		while (minRight->left != NULL) //ищем минимальный элемент в правом поддереве
		{
			numOperation++;
			parent1 = minRight;
			minRight = minRight->left;
		}

		temp->key = minRight->key;
		temp->value = minRight->value;
		savedSubtree = minRight->right;
		temp = minRight;
	}

	if (parent1 == NULL) //если удаляемый элемент лежит в корне
		root = savedSubtree;

	else
	{
		if (temp->key < parent1->key)
		{
			//parent1->left->parent = parent1;
			parent1->left = savedSubtree;
		}
		else
		{
			//parent1->right->parent = parent1;
			parent1->right = savedSubtree;
		}
	}

	//delete temp;
	size--;
	return true;
}

template <typename T1, typename T2>
void BST <T1, T2>::show()
{
	numOperation = 0;
	print(root, 0);
}

template <typename T1, typename T2>
void BST <T1, T2>::print(node<T1, T2> *n, int level)
{
	if (n != NULL)
	{
		print(n->right, level + 1);
		for (int i = 0; i <= 3 * level; i++)
		{
			cout << " ";
		}
		cout << n->key << ", " << n->bal;
		numOperation++;
		cout << endl;

		print(n->left, level + 1);
	}
}

template <typename T1, typename T2>
T2 BST <T1, T2>::getData(T1 k)
{
	numOperation = 1;
	node<T1, T2>* temp = root;
	while (temp != NULL && k != temp->key)
	{
		numOperation++;
		if (k < temp->key)
			temp = temp->left;
		else temp = temp->right;
	}

	if (temp == NULL)
		throw ArrayException("Unknown key");
	else return temp->value;
}

template <typename T1, typename T2>
bool BST <T1, T2>::setData(T1 k, T2 v)
{
	numOperation = 1;
	node<T1, T2>* temp = root;
	while (temp != NULL && k != temp->key)
	{
		numOperation++;
		if (k < temp->key)
			temp = temp->left;
		else temp = temp->right;
	}

	if (temp == NULL)
		return false;
	else
	{
		temp->value = v;
		return true;
	}
}

template <typename T1, typename T2>
T1* BST <T1, T2>::keyList()
{
	T1 *List = new T1[size]; //список ключей
	list<node<T1, T2>*> stek;
	node<T1, T2>* temp;
	int i = 0;
	numOperation = 0;

	if (size != 0)
	{
		stek.push_back(root);

		while (!stek.empty())
		{
			temp = stek.back();
			stek.pop_back();

			List[i] = temp->key;
			i++;

			numOperation++;

			if (temp->right != NULL)
				stek.push_back(temp->right);
			if (temp->left != NULL)
				stek.push_back(temp->left);
		}
	}

	return List;
}

template <typename T1, typename T2>
void BST <T1, T2>::L_R_t(node<T1, T2>* n)
{
	if (n != NULL)
	{
		numOperation++;
		L_R_t(n->left);
		L_R_t(n->right);
		delete n;
		root = NULL;
		size--;
	}
}

template <typename T1, typename T2>
void BST <T1, T2>::clear()
{
	numOperation = 0;
	L_R_t(root);
}

template <typename T1, typename T2>
int BST <T1, T2>::keyNumber(T1 k)
{
	numOperation = 1;
	node<T1, T2>* temp = root;
	int num = 0;
	while (temp != NULL)
	{
		numOperation++;
		if (k < temp->key)
		{
			if (temp->right != NULL)
				num += temp->right->num_descendants + 2; //потомки правого поддерева, оно само и текущая вершина
			else num++;
			temp = temp->left;
		}
		else
		{
			temp = temp->right;
		}
	}
	return num;
}

template <typename T1, typename T2>
BST <T1, T2>::Iterator::Iterator()
{
	tree = NULL;
	cur = NULL;
}

template <typename T1, typename T2>
BST <T1, T2>::Iterator::Iterator(BST<T1, T2>* t, node<T1, T2>* n)
{
	tree = t;
	cur = n;
}

template <typename T1, typename T2>
BST <T1, T2>::Iterator::Iterator(const Iterator&iter)
{
	tree = iter.tree;
	cur = iter.cur;
}

template <typename T1, typename T2>
T2& BST <T1, T2>::Iterator::operator *()
{
	if (cur != NULL)
		return cur->value;
	else throw ArrayException("Item does not exist");
}
