#include "AVL.h"
#include <time.h>
#include <math.h>

typedef unsigned long long INT_64;

//���������� � ��������� ���������� LineRand()
static INT_64 RRand = 15750;
const INT_64 mRand = (1 << 63) - 1;
const INT_64 aRand = 6364136223846793005;
const INT_64 cRand = 1442695040888963407;
//������� ��������� ������� ���������� ����� �� ����� ����������
void sRand() { srand(time(0)); RRand = (INT_64)rand(); }

//������� ��������� ���������� �����
//�������� ������������ ���������    Xi+1=(a*Xi+c)%m
INT_64 LineRand()
{
	INT_64 y1, y2;
	y1 = (aRand*RRand + cRand) % mRand;
	y2 = (aRand*y1 + cRand) % mRand;
	RRand = y1 & 0xFFFFFFFF00000000LL ^ (y2 & 0xFFFFFFFF00000000LL) >> 32;
	return RRand;
}

void test_rand(int N)
{
	//�������� ������ ��� 64 � ��������� ������ ���� INT_64
	AVL<INT_64, int> tree;
	BST<INT_64, int> tree_bst;
	//������ ��� ������, ������� ������������ � ������ 
	INT_64* m = new INT_64[N];
	//��������� ������� ���������� �����
	sRand();
	//���������� ������ � ������� ���������� �� ���������� �������
	for (int i = 0; i < N; i++)
	{
		m[i] = LineRand();
		tree.push(m[i], 1);
		tree_bst.push(m[i], 1);
	}
	//����� ������� ������ �� �����
	cout << "items count AVL:" << tree.Size() << endl;
	cout << "items count BST:" << tree_bst.Size() << endl;
	//��������� ��������� ����������� �������, �������� � ������
	double I = 0, Ibst = 0;
	double D = 0, Dbst = 0;
	double S = 0, Sbst = 0;
	//��������� ������ ��������, 10% - ������� ��������
	for (int i = 0; i < N / 2; i++)
	{
		if (i % 10 == 0)		//10% ��������
		{
			tree.erase(LineRand());
			tree_bst.erase(LineRand());
			D += tree.numOp();
			Dbst += tree_bst.numOp();
			tree.push(m[rand() % N], 1);
			tree_bst.push(m[rand() % N], 1);
			I += tree.numOp();
			Ibst += tree_bst.numOp();
			try
			{
				tree.getData(LineRand());
				tree_bst.getData(LineRand());
				S += tree.numOp();
				Sbst += tree_bst.numOp();
			}
			catch (ArrayException &exception)
			{
				S += tree.numOp();
				Sbst += tree_bst.numOp();
				//std::cerr << "Exception occurred (" << exception.what() << ")\n";
			}
			catch (std::exception &exception)
			{
				S += tree.numOp();
				Sbst += tree_bst.numOp();
				//std::cerr << "Some other std::exception occurred (" << exception.what() << ")\n";
			}
		}
		else  //90% �������� ��������
		{
			int ind = rand() % N;
			tree.erase(m[ind]);
			tree_bst.erase(m[ind]);
			D += tree.numOp();
			Dbst += tree_bst.numOp();
			INT_64 key = LineRand();
			tree.push(key, 1);
			tree_bst.push(key, 1);
			I += tree.numOp();
			Ibst += tree_bst.numOp();
			m[ind] = key;
			try {
				tree.getData(m[rand() % N]);
				tree_bst.getData(m[rand() % N]);
				S += tree.numOp();
				Sbst += tree_bst.numOp();
			}
			//��������� ���������� ��� ������ �������� ������
			catch (ArrayException &exception)
			{
				S += tree.numOp();
				Sbst += tree_bst.numOp();
				//std::cerr << "Exception occurred (" << exception.what() << ")\n";
			}
			catch (std::exception &exception)
			{
				S += tree.numOp();
				Sbst += tree_bst.numOp();
				//std::cerr << "Some other std::exception occurred (" << exception.what() << ")\n";
			}
		}	//����� �����
	}
	//����� �����������:
	//����� ������� ������ ����� �����
	cout << "items count AVL:" << tree.Size() << endl;
	cout << "items count BST:" << tree_bst.Size() << endl;
	//������������� ������ ����������� �������� BST
	cout << "0.25 + log2(n)=" << 0.25 + (log((double)N) / log(2.0)) << endl;
	cout << "1.39*log2(n)=" << 1.39*(log((double)N) / log(2.0)) << endl;
	//����������������� ������ ����������� �������
	cout << "Count insert AVL: " << I / (N / 2) << endl;
	cout << "Count insert BST: " << Ibst / (N / 2) << endl;
	//����������������� ������ ����������� ��������
	cout << "Count delete AVL: " << D / (N / 2) << endl;
	cout << "Count delete BST: " << Dbst / (N / 2) << endl;
	//����������������� ������ ����������� ������
	cout << "Count search AVL: " << S / (N / 2) << endl;
	cout << "Count search BST: " << Sbst / (N / 2) << endl;
	//������������ ������ ������� m[]
	delete[] m;
}
void test_ord(int n)
{
	//�������� ������ ��� 64 � ��������� ������ ���� INT_64
	AVL< INT_64, int > tree;
	BST<INT_64, int> tree_bst;
	//������ ��� ������, ������� ������������ � ������ 
	INT_64* m = new INT_64[n];

	//���������� ������ � ������� ���������� � ������������� ������� //������� �� ��������� [0, 10000, 20000, ... ,10000*n]
	for (int i = 0; i < n; i++) {
		m[i] = i * 10000;
		tree.push(m[i], 1);
		tree_bst.push(m[i], 1);
	}
	//����� ������� ������ �� ����� 
	cout << "items count AVL:" << tree.Size() << endl;
	cout << "items count BST:" << tree_bst.Size() << endl;
	//��������� ��������� ����������� �������, �������� � ������
	double I = 0, Ibst = 0;
	double D = 0, Dbst = 0;
	double S = 0, Sbst = 0;
	//��������� ������� ���������� ����� 
	sRand();
	//��������� ������ ��������, 10% - ������� ��������
	for (int i = 0; i < n / 2; i++)
	{
		if (i % 10 == 0)		//10% ��������
		{
			int ind = rand() % n;
			int k = LineRand() % (10000 * n);
			k = k + !(k % 2);	//��������� �������� ����
			tree.erase(k);
			tree_bst.erase(k);
			D += tree.numOp();
			Dbst += tree_bst.numOp();
			tree.push(m[rand() % n], 1);
			tree_bst.push(m[rand() % n], 1);
			I += tree.numOp();
			Ibst += tree_bst.numOp();
			k = LineRand() % (10000 * n);
			k = k + !(k % 2);	// ��������� �������� ����
			try {
				tree.getData(k);
				tree_bst.getData(k);
				S += tree.numOp();
				Sbst += tree_bst.numOp();
			}
			//��������� ���������� ��� ������ �������� ������
			catch (ArrayException &exception)
			{
				S += tree.numOp();
				Sbst += tree_bst.numOp();
			}
			catch (std::exception &exception)
			{
				S += tree.numOp();
				Sbst += tree_bst.numOp();
			}
		}
		else  //90% �������� ��������
		{
			int ind = rand() % n;
			tree.erase(m[ind]);
			tree_bst.erase(m[ind]);
			D += tree.numOp();
			Dbst += tree_bst.numOp();
			int k = LineRand() % (10000 * n);
			k = k + k % 2;		// ��������� ������ ����
			tree.push(k, 1);
			tree_bst.push(k, 1);
			I += tree.numOp();
			Ibst += tree_bst.numOp();
			m[ind] = k;
			try {
				tree.getData(m[rand() % n]);
				tree_bst.getData(m[rand() % n]);
				S += tree.numOp();
				Sbst += tree_bst.numOp();
			}
			//��������� ���������� ��� ������ �������� ������
			catch (ArrayException &exception)
			{
				S += tree.numOp();
				Sbst += tree_bst.numOp();
			}
			catch (std::exception &exception)
			{
				S += tree.numOp();
				Sbst += tree_bst.numOp();
			}
		}
	}

	//����� �����������:
	// ����� ������� ������ ����� �����
	cout << "items count AVL:" << tree.Size() << endl;
	cout << "items count BST:" << tree.Size() << endl;
	//������������� ������ ����������� �������� BST
	cout << "0.25 + log2(n)=" << 0.25 + (log((double)n) / log(2.0)) << endl;
	cout << "n/2 =" << n / 2 << endl;
	//����������������� ������ ����������� �������
	cout << "Count insert AVL: " << I / (n / 2) << endl;
	cout << "Count insert BST: " << Ibst / (n / 2) << endl;
	//����������������� ������ ����������� ��������
	cout << "Count delete AVL: " << D / (n / 2) << endl;
	cout << "Count delete BST: " << Dbst / (n / 2) << endl;
	//����������������� ������ ����������� ������
	cout << "Count search AVL: " << S / (n / 2) << endl;
	cout << "Count search BST: " << Sbst / (n / 2) << endl;
	//������������ ������ ������� m[]
	delete[] m;
}

