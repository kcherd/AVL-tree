#pragma once
#include "BST.h"
template <typename T1, typename T2>
class AVL : public BST<T1, T2>
{
	node<T1, T2>* AVL_Insert(node<T1, T2>* &t, T1 k, T2 v, bool &h, bool &inserted);
	node<T1, T2>* AVL_Delete(node<T1, T2>* &t, T1 k, bool &h, bool &deleted);
	node<T1, T2>* R(node<T1, T2>* &t);
	node<T1, T2>* L(node<T1, T2> *&t);
	node<T1, T2>* LR(node<T1, T2>* &t);
	node<T1, T2>* RL(node<T1, T2>* &t);
	node<T1, T2>* Del(node<T1, T2>* &t, node<T1, T2> *&t0, bool &h);
	node<T1, T2>* Search(node<T1, T2>* t, T1 k);

public:
	AVL() : BST<T1, T2>() {};
	//AVL(const AVL<T1, T2> &tree) : BST<T1, T2>(const BST<T1, T2> &tree) {};
	bool push(T1 k, T2 v); //вставка по ключу
	bool erase(T1 k); //удаление по ключу
	T2 getData(T1 k); //доступ по чтению
};

template <typename T1, typename T2>
bool AVL<T1, T2> :: push(T1 k, T2 v)
{
	this->numOperation = 0;
	bool flag = false;
	bool h = false;
	AVL_Insert(this->root, k, v, h, flag);
	return flag;
}

template <typename T1, typename T2>
bool AVL<T1, T2> ::erase(T1 k)
{
	this->numOperation = 0;
	bool flag = false;
	bool h = false;
	AVL_Delete(this->root, k, h, flag);
	return flag;
}

template <typename T1, typename T2>
node<T1, T2>* AVL<T1, T2> ::AVL_Insert(node<T1, T2> *&t, T1 k, T2 v, bool &h, bool &inserted)
{
	this->numOperation++;
	bool insert, hh = h;
	if (t == NULL)//вставка в корень
	{
		t = new node<T1, T2>(k, v);
		t->bal = 0;
		h = true;
		hh = true;
		inserted = true;
		insert = true;
		this->size++;
		return t;
	}
	h = false;
	if (k == t->key) //совпадение ключей
	{
		inserted = false;
		insert = false;
		return t;
	}
	if (k < t->key)
	{
		t->left = AVL_Insert(t->left, k, v, hh, insert);
		if (hh) // выросла левая ветвь
		{
			if (t->bal == 1)
			{
				t->bal = 0;
			}
			else if (t->bal == 0)
			{
				t->bal = -1;
				h = true;
			}
			//балансировка
			else if (t->left->bal == -1)
			{
				t = R(t);
			}
			else
			{
				t = LR(t);
			}
		}
	}
	else
	{
		t->right = AVL_Insert(t->right, k, v, hh, insert);
		if (hh) //выросла правая ветвь
		{
			if (t->bal == -1)
			{
				t->bal = 0;
			}
			else if (t->bal == 0)
			{
				t->bal = 1;
				h = true;
			}
			//балансировка
			else if (t->right->bal == 1)
			{
				t = L(t);
			}
			else
			{
				t = RL(t);
			}
		}
	}
	inserted = insert;
	return t;
}

template <typename T1, typename T2>
node<T1, T2>* AVL<T1, T2> ::AVL_Delete(node<T1, T2>* &t, T1 k, bool &h, bool &deleted)
{
	this->numOperation++;
	bool hh = h, del = deleted;
	node<T1, T2> *x;
	h = false;
	if (t == NULL)
	{
		deleted = false;
		return NULL;
	}
	if (k < t->key)
	{
		t->left = AVL_Delete(t->left, k, hh, del);
		if (hh)
		{
			if (t->bal == -1)
			{
				t->bal = 0;
				h = true;
			}
			else if (t->bal == 0)
			{
				t->bal = 1;
			}
			else if (t->right->bal == 0)
			{
				t = L(t);
			}
			else
			{
				h = true;
				if (t->right->bal == 1)
				{
					t = L(t);
				}
				else
				{
					t = RL(t);
				}
			}
		}
		deleted = del;
		return t;
	}
	if (k > t->key)
	{
		t->right = AVL_Delete(t->right, k, hh, del);
		if (hh)
		{
			if (t->bal == 1)
			{
				t->bal = 0;
				h = true;
			}
			else if (t->bal == 0)
			{
				t->bal = -1;
			}
			else if (t->left->bal == 0)
			{
				t = R(t);
			}
			else
			{
				h = true;
				if (t->left->bal == -1)
				{
					t = R(t);
				}
				else
				{
					t = LR(t);
				}
			}
		}
		deleted = del;
		return t;
	}
	//найден удаляемый узел
	deleted = true;
	del = true;
	if (t->left == NULL && t->right == NULL)
	{
		t = NULL;
		h = true;
		hh = true;
		this->size--;
		return NULL;
	}
	if (t->left == NULL)
	{
		x = t->right;
		t = NULL;
		h = true;
		hh = h;
		this->size--;
		if (this->size == 1)
			this->root = x;
		return x;
	}
	if (t->right == NULL)
	{
		x = t->left;
		t = NULL;
		h = true;
		hh = h;
		this->size--;
		if (this->size == 1)
			this->root = x;
		return x;
	}
	t->right = Del(t->right, t, hh);
	if (hh)
	{
		if (t->bal == 1)
		{
			t->bal = 0;
			h = true;
		}
		else if (t->bal == 0)
		{
			t->bal = -1;
		}
		else
		{
			x = t->left;
			if (x->bal == 0)
			{
				t = R(t);
			}
			else
			{
				h = true;
				if (x->bal == -1)
				{
					t = R(t);
				}
				else
				{
					t = LR(t);
				}
			}
		}
	}
	return t;
}

