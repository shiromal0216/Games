//--------------------------------------------------------------------------------------
// 概　要：リソースを管理するクラス
// 作成日：2023/4/27
// 作成者：松戸浩希
//--------------------------------------------------------------------------------------
#pragma once

#include <unordered_map>
#include <vector>
#include <string>

namespace Matsudo
{
	class ResourceManager
	{
	private:

		// リソースリスト
		std::unordered_map<std::wstring, std::vector<uint8_t>> m_list;

	public:

		// データのロード
		bool LoadData(const wchar_t* name);

		// データの解放
		bool ReleaseData(const wchar_t* name);

		// データの取得
		std::vector<uint8_t> GetData(const wchar_t* name);

	};
}
