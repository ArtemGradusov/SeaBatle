#include <iostream>
#include <iomanip>
#include <ctime>
using namespace std;

bool StrategyActive() //функция компа со стратегией или без
{
	int isStrategy;
	cout << "Выберите сложность. 0 - нуб, 1 - имба. Выбор: ";
	cin >> isStrategy;
	if (isStrategy == 0 || isStrategy == 1)
	{
		system("cls");
		return isStrategy;
	}
	else
	{
		system("cls");
		cout << "Неверно выбранная сложность" << endl;
		StrategyActive();
	}
}

bool AutoCreateActive() // функция автоматического создания поля для игрока
{
	int isAuto;
	cout << "Выберите как хотите построить поле. 0 - вручную, 1 - автоматически. Выбор: ";
	cin >> isAuto;
	if (isAuto == 0 || isAuto == 1)
	{
		system("cls");
		return isAuto;
	}
	else
	{
		system("cls");
		cout << "Неверно выбранное построение поля" << endl;
		AutoCreateActive();
	}
}

bool CheckCreate(int Length, int FirstX, int FirstY, int SecondX, int SecondY) // проверка возможности создания корабля по координатам
{
	if (FirstX > 0 && FirstX <= 10 && FirstY > 0 && FirstY <= 10 && SecondX > 0 && SecondX <= 10 && SecondY > 0 && SecondY <= 10) // в переделах массива
	{
		if ((FirstX != SecondX) && (FirstY != SecondY)) // должно быть по горизонтали или вертикали
			return false;
		else
			if ((FirstX - SecondX) == Length || (SecondX - FirstX) == Length || (FirstY - SecondY) == Length || (SecondY - FirstY) == Length) // разница координат = длинне корабля
				return true;
			else
				return false;
	}
	else
		return false;
}

bool CheckShips(int Field[10][10], int X, int Y) // проверка нахождения рядом кораблей
{
	X--; Y--; // уменьшаем координаты для массива
	int quantity = 0;
	if (X == 0) // проверка верха
	{
		if (Y == 0) // левый угол
		{
			if (Field[0][0] != 1 && Field[1][1] != 1 && Field[1][0] != 1 && Field[0][1] != 1)
				return true;
			else
				return false;
		}
		else
			if (Y == 9) // правый угол
			{
				if (Field[0][9] != 1 && Field[1][9] != 1 && Field[0][8] != 1 && Field[1][8] != 1)
					return true;
				else
					return false;
			}
			else // просто сторона
			{
				for (int i = -1; i < 2; i++)
					if ((Field[X][Y + i] != 1) && (Field[X + 1][Y + i] != 1))
					{
						quantity++;
						if (quantity == 3)
							return true;
					}
					else
						return false;
			}
	}
	else
		if (X == 9) // проверка низа
		{
			if (Y == 0) // левый угол
			{
				if (Field[9][0] != 1 && Field[8][0] != 1 && Field[8][1] != 1 && Field[9][1] != 1)
					return true;
				else
					return false;
			}
			else
				if (Y == 9) // правый угол
				{
					if (Field[9][9] != 1 && Field[9][8] != 1 && Field[8][9] != 1 && Field[8][8] != 1)
						return true;
					else
						return false;
				}
				else // просто сторона
				{
					for (int i = -1; i < 2; i++)
						if ((Field[X][Y + i] != 1) && (Field[X - 1][Y + i] != 1))
						{
							quantity++;
							if (quantity == 3)
								return true;
						}
						else
							return false;
				}
		}
		else
			if (Y == 0) // левая сторона 
			{
				for (int i = -1; i < 2; i++)
					if ((Field[X + i][Y] != 1) && (Field[X + i][Y + 1] != 1))
					{
						quantity++;
						if (quantity == 3)
							return true;
					}
					else
						return false;
			}
			else
				if (Y == 9) // правая сторона
				{
					for (int i = -1; i < 2; i++)
						if ((Field[X + i][Y] != 1) && (Field[X + i][Y - 1] != 1))
						{
							quantity++;
							if (quantity == 3)
								return true;
						}
						else
							return false;
				}
				else // центр массива
				{
					for (int i = -1; i < 2; i++)
						if ((Field[X + i][Y] != 1) && (Field[X][Y + i] != 1) && (Field[X + i][Y + i] != 1) && (Field[X + i][Y - i] != 1))
						{
							quantity++;
							if (quantity == 3)
								return true;
						}
						else
							return false;
				}
}

