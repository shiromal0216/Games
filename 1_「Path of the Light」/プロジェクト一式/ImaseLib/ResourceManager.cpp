//--------------------------------------------------------------------------------------
// �T�@�v�F���\�[�X���Ǘ�����N���X
// �쐬���F2023/4/27
// �쐬�ҁF���ˍ_��
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "ResourceManager.h"
#include "ReadData.h"

using namespace Matsudo;

bool ResourceManager::LoadData(const wchar_t* name)
{
    if (m_list.count(name)) return false;

    m_list.emplace(name, DX::ReadData(name));

    return true;
}

bool ResourceManager::ReleaseData(const wchar_t* name)
{
    if (!m_list.count(name)) return false;

    m_list.erase(name);

    return true;
}

std::vector<uint8_t> ResourceManager::GetData(const wchar_t* name)
{
    return m_list.at(name);
}
