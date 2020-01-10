#pragma once
template <typename T1, typename T2>
class node
{
public:
	T1 key; //ключ
	T2 value; // данные
	int num_descendants; //количество потомков
	node *left, *right; // указатели на левого и правого соседа
	int bal, hR, hL;

	node(T1 k, T2 v)
	{
		key = k;
		value = v;
		num_descendants = 0;
		bal = 0;
		hR = 0; 
		hL = 0;
		left = NULL;
		right = NULL;
	}
	node(const node &n)
	{
		key = n->key;
		value = n->value;
		num_descendants = n->num_descendants;
		bal = n->bal;
		hR = n->hR;
		hL = n->hL;
		left = n->left;
		right = n->right;
	}
};
