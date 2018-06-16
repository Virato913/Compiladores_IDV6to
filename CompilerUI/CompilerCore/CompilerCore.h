// CompilerCore.h

#pragma once

#include <Windows.h>
#include "Lexico.h"
#include "ErrorModule.h"

using namespace System;

namespace CompilerCore {

	public ref class Manager
	{
		// TODO: Add your methods for this class here.
	public:
		Manager();
		~Manager();
		cli::array<String^>^ compile(String^ src);
		int getErrorCount();
		
	private:
		Lexico* m_cLexico;
		ErrorModule^ m_cErrorModule;
	};
}
