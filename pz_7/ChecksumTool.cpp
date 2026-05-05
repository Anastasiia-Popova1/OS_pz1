// Задано файл з DLL. Порахувати контрольну суму цього файлу і записати її в кінець DLL. Для підрахунку контрольної суми використовувати алгоритм, як для поштової скриньки
// Скласти функцію для перевірки контрольної суми при завантаженні DLL.
// Вставити перевірку контрольної суми в головну функцію DLL.
// Забезпечити автоматичне формування контрольної суми після змінення DLL.


#include <windows.h>
#include <cstdio>
#include <cstring>

DWORD CalculateChecksum(PBYTE pData, DWORD dwSize) //обчислення контрольної суми (алгоритм з методички)
{
    DWORD dwCS = 0;         //контрольна сума
    DWORD dwFull = dwSize / 4;      //кількість повних 4-байтових блоків
    PDWORD p32Data = (PDWORD)pData; //перетворюємо вказівник для роботи з DWORD
    
    for (DWORD i = 0; i < dwFull; i++)      //цикл по всіх повних 4-байтових блоках
        dwCS += p32Data[i];         //додаємо кожен DWORD до контрольної суми
    
    DWORD dwRemainder = dwSize % 4;      //визначаємо залишок байтів
    if (dwRemainder) //якщо є неповний блок байтів
    {
        PBYTE pRemainder = pData + dwFull * 4;      //вказівник на неповний блок
        DWORD dwLast = 0;               //тимчасове зберігання залишку
        memcpy(&dwLast, pRemainder, dwRemainder);           //копіюємо залишок у DWORD
        dwCS += dwLast;             //додаємо залишок до контрольної суми
    }
    return dwCS;
}

bool AddChecksumToFile(const char* szFileName) //додавання контрольної суми в кінець файлу
{
    HANDLE hFile = CreateFileA(szFileName, GENERIC_READ | GENERIC_WRITE,            //відкриваємо файл для читання та запису
                                FILE_SHARE_READ, NULL, OPEN_EXISTING,   //дозволяємо спільне читання, відкриваємо існуючий
                                FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return false; 
    
    DWORD dwSize = GetFileSize(hFile, NULL);    //отримуємо розмір файлу в байтах
    if (dwSize == INVALID_FILE_SIZE)    //якщо помилка отримання розміру
    {
        CloseHandle(hFile);   
        return false;    
    }
    
    PBYTE pData = new BYTE[dwSize]; //виділяємо пам'ять
    DWORD dwRead; // збереження кількості прочитаних байтів
    if (!ReadFile(hFile, pData, dwSize, &dwRead, NULL) || dwRead != dwSize) //читаємо
    {
        delete[] pData; 
        CloseHandle(hFile);    
        return false;   
    }
    
    DWORD dwChecksum = CalculateChecksum(pData, dwSize); //обчислюємо контрольну суму даних
    delete[] pData; 
    
    SetFilePointer(hFile, 0, NULL, FILE_END); //переміщуємо вказівник у кінець файлу
    DWORD dwWritten; //змінна для збереження кількості записаних байтів
    bool bResult = WriteFile(hFile, &dwChecksum, sizeof(DWORD), &dwWritten, NULL); //записуємо КС в кінець
    
    CloseHandle(hFile);
    
    printf("Контрольна сума: 0x%08X\n", dwChecksum);
    return bResult && (dwWritten == sizeof(DWORD));
}

bool VerifyChecksum(const char* szFileName) //перевірка контрольної суми файлу
{
    HANDLE hFile = CreateFileA(szFileName, GENERIC_READ, FILE_SHARE_READ, // відкриваємо файл для читання
                                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
    if (hFile == INVALID_HANDLE_VALUE) return false; 
    
    DWORD dwSize = GetFileSize(hFile, NULL); //отримуємо повний розмір файлу
    if (dwSize <= sizeof(DWORD)) //якщо файл менший або дорівнює 4 байтам
    {
        CloseHandle(hFile);
        return false;
    }
    
    DWORD dwDataSize = dwSize - sizeof(DWORD);          //розмір даних без останніх 4 байтів
    PBYTE pData = new BYTE[dwDataSize];     //виділяємо пам'ять для даних
    DWORD dwRead;           // змінна для кількості прочитаних байтів
    
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);        
    if (!ReadFile(hFile, pData, dwDataSize, &dwRead, NULL) || dwRead != dwDataSize) //читаємо дані без КС
    {
        delete[] pData; 
        CloseHandle(hFile); 
        return false; 
    }
    
    DWORD dwSavedChecksum; //змінна для збереженої КС
    if (!ReadFile(hFile, &dwSavedChecksum, sizeof(DWORD), &dwRead, NULL)) //читаємо КС з кінця файлу
    {
        delete[] pData; 
        CloseHandle(hFile);
        return false; 
    }
    
    CloseHandle(hFile);
    
    DWORD dwCalculated = CalculateChecksum(pData, dwDataSize); //обчислюємо КС прочитаних даних
    delete[] pData; 
    
    printf("Збережена КС: 0x%08X\n", dwSavedChecksum); 
    printf("Обчислена КС: 0x%08X\n", dwCalculated);
    
    return dwSavedChecksum == dwCalculated;
}

int main(int argc, char* argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    if (argc < 3)
    {
        printf("Використання:\n");
        printf("  %s check <шлях_до_DLL>  - перевірити КС\n", argv[0]);
        printf("  %s add <шлях_до_DLL>    - додати КС\n", argv[0]);
        printf("  %s auto <шлях_до_DLL>   - авторежим\n", argv[0]);
        return 1;
    }
    
    const char* cmd = argv[1];
    const char* file = argv[2];
    
    if (strcmp(cmd, "check") == 0)
    {
        if (VerifyChecksum(file))
            printf("\nКС коректна\n");
        else
            printf("\nКС невірна або відсутня\n");
    }
    else if (strcmp(cmd, "add") == 0)
    {
        if (AddChecksumToFile(file))
            printf("\nКС додана\n");
        else
            printf("\nПомилка\n");
    }
    else if (strcmp(cmd, "auto") == 0)
    {
        if (VerifyChecksum(file))
            printf("\nКС коректна\n");
        else
        {
            printf("\nКС невірна або відсутня\n");
            if (AddChecksumToFile(file))
                printf("\nКС додана\n");
            else
                printf("\nПомилка\n");
        }
    }
    return 0;
}

// Результат:

// PS C:\VS CODE PROJECTS\Site\OS\OS\pz_7>  .\ChecksumTool.exe add MyDLL.dll
// Контрольна сума: 0x72256CC0

// КС додана
// PS C:\VS CODE PROJECTS\Site\OS\OS\pz_7> .\ChecksumTool.exe check MyDLL.dll
// Збережена КС: 0x72256CC0
// Обчислена КС: 0x72256CC0

// КС коректна
// PS C:\VS CODE PROJECTS\Site\OS\OS\pz_7> .\ChecksumTool.exe check MyDLL.dll <- на цьому кроці відбулися зміни в файлі DLL
// Збережена КС: 0x72256CBD
// Обчислена КС: 0xB4871B05

// КС невірна або відсутня
// PS C:\VS CODE PROJECTS\Site\OS\OS\pz_7> .\ChecksumTool.exe auto MyDLL.dll
// Збережена КС: 0x72256CBD
// Обчислена КС: 0xB4871B05

// КС невірна або відсутня
// Контрольна сума: 0x71F94071

// КС додана