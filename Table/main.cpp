#include <iostream>
#include "Table.h"
#include "ScanTable.h"
#include "SortTable.h"
#include "ArrayHashTable.h"
#include "ListHashTable.h"
#include "TreeTable.h"
#include "BalTreeTable.h"
#include <string>
#include <ctime>
#include <windows.h>
#include <conio.h>
using namespace std;

// Цвета для консоли Windows
enum Colors {
    BLACK = 0,          // Черный
    BLUE = 1,           // Синий
    GREEN = 2,          // Зеленый
    CYAN = 3,           // Голубой
    RED = 4,            // Красный
    MAGENTA = 5,        // Пурпурный
    YELLOW = 6,         // Желтый
    WHITE = 7,          // Белый
    BRIGHT_BLUE = 9,    // Яркий синий
    BRIGHT_GREEN = 10,  // Яркий зеленый
    BRIGHT_CYAN = 11,   // Яркий голубой
    BRIGHT_RED = 12,    // Яркий красный
    BRIGHT_MAGENTA = 13,// Яркий пурпурный
    BRIGHT_YELLOW = 14, // Яркий желтый
    BRIGHT_WHITE = 15   // Яркий белый
};

// Типы таблиц в программе
enum TTabMode { 
    SCAN_TABLE = 1,     // Последовательная таблица
    SORT_TABLE = 2,     // Отсортированная таблица
    HASH_TABLE1 = 3,    // Хеш-таблица
    HASH_TABLE2 = 4,    // Хеш-таблица с цепочками
    TREE_TABLE = 5,     // Дерево
    BAL_TREE_TABLE = 6  // Сбалансированное дерево
};

// Функции для работы с консолью

// Устанавливает цвет текста в консоли
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Очищает экран консоли
void clearScreen() {
    system("cls");
}

// Устанавливает позицию курсора в консоли
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Скрывает курсор в консоли
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// Показывает курсор в консоли
void showCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

//// Выводит текст по центру экрана
void printCentered(const string& text, int y) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int x = (width - text.length()) / 4;
    setCursorPosition(x, y);
    cout << text;
}
//void printCentered(const string& text, int y) {
//    // Получаем информацию о размерах консоли
//    CONSOLE_SCREEN_BUFFER_INFO csbi;
//    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
//    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
//    
//    // Удаляем лишние пробелы и табуляции в начале и конце текста
//    string trimmedText = text;
//    while (!trimmedText.empty() && (trimmedText[0] == ' ' || trimmedText[0] == '\t')) {
//        trimmedText.erase(0, 1);
//    }
//    while (!trimmedText.empty() && (trimmedText.back() == ' ' || trimmedText.back() == '\t')) {
//        trimmedText.pop_back();
//    }
//    
//    // Вычисляем позицию для центрирования текста
//    int x = (width - trimmedText.length()) / 2;
//    setCursorPosition(x, y);
//    cout << trimmedText;
//}

// Выводит пункт меню с подсветкой выбранного элемента
void printMenuOption(const string& text, int x, int y, bool selected) {
    setCursorPosition(x, y);
    if (selected) {
        setColor(BRIGHT_GREEN);  // Выбранный пункт выделяем ярко-зеленым
        cout << "> " << text;
    } else {
        setColor(WHITE);         // Невыбранный пункт выводим белым
        cout << "  " << text;
    }
    setColor(WHITE);
}

// Возвращает название типа таблицы
string getTableTypeName(TTabMode mode) {
    switch (mode) {
        case SCAN_TABLE:
            return "Последовательная таблица";
        case SORT_TABLE:
            return "Отсортированная таблица";
        case HASH_TABLE1:
            return "Хеш-таблица";
        case HASH_TABLE2:
            return "Хеш-таблица с цепочками";
        case TREE_TABLE:
            return "Дерево";
        case BAL_TREE_TABLE:
            return "Сбалансированное дерево";
        default:
            return "Неизвестный тип";
    }
}

