#pragma once

#include "windows.h"
#include "vcclr.h"
#include "ErrorModule.h"
#include "Token.h"

#include <vector>
#include <map>
#include <string>

using namespace System;

namespace CompilerCore {
	public enum class LEX_STATE
	{
		START,
		PARSING_ID,
		PARSING_INT,
		PARSING_CHAR,
		PARSING_FLOAT,
		PARSING_RELATIONAL,
		PARSING_ARITHMETIC,
		PARSING_LOGICAL,
		PARSING_SEPARATOR,
		PARSING_DIMEN,
		PARSING_GROUPING,
		PARSING_COMMENT
	};

	public class Lexico
	{
	private:
		msclr::gcroot<ErrorModule^> m_error;
		std::vector<Token*> m_tokens;
		std::map<std::string, std::string> m_keywordMap;
		int iTokenIndex;

	public:
		Lexico(ErrorModule^ errormodule);
		~Lexico();
		void reset();
		const Token* const getNextToken();
		const Token* const peekTokenAt(int index);
		void addToken(std::string lex, TOKEN_TYPE type, int line);
		void addError(int lineNum, char* desc, const char* line);
		int getTokenNumber();
		void clearTokens();
		bool parseSourceCode(const char* src);
		LEX_STATE m_cLexState;
	};
}