template <typename T1, typename T2>
node<T1, T2> *AVL<T1, T2> ::R(node<T1, T2> *&t)
{
	node<T1, T2> *x = t->left;
	t->left = x->right;
	x->right = t;
	if (x->bal == -1)
	{
		t->bal = 0;
		x->bal = 0;
	}
	else
	{
		t->bal = -1;
		x->bal = 1;
	}
	return x;
}

template <typename T1, typename T2>
node<T1, T2> *AVL<T1, T2> ::L(node<T1, T2> *&t)
{
	node<T1, T2> *x = t->right;
	t->right = x->left;
	x->left = t;
	if (x->bal == 1)
	{
		t->bal = 0;
		x->bal = 0;
	}
	else
	{
		t->bal = 1;
		x->bal = -1;
	}
	return x;
}

template <typename T1, typename T2>
node<T1, T2> *AVL<T1, T2> ::LR(node<T1, T2> *&t)
{
	node<T1, T2> *x = t->left;
	node<T1, T2> *y = x->right;
	x->right = y->left;
	y->left = x;
	t->left = y->right;
	y->right = t;
	if (y->bal == -1)
	{
		t->bal = 1;
		x->bal = 0;
	}
	if (y->bal == 0)
	{
		t->bal = 0;
		x->bal = 0;
	}
	if (y->bal == 1)
	{
		t->bal = 0;
		x->bal = -1;
	}
	y->bal = 0;
	return y;
}

template <typename T1, typename T2>
node<T1, T2>* AVL<T1, T2> ::RL(node<T1, T2> *&t)
{
	node<T1, T2> *x = t->right;
	node<T1, T2> *y = x->left;
	x->left = y->right;
	y->right = x;
	t->right = y->left;
	y->left = t;
	if (y->bal == -1)
	{
		t->bal = 0;
		x->bal = 1;
	}
	if (y->bal == 0)
	{
		t->bal = 0;
		x->bal = 0;
	}
	if (y->bal == 1)
	{
		t->bal = -1;
		x->bal = 0;
	}
	y->bal = 0;
	return y;
}

template <typename T1, typename T2>
node<T1, T2>* AVL<T1, T2> ::Del(node<T1, T2>* &t, node<T1, T2> *&t0, bool &h)
{
	bool hh = h;
	h = false;
	if (t->left != NULL)
	{
		this->numOperation++;
		t->left = Del(t->left, t0, hh);
		if (hh)
		{
			if (t->bal == -1)
			{
				t->bal = 0;
				h = true;
			}
			else if (t->bal == 0)
			{
				t->bal = 1;
			}
			else if (t->right->bal == 0)
			{
				t = L(t);
			}
			else
			{
				h = true;
				if (t->right->bal == 1)
				{
					t = L(t);
				}
				else
				{
					t = RL(t);
				}
			}
		}
		return t;
	}
	else
	{
		t0->key = t->key;
		t0->value = t->value;
		node<T1, T2> *x = t->right;
		t = NULL;
		h = true;
		hh = true;
		this->size--;
		return x;
	}
}

template <typename T1, typename T2>
T2 AVL<T1, T2> ::getData(T1 k)
{
	return Search(this->root, k)->value;
}

template <typename T1, typename T2>
node<T1, T2>* AVL<T1, T2> ::Search(node<T1, T2>* t, T1 k)
{
	if (t == NULL)
		throw ArrayException("Unknown key");
	if (k == t->key)
		return t;
	if (k < t->key)
		return Search(t->left, k);
	else return Search(t->right, k);
}
