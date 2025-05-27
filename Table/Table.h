#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include"Record.h"

template<typename TKey, typename TVal>
class Table
{
protected:
    int DataCount;
    int Eff;
    int size;
    int Curr;

public:
    Table() : DataCount(0), Eff(0), size(0), Curr(-1) {}
    virtual ~Table() {}

    int getDataCount() const { return DataCount; }
    int getEff() const { return Eff; }
    void ClearEff() { Eff = 0; }

    bool IsEmpty() const { return DataCount == 0; }
    virtual bool IsFull() const = 0;
    virtual bool Find(TKey key) = 0;
    virtual void Insert(Record<TKey, TVal> rec) = 0;
    virtual void Delete(TKey key) = 0;
    virtual void Reset() = 0;
    virtual void GoNext() = 0;
    virtual bool IsEnd() = 0;
    virtual Record<TKey, TVal> getCurr() = 0;
    virtual TKey getCurrKey() = 0;
    virtual TVal getCurrVal() = 0;
    virtual void fillTab(int size, int keyrange) = 0;
    //virtual void printTab(std::string filename) = 0;
    virtual void clrTab() = 0;
};

template<typename TKey, typename TVal>
std::ostream& operator<<(std::ostream& os, Table<TKey, TVal>& tab)
{
    tab.Reset();
  
    while (!tab.IsEnd())
    {
        os << "Ключ: " << tab.getCurrKey() << ", Значение: " << tab.getCurrVal() << '\n';
        tab.GoNext();
    }
    return os;
}


