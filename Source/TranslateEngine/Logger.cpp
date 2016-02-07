#include "PrecompiledHeaders\stdafx.h"
#include "TranslateEngine\Logger.h"

vector<LogRecord> Logger::records;
const wchar_t* Logger::logFileName = L"log.json";
bool Logger::isInitialized = false;

void Logger::AddRecord(string word)
{
	Initialize();

	transform(word.begin(), word.end(), word.begin(), ::tolower);

	for (size_t i = 0; i < records.size(); ++i) 
	{
		if (records[i].Word == word) 
		{
			records[i].Count++;
			Flush();
			return;
		}
	}

	LogRecord newRecord;
	newRecord.Word = word;
	newRecord.Count = 1;
	records.push_back(newRecord);

	Flush();
}

void Logger::RemoveRecord(string word)
{
}

vector<LogRecord> Logger::GetRecords()
{
	Initialize();
	return records;
}

void Logger::Flush()
{
	WriteRecords();
}

void Logger::Initialize()
{
	if (isInitialized) 
	{
		return;
	}

	ReadRecords();
}

void Logger::ReadRecords()
{
	HANDLE hFile = CreateFile(logFileName, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) 
	{
		return;
	}

	const int bufferSize = 65536;
	static char buffer[bufferSize];
	string json = "";

	DWORD nWritten;
	do 
	{
		ReadFile(hFile, buffer, bufferSize, &nWritten, NULL);
		json += string(buffer);
	} 
	while (nWritten == bufferSize);
	
	CloseHandle(hFile);

	Json::Value root;
	Json::Reader reader;

	records.resize(0);

	if (json.empty()) {
		return;
	}

	bool parsingSuccessful = reader.parse(json, root);
	if (!parsingSuccessful)
	{	
		return;
	}

	for (size_t i = 0; i < root.size(); ++i)
	{
		LogRecord record;
		record.Word = root[i].get("word", "").asString();
		record.Count = root[i].get("count", "").asInt();

		records.push_back(record);
	}
}

void Logger::WriteRecords()
{
	Json::Value root;

	for (size_t i = 0; i < Logger::records.size(); ++i)
	{
		Json::Value record;
		record["word"] = Logger::records[i].Word;
		record["count"] = Logger::records[i].Count;

		root.append(record);
	}

	string json = root.toStyledString();

	HANDLE hFile = CreateFile(logFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	DWORD nWritten;
	const char* buffer = json.c_str();
	WriteFile(hFile, buffer, strlen(buffer), &nWritten, NULL);

	CloseHandle(hFile);
}
