#pragma once
#include "HashTable.h"
#include <list>
#include <vector>
#include <ctime>
#include <stdexcept>
#include"Record.h"
using namespace std;

template<typename TKey, typename TVal>
class ListHashTable : public HashTable<TKey, TVal>
{
protected:
    list<Record<TKey, TVal>>* pList;
    int CurrList;
    typename list<Record<TKey, TVal>>::iterator CurrI;

public:
    ListHashTable(int _size = 100) : HashTable<TKey, TVal>(_size)
    {
        this->pList = new std::list<Record<TKey, TVal>>[this->size];
        this->CurrList = -1;
        this->Reset();
    }

    ~ListHashTable()
    {
        delete[] this->pList;
    }

    bool Find(TKey key) override
    {
        this->CurrList = this->HashFunc(key);
        //this->Eff++; // Учитываем операцию вычисления хеша и доступа к списку
        
        for (this->CurrI = this->pList[this->CurrList].begin();
            this->CurrI != this->pList[this->CurrList].end();
            ++this->CurrI)
        {
            this->Eff++; // Учитываем каждое сравнение ключа
            if (this->CurrI->key == key) {
                return true;
            }
        }
        return false;
    }

    void Insert(Record<TKey, TVal> rec) override
    {
        int listIndex = this->HashFunc(rec.key);
        if (this->Find(rec.key)) {
            throw runtime_error("Key already exists");
        }
        this->pList[listIndex].push_front(rec);
        this->DataCount++;
        this->Eff++;
    }

    void Delete(TKey key) override
    {
        if (!this->Find(key)) {
            throw runtime_error("Key not found");
        }
        this->pList[this->CurrList].erase(this->CurrI);
        this->DataCount--;
        this->Eff++;
    }

    void fillTab(int size, int keyrange) override
    {
        if (size <= 0 || size > this->size)
            throw runtime_error("Invalid value for the table size");

        this->clrTab();
        srand(static_cast<unsigned>(time(nullptr)));
        vector<bool> usedKeys(keyrange, false);

        for (int i = 0; i < size; i++) {
            int key;
            do {
                key = rand() % keyrange;
            } while (usedKeys[key]);

            usedKeys[key] = true;
            //int val = rand() % 2000 - 1000;
            string val = "value_" + to_string(rand() % 1000);
            this->Insert(Record<TKey, TVal> (key, val));
        }
    }

    void clrTab() override
    {
        for (int i = 0; i < this->size; i++) {
            this->pList[i].clear();
        }
        this->DataCount = 0;
        this->CurrList = -1;
    }

    void Reset() override
    {
        this->CurrList = 0;
        while (this->CurrList < this->size && this->pList[this->CurrList].empty())
        {
            this->CurrList++;
        }
        if (this->CurrList < this->size)
        {
            this->CurrI = this->pList[this->CurrList].begin();
        }
    }

    void GoNext() override
    {
        if (this->IsEnd()) return;

        ++this->CurrI;
        while (this->CurrI == this->pList[this->CurrList].end())
        {
            this->CurrList++;
            if (this->CurrList >= this->size) break;
            this->CurrI = this->pList[this->CurrList].begin();
        }
    }

    bool IsEnd() override
    {
        return this->CurrList >= this->size;
    }

    Record<TKey, TVal> getCurr() override
    {
        if (this->IsEnd()) throw "Iterator at end";
        return *this->CurrI;
    }

    TKey getCurrKey() override
    {
        if (this->IsEnd()) throw "Iterator at end";
        return this->CurrI->key;
    }

    TVal getCurrVal() override
    {
        if (this->IsEnd()) throw "Iterator at end";
        return this->CurrI->val;
    }

    bool IsFull() const override
    {
        return false;
    }

    void PrintTable(ostream& os = cout) const {
        
        for (int i = 0; i < this->size; ++i) {
            os << "[" << i << "]: ";
            if (this->pList[i].empty()) {
                os << "Empty\n";
            }
            else {
                for (const auto& rec : this->pList[i]) {
                    os << "(" << rec.key << ", " << rec.val << ") -> ";
                }
                os << "nullptr\n";
            }
        }
    }


};
