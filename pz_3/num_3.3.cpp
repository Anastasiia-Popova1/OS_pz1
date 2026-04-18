// Завдання 3.
// 1. Сформувати динамічний масив з елементів структурного типу. Структурний тип визначений у варіанті.
// 2. Роздрукувати сформований масив.
// 3. Виконати пошук елементів у масиві, що задовольняють заданій у варіанті умові і сформувати з них новий масив.
// 4. Роздрукувати отриманий масив.
// 5. Сформувати динамічний масив, що складається з динамічних рядків.
// 6. Роздрукувати сформований масив.
// 7. Виконати обробку цього масиву.
// 8. Роздрукувати отриманий масив.

// № варианта - 11
// Структура - 
// struct person
// {
// char*name;
// int age;
// };
// Критерій для пошуку в масиві структур - Вік менше 18.
// Завдання для обробки масиву рядків - Видалити рядок з номером К.

#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>

using namespace std;

struct person
{
    char* name;
    int age;
};

person make_person()            //створюємо персону за структурою
{
    int Age;
    char Name[20];

    cout << "Name? ";
    cin >> Name;
    cout << "Age? ";
    cin >> Age;
    person p;
    p.name = new char[strlen(Name) + 1];        //динамічна пам'ять для ім'я
    strcpy(p.name, Name);                       //копіюємо вміст тимчасового масиву Name у динамічну пам'ять
    p.age = Age;
    return p;
}

void print_person(person p)                 //друк однієї персони
{
    cout << "Name: " << p.name << "\tAge: " << p.age;
}


person* createPersonArray(int& size)            //формування динамічного масиву структур
{
    cout << "Введіть кількість осіб: ";
    cin >> size;
    
    person* arr = new person[size];         //виділяємо пам'ять для масиву 
    
    cout << "Введіть дані про осіб:" << endl;
    for (int i = 0; i < size; i++)          //заповнення всіх елементів масиву
    {
        cout << "Особа " << i + 1 << ":" << endl;
        arr[i] = make_person();             //виклик функції створення однієї структури та присвоєння її i-му елементу масиву
    }
    
    return arr;
}

void printPersonArray(person* arr, int size)        //друк всього масиву
{
    if (size == 0)
    {
        cout << "Масив порожній" << endl;
        return;
    }
    
    for (int i = 0; i < size; i++)
    {
        cout << i + 1 << ". ";
        print_person(arr[i]);       //проходимося по всіх елементах та викликаємо функцію для виведення однієї структури
        cout << endl;
    }
}

person* findUnderage(person* arr, int size, int& newSize)           //пошуку елементів з віком менше 18
{
    newSize = 0;
    
    for (int i = 0; i < size; i++)              //підрахунок потрібних елементів
    {
        if (arr[i].age < 18)
        {
            newSize++;
        }
    }
    if (newSize == 0)
    {
        cout << "Персони з віком менше 18 не знайдені." << endl;
        return nullptr;
    }
    
    person* result = new person[newSize];           //виділяємо пам'ять під новий масив 
    int index = 0;

    for (int i = 0; i < size; i++)              //копіюємо дані
    {
        if (arr[i].age < 18)
        {
            result[index].name = new char[strlen(arr[i].name) + 1];         // динамічне виділення пам'яті під копію імені
            strcpy(result[index].name, arr[i].name);            //копіюємо ім'я зі старого масиву в новий
            result[index].age = arr[i].age;             //копіюємо вік
            index++;            //наступний елемент масива
        }
    }
    
    return result;
}

void deletePersonArray(person* arr, int size)   //видалення динамічного масиву структур
{
    for (int i = 0; i < size; i++)
    {
        delete[] arr[i].name;
    }
    delete[] arr;
}

char** createStringArray(int& size)         //створення динамічного масиву динамічних рядків
{
    cout << "Введіть кількість рядків: ";
    cin >> size;
    cin.ignore(); 
    
    char** arr = new char*[size];               //динамічне виділення пам'яті для масиву вказівникі
    
    cout << "Введіть рядки:" << endl;
    for (int i = 0; i < size; i++)
    {
        char buffer[100];
        cout << "Рядок " << i + 1 << ": ";
        cin.getline(buffer, 100);           //зчитування рядка з пробілами до 100 символів
        
        arr[i] = new char[strlen(buffer) + 1];  //виділення динамічної пам'яті рівно під довжину введеного рядка
        strcpy(arr[i], buffer);         // копіювання рядка з буфера в динамічну пам'ять
    }           
    
    return arr;
}

void printStringArray(char** arr, int size)         //друк масиву рядків 
{
    if (size == 0)
    {
        cout << "Масив порожній" << endl;
        return;
    }
    
    for (int i = 0; i < size; i++)
    {
        cout << i + 1 << ". " << arr[i] << endl;
    }
}


char** deleteRow(char** arr, int& size, int k)          //видалення рядка з номером К
{

    if (size == 0)
    {
        cout << "Масив порожній" << endl;
        return arr;
    }
    
    if (k < 1 || k > size)                  //перевірка k
    {
        cout << "Номер рядка " << k << " виходить за межі масиву (1-" << size << ")." << endl;
        return arr;
    }
    
    int index = k - 1;
    delete[] arr[index];        //видаляємо рядок 

    char** newArr = new char*[size - 1];        //новий масив меншого розміру
    
    for (int i = 0, j = 0; i < size; i++)       //копіюємо всі елементи, крім видаленого
    {
        if (i != index)
        {
            newArr[j] = new char[strlen(arr[i]) + 1];
            strcpy(newArr[j], arr[i]);
            j++;
        }
    }
    delete[] arr;
    size--;
    cout << "Рядок з номером " << k << " видалено." << endl;
    return newArr;
}

void deleteStringArray(char** arr, int size)    //видалення всього масиву рядків
{
    for (int i = 0; i < size; i++)
    {
        delete[] arr[i];
    }
    delete[] arr;
}

int main()
{   
    person* persons = nullptr;
    int personsCount = 0;
    
    persons = createPersonArray(personsCount);
    
    cout << "\nСформований масив осіб:" << endl;
    printPersonArray(persons, personsCount);
    
    int underageCount = 0;
    person* underagePersons = findUnderage(persons, personsCount, underageCount);

    if (underagePersons != nullptr)
    {
        cout << "\nМасив осіб з віком менше 18:" << endl;
        printPersonArray(underagePersons, underageCount);
        deletePersonArray(underagePersons, underageCount);
    }
    
    char** strings = nullptr;
    int stringsCount = 0;
    
    strings = createStringArray(stringsCount);
    
    cout << "\nСформований масив рядків:" << endl;
    printStringArray(strings, stringsCount);
    
    int k;
    cout << "\nВведіть номер рядка для видалення: ";
    cin >> k;
    
    strings = deleteRow(strings, stringsCount, k);
    
    cout << "\nМасив рядків після видалення:" << endl;
    printStringArray(strings, stringsCount);
    
    deletePersonArray(persons, personsCount);
    deleteStringArray(strings, stringsCount);
    
    return 0;
}