void PrintField(int Field[10][10], bool PlayerField) // отображение поля 
{
	if (PlayerField)
		cout << "Ваше поле" << endl;
	else
		cout << "Поле противника" << endl;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			switch (Field[i][j])
			{
			case 0: cout << setw(3) << "O"; break; // пустое поле
			case 1:if (PlayerField)
				cout << setw(3) << "[]"; // корабль
				  else
				cout << setw(3) << "O"; // отображение корабля для вражеского поля
				break;
			case 2: cout << setw(3) << "#"; break; // промах
			case 3: cout << setw(3) << "Ж"; break; // подбитый частично
			case 4: cout << setw(3) << "+"; break; // подбитый полностью
			default:
				break;
			}
		}
		cout << "\n";
	}
}

void CreateShip(int Field[10][10], int FirstX, int FirstY, int SecondX, int SecondY, int Length) // создание корабля на поле
{
	FirstX--; FirstY--; SecondX--; SecondY--; // уменьшение координат под массив
	int i = 0;
	if (FirstX == SecondX) // Если рисуем по Х
	{
		if (FirstY < SecondY) // если первые координаты первее
			while (Length > -1) // рисуем пока не закончится длинна корабля
			{
				Field[FirstX][FirstY + i] = 1;
				i++;
				Length--;
			}
		else // если вторые координаты первее
		{
			while (Length > -1) // рисуем пока не закончится длинна корабля
			{
				Field[SecondX][SecondY + i] = 1;
				i++;
				Length--;
			}
		}
	}
	else
	{
		if (FirstX < SecondX) // если первые координаты первее
			while (Length > -1) // рисуем пока не закончится длинна корабля
			{
				Field[FirstX + i][FirstY] = 1;
				i++;
				Length--;
			}
		else // если вторые координаты первее
		{
			while (Length > -1) // рисуем пока не закончится длинна корабля
			{
				Field[SecondX + i][SecondY] = 1;
				i++;
				Length--;
			}
		}
	}
}

int CreatePlayerField(int Field[10][10], int Length) // создание поля игрока
{
	int FirstX, FirstY, SecondX, SecondY; // координаты двух точек
	PrintField(Field, 1); // отображение поля
	cout << "Размещается " << Length + 1 << "-палубник\n";
	cout << "Введите первую координату X: ";
	cin >> FirstX;
	cout << "Введите первую координату Y: ";
	cin >> FirstY;
	cout << "Введите вторую координату X: ";
	cin >> SecondX;
	cout << "Введите вторую координату Y: ";
	cin >> SecondY;

	if (CheckCreate(Length, FirstX, FirstY, SecondX, SecondY)) // проверка возможности создания корабля по координатам 
	{
		if ((CheckShips(Field, FirstX, FirstY)) && (CheckShips(Field, SecondX, SecondY))) // проверка нахождения рядом кораблей// проверка нахождения рядом кораблей
		{
			CreateShip(Field, FirstX, FirstY, SecondX, SecondY, Length); // создание корабля на поле
			system("cls");
			return **Field;
		}
		else // если пересекает
		{
			system("cls");
			cout << "Корабль пересекает другие корабли\n";
			CreatePlayerField(Field, Length);
		}
	}
	else // если неверные координаты
	{
		system("cls");
		cout << "Нерпавильные координаты\n";
		CreatePlayerField(Field, Length);
	}
}

int ComputerCreateField(int Field[10][10], int Length) // создание поля компьютера
{
	int i = 0;
	int FirstX, FirstY, SecondX, SecondY;

	FirstX = 1 + rand() % 10; // рандом координата Х
	FirstY = 1 + rand() % 10; // рандом координата Y
	i = rand() % 4; // рандом выбор стороны

	switch (i)
	{
	case 0:SecondX = FirstX + Length; // вправо
		SecondY = FirstY;
		break;
	case 1:SecondX = FirstX - Length; // влево
		SecondY = FirstY;
		break;
	case 2:SecondY = FirstY + Length; // вниз
		SecondX = FirstX;
		break;
	case 3:SecondY = FirstY - Length; // вверх
		SecondX = FirstX;
		break;
	default:
		break;
	}

	if (CheckCreate(Length, FirstX, FirstY, SecondX, SecondY)) // теже проверки, что и при создании поля игрока
	{
		if ((CheckShips(Field, FirstX, FirstY)) && (CheckShips(Field, SecondX, SecondY)))
		{
			cout << FirstX << FirstY << SecondX << SecondY;
			CreateShip(Field, FirstX, FirstY, SecondX, SecondY, Length);
			system("cls");
			return **Field;
		}
		else
		{
			system("cls");
			ComputerCreateField(Field, Length);
		}
	}
	else
	{
		system("cls");
		ComputerCreateField(Field, Length);
	}
}