// Выводит заголовок меню с информацией о текущем типе таблицы
void printHeader(TTabMode mode) {
    clearScreen();
    setColor(BRIGHT_BLUE);
    printCentered("=== УПРАВЛЕНИЕ ТАБЛИЦАМИ И ДЕРЕВЬЯМИ ===", 1);
    setColor(BRIGHT_GREEN);
    printCentered(getTableTypeName(mode), 2);
    setColor(BRIGHT_CYAN);
    printCentered(" > Выберите действие с помощью стрелок ↑↓", 3);
    setColor(WHITE);
    cout << endl;
}

// Выводит строку с поддержкой Unicode
void printUnicodeString(const string& str) {
    // Конвертируем строку в широкий формат для поддержки Unicode
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
    
    // Выводим строку в консоль
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), wstr.c_str(), (DWORD)wstr.size(), NULL, NULL);
}

template <typename TKey, typename TVal>
void PrintTable(Table<TKey, TVal>& table, const string& tableName) {
    cout << "\n";
    printUnicodeString(tableName);
    printUnicodeString(", содержание:\n");
    
    if (dynamic_cast<BalTreeTable<TKey, TVal>*>(&table)) {
        auto* treeTab = dynamic_cast<BalTreeTable<TKey, TVal>*>(&table);
        if (treeTab) {
            setColor(BRIGHT_CYAN);
            printUnicodeString("Структура сбалансированного дерева (числа в скобках - факторы баланса):\n");
            setColor(WHITE);
            treeTab->PrintTree();
            return;
        }
    }
    
    table.Reset();
    while (!table.IsEnd()) {
        setColor(BRIGHT_CYAN);
        printUnicodeString("Ключ: ");
        setColor(WHITE);
        cout << table.getCurrKey();
        setColor(BRIGHT_CYAN);
        printUnicodeString(", Значение: ");
        setColor(WHITE);
        printUnicodeString(table.getCurrVal());
        cout << endl;
        table.GoNext();
    }
    setColor(WHITE);
}

Table<int, string>* pTab = nullptr;
int* pKeys = nullptr;
string* pVals = nullptr;
int DataCount = 0, MemSize = 0;

void CreateTable(TTabMode mode) {
    int MaxKey;
    clearScreen();
    printHeader(mode);
    
    while (true) {
        setCursorPosition(10, 5);
        setColor(BRIGHT_YELLOW);
        printUnicodeString("Введите количество записей: ");
        setColor(WHITE);
        cin >> DataCount;
        
        if (DataCount <= 0) {
            setCursorPosition(10, 6);
            setColor(BRIGHT_RED);
            printUnicodeString("Ошибка: количество записей должно быть положительным числом\n");
            setColor(WHITE);
            _getch();
            clearScreen();
            printHeader(mode);
            continue;
        }
        break;
    }
    
    while (true) {
        setCursorPosition(10, 6);
        setColor(BRIGHT_YELLOW);
        printUnicodeString("Введите максимальный ключ: ");
        setColor(WHITE);
        cin >> MaxKey;
        
        if (MaxKey < DataCount) {
            setCursorPosition(10, 7);
            setColor(BRIGHT_RED);
            printUnicodeString("Ошибка: максимальный ключ должен быть не меньше количества записей\n");
            setColor(WHITE);
            _getch();
            clearScreen();
            printHeader(mode);
            setCursorPosition(10, 5);
            setColor(BRIGHT_YELLOW);
            printUnicodeString("Введите количество записей: ");
            setColor(WHITE);
            cout << DataCount << endl;
            continue;
        }
        break;
    }

    MemSize = DataCount + 1;  

    switch (mode) {
    case SCAN_TABLE:
        pTab = new ScanTable<int, string>(MemSize);
        break;
    case SORT_TABLE:
        pTab = new SortTable<int, string>(MemSize);
        break;
    case HASH_TABLE1:
        pTab = new ArrayHashTable<int, string>(MemSize);
        break;
    case HASH_TABLE2:
        pTab = new ListHashTable<int, string>(DataCount);
        break;
    case TREE_TABLE:
        pTab = new TreeTable<int, string>(DataCount);
        break;
    case BAL_TREE_TABLE:
        pTab = new BalTreeTable<int, string>(DataCount);
        break;
    }

    pKeys = new int[MemSize];
    pVals = new string[MemSize];

    try {
        pTab->fillTab(DataCount, MaxKey);
        pTab->ClearEff();
    }
    catch (const runtime_error& e) {
        setCursorPosition(10, 8);
        setColor(BRIGHT_RED);
        printUnicodeString("Ошибка при заполнении таблицы: ");
        printUnicodeString(e.what());
        printUnicodeString("\nНажмите любую клавишу для возврата в меню...");
        setColor(WHITE);
        _getch();
        delete pTab;
        delete[] pKeys;
        delete[] pVals;
        pTab = nullptr;
        pKeys = nullptr;
        pVals = nullptr;
    }
}

