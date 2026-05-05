// 1. Скласти функції для виконання арифметичних операцій +, -, *, / для 64-бітних чисел з урахування переповнень.
// 2. Створити динамічну бібліотеку з цими функціями.
// 3. Створити проект для перевірки бібліотеки в режимі статичного завантаження.
// 4. Створити проект для перевірки бібліотеки в режимі динамічного завантаження.


#include "my_dll.h"          
#include <climits>        

ArithmeticResult add64(int64_t a, int64_t b) {  //додавання 64-бітних чисел
    ArithmeticResult res;    //змінна для результату

    if ((b > 0 && a > INT64_MAX - b) || (b < 0 && a < INT64_MIN - b)) {     //перевірка переповнення при додаванні
        res.overflow = true;   //ознака переповнення
        res.result = 0;     //результат 0
    } else {
        res.overflow = false;  //переповнення немає
        res.result = a + b;  //нормальне додавання
    }
    return res;                
}

ArithmeticResult sub64(int64_t a, int64_t b) {    //віднімання 64-бітних чисел
    ArithmeticResult res;            
    
    if ((b < 0 && a > INT64_MAX + b) || (b > 0 && a < INT64_MIN + b)) { //перевірка переповнення при відніманні
    //далі - все те ж саме, що й при додаванні 
        res.overflow = true;   
        res.result = 0;        
    } else {
        res.overflow = false;  
        res.result = a - b;    
    }
    return res;             
}
ArithmeticResult mul64(int64_t a, int64_t b) {    // множення 64-бітних чисел
    ArithmeticResult res;                       
    if (a == 0 || b == 0) {     //якщо один з множників 0, то переповнення нема, результат 0                 
        res.overflow = false;                 
        res.result = 0;                     
        return res;    
    }

    if (a > INT64_MAX / b || a < INT64_MIN / b) {   //перевірка переповнення при множенні
        res.overflow = true;   
        res.result = 0;        
    } else {
        res.overflow = false; 
        res.result = a * b;   
    }
    return res;                
}

ArithmeticResult div64(int64_t a, int64_t b, bool *div_by_zero) { //ділення з перевіркою на нуль
    ArithmeticResult res;                   
    *div_by_zero = false;     //спочатку вважаємо, що ділення на нуль не було
    
    if (b == 0) {   //якщо дільник 0
        *div_by_zero = true;     //оознака ділення на нуль, результат 0
        res.overflow = false;                   
        res.result = 0;                       
        return res;                          
    }
    if (a == INT64_MIN && b == -1) {    //перевірка ділення INT64_MIN на -1, що дає переповнення
        res.overflow = true;   
        res.result = 0;       
    } else {
        res.overflow = false;  
        res.result = a / b;    
    }
    return res;         
}