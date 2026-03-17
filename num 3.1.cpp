// Завдання 3.1. 
// При вирішенні задачі, використовувати всі типи циклів (for, while, do while).
// 11.	Дана послідовність з n цілих чисел. Знайти різницю 
// мінімального та максимального елементів в цій послідовності.

#include <iostream>
using namespace std;

int main() {
    int n, number, min, max, difference;         
    cout << "Введіть кількість елементів послідовності n: ";
    cin >> n;

    cout << "Введіть число 1: ";
    cin >> number;
    min = max = number;

    for (int i = 2; i <= n; i++) {
        cout << "Введіть число " << i << ": ";
        cin >> number;
        if (number < min) {
            min = number;
        }
        if (number > max) {
            max = number;
        }
    }
    difference = max - min;
    cout << "Різниця: " << difference << endl;
}