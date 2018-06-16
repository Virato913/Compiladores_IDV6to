#pragma once

using namespace System;

namespace CompilerCore {
	public enum class ERROR_PHASE
	{
		LEXICO,
		SINTACTICO,
		SEMANTICO
	};

	public ref class ErrorModule
	{
	private:
		int m_iMaxErrors;
		int m_iNumErrors;
		cli::array<String^>^ m_strErrors;
	public:
		ErrorModule();
		~ErrorModule();
		void reset();
		void addError(ERROR_PHASE p, int lineNum, String^ desc, String^ line);
		int getErrorsNumber();
		int getMaxErrors();
		cli::array<String^>^ returnErrorString();
	};
}
