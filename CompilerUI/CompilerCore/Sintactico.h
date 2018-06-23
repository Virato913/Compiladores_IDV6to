#pragma once

#include "Lexico.h"
#include "ErrorModule.h"
#include "SymTable.h"

using namespace System;

namespace CompilerCore
{
	public struct SyntaxBlob
	{
		std::string strSymbol;
		int iDimension;
		std::string strVarType;
		int iLineNum;
		std::string strFunctionName;
	};

	public class SyntaxState
	{
	private:
		std::vector<SyntaxBlob> m_vBlob;

	public:
		virtual ~SyntaxState() {}
		virtual void Process(Lexico* lexico, SymTable* symbolTable, ErrorModule^ errorModule) {}
		virtual SyntaxState* Process(Lexico* lexico, int tokenIndex) = 0;
		void AddBlob(const Token* token)
		{
			SyntaxBlob newBlob;
			newBlob.strSymbol = token->GetLex();
			newBlob.iDimension = 0;
			newBlob.strVarType = token->GetType();
			newBlob.iLineNum = token->GetLine();
			newBlob.strFunctionName = "";
			m_vBlob.push_back(newBlob);
		}
		void AddToTable(SymTable* symbolTable)
		{
			for (int i = 0; i < m_vBlob.size(); i++)
			{
				SYMTABLE_CATEGORY cat;
				if (m_vBlob[i].strFunctionName == "")
				{
					cat = SYMTABLE_CATEGORY::GLOBAL;
				}
				else
				{
					cat = SYMTABLE_CATEGORY::PARAMETER;
				}
				symbolTable->addSymbol(m_vBlob[i].strSymbol, cat, m_vBlob[i].iDimension, m_vBlob[i].strVarType, m_vBlob[i].iLineNum, m_vBlob[i].strFunctionName);
			}
			m_vBlob.clear();
		}
		void AddSyntaxError(ErrorModule^ errorModule, const Token* cToken, char* desc)
		{
			String^ strDesc = gcnew String(desc);
			String^ strLine = gcnew String(cToken->GetLex().c_str());
			errorModule->addError(ERROR_PHASE::SINTACTICO, cToken->GetLine(), strDesc, strLine);
		}
	};

	public class ProgramState : public SyntaxState
	{
	public:
		virtual void Process(Lexico* lexico, SymTable* symbolTable, ErrorModule^ errorModule) {}
		virtual SyntaxState* Process(Lexico* lexico, int tokenIndex)
		{
			return nullptr;
		}
	};

	public class VarState : public SyntaxState
	{
	public:
		virtual void Process(Lexico* lexico, SymTable* symbolTable, ErrorModule^ errorModule) {}
		virtual SyntaxState* Process(Lexico* lexico, int tokenIndex)
		{
			return nullptr;
		}
	};

	public class FunctionState : public SyntaxState
	{
	public:
		virtual void Process(Lexico* lexico, SymTable* symbolTable, ErrorModule^ errorModule) {}
		virtual SyntaxState* Process(Lexico* lexico, int tokenIndex)
		{
			return nullptr;
		}
	};

	public class ParamState : public SyntaxState
	{
	public:
		virtual void Process(Lexico* lexico, SymTable* symbolTable, ErrorModule^ errorModule) {}
		virtual SyntaxState* Process(Lexico* lexico, int tokenIndex)
		{
			return nullptr;
		}
	};

	public class StartState : public SyntaxState
	{
	public:
		virtual void Process(Lexico* lexico, SymTable* symbolTable, ErrorModule^ errorModule) {}
		virtual SyntaxState* Process(Lexico* lexico, int tokenIndex)
		{
			if (lexico->peekTokenAt(tokenIndex)->GetLex() == "var")
				return new VarState();
			if (lexico->peekTokenAt(tokenIndex)->GetLex() == "program")
				return new ProgramState();
			if (lexico->peekTokenAt(tokenIndex)->GetLex() == "function")
				return new FunctionState();
			if (lexico->peekTokenAt(tokenIndex)->GetLex() == "(")
				return new ParamState();
			return nullptr;
		}
	};

	public class Sintactico
	{
	private:
		Lexico* m_cLexico;
		SymTable* m_cSymbolTable;
		msclr::gcroot<ErrorModule^> m_cErrorModule;
		int tokenIndex;

	public:
		Sintactico(Lexico* Lexico, SymTable* SymbolTable, ErrorModule^ ErrorModule)
		{
			m_cLexico = Lexico;
			m_cSymbolTable = SymbolTable;
			m_cErrorModule = ErrorModule;
			tokenIndex = 0;
		}
		void RunStates()
		{

		}
		void Clear()
		{

		}
	};
}