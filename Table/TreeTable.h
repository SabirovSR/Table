#pragma once
#include <stack>
#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

#define BalOK    0
#define BalLeft -1
#define BalRight 1

template<typename TKey, typename TVal>
struct TreeNode {
    Record<TKey, TVal> rec;
    TreeNode<TKey, TVal>* pLeft = nullptr;   
    TreeNode<TKey, TVal>* pRight = nullptr;
    int bal = BalOK;
    TreeNode() : pLeft(nullptr), pRight(nullptr), bal(0) {}
    TreeNode(const Record<TKey, TVal>& r) : rec(r), pLeft(nullptr), pRight(nullptr), bal(0) {}
};

template<typename TKey, typename TVal>
class TreeTable : public Table<TKey, TVal> {
protected:
    TreeNode<TKey, TVal>* pRoot = nullptr;
    TreeNode<TKey, TVal>* pCurr = nullptr;
    TreeNode<TKey, TVal>* pPrev = nullptr;
    stack<TreeNode<TKey, TVal>*> st;
    int pos = 0;

public:
    TreeTable() : Table<TKey, TVal>() {}
    TreeTable(int sz) : Table<TKey, TVal>() {
        this->size = sz;
    }

    ~TreeTable() {
        clearTree(pRoot);
    }

    bool IsFull() const override {
        return false;
    }

    bool Find(TKey key) {
        pCurr = pRoot;
        pPrev = nullptr;
        while (pCurr != nullptr) {
            this->Eff++;
            if (pCurr->rec.key == key)
                return true;
            else if (key < pCurr->rec.key) {
                pPrev = pCurr;
                pCurr = pCurr->pLeft;
            }
            else {
                pPrev = pCurr;
                pCurr = pCurr->pRight;
            }
        }
        pCurr = pPrev;
        return false;
    }

    void Insert(Record<TKey, TVal> rec) override {
        if (Find(rec.key)) {
            throw std::runtime_error("Key already exists");
        }

        TreeNode<TKey, TVal>* newN = new TreeNode<TKey, TVal>;
        newN->rec = rec;
        newN->pLeft = newN->pRight = nullptr;

        if (pRoot == nullptr) {
            pRoot = newN;
        }
        else if (rec.key < pCurr->rec.key) {
            pCurr->pLeft = newN;
        }
        else {
            pCurr->pRight = newN;
        }

        this->DataCount++;
        this->Eff++;
    }

    void Delete(TKey key) {
        if (!Find(key)) return;

        TreeNode<TKey, TVal>* pDel = pCurr;

        if (pCurr->pLeft == nullptr && pCurr->pRight == nullptr) {  // no children
            if (pPrev == nullptr) pRoot = nullptr;
            else if (pPrev->pLeft == pCurr) pPrev->pLeft = nullptr;
            else pPrev->pRight = nullptr;
        }
        else if (pCurr->pLeft == nullptr || pCurr->pRight == nullptr) {  // one child
            TreeNode<TKey, TVal>* child = (pCurr->pLeft) ? pCurr->pLeft : pCurr->pRight;
            if (pPrev == nullptr) pRoot = child;
            else if (pPrev->pLeft == pCurr) pPrev->pLeft = child;
            else pPrev->pRight = child;
        }
        else {  // two children
            TreeNode<TKey, TVal>* tmp = pCurr->pLeft;
            TreeNode<TKey, TVal>* tmpPrev = pCurr;
            while (tmp->pRight != nullptr) {
                this->Eff++;
                tmpPrev = tmp;
                tmp = tmp->pRight;
            }
            pCurr->rec = tmp->rec;
            if (tmpPrev == pCurr)
                tmpPrev->pLeft = tmp->pLeft;
            else
                tmpPrev->pRight = tmp->pLeft;
            pDel = tmp;
        }

        delete pDel;
        this->DataCount--;
        this->Eff++;
    }

    void Reset() {
        while (!st.empty()) st.pop();
        pCurr = pRoot;
        while (pCurr != nullptr) {
            st.push(pCurr);
            pCurr = pCurr->pLeft;
        }
        if (!st.empty()) {
            pCurr = st.top();
        }
        pos = 0;
    }

    void GoNext() {
        if (st.empty()) return;
        TreeNode<TKey, TVal>* node = st.top(); st.pop();
        pCurr = node->pRight;
        while (pCurr != nullptr) {
            st.push(pCurr);
            pCurr = pCurr->pLeft;
        }
        if (!st.empty()) {
            pCurr = st.top();
        }
        pos++;
    }

    bool IsEnd() {
        return pos >= this->DataCount;
    }

    void PrintRec(ostream& os, TreeNode<TKey, TVal>* node, int level, bool isRight, const string& prefix) {
        if (!node) return;

        // Определяем символы для отрисовки связей
        const string horizontal = "──";
        const string vertical = "│  ";
        const string cornerRight = "┌──";
        const string cornerLeft = "└──";
        const string empty = "   ";

        // Отрисовываем правую ветвь (верхнюю часть)
        PrintRec(os, node->pRight, level + 1, true, prefix + (isRight ? empty : vertical));

        // Выводим текущий узел
        os << prefix;
        os << (isRight ? cornerRight : cornerLeft);

        // Выводим узел с информацией о балансе
        os << node->rec.key;

        os << " (" << node->rec.val << ")" << endl;

        // Отрисовываем левую ветвь (нижнюю часть)
        PrintRec(os, node->pLeft, level + 1, false, prefix + (isRight ? vertical : empty));
    }

    void Print(ostream& os) {
        if (this->pRoot == nullptr) {
            os << "Пустое дерево\n";
            return;
        }
        os << "Дерево:\n\n";

        PrintRec(os, this->pRoot, 0, false, "");
    }

    Record<TKey, TVal> getCurr() override {
        return pCurr ? pCurr->rec : Record<TKey, TVal>{};
    }

    TKey getCurrKey() override {
        return pCurr ? pCurr->rec.key : TKey{};
    }

    TVal getCurrVal() override {
        return pCurr ? pCurr->rec.val : TVal{};
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
            string val = "value_" + to_string(rand() % 1000);
            this->Insert(Record<TKey, TVal>(key, val));
        }
    }

    void clrTab() override {
        clearTree(pRoot);
        pRoot = nullptr;
        this->DataCount = 0;
        this->Eff = 0;
        while (!st.empty()) st.pop();
        pCurr = pPrev = nullptr;
        pos = 0;
    }

    void clearTree(TreeNode<TKey, TVal>* pNode) {
        if (!pNode) return;
        clearTree(pNode->pLeft);
        clearTree(pNode->pRight);
        delete pNode;
    }
};