int FullDamage(int Field[10][10]) // функция полного уничтожения корабля
{
	int Xcoord, Ycoord, SecondCoord; // Х, Y, координата отличная (Например: 1,1 | 3,1 - другая координата 3)
	int Points[2][2]; // первая и последняя точка корабля
	int equal = -1; // проверочная переменная

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			if (Field[i][j] == 3)
				Field[i][j] = 4; // меняем 3 на 4

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			if (Field[i][j] == 4) // ищем первую и последнюю точку корабля
			{
				if (equal == -1)
				{
					equal = i;

					Points[0][0] = i;
					Points[0][1] = j;

					Points[1][0] = i;
					Points[1][1] = j;
				}
				else
					if (Points[0][0] == i)
						Points[1][1] = j;
					else
						if (Points[0][1] == j)
						{
							equal = j;
							Points[1][0] = i;
						}
			}

	if (Points[0][0] == Points[1][0]) // Х одинаков
	{
		Xcoord = Points[0][0];
		Ycoord = Points[0][1];
		SecondCoord = Points[1][1]; // другая Y

		for (int i = -1; i < 2; i++)
			for (int j = -1; j < 2; j++)
				if (((Xcoord + i) >= 0) && ((Xcoord + i) <= 9) && ((Ycoord + j) >= 0) && ((Ycoord + j) <= 9)) // в пределах массива
					if ((((Xcoord + i) != Xcoord) || ((Ycoord + j) != (Ycoord))) == true) // условия, чтобы не поменял часть корабля на мимо
						if ((((Xcoord + i) != Xcoord) || ((Ycoord + j) != (Ycoord + 1))) == true) // условия, чтобы не поменял вторую часть корабля на мимо
							Field[Xcoord + i][Ycoord + j] = 2; // запоняем рядоом с подбитым кораблем мимо

		for (int i = -1; i < 2; i++)
			for (int j = -1; j < 2; j++) // тоже самое для другой точки
				if (((Xcoord + i) >= 0) && ((Xcoord + i) <= 9) && ((SecondCoord + j) >= 0) && ((SecondCoord + j) <= 9))
					if ((((Xcoord + i) != Xcoord) || ((SecondCoord + j) != (SecondCoord))) == true)
						if ((((Xcoord + i) != Xcoord) || ((SecondCoord + j) != (SecondCoord - 1))) == true)
							Field[Xcoord + i][SecondCoord + j] = 2;

		return **Field;
	}
	else // Y одинаков
	{
		Xcoord = Points[0][0];
		Ycoord = Points[0][1];
		SecondCoord = Points[1][0]; // другой Х

		for (int i = -1; i < 2; i++)
			for (int j = -1; j < 2; j++) // тоже самое для первой точки по Y
				if (((Xcoord + i) >= 0) && ((Xcoord + i) <= 9) && ((Ycoord + j) >= 0) && ((Ycoord + j) <= 9))
					if ((((Xcoord + i) != Xcoord) || ((Ycoord + j) != (Ycoord))) == true)
						if ((((Xcoord + i) != (Xcoord + 1)) || ((Ycoord + j) != Ycoord)) == true)
							Field[Xcoord + i][Ycoord + j] = 2;

		for (int i = -1; i < 2; i++)
			for (int j = -1; j < 2; j++) // тоже самое для другой точки по Y
				if (((SecondCoord + i) >= 0) && ((SecondCoord + i) <= 9) && ((Ycoord + j) >= 0) && ((Ycoord + j) <= 9))
					if ((((SecondCoord + i) != SecondCoord) || ((Ycoord + j) != (Ycoord))) == true)
						if ((((SecondCoord + i) != (SecondCoord - 1)) || ((Ycoord + j) != Ycoord)) == true)
							Field[SecondCoord + i][Ycoord + j] = 2;

		return **Field;
	}
}

