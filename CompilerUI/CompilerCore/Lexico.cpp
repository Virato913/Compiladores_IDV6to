#include "stdafx.h"
#include "Lexico.h"

using namespace CompilerCore;

Lexico::Lexico(ErrorModule^ errormodule)
{
	m_error = errormodule;
	this->iTokenIndex = -1;
	m_keywordMap.insert(std::make_pair("var", "var"));
	m_keywordMap.insert(std::make_pair("int", "int"));
	m_keywordMap.insert(std::make_pair("float", "float"));
	m_keywordMap.insert(std::make_pair("bool", "bool"));
	m_keywordMap.insert(std::make_pair("string", "string"));
	m_keywordMap.insert(std::make_pair("function", "function"));
	m_keywordMap.insert(std::make_pair("main", "main"));
	m_keywordMap.insert(std::make_pair("if", "if"));
	m_keywordMap.insert(std::make_pair("else", "else"));
	m_keywordMap.insert(std::make_pair("while", "while"));
	m_keywordMap.insert(std::make_pair("for", "for"));
	m_keywordMap.insert(std::make_pair("switch", "default"));
	m_keywordMap.insert(std::make_pair("default", "default"));
	m_keywordMap.insert(std::make_pair("return", "return"));
	m_keywordMap.insert(std::make_pair("case", "case"));
}

Lexico::~Lexico()
{

}

void Lexico::reset()
{
	this->m_error->reset();
	this->m_cLexState = LEX_STATE::START;
	this->m_tokens.clear();
	this->iTokenIndex = -1;
}

const Token* const Lexico::getNextToken()
{
	return m_tokens[++this->iTokenIndex];
}

const Token* const Lexico::peekTokenAt(int index)
{
	return m_tokens[index];
}

void Lexico::addToken(std::string lex, CompilerCore::TOKEN_TYPE type, int line)
{
	Token* p = new Token(lex, type, line);
	m_tokens.push_back(p);
}

void Lexico::addError(int lineNum, char* desc, const char* line)
{
	String^ strDesc = gcnew String(desc);
	String^ strLine = gcnew String(line);
	m_error->addError(ERROR_PHASE::LEXICO, lineNum, strDesc, strLine);
}

int Lexico::getTokenNumber()
{
	return m_tokens.size();
}

void Lexico::clearTokens()
{
	m_tokens.clear();
}

