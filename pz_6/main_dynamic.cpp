// 1. Скласти функції для виконання арифметичних операцій +, -, *, / для 64-бітних чисел з урахування переповнень.
// 2. Створити динамічну бібліотеку з цими функціями.
// 3. Створити проект для перевірки бібліотеки в режимі статичного завантаження.
// 4. Створити проект для перевірки бібліотеки в режимі динамічного завантаження.


#include <iostream>
#include <windows.h>
#include <cstdint>

using namespace std;  

typedef struct {
    int64_t result;
    bool overflow;
} ArithmeticResult;

typedef ArithmeticResult (*AddFunc)(int64_t, int64_t);
typedef ArithmeticResult (*SubFunc)(int64_t, int64_t);
typedef ArithmeticResult (*MulFunc)(int64_t, int64_t);
typedef ArithmeticResult (*DivFunc)(int64_t, int64_t, bool*);

int main() {
    HMODULE hLib = LoadLibraryA("my_arithmetic.dll");

    if (hLib == NULL) {                                    
        cout << "Помилка" << endl;
        return 1;
    }
     ArithmeticResult r;
     bool div_by_zero;
    
    AddFunc add64 = (AddFunc)GetProcAddress(hLib, "add64");
    SubFunc sub64 = (SubFunc)GetProcAddress(hLib, "sub64");
    MulFunc mul64 = (MulFunc)GetProcAddress(hLib, "mul64");
    DivFunc div64 = (DivFunc)GetProcAddress(hLib, "div64");
    
    if (!add64 || !sub64 || !mul64 || !div64) {        
        cout << "Помилка" << endl;
        FreeLibrary(hLib);
        return 1;
    }

    cout << "Динамічне завантаження DLL" << endl << endl;
    
    r = add64(9223372036854775800LL, 100);      //додавання з переповненням
    cout << "9223372036854775800 + 100 = " << r.result;
    if (r.overflow) cout << " (переповнення)";
    cout << endl;
    
    r = add64(100, 200);                //додавання звичайних чисел
    cout << "100 + 200 = " << r.result << endl;
    

    r = sub64(-9223372036854775800LL, 100);     //віднімання з переповненням
    cout << "-9223372036854775800 - 100 = " << r.result;
    if (r.overflow) cout << " (переповнення)";
    cout << endl;
    
    r = sub64(100, 30);         //віднімання звичайних чисел
    cout << "100 - 30 = " << r.result << endl;

    r = mul64(3000000000LL, 3000000000LL);  //множення з переповненням
    cout << "3000000000 * 3000000000 = " << r.result;
    if (r.overflow) cout << " (переповнення)";
    cout << endl;
    
    r = mul64(5, 6);        //множення звичайних чисел
    cout << "5 * 6 = " << r.result << endl;
    
    r = div64(100, 0, &div_by_zero);        //ділення на нуль
    if (div_by_zero) {                                 
        cout << "100 / 0 = ділення на нуль" << endl;   
    }
    
    r = div64(100, 3, &div_by_zero);    //звичайне ділення
    cout << "100 / 3 = " << r.result << endl;

    FreeLibrary(hLib);
    
    return 0;
}

// Динамічне завантаження DLL

// 9223372036854775800 + 100 = 0 (переповнення)
// 100 + 200 = 300
// -9223372036854775800 - 100 = 0 (переповнення)
// 100 - 30 = 70
// 3000000000 * 3000000000 = 9000000000000000000
// 5 * 6 = 30
// 100 / 0 = ділення на нуль
// 100 / 3 = 33