bool CheckDiagonal(int Field[10][10], bool diagonal) // функция для перехода на другую диагональ, когда комп отстрелял одну
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			switch (diagonal) // определяем на какой из диагоналей он был | true - если есть еще для выстрела
			{
			case 0:
				if ((i % 2) == 0 && (j % 2) == 0) // проверяем Х четное двум
					if (Field[i][j] == 0)
						return true;
					else
						if ((i % 2) != 0 && (j % 2) != 0) // проверяем Х нечетное двум
							if (Field[i][j] == 0)
								return true;

				break;

			case 1:
				if ((i % 2) == 0 && (j % 2) != 0) // проверяем Х четное двум
					if (Field[i][j] == 0)
						return true;
					else
						if ((i % 2) != 0 && (j % 2) == 0) // проверяем Х нечетное двум
							if (Field[i][j] == 0)
								return true;

				break;
			default:
				break;
			}
		}

	return false; // если не осталось для стрельбы возвращаем false
}

int CheckWinAndAttack(int Field[10][10], bool isAttack) // isAttack - false(проверка на победу), true(проверка на частично подбитые корабли)
{
	int FullQuantity = 0, quantity = 0; // FullQuantity - счет для потопленников | quantity - счет для частично подбитых
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (Field[i][j] == 4) // полностью
				FullQuantity++;
			else
				if (isAttack == true && Field[i][j] == 3) // частично
					quantity++;
		}
	}

	if (FullQuantity == 20) // если все потоплены
		return 1;
	else
		if (isAttack == true && quantity > 1) // если подбито 2 и больше частей корабля
			return 2;
		else
			if (isAttack == true && quantity == 1) // если подбита 1 часть корабля 
				return 3;
			else
				return 0;
}

bool CheckAttack(int Field[10][10])	// проверка потоплен ли корабль полностью
{
	int equal = -1;
	int Points[2][2];
	int quantity = 0;

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++) // нахождение двух точек как всегда
			if (Field[i][j] == 3)
			{
				quantity++;
				if (equal == -1)
				{
					equal = i;

					Points[0][0] = i;
					Points[0][1] = j;

					Points[1][0] = i;
					Points[1][1] = j;
				}
				else
					if (Points[0][0] == i)
						Points[1][1] = j;
					else
						if (Points[0][1] == j)
							Points[1][0] = i;
			}

	Points[0][0]++; // переводим массив в координаты
	Points[0][1]++;
	Points[1][0]++;
	Points[1][1]++;


	if (equal != -1 && (CheckShips(Field, Points[0][0], Points[0][1])) && (CheckShips(Field, Points[1][0], Points[1][1]))) // использование equal, если подбитые отсутствуют | функции для проверки если ли рядом корабли
		true; // если проверка прошла, то рядом нет кораблей и можно подбитые делать как потопленный корабль
	else
		return false; // если нет, то продолжаем искать пока все не потопим
}

void AttackNextPoint(int Field[10][10], int ResultRandom[2]) // функция стратегии, когда зацепил две точки корабля
{
	bool Check; // по Х или Y
	int ForRandom;
	int Points[2][2];
	int equal = -1;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++) // поиск двух крайних точек подбитого корабля
		{
			if (Field[i][j] == 3)
				if (equal == -1)
				{
					equal = i;

					Points[0][0] = i;
					Points[0][1] = j;

					Points[1][0] = i;
					Points[1][1] = j;
				}
				else
					if (Points[0][0] == i)
					{
						Check = true;
						Points[1][1] = j;
					}
					else
						if (Points[0][1] == j)
						{
							equal = j;
							Check = false;
							Points[1][0] = i;
						}
		}
	}

	ForRandom = rand() % 2; // рандом в какую сторону

	if (Check) // если две точки по Х
	{
		if (ForRandom == 0) // если рандом 0, то влево
		{
			Points[0][1]--;
			ResultRandom[0] = equal;
			ResultRandom[1] = Points[0][1];
		}
		else // иначе вправо
		{
			Points[1][1]++;
			ResultRandom[0] = equal;
			ResultRandom[1] = Points[1][1];
		}
	}
	else // если две точки по Y
		if (ForRandom == 0) // если рандом 0, то вниз
		{
			Points[0][0]--;
			ResultRandom[0] = Points[0][0];
			ResultRandom[1] = equal;
			cout << equal;
		}
		else // иначе вверх
		{
			Points[1][0]++;
			ResultRandom[0] = Points[1][0];
			ResultRandom[1] = equal;
			cout << equal;
		}
}

