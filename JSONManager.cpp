#include "pch.h"
#include "JSONManager.h"

using namespace rapidjson;
using namespace DX;

JsonManager::JsonManager()
{

}

JsonManager::~JsonManager()
{
	m_documents.clear();
}

WDocument* JsonManager::CreateDocument(LPCWSTR filename)
{
	WDocument* pDocument = NULL;

	auto result = m_documents.insert(std::pair<std::wstring, 
		std::unique_ptr<WDocument>>(filename, nullptr));
	if (result.second == true)
	{
		std::wstringstream stream;
		std::wifstream file(filename, std::wifstream::binary);
		if (!file)
		{
			throw std::exception("fail to load json file");
			file.close();
			return nullptr;
		}

		stream << file.rdbuf();
		file.close();
		result.first->second = std::move(std::make_unique<WDocument>());
		pDocument = result.first->second.get();
		pDocument->Parse(stream.str().c_str());
	}
	else
		pDocument = result.first->second.get();

	return pDocument;
}

void JsonManager::Remove(LPCWSTR filename)
{
	m_documents.erase(filename);
}

void JsonManager::ReleaseAll()
{
	m_documents.clear();
}
