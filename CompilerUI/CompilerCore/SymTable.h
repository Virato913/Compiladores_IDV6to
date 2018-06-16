#pragma once

#include <map>
#include <vector>

using namespace System;

namespace CompilerCore
{
	enum class SYMTABLE_CATEGORY
	{
		UNDEFINED = 0,
		GLOBAL,
		LOCAL,
		PARAMETER,
		FUNCTION
	};

	//template<typename T>
	public class Node
	{
	private:
		std::string m_strName;
		SYMTABLE_CATEGORY m_cCateg;
		std::string m_strType;
		int m_iDimen;
		//std::vector<> m_vValues;
		Node* m_cpLocalNode;

	public:
		Node()
		{
			m_cpLocalNode = nullptr;
		}
		Node(SYMTABLE_CATEGORY categ, std::string type, int dimen)
		{
			this->m_cCateg = categ;
			this->m_strType = type;
			this->m_iDimen = dimen;
		}
		~Node()
		{

		}
		void setCateg(SYMTABLE_CATEGORY categ)
		{
			this->m_cCateg = categ;
		}
		void setType(std::string type)
		{
			this->m_strType = type;
		}
		void setDimen(int dimen)
		{
			this->m_iDimen = dimen;
		}
		void setLocalNode(Node* newNode)
		{
			this->m_cpLocalNode = newNode;
		}
		SYMTABLE_CATEGORY getCateg()
		{
			return this->m_cCateg;
		}
		std::string getType()
		{
			return this->m_strType;
		}
		Node* getLocalNode()
		{
			return m_cpLocalNode;
		}
	};

	public class LocalNode final : public Node
	{
	private:
		std::string m_functionName;

	public:
		LocalNode()
		{

		}
		LocalNode(SYMTABLE_CATEGORY categ, std::string type, int dimen, std::string funcName)
		{
			this->setCateg(categ);
			this->setType(type);
			this->setDimen(dimen);
			this->m_functionName = funcName;
		}
		~LocalNode()
		{

		}
	};

	public class SymTable
	{
	private:
		std::map<std::string, Node*> m_hashTable;

	public:
		SymTable()
		{

		}

		~SymTable()
		{

		}

		void reset()
		{

		}

		bool addSymbol(std::string sym, SYMTABLE_CATEGORY cat, int dimen, std::string type, int lineNum, std::string funcName)
		{
			if (!symbolExists(sym))
			{
				if (cat == SYMTABLE_CATEGORY::GLOBAL || cat == SYMTABLE_CATEGORY::FUNCTION)
				{
					Node* pivot = new Node(cat, type, dimen);
					m_hashTable.insert(std::make_pair(sym, pivot));
				}
				else
				{
					Node* pivot = new Node();
					m_hashTable.insert(std::make_pair(sym, pivot));
					pivot = new LocalNode(cat, type, lineNum, funcName);
					(m_hashTable.find(sym)->second)->setLocalNode(pivot);
				}
				return true;
			}
			else
			{
				if (symbolExists(sym, cat))
				{
					return false;
				}
				Node* pivot = checkNodes(m_hashTable.find(sym)->second);
				pivot = pivot->getLocalNode();
				pivot = new LocalNode(cat, type, dimen, funcName);
				return true;
			}
			return false;
		}

		bool symbolExists(std::string sym)
		{
			return m_hashTable.find(sym) != m_hashTable.end();
		}

		bool symbolExists(std::string sym, SYMTABLE_CATEGORY cat)
		{
			if (m_hashTable.find(sym)!=m_hashTable.end())
			{
				return checkNodes(m_hashTable.find(sym)->second, cat);
			}
			else
			{
				return false;
			}
		}

		std::string getSymbolType(std::string sym, SYMTABLE_CATEGORY cat)
		{
			if (symbolExists(sym, cat))
			{
				return m_hashTable.find(sym)->second->getType();
			}
			else
			{
				return nullptr;
			}
		}

		bool checkNodes(Node* currentNode, SYMTABLE_CATEGORY cat)
		{
			if (currentNode->getCateg() == cat)
			{
				return true;
			}
			else
			{
				if (currentNode->getLocalNode() == nullptr)
				{
					return false;
				}
				else
				{
					return checkNodes(currentNode->getLocalNode(), cat);
				}
			}
		}

		Node* checkNodes(Node* currentNode)
		{
			if (currentNode->getLocalNode() != nullptr)
			{
				return checkNodes(currentNode->getLocalNode());
			}
			else
			{
				return currentNode;
			}
		}
	};
}