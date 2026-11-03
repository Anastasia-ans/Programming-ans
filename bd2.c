#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // Библиотека для работы с Win32 API и потоками

// Определение структуры с использованием typedef
typedef struct
{
    int id;
    float value;
} Item;

// Структура для передачи данных в поток окна
typedef struct
{
    Item *arr;
    int size;
} WindowData;

// Прототипы функций работы с массивом
void fillArray(Item *arr, int size);
void printArray(const Item *arr, int size);
void sortByFirstCriteria(Item *arr, int size);
void sortBySecondCriteria(Item *arr, int size);

// Прототипы функций для графического окна
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI WindowThreadFunc(LPVOID lpParam);

int main()
{
    Item array[10]; // Полноценный массив на 10 элементов
    int size = 10;
    int choice;
    HANDLE hThread = NULL;
    WindowData *sharedData = NULL;

    while (1)
    {
        printf("\n--- Menu ---\n");
        printf("1 - Fill array\n");
        printf("2 - Print array\n");
        printf("3 - Sort by ID\n");
        printf("4 - Sort by Value\n");
        printf("5 - Open GUI Window (Show Array)\n"); // Новый пункт меню
        printf("0 - Exit\n");
        printf("Select a menu item: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Error: enter a number.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
                // очистка буфера
            }
            continue;
        }

        switch (choice)
        {
        case 1:
            fillArray(array, size);
            break;
        case 2:
            printArray(array, size);
            break;
        case 3:
            sortByFirstCriteria(array, size);
            printf("Sorted by ID.\n");
            break;
        case 4:
            sortBySecondCriteria(array, size);
            printf("Sorted by Value.\n");
            break;
        case 5:
            // Создаем окно в отдельном потоке, если оно еще не создано
            if (hThread == NULL)
            {
                sharedData = (WindowData *)malloc(sizeof(WindowData));
                sharedData->arr = array;
                sharedData->size = size;
                hThread = CreateThread(NULL, 0, WindowThreadFunc, sharedData, 0, NULL);
                printf("GUI Window opened.\n");
            }
            else
            {
                // Если окно уже открыто, просто сообщаем, что данные обновились в памяти
                printf("GUI Window is already running. Data updated.\n");
                // Принудительно вызываем перерисовку окна через InvalidateRect
                HWND hwndTarget = FindWindowA("Sample Window Class", "My C GUI Window with Items");
                if (hwndTarget)
                    InvalidateRect(hwndTarget, NULL, TRUE);
            }
            break;
        case 0:
            printf("Exit.\n");
            if (hThread)
                CloseHandle(hThread);
            return 0;
        default:
            printf("Invalid choice.\n");
            break;
        }
    }
    return 0;
}

// Глобальная переменная для доступа к данным из оконной процедуры
static WindowData *g_WindowData = NULL;

// --- Функции для работы с графическим окном (Win32 API) ---

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Заголовок в окне
        TextOutA(hdc, 20, 20, "--- Array contents in GUI ---", 28);

        if (g_WindowData && g_WindowData->arr)
        {
            char buffer[128];
            int yOffset = 50;
            for (int i = 0; i < g_WindowData->size; i++)
            {
                int len = snprintf(buffer, sizeof(buffer), "[%d] ID: %d, Value: %.2f",
                                   i + 1, g_WindowData->arr[i].id, g_WindowData->arr[i].value);
                TextOutA(hdc, 20, yOffset, buffer, len);
                yOffset += 22; // Смещение вниз для следующей строки
            }
        }
        else
        {
            TextOutA(hdc, 20, 50, "Array is not initialized.", 25);
        }

        EndPaint(hwnd, &ps);
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

DWORD WINAPI WindowThreadFunc(LPVOID lpParam)
{
    g_WindowData = (WindowData *)lpParam;
    HINSTANCE hInstance = GetModuleHandle(NULL);
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0, CLASS_NAME, "My C GUI Window with Items",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 420, 320,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, SW_SHOWNORMAL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// --- Функции работы с массивом ---

void fillArray(Item *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("Item %d - Enter ID: ", i + 1);
        while (scanf("%d", &arr[i].id) != 1)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            printf("Invalid input. Enter ID again: ");
        }
        printf("Item %d - Enter Value: ", i + 1);
        while (scanf("%f", &arr[i].value) != 1)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            printf("Invalid input. Enter Value again: ");
        }
    }
}

void printArray(const Item *arr, int size)
{
    if (size == 0)
    {
        printf("Array is empty.\n");
        return;
    }
    printf("--- Array contents ---\n");
    for (int i = 0; i < size; i++)
    {
        printf("[%d] ID: %d, Value: %.2f\n", i + 1, arr[i].id, arr[i].value);
    }
}

void sortByFirstCriteria(Item *arr, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (arr[j].id > arr[j + 1].id)
            {
                Item temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void sortBySecondCriteria(Item *arr, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (arr[j].value > arr[j + 1].value)
            {
                Item temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
