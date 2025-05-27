
#pragma once
#include "Table.h"
#include "TreeTable.h"
#include <vector>
#include <stdexcept>
using namespace std;

#define H_OK      0
#define H_INC	  1
#define H_DEC	 -1

template<typename TKey, typename TVal>
class BalTreeTable : public TreeTable <TKey, TVal> {
protected:

	struct BalanceInfo {
		bool isBalanced;
		int height;
	};

			
	int InsBalTree(TreeNode<TKey, TVal>*& pNode, Record<TKey, TVal> rec) {
		if (pNode == nullptr) {
			pNode = new TreeNode<TKey, TVal>(rec);
			pNode->bal = BalOK;
			this->DataCount++;
			return H_INC; // высота выросла
		}
		this->Eff++;
		int res = H_OK;
		if (rec.key < pNode->rec.key) {
			int tmp = InsBalTree(pNode->pLeft, rec);
			if (tmp == H_INC) {
				if (pNode->bal == BalRight) {
					pNode->bal = BalOK;
					res = H_OK;
				}
				else if (pNode->bal == BalOK) {
					pNode->bal = BalLeft;
					res = H_INC;
				}
				else if (pNode->bal == BalLeft) {
					res = BalTreeLeft(pNode);
				}
			}
			else
				res = tmp;
		}
		else if (rec.key > pNode->rec.key) {
			int tmp = InsBalTree(pNode->pRight, rec);
			if (tmp == H_INC) {
				if (pNode->bal == BalLeft) {
					pNode->bal = BalOK;
					res = H_OK;
				}
				else if (pNode->bal == BalOK) {
					pNode->bal = BalRight;
					res = H_INC;
				}
				else if (pNode->bal == BalRight) {
					res = BalTreeRight(pNode);
				}
			}
			else
				res = tmp;
		}
		else {
				res = H_OK;
		}
		return res;
	}



	int DelBalTree(TreeNode<TKey, TVal>*& pNode, TKey key)
	{
		int res = H_OK;
		this->Eff++;
		if (pNode == nullptr) return res;

		if (key < pNode->rec.key) {
			int tmp = DelBalTree(pNode->pLeft, key);
			if (tmp == H_DEC) res = BalTreeRight(pNode);
		}
		else if (key > pNode->rec.key) {
			int tmp = DelBalTree(pNode->pRight, key);
			if (tmp == H_DEC) res = BalTreeLeft(pNode);
		}
		else {
			this->DataCount--;

			if (!pNode->pLeft && !pNode->pRight) {
				delete pNode;
				pNode = nullptr;
				res = H_DEC;
				this->Eff++;
			}
			else if (!pNode->pLeft)
			{
				TreeNode<TKey, TVal>* temp = pNode;
				pNode = pNode->pRight;
				delete temp;
				res = H_DEC;
				this->Eff += 2;
			}
			else if (!pNode->pRight)
			{
				TreeNode<TKey, TVal>* temp = pNode;
				pNode = pNode->pLeft;
				delete temp;
				res = H_DEC;
				this->Eff += 2;
			}
			else
			{
				TreeNode<TKey, TVal>* minNode = FindMin(pNode->pRight);
				pNode->rec = minNode->rec;
				int tmp = RemoveMin(pNode->pRight);
				if (tmp == H_DEC)
				{
					res = BalTreeLeft(pNode);
				}
				this->Eff++;
			}
		}
		return res;
	}



	TreeNode<TKey, TVal>* FindMin(TreeNode<TKey, TVal>* pNode)  {
		if (!pNode)
		{
			return nullptr;
		}
		while (pNode->pLeft != nullptr) {
			pNode = pNode->pLeft;
			this->Eff++;
		}
		return pNode;
	}



	int RemoveMin(TreeNode<TKey, TVal>*& pNode)
	{
		int res = H_DEC;
		this->Eff++;
		if (!pNode)
		{
			return H_OK;
		}
		if (!pNode->pLeft)
		{
			TreeNode<TKey, TVal>* temp = pNode;
			pNode = pNode->pRight;
			delete temp;
			this->Eff += 2;
		}
		else
		{
			int tmp = RemoveMin(pNode->pLeft);
			if (tmp == H_DEC)
			{
				res = BalTreeRight(pNode);
			}
		}
		return res;
	}


