#pragma once

#include <string>
#include <map>

namespace CompilerCore {
	public enum class TOKEN_TYPE
	{
		UNDEFINED = 0,
		ID,
		INT,
		FLOAT,
		STRING,
		KEYWORD,
		RELATIONAL_OP,
		ARITHMETIC_OP,
		LOGICAL_OP,
		NEGATION_OP,
		SEPARATOR,
		ASIGN,
		DIMESIONS,
		GROUPING
	};

	public class Token
	{
	private:
		std::string m_strLexema;
		TOKEN_TYPE m_cTokenType;
		int m_iLineNumber;
		std::map<int, std::string> m_tokenMap;

	public:
		Token(std::string lex, TOKEN_TYPE type, int line);
		~Token();
		const std::string GetLex() const;
		const std::string GetType() const;
	};
}