bool Lexico::parseSourceCode(const char* src)
{
	bool stringOpen = false, commentOpen = false;
	int commentLine;
	int ended = 2;
	int iCurrentLine = 1;
	const char* cCurrChar = src;
	const char* cCurrLine = src;
	int errorPosition = -1;
	char cErrorBuffer[1000];
	char comment[1000];
	*comment = NULL;
	std::string sTokenBuffer;
	m_cLexState = LEX_STATE::START;
	memset(cErrorBuffer, 0, sizeof(cErrorBuffer));
	while (ended>0 && m_error->getErrorsNumber() <= m_error->getMaxErrors())
	{
		switch (m_cLexState)
		{
		case LEX_STATE::START:
			if (isalpha(*cCurrChar) || (*cCurrChar == '_'))
			{
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_cLexState = LEX_STATE::PARSING_ID;
			}
			else if (isdigit(*cCurrChar))
			{
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_cLexState = LEX_STATE::PARSING_INT;
			}
			else if (*cCurrChar == '<' || *cCurrChar == '>' || *cCurrChar == '!' || *cCurrChar == '=')
			{
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_cLexState = LEX_STATE::PARSING_RELATIONAL;
			}
			else if (*cCurrChar == '+' || *cCurrChar == '-' || *cCurrChar == '*' || *cCurrChar == '/' || *cCurrChar == '^' || *cCurrChar == '%')
			{
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_cLexState = LEX_STATE::PARSING_ARITHMETIC;
			}
			else if (*cCurrChar == '(' || *cCurrChar == ')' || *cCurrChar == '{' || *cCurrChar == '}' || *cCurrChar == '[' || *cCurrChar == ']')
			{
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_cLexState = LEX_STATE::PARSING_GROUPING;
			}
			else if (*cCurrChar == '&' || *cCurrChar == '|')
			{
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_cLexState = LEX_STATE::PARSING_LOGICAL;
			}
			else if (*cCurrChar == 34)
			{
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				stringOpen = true;
				m_cLexState = LEX_STATE::PARSING_CHAR;
			}
			else if (*cCurrChar == ':' || *cCurrChar == ',' || *cCurrChar == ';')
			{
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_cLexState = LEX_STATE::PARSING_SEPARATOR;
			}
			else if (*cCurrChar == '.')
			{
				sTokenBuffer.clear();
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_cLexState = LEX_STATE::PARSING_FLOAT;
			}
			else if (*cCurrChar == 9)
			{
				cCurrChar++;
			}
			else if (*cCurrChar == '\n')
			{
				cCurrChar++;
				iCurrentLine++;
				cCurrLine = cCurrChar;
			}
			else if (*cCurrChar == '\r')
			{
				if (stringOpen)
				{
					errorPosition = cCurrChar - cCurrLine;
					memcpy(cErrorBuffer, cCurrLine, errorPosition);
					cErrorBuffer[errorPosition] = '\0';
					addError(iCurrentLine, "Invalid string", cErrorBuffer);
					stringOpen = false;
				}
				cCurrChar++;
				cCurrChar++;
				iCurrentLine++;
				cCurrLine = cCurrChar;
			}
			else if (*cCurrChar == 32)
			{
				cCurrChar++;
			}
			else if (*cCurrChar == '\0')
			{

			}
			else
			{
				errorPosition = cCurrChar - cCurrLine;
				memcpy(cErrorBuffer, cCurrLine, errorPosition);
				cErrorBuffer[errorPosition] = '\0';
				addError(iCurrentLine, "Invalid character", cErrorBuffer);
				cCurrChar++;
			}
			break;
		case LEX_STATE::PARSING_ID:
			if (isalpha(*cCurrChar) || (*cCurrChar == '_') || isdigit(*cCurrChar))
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
			}
			else
			{
				if (m_keywordMap.find(sTokenBuffer) != m_keywordMap.end())
				{
					addToken(sTokenBuffer, TOKEN_TYPE::KEYWORD, iCurrentLine);
				}
				else
				{
					addToken(sTokenBuffer, TOKEN_TYPE::ID, iCurrentLine);
				}
				m_cLexState = LEX_STATE::START;
			}
			break;
		case LEX_STATE::PARSING_INT:
			if (isdigit(*cCurrChar))
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
			}
			else if (*cCurrChar == '.')
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_cLexState = LEX_STATE::PARSING_FLOAT;
			}
			else
			{
				addToken(sTokenBuffer, TOKEN_TYPE::INT, iCurrentLine);
				m_cLexState = LEX_STATE::START;
			}
			break;
		case LEX_STATE::PARSING_FLOAT:
			if (isdigit(*cCurrChar))
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
			}
			else if (sTokenBuffer.back() == '.')
			{
				cCurrChar++;
				errorPosition = cCurrChar - cCurrLine;
				memcpy(cErrorBuffer, cCurrLine, errorPosition);
				cErrorBuffer[errorPosition] = '\0';
				addError(iCurrentLine, "Invalid float", cErrorBuffer);
				m_cLexState = LEX_STATE::START;
			}
			else
			{
				addToken(sTokenBuffer, TOKEN_TYPE::FLOAT, iCurrentLine);
				m_cLexState = LEX_STATE::START;
			}
			break;
		case LEX_STATE::PARSING_ARITHMETIC:
			if (*cCurrChar == '+' || *cCurrChar == '-' && sTokenBuffer.back() == *cCurrChar)
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				addToken(sTokenBuffer, TOKEN_TYPE::ARITHMETIC_OP, iCurrentLine);
				m_cLexState = LEX_STATE::START;
			}
			else if (*cCurrChar == '*' && sTokenBuffer.back() == '/')
			{
				commentOpen++;
				commentLine = iCurrentLine;
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_cLexState = LEX_STATE::PARSING_COMMENT;
			}
			else
			{
				addToken(sTokenBuffer, TOKEN_TYPE::ARITHMETIC_OP, iCurrentLine);
				m_cLexState = LEX_STATE::START;
			}
			break;
		case LEX_STATE::PARSING_RELATIONAL:
			if (*cCurrChar == '=' && (sTokenBuffer.back() == '>' || sTokenBuffer.back() == '<' || sTokenBuffer.back() == '=' || sTokenBuffer.back() == '!'))
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				addToken(sTokenBuffer, TOKEN_TYPE::RELATIONAL_OP, iCurrentLine);
				m_cLexState = LEX_STATE::START;
			}
			else if (*cCurrChar == sTokenBuffer.back())
			{
				errorPosition = cCurrChar - cCurrLine;
				memcpy(cErrorBuffer, cCurrLine, errorPosition);
				cErrorBuffer[errorPosition] = '\0';
				addError(iCurrentLine, "Invalid relational operator", cErrorBuffer);
				m_cLexState = LEX_STATE::START;
			}
			else
			{
				if (sTokenBuffer.back() == '=')
				{
					addToken(sTokenBuffer, TOKEN_TYPE::ASIGN, iCurrentLine);
				}
				else if (sTokenBuffer.back() == '!')
				{
					addToken(sTokenBuffer, TOKEN_TYPE::NEGATION_OP, iCurrentLine);
				}
				else
				{
					addToken(sTokenBuffer, TOKEN_TYPE::RELATIONAL_OP, iCurrentLine);
				}
				m_cLexState = LEX_STATE::START;
			}
			break;
		case LEX_STATE::PARSING_GROUPING:
			addToken(sTokenBuffer, TOKEN_TYPE::GROUPING, iCurrentLine);
			m_cLexState = LEX_STATE::START;
			break;
		case LEX_STATE::PARSING_LOGICAL:
			if (*cCurrChar == sTokenBuffer.back())
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				addToken(sTokenBuffer, TOKEN_TYPE::LOGICAL_OP, iCurrentLine);
				m_cLexState = LEX_STATE::START;
			}
			else
			{
				errorPosition = cCurrChar - cCurrLine;
				memcpy(cErrorBuffer, cCurrLine, errorPosition);
				cErrorBuffer[errorPosition] = '\0';
				if (sTokenBuffer.back() == '&')
				{
					addError(iCurrentLine, "Invalid logical 'AND' operator", cErrorBuffer);
				}
				else if (sTokenBuffer.back() == '|')
				{
					addError(iCurrentLine, "Invalid logical 'OR' operator", cErrorBuffer);
				}
				m_cLexState = LEX_STATE::START;
			}
			break;
		case LEX_STATE::PARSING_CHAR:
			if (*cCurrChar != 34 && *cCurrChar != '\r')
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
			}
			else if (*cCurrChar == '\r')
			{
				if (stringOpen)
				{
					errorPosition = cCurrChar - cCurrLine;
					memcpy(cErrorBuffer, cCurrLine, errorPosition);
					cErrorBuffer[errorPosition] = '\0';
					addError(iCurrentLine, "Invalid string", cErrorBuffer);
					stringOpen = false;
					cCurrChar++;
					cCurrChar++;
					iCurrentLine++;
					cCurrLine = cCurrChar;
					m_cLexState = LEX_STATE::START;
				}
			}
			else if (*cCurrChar == 34 && stringOpen == true)
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				stringOpen = false;
				addToken(sTokenBuffer, TOKEN_TYPE::STRING, iCurrentLine);
				m_cLexState = LEX_STATE::START;
				sTokenBuffer.clear();
			}
			break;
		case LEX_STATE::PARSING_COMMENT:
			if (*cCurrChar == '\n')
			{
				cCurrChar++;
				iCurrentLine++;
				if (!*comment)
				{
					int commentPos = cCurrChar - cCurrLine;
					memcpy(comment, cCurrLine, commentPos);
					comment[commentPos] = '\0';
				}
			}
			else if (*cCurrChar == '/' && sTokenBuffer.back() == '*')
			{
				commentOpen=false;
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
				m_cLexState = LEX_STATE::START;
				sTokenBuffer.clear();
				*comment = NULL;
			}
			else
			{
				sTokenBuffer.append(cCurrChar, 1);
				cCurrChar++;
			}
			break;
		case LEX_STATE::PARSING_SEPARATOR:
			if (*cCurrChar != ',' || *cCurrChar != ':' || *cCurrChar != ';' && isalpha(*cCurrChar) || isdigit(*cCurrChar))
			{
				addToken(sTokenBuffer, TOKEN_TYPE::SEPARATOR, iCurrentLine);
				m_cLexState = LEX_STATE::START;
			}
			else
			{
				errorPosition = cCurrChar - cCurrLine;
				memcpy(cErrorBuffer, cCurrLine, errorPosition);
				cErrorBuffer[errorPosition] = '\0';
				addError(iCurrentLine, "Invalid separator", cErrorBuffer);
			}
			break;
		}
		if (*cCurrChar == '\0'&&sTokenBuffer.size() > 0)
		{
			--ended;
		}
		if (*cCurrChar == '\0'&& sTokenBuffer.size() == 0)
		{
			ended = 0;
		}
	}
	if (stringOpen)
	{
		errorPosition = cCurrChar - cCurrLine;
		memcpy(cErrorBuffer, cCurrLine, errorPosition);
		cErrorBuffer[errorPosition] = '\0';
		addError(iCurrentLine, "Invalid string", cErrorBuffer);
	}
	if (commentOpen)
	{
		errorPosition = cCurrChar - cCurrLine;
		memcpy(cErrorBuffer, cCurrLine, errorPosition);
		cErrorBuffer[errorPosition] = '\0';
		addError(commentLine, "Open comment", comment);
	}
	return m_error->getErrorsNumber() == 0;
}