	int BalTreeRight(TreeNode<TKey, TVal>*& pNode) {
		int res = H_OK;
		if (pNode->bal == BalLeft)
		{
			pNode->bal = BalOK;
		}
		else if (pNode->bal == BalOK)
		{
			pNode->bal = BalRight;
			res = H_INC;
		}
		else {
			TreeNode<TKey, TVal>* pR = pNode->pRight;
			if (pR->bal == BalRight)
			{
				pNode->pRight = pR->pLeft;
				pR->pLeft = pNode;
				pNode->bal = BalOK;
				pR->bal = BalOK;
				pNode = pR;
				this->Eff += 2;
			}
			else if (pR->bal == BalLeft)
			{
				TreeNode<TKey, TVal>* pL = pR->pLeft;
				pR->pLeft = pL->pRight;
				pL->pRight = pR;
				pNode->pRight = pL->pLeft;
				pL->pLeft = pNode;

				if (pL->bal == BalRight)
				{
					pNode->bal = BalLeft;
					pR->bal = BalOK;
				}
				else if (pL->bal == BalLeft)
				{
					pNode->bal = BalOK;
					pR->bal = BalRight;
				}
				else
				{
					pNode->bal = BalOK;
					pR->bal = BalOK;
				}
				pL->bal = BalOK;
				pNode = pL;
				this->Eff += 4;
			}
		}
		return res;
	}




	int BalTreeLeft(TreeNode<TKey, TVal>*& pNode) {
		int res = H_OK;
		if (pNode->bal == BalRight)
		{
			pNode->bal = BalOK;
		}
		else if (pNode->bal == BalOK)
		{
			pNode->bal = BalLeft;
			res = H_INC;
		}
		else
		{
			TreeNode<TKey, TVal>* pL = pNode->pLeft;
			if (pL->bal == BalLeft)
			{
				pNode->pLeft = pL->pRight;
				pL->pRight = pNode;
				pNode->bal = BalOK;
				pL->bal = BalOK;
				pNode = pL;
				this->Eff += 2;
			}
			else if (pL->bal == BalRight)
			{
				TreeNode<TKey, TVal>* pR = pL->pRight;
				pL->pRight = pR->pLeft;
				pR->pLeft = pL;
				pNode->pLeft = pR->pRight;
				pR->pRight = pNode;

				if (pR->bal == BalLeft)
				{
					pNode->bal = BalRight;
					pL->bal = BalOK;
				}
				else if (pR->bal == BalRight)
				{
					pNode->bal = BalOK;
					pL->bal = BalLeft;
				}
				else
				{
					pNode->bal = BalOK;
					pL->bal = BalOK;
				}
				pR->bal = BalOK;
				pNode = pR;
				this->Eff += 4;
			}
		}
		return res;
	}

	void DestroyNode(TreeNode<TKey, TVal>* pNode) {
		if (pNode == nullptr) return;
		DestroyNode(pNode->pLeft);
		DestroyNode(pNode->pRight);
		delete pNode;
	}

	void PrintNode(ostream& os, TreeNode<TKey, TVal>* p, string prefix = "", bool isLeft = true) {
		if (p == nullptr) return;

		os << prefix;
		os << (isLeft ? "|--" : "\\--");
		os << p->rec.key << "(" << p->bal << "): " << p->rec.val << endl;

		string childPrefix = prefix + (isLeft ? "|   " : "    ");

		
		PrintNode(os, p->pRight, childPrefix, false);
		
		PrintNode(os, p->pLeft, childPrefix, true);
	}

public:
	BalTreeTable() {
		this->pRoot = nullptr;
		this->DataCount = 0;
	}

	BalTreeTable(int sz)  {
		this->size = sz;
		this->pRoot = nullptr;
		this->DataCount = 0;
	}

	bool Empty() const {
		return this->pRoot == nullptr;
	}


	void Insert(Record<TKey, TVal> rec)

	{
		if (this->Find(rec.key)) {
			throw std::runtime_error("Key already exists");
		}
		InsBalTree(this->pRoot, rec);
	}


	void Delete(TKey key) 
	{
		if (Empty()) return;  
		DelBalTree(this->pRoot, key);
	}

	void fillTab(int size, int keyrange) 
	{
		if (size <= 0 || size > this->size)
			throw std::runtime_error("Invalid value for the table size");

		this->clrTab();
		srand(static_cast<unsigned>(time(nullptr)));
		std::vector<bool> usedKeys(keyrange, false);

		for (int i = 0; i < size; ++i) {
			int key;
			do {
				key = rand() % keyrange;
			} while (usedKeys[key]);

			usedKeys[key] = true;
			std::string val = "value_" + std::to_string(rand() % 1000);
			this->Insert(Record<TKey, TVal>(key, val));
		}
	}

	void PrintTree(ostream& os = cout) {
		PrintNode(os, this->pRoot);
	}

	
	~BalTreeTable() {
		DestroyNode(this->pRoot);
	}
};
