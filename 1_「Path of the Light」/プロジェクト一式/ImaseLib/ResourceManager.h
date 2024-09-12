//--------------------------------------------------------------------------------------
// �T�@�v�F���\�[�X���Ǘ�����N���X
// �쐬���F2023/4/27
// �쐬�ҁF���ˍ_��
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

		// ���\�[�X���X�g
		std::unordered_map<std::wstring, std::vector<uint8_t>> m_list;

	public:

		// �f�[�^�̃��[�h
		bool LoadData(const wchar_t* name);

		// �f�[�^�̉��
		bool ReleaseData(const wchar_t* name);

		// �f�[�^�̎擾
		std::vector<uint8_t> GetData(const wchar_t* name);

	};
}
