#pragma once
#include "log.h"
#include <stdio.h>
#include <windows.h>

class LogFile : public Logger
{
public:
	LogFile();
	~LogFile();

	virtual void Log(const wchar_t *type, const wchar_t *string, char*);
private:
	CRITICAL_SECTION critSect;
	FILE * fileHandle;
};