bool ProcessTable(TTabMode mode) {
    const int MENU_ITEMS = 6;
    const string menuItems[] = {
        "Выход",
        "Поиск",
        "Вставка",
        "Удаление",
        "Печать",
        "Создать новую таблицу"
    };
    
    int selectedItem = 0;
    bool running = true;

    while (running) {
        printHeader(mode);
        
        // Print menu items
        for (int i = 0; i < MENU_ITEMS; i++) {
            printMenuOption(menuItems[i], 10, 5 + i, i == selectedItem);
        }

        // Handle input
        int key = _getch();
        if (key == 224) { // Arrow key pressed
            key = _getch();
            switch (key) {
                case 72: // Up arrow
                    selectedItem = (selectedItem - 1 + MENU_ITEMS) % MENU_ITEMS;
                    break;
                case 80: // Down arrow
                    selectedItem = (selectedItem + 1) % MENU_ITEMS;
                    break;
            }
        }
        else if (key == 13) { // Enter pressed
            int cmd = selectedItem;
            int key;
            string rec;

            switch (cmd) {
                case 0: // Exit
                    return false;

                case 1: // Search
                    clearScreen();
                    printHeader(mode);
                    setCursorPosition(10, 5);
                    setColor(BRIGHT_YELLOW);
                    cout << "Введите ключ: ";
                    setColor(WHITE);
                    cin >> key;
                    pTab->ClearEff();
                    setCursorPosition(10, 6);
                    setColor(BRIGHT_GREEN);
                    cout << "Результат: ";
                    setColor(WHITE);
                    cout << pTab->Find(key) << endl;
                    setCursorPosition(10, 7);
                    setColor(BRIGHT_CYAN);
                    cout << "Эффективность: ";
                    setColor(WHITE);
                    cout << pTab->getEff() << endl;
                    cout << "\n";
                    setColor(BRIGHT_YELLOW);
                    cout << "Нажмите любую клавишу для продолжения...";
                    setColor(WHITE);
                    _getch();
                    break;

                case 2: // Insert
                    clearScreen();
                    printHeader(mode);
                    setCursorPosition(10, 5);
                    setColor(BRIGHT_YELLOW);
                    cout << "Введите ключ: ";
                    setColor(WHITE);
                    cin >> key;
                    setCursorPosition(10, 6);
                    setColor(BRIGHT_YELLOW);
                    cout << "Введите значение: ";
                    setColor(WHITE);
                    cin >> rec;
                    pTab->ClearEff();
                    
                    try {
                        pTab->Insert(Record<int, string>(key, rec));
                        setCursorPosition(10, 7);
                        setColor(BRIGHT_GREEN);
                        cout << "Элемент успешно добавлен!" << endl;
                    }
                    catch (const runtime_error& e) {
                        setCursorPosition(10, 7);
                        setColor(BRIGHT_RED);
                        cout << "Ошибка вставки: " << e.what() << endl;
                    }
                    setCursorPosition(10, 8);
                    setColor(BRIGHT_CYAN);
                    cout << "Эффективность: ";
                    setColor(WHITE);
                    cout << pTab->getEff() << endl;
                    cout << "\n";
                    setColor(BRIGHT_YELLOW);
                    cout << "Нажмите любую клавишу для продолжения...";
                    setColor(WHITE);
                    _getch();
                    break;

                case 3: // Delete
                    clearScreen();
                    printHeader(mode);
                    setCursorPosition(10, 5);
                    setColor(BRIGHT_YELLOW);
                    cout << "Введите ключ: ";
                    setColor(WHITE);
                    cin >> key;
                    pTab->ClearEff();
                    try {
                        pTab->Delete(key);
                        setCursorPosition(10, 6);
                        setColor(BRIGHT_GREEN);
                        cout << "Элемент успешно удален!" << endl;
                    }
                    catch (const runtime_error& e) {
                        setCursorPosition(10, 6);
                        setColor(BRIGHT_RED);
                        cout << "Ошибка удаления: " << e.what() << endl;
                    }
                    setCursorPosition(10, 7);
                    setColor(BRIGHT_CYAN);
                    cout << "Эффективность: ";
                    setColor(WHITE);
                    cout << pTab->getEff() << endl;
                    cout << "\n";
                    setColor(BRIGHT_YELLOW);
                    cout << "Нажмите любую клавишу для продолжения...";
                    setColor(WHITE);
                    _getch();
                    break;

                case 4: // Print
                    clearScreen();
                    printHeader(mode);
                    pTab->ClearEff();
                    if (mode == HASH_TABLE2) {
                        auto* hashTab = dynamic_cast<ListHashTable<int, string>*>(pTab);
                        if (hashTab) {
                            hashTab->PrintTable();
                        }
                        else {
                            setColor(BRIGHT_RED);
                            printUnicodeString("Ошибка: таблица не является ListHashTable\n");
                        }
                    }
                    else if (mode == TREE_TABLE) {
                        auto* treeTab = dynamic_cast<TreeTable<int, string>*>(pTab);
                        if (treeTab) {
                            treeTab->Print(cout);
                        }
                        else {
                            setColor(BRIGHT_RED);
                            printUnicodeString("Ошибка: таблица не является TreeTable\n");
                        }
                    }
                    else if (mode == BAL_TREE_TABLE) {
                        auto* treeTab = dynamic_cast<BalTreeTable<int, string>*>(pTab);
                        if (treeTab) {
                            treeTab->PrintTree();
                        }
                        else {
                            setColor(BRIGHT_RED);
                            printUnicodeString("Ошибка: таблица не является TreeTable\n");
                        }
                    }
                    else {
                        PrintTable(*pTab, "Текущая таблица");
                    }
                    cout << "\n";
                    setColor(BRIGHT_YELLOW);
                    printUnicodeString("Нажмите любую клавишу для продолжения...");
                    setColor(WHITE);
                    _getch();
                    break;

                case 5: // Create new table
                    return true;
            }
        }
    }
    return false;
}