int menu(bool j)
{
	int variant;
	if (j == 0)
	{
		cout << "* * * * * * * AVL-������ * * * * * * *" << endl;
		cout << "�������� ��������:" << endl;
		cout << "0. ������������ ����� ������\n"
			<< "1. ������ ������\n"
			<< "2. ������� ������\n"
			<< "3. �������� �� �������\n"
			<< "4. ������ �� ������ � ������ �� �����\n"
			<< "5. ������� ������ � �������� ������\n"
			<< "6. �������� ������ � �������� ������\n"
			<< "7. �������� ��������� begin\n"
			<< "8. �������� ��������� rbegin\n"
			<< "9. �������� ��������� end\n"
			<< "10. �������� ++ ��� ���������\n"
			<< "11. �������� -- ��� ���������\n"
			<< "12. ������ � �������� �������� ����� �������� �� ������\n"
			<< "13. ������ � �������� �������� ����� �������� �� ������\n"
			<< "14. ��������� �������� ��������� � end (���������)\n"
			<< "15. ��������� �������� ��������� � end (�����������)\n"
			<< "16. ���� ������������ ��� ���������� ������\n"
			<< "17. ���� ������������ ��� ������������ ������\n"
			<< "18. ����� �� ���������\n"
			<< endl;
	}
	cout << " >>> ";
	cin >> variant;
	return variant;
}