int AttackPosition(int Field[10][10], bool isXcoord, int Position) // атака от 1 зацепленной части корабля | Position - (-1) это Y / от 0 до 9 это X
{
	int line, Temp;
	int Xcoord, Ycoord;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (Field[i][j] == 3) // координаты подбитого корабля
			{
				Xcoord = i;
				Ycoord = j;
			}
		}
	}

	if (9 >= Position && Position >= 0) // проверка Х или Y
	{
		if (Xcoord == Position) // если Y не менялся
		{
			do
			{
				Temp = Ycoord;
				line = -1 + rand() % 3;
				Temp += line;
			} while ((Temp - Ycoord == 0) && (0 >= Temp) && (Temp <= 9)); // условие обязательно уйти в сторону + в пределах массива
		}
		else
			Temp = Ycoord; // если Y поменялся, то Х должен остаться тот же
	}
	else // от точки подбитого делаем рандом Y
		do
		{
			Temp = Xcoord;
			line = -1 + rand() % 3;
			Temp += line;
		} while ((0 >= Temp) && (Temp <= 9)); // условие в пределах массива

		return Temp; // возвращаем новую координату
}

int Attack(int Field[10][10], bool isPlayer, bool Error, bool isStrategy, int SecondField[10][10], bool diagonal) // функция атаки
{
	int Position[2]; // для атаки, если подбито 2+ части
	int Xcoord, Ycoord;
	int random = 0;
	bool isOK; // для определения какая диагональ
	int ForRandom[2][5] = { {0, 2, 4, 6, 8},
							{1, 3, 5, 7, 9} }; // для стрельбы по дигонали

	if (CheckAttack(Field)) // проверка полного потопления
	{
		FullDamage(Field); // топим
		system("cls");
		if (isPlayer) // показываем в порядке, если ход игрока
		{
			PrintField(SecondField, 1);
			cout << endl;
			PrintField(Field, 0);
		}
		else // иначе так
		{
			PrintField(Field, 1);
			cout << endl;
			PrintField(SecondField, 0);
		}

		if (CheckWinAndAttack(Field, false)) // проверка всех топленников. Если да, то функция окончена, в ином случае продолжаем атаку
			return **Field;
		else
			Attack(Field, isPlayer, 0, isStrategy, SecondField, diagonal);
	}

	if (isPlayer) // для игрока
	{
		if (Error) // Если неправильная атака
			cout << "Не правильные координаты" << endl;

		cout << "Введите координату X: ";
		cin >> Xcoord;
		cout << "Введите координату Y: ";
		cin >> Ycoord;
		Xcoord--; Ycoord--;
	}
	else // для компа
	{
		if (isStrategy) // комп с умом
		{
			Xcoord = 1 + rand() % 10; // рандом Х

			if ((Xcoord % 2) == 0) // проверка четности Х
			{
				if (diagonal) // проверка какая диагональ
					isOK = true;
				else
					isOK = false;
			}
			else // если нечетно
				if (diagonal) // проверка какая диагональ// проверка какая диагональ
					isOK = false;
				else
					isOK = true;

			random = rand() % 5; //рандом линии

			if (isOK) // рандом точка по линии в зависимости от диагонали
				Ycoord = ForRandom[1][random];
			else
				Ycoord = ForRandom[0][random];

			Xcoord--; // перевод в систему массива

			if (CheckWinAndAttack(Field, 1) == 3) // проверка подбита ли 1 часть
			{
				Xcoord = AttackPosition(Field, true, -1); // функции атаки от 1 части
				Ycoord = AttackPosition(Field, false, Xcoord);
			}
			else
				if (CheckWinAndAttack(Field, 1) == 2) // проверка подбита ли 2+ части
				{
					AttackNextPoint(Field, Position); // функции атаки от 2+ частей
					Xcoord = Position[0];
					Ycoord = Position[1];
				}
		}
		else // комп без ума, чисто рандом координаты
		{
			Xcoord = rand() % 10;
			Ycoord = rand() % 10;
		}
	}

	switch (Field[Xcoord][Ycoord]) // вставляем точку выстрела по координатам
	{
	case 0: Field[Xcoord][Ycoord] = 2; break; // если не попал, то знак мимо
	case 1: Field[Xcoord][Ycoord] = 3; break; // если попал, то подбитие
	case 2:	Attack(Field, isPlayer, 1, isStrategy, SecondField, diagonal); break; // если выстрел в мимо - ошибка
	case 3: Attack(Field, isPlayer, 1, isStrategy, SecondField, diagonal); break; // если выстрел в подбитый - ошибка
	case 4: Attack(Field, isPlayer, 1, isStrategy, SecondField, diagonal); break; // если выстрел в потопленный - ошибка
	default:
		break;
	}
	return **Field;
}

