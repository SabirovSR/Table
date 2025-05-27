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

// Windows console colors
enum Colors {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7,
    BRIGHT_BLUE = 9,
    BRIGHT_GREEN = 10,
    BRIGHT_CYAN = 11,
    BRIGHT_RED = 12,
    BRIGHT_MAGENTA = 13,
    BRIGHT_YELLOW = 14,
    BRIGHT_WHITE = 15
};

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void clearScreen() {
    system("cls");
}

void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void showCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void printCentered(const string& text, int y) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int x = (width - text.length()) / 2;
    setCursorPosition(x, y);
    cout << text;
}

void printMenuOption(const string& text, int x, int y, bool selected) {
    setCursorPosition(x, y);
    if (selected) {
        setColor(BRIGHT_GREEN);
        cout << "> " << text;
    } else {
        setColor(WHITE);
        cout << "  " << text;
    }
    setColor(WHITE);
}

void printHeader() {
    clearScreen();
    setColor(BRIGHT_BLUE);
    printCentered("=== УПРАВЛЕНИЕ ТАБЛИЦАМИ ===", 1);
    setColor(BRIGHT_CYAN);
    printCentered("Выберите действие с помощью стрелок ↑↓", 2);
    setColor(WHITE);
    cout << endl;
}

void printUnicodeString(const string& str) {
    // Convert string to wide string
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
    
    // Write to console
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), wstr.c_str(), (DWORD)wstr.size(), NULL, NULL);
}

template <typename TKey, typename TVal>
void PrintTable(Table<TKey, TVal>& table, const string& tableName) {
    cout << "\n";
    printUnicodeString(tableName);
    printUnicodeString(" contents:\n");
    
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

enum TTabMode { SCAN_TABLE = 1, SORT_TABLE = 2, HASH_TABLE1 = 3, HASH_TABLE2 = 4, TREE_TABLE=5, BAL_TREE_TABLE = 6
};

Table<int, string>* pTab = nullptr;
int* pKeys = nullptr;
string* pVals = nullptr;
int DataCount = 0, MemSize = 0;

void CreateTable(TTabMode mode) {
    int MaxKey;
    clearScreen();
    printHeader();
    
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
            printHeader();
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
            printHeader();
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
        printHeader();
        
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
                    printHeader();
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
                    printHeader();
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
                    printHeader();
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
                    printHeader();
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
        printHeader();
        
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

