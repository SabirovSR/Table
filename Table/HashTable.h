#pragma once
#include "Table.h"
#include "Record.h"

template<typename TKey, typename TVal>
class HashTable : public Table<TKey, TVal>
{
public:
    HashTable(int _size) {
        this->size = _size * 2;
    }
    
protected:
    virtual int HashFunc(TKey key)
    {
        /*const int prime = 50;
        int hash = 0;
        while (key > 0) {
            hash = (hash * prime + key % 10) % this->size;
            key /= 10;
        }
        return hash;*/
        //return key % 10;
        return key % this->size;
    }
};