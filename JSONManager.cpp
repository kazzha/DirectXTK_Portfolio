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

// JSON 파일을 로드, 파싱해 rapidjson::WDocument 객체 생성. 파일이 처음 로드되는 경우만 오픈 및 파싱 수행
WDocument* JsonManager::CreateDocument(LPCWSTR filename)
{
	WDocument* pDocument = NULL;

	auto result = m_documents.insert(std::pair<std::wstring, 
		std::unique_ptr<WDocument>>(filename, nullptr)); // insert 결과 이미 있으면 false (pair: 넣은자리 이터레이터, 불값)
	if (result.second == true)
	{
		std::wstringstream stream;
		std::wifstream file(filename, std::wifstream::binary); // filename을 바이너리모드로 열다..
		if (!file)
		{
			throw std::exception("fail to load json file");
			file.close();
			return nullptr;
		}

		stream << file.rdbuf(); // file을 stream에 씀
		file.close();
		result.first->second = std::move(std::make_unique<WDocument>()); // 새로운 WDocument를 생성해 할당
		pDocument = result.first->second.get(); // move를 사용해 ptr의 소유권을 옮겨 메모리 사용량 줄임
		pDocument->Parse(stream.str().c_str()); // stream.str()은 std::wstring 반환(파일 내의 내용). RAPID JSON이 C스타일 문자열을 사용.
	}
	else
		pDocument = result.first->second.get();

	return pDocument;
}

void JsonManager::Remove(LPCWSTR filename) // 특정 파일을 맵에서 제거
{
	m_documents.erase(filename);
}

void JsonManager::ReleaseAll() // 모두 삭제
{
	m_documents.clear();
}