int main() {
    // Set console to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    // Enable virtual terminal processing for Windows 10
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    
    hideCursor();

    const int TABLE_TYPES = 6;
    const string tableTypes[] = {
        "Последовательная таблица",
        "Отсортированная таблица",
        "Хеш-таблица",
        "Хеш-таблица с цепочками",
        "Дерево",
        "Сбалансированное дерево"
    };

    int selectedType = 0;
    bool running = true;

    while (running) {
        printHeader((TTabMode)(selectedType + 1));
        
        // Print table type selection menu
        for (int i = 0; i < TABLE_TYPES; i++) {
            printMenuOption(tableTypes[i], 10, 5 + i, i == selectedType);
        }

        // Handle input
        int key = _getch();
        if (key == 224) { // Arrow key pressed
            key = _getch();
            switch (key) {
                case 72: // Up arrow
                    selectedType = (selectedType - 1 + TABLE_TYPES) % TABLE_TYPES;
                    break;
                case 80: // Down arrow
                    selectedType = (selectedType + 1) % TABLE_TYPES;
                    break;
            }
        }
        else if (key == 13) { // Enter pressed
            CreateTable((TTabMode)(selectedType + 1));
            if (!ProcessTable((TTabMode)(selectedType + 1))) {
                running = false;
            }
        }
        else if (key == 27) { // Escape pressed
            running = false;
        }
    }

    delete[] pKeys;
    delete[] pVals;
    showCursor();
    return 0;
}