int CheckStep(int Field[10][10]) // функция учета чей ход
{
	int quantity = 0;
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			if (Field[i][j] == 3 || Field[i][j] == 4)
				quantity++;

	return quantity; // возвращает сумму подбитых и потопленных частей
}

int StartGame(int PlayerField[10][10], int ComputerField[10][10], bool isStrategy)
{
	bool WhoHasWon = true, diagonal = true, NewDiagonal; // WhoHasWon - кто победил, diagonal - какая диагональ из рандома, NewDiagonal - если одна диагональ закончилась
	int Step = 0; // первый ходит игрок
	int PlayerStep = 0, ComputerStep = 0; // счетчик подбитых игроком и компом, чтобы продолжить атаку при подбитии корабля

	if (isStrategy) // рандом диагональ, если стратегия
		diagonal = rand() % 2;

	while (1)
	{
		if (Step == 0) // игрок
		{
			if (CheckWinAndAttack(PlayerField, 0)) // проверка поля, если все подбиты - проиграл
			{
				WhoHasWon = false;
				break;
			}

			system("cls"); // отображение полей
			PrintField(PlayerField, 1);
			cout << endl;
			PrintField(ComputerField, 0);

			Attack(ComputerField, true, 0, isStrategy, PlayerField, diagonal); // атака

			if (CheckStep(ComputerField) == PlayerStep) // если попал, то счетчики разные. Счетчик ++, ход еще раз
				Step = 1;
			else
				PlayerStep++;
		}
		else // комп
		{
			if (CheckWinAndAttack(ComputerField, 0)) // проверка поля, если все подбиты - проиграл
			{
				WhoHasWon = true;
				break;
			}

			system("cls"); // отображение полей
			PrintField(PlayerField, 1);
			cout << endl;
			PrintField(ComputerField, 0);

			if (isStrategy) // проверка диагоналей, чтобы перепрыгнуть, если закончаться ходы на одной из них
				NewDiagonal = CheckDiagonal(PlayerField, diagonal);
			else
				NewDiagonal = diagonal;

			Attack(PlayerField, false, 0, isStrategy, ComputerField, NewDiagonal); // атака

			if (CheckStep(PlayerField) == ComputerStep) // если попал, то счетчики разные. Счетчик ++, ход еще раз
				Step = 0;
			else
				ComputerStep++;
		}
	}

	if (WhoHasWon) // отображение кто победил
		cout << "Вы победили!" << endl;
	else
		cout << "Вы проиграли!" << endl;

	return 0;
}

int main()
{
	setlocale(LC_ALL, "Rus");
	srand(time(NULL));

	int StartPoint = 0; // точка отчета для количества кораблей
	int Ships[10] = { 3, 2, 2, 1, 1, 1, 0, 0, 0, 0 }; // длинна кораблей
	int PlayerField[10][10]; // поле игрока
	int ComputerField[10][10]; // поле компа

	bool isStrategy = StrategyActive(); // игрок хочет имбу компа?
	bool AutoCreate = AutoCreateActive(); // игрок хочет авто заполнение поля?

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++) // делаем чистые поля
		{
			ComputerField[i][j] = 0;
			PlayerField[i][j] = 0;
		}

	if (AutoCreate) // если игрок выбрал авто, то функция создания как у компа
		while (StartPoint < 10)
		{
			ComputerCreateField(PlayerField, Ships[StartPoint]);
			StartPoint++;
		}
	else // иначе вручную
		while (StartPoint < 10)
		{
			CreatePlayerField(PlayerField, Ships[StartPoint]);
			StartPoint++;
		}

	StartPoint = 0; // сброс счетчика количества кораблей для второго поля
	while (StartPoint < 10)
	{
		ComputerCreateField(ComputerField, Ships[StartPoint]);
		StartPoint++;
	}

	StartGame(PlayerField, ComputerField, isStrategy); // функция начала игры

	return 0;
}