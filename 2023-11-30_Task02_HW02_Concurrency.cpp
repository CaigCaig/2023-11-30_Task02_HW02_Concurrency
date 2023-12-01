#include <iostream>
#include <thread>
#include <windows.h>
#include <condition_variable>
#include <chrono>
#include <random>
#include <queue>
#include <exception>
#include <future>

using namespace std;
using namespace std::chrono_literals;
using namespace std::chrono;

once_flag f;
mutex m;

void prt_hdr()
{
	cout << "#\tid\t\tProgress Bar\t\tTime" << endl;
}

void DrawRowProgress(short i, int N)
{
	call_once(f, prt_hdr);
	unique_lock<mutex> lg(m);
	double progress = 0;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dist_err(0, 24);
	int error = dist_err(rd);
	uniform_int_distribution<int> dist(1, 4);
	COORD coord = { 0, i + 1 };
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	cout << i << "\t" << this_thread::get_id();
	lg.unlock();
	auto start = chrono::high_resolution_clock::now();
	while (progress < 100)
	{
		lg.lock();
		coord.X = 15 + static_cast<int>(progress / 4);
		coord.Y = i + 1;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		try
		{
			if ((coord.X - 15) == error)
				throw exception();
		}
		catch (exception)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		}
		cout << char(219);
		progress += dist(rd);
		lg.unlock();
		this_thread::sleep_for(200ms);
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double, milli> time = end - start;
	lg.lock();
	coord.X = 46;
	coord.Y = i + 1;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	cout << time.count();
}

void task2()
{
	short N = 25;
	thread* t = new thread[N];
	for (short i = 0; i < N; i++) t[i] = thread(DrawRowProgress, i, N);
	for (short i = 0; i < N; i++) t[i].join();
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, N + 1 });
}

int main()
{
	task2();

	system("pause");
}

/*
Прогресс - бар

Создайте консольное приложение для имитации многопоточного расчёта.
Количество потоков, длина расчёта должны быть заданы переменными.
В консоль во время работы программы должны построчно для каждого потока выводиться :

• номер потока по порядку;

• идентификатор потока;

• заполняющийся индикатор наподобие прогресс - бара, визуализирующий процесс «расчёта»;

• после завершения работы каждого потока в соответствующей строке суммарное время, затраченное на работу потока.
Строки прогресс - баров каждого потока должны выводиться одновремено.Время появления каждого нового символа в строке прогресс - бара подберите так, чтобы процесс заполнения строки был виден.Пример работы программы по ссылке.

Дополнение к заданию 2 *
Во время очередной итерации «расчёта» сымитируйте со случайной вероятностью возникновение ошибки(exception), которая не должна приводить к прекращению работы потока или программы.При этом этот факт должен визуализироваться отдельным цветом на прогресс - баре.
*/