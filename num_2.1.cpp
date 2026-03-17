// Завдання 2.1
// Скласти алгоритм та програму для обчислення значень функції f(x) = 3х - sin x, де...

#include <iostream>
#include <cmath>
using namespace std;

int main() {
    double x, y, z, c;
    cout << "Введіть c (за варіантом 11 - 1.6): ";
    cin >> c;
    c = 1.6;
    z = cos(c);
    if (z < 0)                          
       x = 3 * pow(tan(z), 3);          // 3tg³z
    else if (z >= 0 && z <= 8)          
        x = 1 / pow(cos(z), 4);         // 1 / cos⁴z
    else
        x = cos(z) + 2 * z;             // cos z +2z
    y = 3 * x - sin(x);                 // 3х - sin x

    cout << "y = " << y;
}