#include "AVL.h"
#include <time.h>
#include <math.h>

typedef unsigned long long INT_64;

//переменная и константы генератора LineRand()
static INT_64 RRand = 15750;
const INT_64 mRand = (1 << 63) - 1;
const INT_64 aRand = 6364136223846793005;
const INT_64 cRand = 1442695040888963407;
//функция установки первого случайного числа от часов компьютера
void sRand() { srand(time(0)); RRand = (INT_64)rand(); }

//функция генерации случайного числа
//линейный конгруэнтный генератор    Xi+1=(a*Xi+c)%m
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
	//создание дерева для 64 – разрядных ключей типа INT_64
	AVL<INT_64, int> tree;
	BST<INT_64, int> tree_bst;
	//массив для ключей, которые присутствуют в дереве 
	INT_64* m = new INT_64[N];
	//установка первого случайного числа
	sRand();
	//заполнение дерева и массива элементами со случайными ключами
	for (int i = 0; i < N; i++)
	{
		m[i] = LineRand();
		tree.push(m[i], 1);
		tree_bst.push(m[i], 1);
	}
	//вывод размера дерева до теста
	cout << "items count AVL:" << tree.Size() << endl;
	cout << "items count BST:" << tree_bst.Size() << endl;
	//обнуление счётчиков трудоёмкости вставки, удаления и поиска
	double I = 0, Ibst = 0;
	double D = 0, Dbst = 0;
	double S = 0, Sbst = 0;
	//генерация потока операций, 10% - промахи операций
	for (int i = 0; i < N / 2; i++)
	{
		if (i % 10 == 0)		//10% промахов
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
		else  //90% успешных операций
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
			//обработка исключения при ошибке операции поиска
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
		}	//конец теста
	}
	//вывод результатов:
	//вывод размера дерева после теста
	cout << "items count AVL:" << tree.Size() << endl;
	cout << "items count BST:" << tree_bst.Size() << endl;
	//теоретической оценки трудоёмкости операций BST
	cout << "0.25 + log2(n)=" << 0.25 + (log((double)N) / log(2.0)) << endl;
	cout << "1.39*log2(n)=" << 1.39*(log((double)N) / log(2.0)) << endl;
	//экспериментальной оценки трудоёмкости вставки
	cout << "Count insert AVL: " << I / (N / 2) << endl;
	cout << "Count insert BST: " << Ibst / (N / 2) << endl;
	//экспериментальной оценки трудоёмкости удаления
	cout << "Count delete AVL: " << D / (N / 2) << endl;
	cout << "Count delete BST: " << Dbst / (N / 2) << endl;
	//экспериментальной оценки трудоёмкости поиска
	cout << "Count search AVL: " << S / (N / 2) << endl;
	cout << "Count search BST: " << Sbst / (N / 2) << endl;
	//освобождение памяти массива m[]
	delete[] m;
}
void test_ord(int n)
{
	//создание дерева для 64 – разрядных ключей типа INT_64
	AVL< INT_64, int > tree;
	BST<INT_64, int> tree_bst;
	//массив для ключей, которые присутствуют в дереве 
	INT_64* m = new INT_64[n];

	//заполнение дерева и массива элементами с возрастающими чётными //ключами на интервале [0, 10000, 20000, ... ,10000*n]
	for (int i = 0; i < n; i++) {
		m[i] = i * 10000;
		tree.push(m[i], 1);
		tree_bst.push(m[i], 1);
	}
	//вывод размера дерева до теста 
	cout << "items count AVL:" << tree.Size() << endl;
	cout << "items count BST:" << tree_bst.Size() << endl;
	//обнуление счётчиков трудоёмкости вставки, удаления и поиска
	double I = 0, Ibst = 0;
	double D = 0, Dbst = 0;
	double S = 0, Sbst = 0;
	//установка первого случайного числа 
	sRand();
	//генерация потока операций, 10% - промахи операций
	for (int i = 0; i < n / 2; i++)
	{
		if (i % 10 == 0)		//10% промахов
		{
			int ind = rand() % n;
			int k = LineRand() % (10000 * n);
			k = k + !(k % 2);	//случайный нечётный ключ
			tree.erase(k);
			tree_bst.erase(k);
			D += tree.numOp();
			Dbst += tree_bst.numOp();
			tree.push(m[rand() % n], 1);
			tree_bst.push(m[rand() % n], 1);
			I += tree.numOp();
			Ibst += tree_bst.numOp();
			k = LineRand() % (10000 * n);
			k = k + !(k % 2);	// случайный нечётный ключ
			try {
				tree.getData(k);
				tree_bst.getData(k);
				S += tree.numOp();
				Sbst += tree_bst.numOp();
			}
			//обработка исключения при ошибке операции поиска
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
		else  //90% успешных операций
		{
			int ind = rand() % n;
			tree.erase(m[ind]);
			tree_bst.erase(m[ind]);
			D += tree.numOp();
			Dbst += tree_bst.numOp();
			int k = LineRand() % (10000 * n);
			k = k + k % 2;		// случайный чётный ключ
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
			//обработка исключения при ошибке операции поиска
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

	//вывод результатов:
	// вывод размера дерева после теста
	cout << "items count AVL:" << tree.Size() << endl;
	cout << "items count BST:" << tree.Size() << endl;
	//теоретической оценки трудоёмкости операций BST
	cout << "0.25 + log2(n)=" << 0.25 + (log((double)n) / log(2.0)) << endl;
	cout << "n/2 =" << n / 2 << endl;
	//экспериментальной оценки трудоёмкости вставки
	cout << "Count insert AVL: " << I / (n / 2) << endl;
	cout << "Count insert BST: " << Ibst / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости удаления
	cout << "Count delete AVL: " << D / (n / 2) << endl;
	cout << "Count delete BST: " << Dbst / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости поиска
	cout << "Count search AVL: " << S / (n / 2) << endl;
	cout << "Count search BST: " << Sbst / (n / 2) << endl;
	//освобождение памяти массива m[]
	delete[] m;
}

int menu(bool j)
{
	int variant;
	if (j == 0)
	{
		cout << "* * * * * * * AVL-дерево * * * * * * *" << endl;
		cout << "Выберите действие:" << endl;
		cout << "0. Вертикальный вывод дерева\n"
			<< "1. Размер дерева\n"
			<< "2. Очистка дерева\n"
			<< "3. Проверка на пустоту\n"
			<< "4. Доступ по чтению к данным по ключу\n"
			<< "5. Вставка данных с заданным ключом\n"
			<< "6. Удаление данных с заданным ключом\n"
			<< "7. Создание итератора begin\n"
			<< "8. Создание итератора rbegin\n"
			<< "9. Создание итератора end\n"
			<< "10. Операция ++ для итератора\n"
			<< "11. Операция -- для итератора\n"
			<< "12. Доступ к значению элемента через итератор по чтению\n"
			<< "13. Доступ к значению элемента через итератор по записи\n"
			<< "14. Сравнение текущего итератора с end (равенство)\n"
			<< "15. Сравнение текущего итератора с end (неравенство)\n"
			<< "16. Тест трудоемкости для рандомного дерева\n"
			<< "17. Тест трудоемкости для вырожденного дерева\n"
			<< "18. Выход из программы\n"
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
			cout << "Вертикальный вывод дерева" << endl;
			tree->show();
			break;
		case 1:
			cout << "Размер дерева" << endl;
			cout << tree->Size() << endl;
			break;
		case 2:
			cout << "Очистка дерева" << endl;
			tree->clear();
			break;
		case 3:
			cout << "Проверка на пустоту" << endl;
			cout << tree->is_empty() << endl;
			break;
		case 4:
			cout << "Доступ по чтению к данным по ключу" << endl;
			cout << "Введите ключ: ";
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
			cout << "Вставка данных с заданным ключом" << endl;
			cout << "Введите ключ: ";
			cin >> k;
			cout << "Введите значение: ";
			cin >> v;
			cout << tree->push(k, v) << endl;
			break;
		case 6:
			cout << "Удаление данных с заданным ключом" << endl;
			cout << "Введите ключ: ";
			cin >> k;
			cout << tree->erase(k) << endl;
			break;
		case 7:
			cout << "Создание итератора begin" << endl;
			iter1 = tree->begin();
			break;
		case 8:
			cout << "Создание итератора rbegin" << endl;
			iter1 = tree->rbegin();
			break;
		case 9:
			cout << "Создание итератора end" << endl;
			iter1 = tree->end();
			break;
		case 10:
			cout << "Операция ++ для итератора" << endl;
			++iter1;
			break;
		case 11:
			cout << "Операция -- для итератора" << endl;
			--iter1;
			break;
		case 12:
			cout << "Доступ к значению элемента через итератор по чтению" << endl;
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
			cout << "Доступ к значению элемента через итератор по записи" << endl;
			cout << "Введите знаечние: ";
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
			cout << "Сравнение текущего итератора с end (равенство)" << endl;
			iter2 = tree->end();
			temp = iter1 == iter2;
			cout << temp << endl;
			break;
		case 15:
			cout << "Сравнение текущего итератора с end (неравенство)" << endl;
			iter2 = tree->end();
			temp = iter1 != iter2;
			cout << temp << endl;
			break;
		case 16:
			cout << "Тестирование трудоемкости для рандомного дерева" << endl;
			cout << "Задайте размер" << endl;
			cin >> N;
			test_rand(N);
			break;
		case 17:
			cout << "Тестирование трудоемкости для вырожденного дерева" << endl;
			cout << "Задайте размер" << endl;
			cin >> N;
			test_ord(N);
			break;
		}
		variant = menu(1);
	}

	system("pause");
	return 1;
}
