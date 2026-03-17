// Завдання 3.2. 
// Скласти програму обчислення значень функції F(x) = √х · cos²х на 
// інтервалі [a;b] з кроком h. Результат представити у вигляді таблиці 
// перший стовпчик якої – значення аргументу, другий – відповідне значення функції.



#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

int main(){
double a, b, h;
cout << "Введіть перше значення межі інтервалу ";
cin >> a;
cout << "Введіть друге значення межі інтервалу ";
cin >> b;
cout << "Введіть крок ";
cin >> h;

    cout << "|    x    |   F(x)    |" << endl;

    cout << "\n--- Цикл while ---" << endl;                  // цикл в while
    double x = a;
    while (x <= b) { 
        if (x >= 0) {
            double F = sqrt(x) * cos(x) * cos(x);
            cout << "| " << fixed << setprecision(4) << setw(6) << x 
                 << " | " << setw(8) << F << " |" << endl;
        } else {
            cout << "| " << fixed << setprecision(4) << setw(6) << x 
                 << " |   не визнач. |" << endl;
        }
        x += h;
    }
    
    cout << "\n--- Цикл do-while ---" << endl;          // цикл do-while
    x = a;
    if (x <= b) {
        do {
            if (x >= 0) {
                double F = sqrt(x) * cos(x) * cos(x);
                cout << "| " << fixed << setprecision(4) << setw(6) << x 
                     << " | " << setw(8) << F << " |" << endl;
            } else {
                cout << "| " << fixed << setprecision(4) << setw(6) << x 
                     << " |   не визнач. |" << endl;
            }
            x += h;
        } while (x <= b + h / 2);
    }
}