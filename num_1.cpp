// Завдання 1.
// 1.	Організувати введення з клавіатури дробового числа.
// 2.	Вивести отримане число з двома знаками після коми.

#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    double number;
    cout << "Введіть дріб: ";
    cin >> number;
    cout << fixed << setprecision(2) << "Ви ввели: " << number << endl;
    return 0;
}