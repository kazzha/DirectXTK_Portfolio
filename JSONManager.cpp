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

// JSON ������ �ε�, �Ľ��� rapidjson::WDocument ��ü ����. ������ ó�� �ε�Ǵ� ��츸 ���� �� �Ľ� ����
WDocument* JsonManager::CreateDocument(LPCWSTR filename)
{
	WDocument* pDocument = NULL;

	auto result = m_documents.insert(std::pair<std::wstring, 
		std::unique_ptr<WDocument>>(filename, nullptr)); // insert ��� �̹� ������ false (pair: �����ڸ� ���ͷ�����, �Ұ�)
	if (result.second == true)
	{
		std::wstringstream stream;
		std::wifstream file(filename, std::wifstream::binary); // filename�� ���̳ʸ����� ����..
		if (!file)
		{
			throw std::exception("fail to load json file");
			file.close();
			return nullptr;
		}

		stream << file.rdbuf(); // file�� stream�� ��
		file.close();
		result.first->second = std::move(std::make_unique<WDocument>()); // ���ο� WDocument�� ������ �Ҵ�
		pDocument = result.first->second.get(); // move�� ����� ptr�� �������� �Ű� �޸� ��뷮 ����
		pDocument->Parse(stream.str().c_str()); // stream.str()�� std::wstring ��ȯ(���� ���� ����). RAPID JSON�� C��Ÿ�� ���ڿ��� ���.
	}
	else
		pDocument = result.first->second.get();

	return pDocument;
}

void JsonManager::Remove(LPCWSTR filename) // Ư�� ������ �ʿ��� ����
{
	m_documents.erase(filename);
}

void JsonManager::ReleaseAll() // ��� ����
{
	m_documents.clear();
}
