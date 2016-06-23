#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Entities\LogRecord.h"

class DictionaryLogger 
{
public:
	static void AddRecord(string word);
	static void RemoveRecord(string word);
	static vector<LogRecord> GetRecords();
	static void Flush();

private:
	static vector<LogRecord> records;
	static const wchar_t* logFileName;
	static bool isInitialized;
	
	static void Initialize();
	static void ReadRecords();
	static void WriteRecords();
};