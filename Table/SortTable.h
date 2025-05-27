#pragma once
#include "ArrayTable.h"
#include <vector>
#include <ctime>
#include <stdexcept>
#include <algorithm>
#include"Record.h"
using namespace std;

template<typename TKey, typename TVal>
class SortTable : public ArrayTable<TKey, TVal>
{
public:
    SortTable(int _size) : ArrayTable<TKey, TVal>(_size) {}

    bool Find(TKey key) override
    {
        int start = 0, finish = this->DataCount - 1;
        while (start <= finish) {
            this->Eff++;
            int ind = (start + finish) / 2;
            if (this->pRec[ind].key > key) {
                finish = ind - 1;
            }
            else if (this->pRec[ind].key < key) {
                start = ind + 1;
            }
            else {
                this->Curr = ind;
                return true;
            }
        }
        this->Curr = start;
        return false;
    }

    void Insert(Record<TKey, TVal> rec) override
    {
        
        bool res = this->Find(rec.key);
        if (res) throw runtime_error("Key already exists");

        if (this->IsFull()) {
            throw runtime_error("Table overflow");
        }

        if (this->Curr >= this->size) {
            throw runtime_error("Invalid insert position (Curr >= size)");
        }

        for (int i = this->DataCount; i > this->Curr; i--) {
            this->pRec[i] = this->pRec[i - 1];
            this->Eff++;
        }
        this->pRec[this->Curr] = rec;
        this->DataCount++;
    }

    void Delete(TKey key) override
    {
        bool res = this->Find(key);
        if (!res) throw runtime_error("Key not found");

        for (int i = this->Curr; i < this->DataCount - 1; i++) {
            this->pRec[i] = this->pRec[i + 1];
            this->Eff++;
        }
        this->DataCount--;
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
            string val = to_string(rand() % 1000);
            this->Insert(Record<TKey, TVal>(key, val));
        }
    }

    void clrTab() override
    {
        for (int i = 0; i < this->DataCount; i++)
            this->pRec[i] = Record<TKey, TVal>();
        this->DataCount = 0;
    }

    void QSort()
    {
        QSortRec(0, this->DataCount - 1);
    }

    void MergeSort()
    {
        MergeSortRec(0, this->DataCount - 1);
    }

   

private:
    void QSortRec(int start, int finish)
    {
        if (start >= finish) return;

        int left = start, right = finish;
        Record<TKey, TVal> pivot = this->pRec[(start + finish) / 2];

        while (left <= right) {
            while (this->pRec[left].key < pivot.key) left++;
            while (this->pRec[right].key > pivot.key) right--;
            if (left <= right) {
                std::swap(this->pRec[left], this->pRec[right]);
                left++;
                right--;
            }
            this->Eff++;
        }

        QSortRec(start, right);
        QSortRec(left, finish);
    }

    void Merge(int left, int middle, int right)
    {
        int i = left, j = middle + 1, k = 0;
        Record<TKey, TVal>* tmp = new Record<TKey, TVal>[right - left + 1];

        while (i <= middle && j <= right) {
            if (this->pRec[i].key <= this->pRec[j].key) {
                tmp[k++] = this->pRec[i++];
            }
            else {
                tmp[k++] = this->pRec[j++];
            }
            this->Eff++;
        }

        while (i <= middle) {
            tmp[k++] = this->pRec[i++];
            this->Eff++;
        }

        while (j <= right) {
            tmp[k++] = this->pRec[j++];
            this->Eff++;
        }

        for (i = left, k = 0; i <= right; i++, k++) {
            this->pRec[i] = tmp[k];
        }

        delete[] tmp;
    }

    void MergeSortRec(int left, int right)
    {
        if (left >= right) return;

        int middle = (left + right) / 2;
        MergeSortRec(left, middle);
        MergeSortRec(middle + 1, right);
        Merge(left, middle, right);
    }


};

