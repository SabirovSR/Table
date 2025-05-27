#pragma once
#include "ArrayTable.h"
#include <vector>
#include <ctime>
#include <stdexcept>
#include"Record.h"
using namespace std;

template<typename TKey, typename TVal>
class ScanTable : public ArrayTable<TKey, TVal>
{
public:
    ScanTable(int _size) : ArrayTable<TKey, TVal>(_size) {}

    bool Find(TKey key) override
    {
        for (int i = 0; i < this->DataCount; i++) {
            this->Eff++;
            if (key == this->pRec[i].key) {
                this->Curr = i;
                return true;
            }
        }
        this->Curr = this->DataCount;
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

        if (this->Curr >= this->size) {
            throw runtime_error("Invalid insert position (Curr >= size)");
        }

        this->Eff++;
        this->pRec[this->Curr] = rec;
        this->DataCount++;
    }


    void Delete(TKey key) override
    {
        if (!this->Find(key)) {
            throw runtime_error("Key not found");
        }
        else {
            this->pRec[this->Curr] = this->pRec[this->DataCount - 1];
            this->DataCount--;
            this->Eff++;
        }
    }

    void fillTab(int size, int keyrange) override
    {
        if (size <= 0 || size > this->size)
            throw runtime_error("Invalid size");

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
            this->Insert(Record<TKey, TVal>(key, val));
        }
    }

    void clrTab() override
    {
        while (this->DataCount > 0)
            this->Delete(this->pRec[0].key);
    }

    Record<TKey, TVal> getCurr() override { return this->pRec[this->Curr]; }
    TKey getCurrKey() override { return this->pRec[this->Curr].key; }
    TVal getCurrVal() override { return this->pRec[this->Curr].val; }

};

