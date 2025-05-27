#pragma once
#include "HashTable.h"
#include "Table.h"
#include <vector>
#include<string>
#include"Record.h"
using namespace std;

template<typename TKey, typename TVal>
class ArrayHashTable : public HashTable<TKey, TVal>
{
protected:
    Record<TKey, TVal>* pRec;
    Record<TKey, TVal> Free, Del;
    int step, Curr;

public:
    ArrayHashTable(int _size, int _step = 13) : HashTable<TKey, TVal>(_size)
    {
        this->step = _step;
        this->pRec = new Record<TKey, TVal>[this->size];
        this->Free = Record<TKey, TVal>{ TKey(), TVal() };
        this->Del = Record<TKey, TVal>{ TKey(), TVal() };
        for (int i = 0; i < this->size; i++) {
            this->pRec[i] = this->Free;
        }
        this->Curr = 0;
    }

    ~ArrayHashTable()
    {
        delete[] this->pRec;
    }

    bool IsFull() const override { return this->DataCount >= this->size; }

    bool Find(TKey key) override
    {
        this->Curr = this->HashFunc(key);
        int tmp = -1;
        for (int i = 0; i < this->size; i++) {
            this->Eff++;
            if (this->pRec[this->Curr].key == this->Free.key) break;
            else if (this->pRec[this->Curr].key == this->Del.key && tmp == -1) tmp = this->Curr;
            else if (this->pRec[this->Curr].key == key) return true;
            this->Curr = (this->Curr + this->step) % this->size;
        }
        if (tmp != -1) this->Curr = tmp;
        return false;
    }

    void Insert(Record<TKey, TVal> rec) override
    {
        if (this->Find(rec.key)) {
            throw runtime_error("Key already exists");
        }

        if (this->IsFull()) {
            throw runtime_error("Table overflow");
        }

        this->pRec[this->Curr] = rec;
        this->DataCount++;
        this->Eff++;
    }

    void Delete(TKey key) override
    {
        if (this->Find(key)) {
            this->pRec[this->Curr] = this->Del;
            this->DataCount--;
            this->Eff++;
        }
        else {
            throw "Key not found";
        }
    }

    void Reset() override
    {
        this->Curr = 0;
        while ((this->Curr < this->size) &&
            (this->pRec[this->Curr].key == this->Del.key ||
                this->pRec[this->Curr].key == this->Free.key)) {
            this->Curr++;
        }
    }

    void GoNext() override
    {
        this->Curr++;
        while ((this->Curr < this->size) &&
            (this->pRec[this->Curr].key == this->Del.key ||
                this->pRec[this->Curr].key == this->Free.key)) {
            this->Curr++;
        }
    }

    bool IsEnd() override { return this->Curr == this->size; }

    Record<TKey, TVal> getCurr() override { return this->pRec[this->Curr]; }
    TKey getCurrKey() override { return this->pRec[this->Curr].key; }
    TVal getCurrVal() override { return this->pRec[this->Curr].val; }

    void fillTab(int size, int keyrange) override
    {
        if (size <= 0 || size > this->size) throw "Invalid size";

        this->clrTab();
        srand(time(0));
        vector<bool> usedKeys(keyrange, false);

        for (int i = 0; i < size; i++) {
            int key;
            do {
                key = rand() % keyrange;
            } while (usedKeys[key]);

            usedKeys[key] = true;
            //int val = rand() % 2000 - 1000;
            string val = "value_" + to_string(rand() % 1000);
            this->Insert(Record<TKey, TVal>(key, val));
        }
    }

    void clrTab() override
    {
        for (int i = 0; i < this->size; i++) {
            this->pRec[i] = this->Free;
        }
        this->DataCount = 0;
        this->Curr = 0;
    }
};

