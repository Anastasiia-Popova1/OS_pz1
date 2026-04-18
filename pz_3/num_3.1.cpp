// Завдання 1.
// 11) Дано натуральне число n. Серед чисел 1,2...,n знайти всі ті, які можна представити у вигляді суми 
// квадратів двох натуральних чисел. Визначити функцію, що дозволяє розпізнати повні квадрати.

#include <iostream>
#include <cmath>
using namespace std;


bool isPerfectSquare(int num) {     //перевірка числа на повний квадрат
    if (num < 0) return false;      
    int root = sqrt(num);
    return (root * root == num);
}

bool isSumOfTwoSquares(int num) {        //чи є число сумою квадратів двох натуральних чисел
    for (int a = 1; a * a < num; a++) {
        int b_squared = num - a * a;        //знаходимо різницю, яка може бути квадратом
        if (isPerfectSquare(b_squared)) {    //перевірка 
            int b = sqrt(b_squared);      // якщо число - квадрат, то знаходимо його
            if (b > 0) { 
                return true;
            }
        }
    }
    return false;
}

int main() {
    int n;
    
    cout << "Введіть натуральне число n: ";
    cin >> n;
    
    cout << "Числа від 1 до " << n << ", які можна представити у вигляді суми квадратів двох натуральних чисел:" << endl;
    
    bool found = false;
    
    for (int i = 1; i <= n; i++) {              //підставляємо числа 1...n в функцію
        if (isSumOfTwoSquares(i)) {
            cout << i << " = ";
            
            for (int a = 1; a * a < i; a++) {               
                int b_squared = i - a * a;
                if (isPerfectSquare(b_squared)) {
                    int b = sqrt(b_squared);
                    cout << a << "^2 + " << b << "^2";      //друкуємо сумму квадратів
                    break;
                }
            }
            cout << endl;
            found = true;
        }
    }
    
    if (!found) {
        cout << "Таких чисел не знайдено" << endl;
    }
    return 0;
}