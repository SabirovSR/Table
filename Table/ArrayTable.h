#pragma once
#include "Table.h"
#include"Record.h"

template<typename TKey, typename TVal>
class ArrayTable : public Table<TKey, TVal>
{
protected:
    Record<TKey, TVal>* pRec;

public:
    ArrayTable(int _size = 10)
    {
        this->size = _size;
        this->pRec = new Record<TKey, TVal>[this->size];
        this->Curr = 0;
        this->DataCount = 0;
        this->Eff = 0;
    }

    ArrayTable(const ArrayTable& at)
    {
        this->size = at.size;
        this->Curr = at.Curr;
        this->DataCount = at.DataCount;
        this->Eff = at.Eff;
        this->pRec = new Record<TKey, TVal>[this->size];
        for (int i = 0; i < this->size; i++) {
            this->pRec[i] = at.pRec[i];
        }
    }

    ~ArrayTable() { delete[] this->pRec; }

    bool IsFull() const override { return this->DataCount >= this->size; }

    void Reset() override { this->Curr = 0; }
    void GoNext() override { this->Curr++; }
    bool IsEnd() override { return this->DataCount == this->Curr; }

    Record<TKey, TVal> getCurr() override { return this->pRec[this->Curr]; }
    TKey getCurrKey() override { return this->pRec[this->Curr].key; }
    TVal getCurrVal() override { return this->pRec[this->Curr].val; }

    virtual void fillTab(int size, int keyrange) = 0;
    virtual void clrTab() = 0;
};