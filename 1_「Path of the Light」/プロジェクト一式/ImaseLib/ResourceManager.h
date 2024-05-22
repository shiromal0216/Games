//--------------------------------------------------------------------------------------
// File: ResourceManager.h
//
// ���\�[�X���Ǘ�����N���X
//
// Date: 2023.4.27
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include <unordered_map>
#include <vector>
#include <string>

namespace Imase
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
