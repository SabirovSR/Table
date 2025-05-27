#pragma once
#include "Table.h"
#include"Record.h"

template<typename TKey, typename TVal>
class HashTable : public Table<TKey, TVal>
{
public:
    HashTable(int _size) {
        this->size = _size;
    }
    
protected:
    virtual int HashFunc(TKey key)
    {
        return key % this->size;
    }
};