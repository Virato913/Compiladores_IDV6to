// This is the main DLL file.

#include "stdafx.h"
#include "CompilerCore.h"

CompilerCore::Manager::Manager()
{
	m_cErrorModule = gcnew ErrorModule;
	m_cLexico = new Lexico(m_cErrorModule);
}

CompilerCore::Manager::~Manager()
{
	delete m_cLexico;
}

cli::array<String^>^ CompilerCore::Manager::compile(String^ src)
{
	m_cErrorModule->reset();
	m_cLexico->reset();

	cli::array<String^>^ compilationDetails;
	cli::array<String^>^ errorDetails = m_cErrorModule->returnErrorString();

	m_cLexico->parseSourceCode((const char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(src).ToPointer());
	compilationDetails = gcnew cli::array<String^>(m_cLexico->getTokenNumber() + m_cErrorModule->getErrorsNumber());
	for (int i = 0; i < m_cErrorModule->getErrorsNumber(); i++)
	{
		compilationDetails[i] = errorDetails[i];
	}
	//compilationDetails[m_cErrorModule->getErrorsNumber()] = "Whatevs";
	for (int i = m_cErrorModule->getErrorsNumber(); i < (m_cLexico->getTokenNumber() + m_cErrorModule->getErrorsNumber()); i++)
	{
		std::string a;
		const Token* t = m_cLexico->getNextToken();
		a = t->GetLex() + " " + t->GetType();
		compilationDetails[i] = gcnew String(a.c_str());
	}
	return compilationDetails;
}

int CompilerCore::Manager::getErrorCount()
{
	return m_cErrorModule->getErrorsNumber();
}