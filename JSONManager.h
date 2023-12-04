#pragma once

namespace rapidjson
{
	using WDocument = GenericDocument<UTF16<>>;
	using WValue = GenericValue<UTF16<>>;
}

namespace DX
{
	class JSONManager
	{
	public:
		static JSONManager& Instance()
		{
			static JsonManager instance;
			return instance;
		}

	public:
		JSONManager();
	};

}

