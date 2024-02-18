#pragma once

namespace rapidjson
{
	using WDocument = GenericDocument<UTF16<>>;
	using WValue = GenericValue<UTF16<>>;
}

namespace DX
{
	class JsonManager
	{
	public:
		static JsonManager& Instance()
		{
			static JsonManager instance;
			return instance;
		}

	public:
		JsonManager();
		~JsonManager();

		JsonManager(JsonManager&&) = delete;
		JsonManager& operator= (JsonManager&&) = delete;

		JsonManager(JsonManager const&) = delete;
		JsonManager& operator= (JsonManager const&) = delete;

		rapidjson::WDocument* CreateDocument(LPCWSTR filename); 
		void Remove(LPCWSTR filename);
		void ReleaseAll();

	private:
		std::map<std::wstring, std::unique_ptr<rapidjson::WDocument>> m_documents;
		
	};

}