int main()
{
	setlocale(LC_ALL, "rus");
	int variant = menu(0);
	int k, v, N;
	bool temp;
	AVL<int, int> * tree = new AVL<int, int>();
	AVL<int, int>::Iterator iter1, iter2;

	while (variant != 18)
	{
		switch (variant)
		{
		case 0:
			cout << "������������ ����� ������" << endl;
			tree->show();
			break;
		case 1:
			cout << "������ ������" << endl;
			cout << tree->Size() << endl;
			break;
		case 2:
			cout << "������� ������" << endl;
			tree->clear();
			break;
		case 3:
			cout << "�������� �� �������" << endl;
			cout << tree->is_empty() << endl;
			break;
		case 4:
			cout << "������ �� ������ � ������ �� �����" << endl;
			cout << "������� ����: ";
			cin >> k;
			try
			{
				cout << tree->getData(k) << endl;
			}
			catch (ArrayException &exception)
			{
				std::cerr << "Exception occurred (" << exception.what() << ")\n";
			}
			catch (std::exception &exception)
			{
				std::cerr << "Some other std::exception occurred (" << exception.what() << ")\n";
			}
			break;
		case 5:
			cout << "������� ������ � �������� ������" << endl;
			cout << "������� ����: ";
			cin >> k;
			cout << "������� ��������: ";
			cin >> v;
			cout << tree->push(k, v) << endl;
			break;
		case 6:
			cout << "�������� ������ � �������� ������" << endl;
			cout << "������� ����: ";
			cin >> k;
			cout << tree->erase(k) << endl;
			break;
		case 7:
			cout << "�������� ��������� begin" << endl;
			iter1 = tree->begin();
			break;
		case 8:
			cout << "�������� ��������� rbegin" << endl;
			iter1 = tree->rbegin();
			break;
		case 9:
			cout << "�������� ��������� end" << endl;
			iter1 = tree->end();
			break;
		case 10:
			cout << "�������� ++ ��� ���������" << endl;
			++iter1;
			break;
		case 11:
			cout << "�������� -- ��� ���������" << endl;
			--iter1;
			break;
		case 12:
			cout << "������ � �������� �������� ����� �������� �� ������" << endl;
			try
			{
				cout << *iter1 << endl;
			}
			catch (ArrayException &exception)
			{
				std::cerr << "Exception occurred (" << exception.what() << ")\n";
			}
			catch (std::exception &exception)
			{
				std::cerr << "Some other std::exception occurred (" << exception.what() << ")\n";
			}
			break;
		case 13:
			cout << "������ � �������� �������� ����� �������� �� ������" << endl;
			cout << "������� ��������: ";
			cin >> v;
			try
			{
				*iter1 = v;
			}
			catch (ArrayException &exception)
			{
				std::cerr << "Exception occurred (" << exception.what() << ")\n";
			}
			catch (std::exception &exception)
			{
				std::cerr << "Some other std::exception occurred (" << exception.what() << ")\n";
			}
			break;
		case 14:
			cout << "��������� �������� ��������� � end (���������)" << endl;
			iter2 = tree->end();
			temp = iter1 == iter2;
			cout << temp << endl;
			break;
		case 15:
			cout << "��������� �������� ��������� � end (�����������)" << endl;
			iter2 = tree->end();
			temp = iter1 != iter2;
			cout << temp << endl;
			break;
		case 16:
			cout << "������������ ������������ ��� ���������� ������" << endl;
			cout << "������� ������" << endl;
			cin >> N;
			test_rand(N);
			break;
		case 17:
			cout << "������������ ������������ ��� ������������ ������" << endl;
			cout << "������� ������" << endl;
			cin >> N;
			test_ord(N);
			break;
		}
		variant = menu(1);
	}

	system("pause");
	return 1;
}