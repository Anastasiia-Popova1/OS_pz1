// Задано файл з DLL. Порахувати контрольну суму цього файлу і записати її в кінець DLL. 
// Для підрахунку контрольної суми використовувати алгоритм, як для поштової скриньки
// Скласти функцію для перевірки контрольної суми при завантаженні DLL.
// Вставити перевірку контрольної суми в головну функцію DLL.
// Забезпечити автоматичне формування контрольної суми після змінення DLL.


#include "MyDLL.h"
#include <windows.h> 

static BOOL VerifyCurrentDLLChecksum()      //перевірка контрольної суми поточної DLL
{
    char szDllPath[MAX_PATH]; //масив для зберігання шляху до поточної DLL
    
    HMODULE hModule; //дескриптор модуля
    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,  //отримуємо дескриптор DLL за адресою функції
                        (LPCSTR)&VerifyCurrentDLLChecksum, &hModule);       //вказуємо адресу функції
    GetModuleFileNameA(hModule, szDllPath, MAX_PATH);        //отримуємо повний шлях DLL
    
    HANDLE hFile = CreateFileA(szDllPath, GENERIC_READ, FILE_SHARE_READ, //відкриваємо DLL для читання
                                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
    if (hFile == INVALID_HANDLE_VALUE) return FALSE; 
    
    DWORD dwSize = GetFileSize(hFile, NULL); //отримуємо розмір файлу DLL
    if (dwSize <= sizeof(DWORD)) //якщо файл менший за 4 байти
    {
        CloseHandle(hFile);
        return FALSE; 
    }
    
    DWORD dwDataSize = dwSize - sizeof(DWORD);      //розмір даних без останніх 4 байт
    BYTE* pData = new BYTE[dwDataSize];         //виділяємо пам'ять для читання даних
    DWORD dwRead;           //змінна для кількості прочитаних байтів
    
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);             //переміщуємо вказівник на початок файлу
    if (!ReadFile(hFile, pData, dwDataSize, &dwRead, NULL) || dwRead != dwDataSize) //читаємо дані 
    {
        delete[] pData;
        CloseHandle(hFile); 
        return FALSE; 
    }
    
    DWORD dwSavedChecksum; //змінна для збереженої в файлі контрольної суми
    if (!ReadFile(hFile, &dwSavedChecksum, sizeof(DWORD), &dwRead, NULL)) // Читаємо КС 
    {
        delete[] pData; 
        CloseHandle(hFile); 
        return FALSE; 
    }
    
    CloseHandle(hFile); \
    
    // Обчислюємо контрольну суму (алгоритм з методички)
    DWORD dwCS = 0; 
    DWORD dwFull = dwDataSize / 4;          //кількість повних 4-байтових блоків
    DWORD* p32Data = (DWORD*)pData;         //перетворюємо вказівник
    for (DWORD i = 0; i < dwFull; i++)          //цикл по всіх повних блоках
        dwCS += p32Data[i];             //додаємо кожен DWORD до суми
    
    DWORD dwRemainder = dwDataSize % 4;     //залишок байтів (менше 4)
    if (dwRemainder)                    //якщо є неповний блок
    {
        BYTE* pRemainder = pData + dwFull * 4;              //вказівник на неповний блок
        DWORD dwLast = 0;          
        memcpy(&dwLast, pRemainder, dwRemainder);           //копіюємо залишок у DWORD
        dwCS += dwLast;             //додаємо  до суми
    }
    
    delete[] pData; 
    
    return dwCS == dwSavedChecksum;
}

extern "C" 
{
    __declspec(dllexport) int Add(int a, int b) 
    {
        return a + b; 
    }
    
    __declspec(dllexport) void ShowMessage(const char* text) 
    {
        MessageBoxA(NULL, text, "DLL Message", MB_OK);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) //перевіряємо причину виклику
    {
    case DLL_PROCESS_ATTACH:       
        if (!VerifyCurrentDLLChecksum()) 
        {
            MessageBoxA(NULL, 
                        "DLL була змінена!\nКонтрольна сума не співпадає.", 
                        "Помилка", 
                        MB_OK | MB_ICONERROR);
            return FALSE;
        }
        break;
    }
    return TRUE; 
}