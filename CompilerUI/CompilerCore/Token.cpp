#include "stdafx.h"
#include "Token.h"

using namespace CompilerCore;

Token::Token(std::string lex, TOKEN_TYPE type, int line)
{
	this->m_strLexema = lex;
	this->m_cTokenType = type;
	this->m_iLineNumber = line;
	m_tokenMap.insert(std::make_pair(0, "UNDEFINED"));
	m_tokenMap.insert(std::make_pair(1, "ID"));
	m_tokenMap.insert(std::make_pair(2, "INT"));
	m_tokenMap.insert(std::make_pair(3, "FLOAT"));
	m_tokenMap.insert(std::make_pair(4, "STRING"));
	m_tokenMap.insert(std::make_pair(5, "KEYWORD"));
	m_tokenMap.insert(std::make_pair(6, "RELATIONAL OPERATOR"));
	m_tokenMap.insert(std::make_pair(7, "ARITHMETIC OPERATOR"));
	m_tokenMap.insert(std::make_pair(8, "LOGICAL OPERATOR"));
	m_tokenMap.insert(std::make_pair(9, "NEGATION OPERATOR"));
	m_tokenMap.insert(std::make_pair(10, "SEPARATOR"));
	m_tokenMap.insert(std::make_pair(11, "ASIGN"));
	m_tokenMap.insert(std::make_pair(12, "DIMESIONS"));
	m_tokenMap.insert(std::make_pair(13, "GROUPING"));
}

Token::~Token()
{
	
}

const std::string Token::GetLex() const
{
	return m_strLexema;
}

const std::string Token::GetType() const
{
	return m_tokenMap.find((int)m_cTokenType)->second;
}

const int Token::GetLine() const
{
	return m_iLineNumber;
}