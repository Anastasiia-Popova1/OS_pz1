// Завдання 2.
// 1. Використовуючи функції сформувати за допомогою ДСЧ одновимірний масив і вивести його на друк.
// 2. Виконати обробку одновимірного масиву у відповідності з варіантом, використовуючи функції, результат вивести на друк.
// Варіант - 11
// Одновимірний масив - Створити новий масив з номерів елементів, значення яких дорівнюють 0.
// Двовимірний масив - Перевернути всі парні рядки матриці.

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_SIZE = 100;


void formArray1D(int arr[], int n) {        // генерація одновимір. мас.
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 20 - 10;
    }
}

void printArray1D(int arr[], int n) {       //друк
    cout << "[ ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << "]" << endl;
}

void processArray1D(int arr[], int n) {         //формування масиву з номерів елементів, які == 0
    int zeroIndices[MAX_SIZE];
    int zeroCount = 0;
    

    for (int i = 0; i < n; i++) {
        if (arr[i] == 0) {
            zeroIndices[zeroCount] = i;         //якщо елемент старого масиву дорівнює 0, то елемент нового масиву дорівнює індексу поточного старого елемента
            zeroCount++;               //лічильник збільшується
        }
    }
    cout << "Номери елементів, значення яких дорівнюють 0: ";
    if (zeroCount == 0) {
        cout << "немає нульових елементів";
    } else {
        cout << "[ ";
        for (int i = 0; i < zeroCount; i++) {
            cout << zeroIndices[i] << " ";
        }
        cout << "]";
    }
    cout << endl;
}

void formArray2D(int arr[][MAX_SIZE], int rows, int cols) {     // генерація двовимір. мас.
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            arr[i][j] = rand() % 100 - 50;
        }
    }
}

void printArray2D(int arr[][MAX_SIZE], int rows, int cols) {        //друк 
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << arr[i][j] << "\t";
        }
        cout << endl;
    }
}


void reverseRow(int row[], int cols) {              //переворот рядка
    for (int i = 0; i < cols / 2; i++) {
        int temp = row[i];
        row[i] = row[cols - 1 - i];             //елемент зліва дорівнює дзеркальному елементу з кінця
        row[cols - 1 - i] = temp;          //навпаки
    }
}


void processArray2D(int arr[][MAX_SIZE], int rows, int cols) {      //переворот всіх рядків 
    for (int i = 0; i < rows; i++) {
        if (i % 2 == 0) {                       //проходимося по парних рядках та викликаємо минулу функцію
            reverseRow(arr[i], cols);
        }
    }
}

int main() {
    srand(time(NULL));
    
    int arr1D[MAX_SIZE];
    int n;
    
    cout << "Одновимірний масив" << endl;
    cout << "Введіть розмір масиву: ";
    cin >> n;
    
    while (n <= 0 || n > MAX_SIZE) {
        cout << "Некоректний розмір. Введіть число від 1 до " << MAX_SIZE << ": ";
        cin >> n;
    }
    
    formArray1D(arr1D, n);
    cout << "Згенерований масив: ";
    printArray1D(arr1D, n);
    
    processArray1D(arr1D, n);
    
    cout << endl;
    
    int arr2D[MAX_SIZE][MAX_SIZE];
    int rows, cols;
    
    cout << "Двовимірний масив" << endl;
    cout << "Введіть кількість рядків: ";
    cin >> rows;
    cout << "Введіть кількість стовпців: ";
    cin >> cols;
    
    while (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
        cout << "Некоректний розмір" << endl;
        cout << "Рядки (1-" << MAX_SIZE << "): ";
        cin >> rows;
        cout << "Стовпці (1-" << MAX_SIZE << "): ";
        cin >> cols;
    }
    
    formArray2D(arr2D, rows, cols);
    cout << "\nОригінальна матриця:" << endl;
    printArray2D(arr2D, rows, cols);
    
    processArray2D(arr2D, rows, cols);
    cout << "\nМатриця після перевороту парних рядків:" << endl;
    printArray2D(arr2D, rows, cols);
    return 0;
}