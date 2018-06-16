#include "stdafx.h"
#include "ErrorModule.h"

using namespace CompilerCore;

ErrorModule::ErrorModule()
{
	this->m_iMaxErrors = 30;
	this->m_iNumErrors = 0;
	this->m_strErrors = gcnew cli::array<String^>(m_iMaxErrors);
}

ErrorModule::~ErrorModule()
{

}

void ErrorModule::reset()
{
	this->m_iNumErrors = 0;
	this->m_strErrors = gcnew cli::array<String^>(m_iMaxErrors);
}

void ErrorModule::addError(ERROR_PHASE phase, int numLine, String^ sDesc, String^ sLine)
{
	m_strErrors[m_iNumErrors] = (sDesc + " found in line " + numLine + ": " + sLine);
	m_iNumErrors++;
}

int ErrorModule::getErrorsNumber()
{
	return m_iNumErrors;
}

int ErrorModule::getMaxErrors()
{
	return m_iMaxErrors;
}

cli::array<String^>^ ErrorModule::returnErrorString()
{
	return m_strErrors;
}