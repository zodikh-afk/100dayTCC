#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <windows.h>

using namespace std;

void Timer(int seconds) { }

int main() {
    setlocale(LC_ALL, "ukr");

    thread t(Timer, 100);

    return 0;
}



