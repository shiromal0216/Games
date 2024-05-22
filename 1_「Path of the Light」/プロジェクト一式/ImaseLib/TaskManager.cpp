//--------------------------------------------------------------------------------------
// File: TaskManager.cpp
//
// �^�X�N���Ǘ�����N���X
//
// Date: 2023.3.20
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "TaskManager.h"

using namespace Imase;

// �q���^�X�N���X�g����w��^�X�N���폜����֐�
bool TaskConnectInfo::DeleteChildList(Task* task)
{
	for (auto it = m_children.begin(); it != m_children.end(); it++)
	{
		if ((*it) == task)
		{
			m_children.erase(it);
			return true;
		}
	}
	return false;
}

// �w�肵���^�X�N�̎q���ɂ���ύX����֐�
void Task::ChangeParent(Task* parent)
{
	// ���݂̐e�^�X�N���擾
	Task* currentParent = m_connect.GetParent();

	// �e���ς��Ȃ��ꍇ�͉������Ȃ�
	if (currentParent == parent) return;

	// ���݂̐e�^�X�N�̎q�����X�g����^�X�N���폜
	currentParent->GetTaskConnectInfo().DeleteChildList(this);

	// �e�^�X�N��ݒ�
	GetTaskConnectInfo().SetParent(parent);

	// �e�^�X�N�̎q�����X�g�ɒǉ�
	parent->GetTaskConnectInfo().AddChild(this);
}

// �X�V�֐�
void TaskManager::Update(float elapsedTime)
{
	// ���[�g�^�X�N����q���^�X�N�Ɍ������čċN�I�ɍX�V���������s����
	ChildTaskUpdate(m_rootTask, elapsedTime);

	// ���s���̃^�X�N�����[�g�^�X�N�ɐݒ�
	m_currentTask = m_rootTask;
}

// �`��֐�
void TaskManager::Render()
{
	// �`�揇���Ǘ��e�[�u���ɏ]���ă^�X�N�̕`��֐����Ăяo��
	for (auto it = m_ot.begin(); it != m_ot.end(); it++)
	{
		// ���s���̃^�X�N��ݒ�
		m_currentTask = (*it);

		(*it)->Render();
	}

	// �`�揇���Ǘ��e�[�u���N���A
	m_ot.clear();

	// ���s���̃^�X�N�����[�g�^�X�N�ɐݒ�
	m_currentTask = m_rootTask;
}

// �^�X�N�̍폜�i�q���^�X�N���S�Ď��ʁj
void TaskManager::DeleteTask(Task* task)
{
	assert(task != nullptr);

	// �q���^�X�N���ċA�ō폜���Ă���
	auto childList = task->GetTaskConnectInfo().GetChildList();
	while (childList->empty() != true)
	{
		DeleteTask(childList->back());
		childList->pop_back();
	}

	// ����������������
	delete task;
}

// �q���^�X�N�̍X�V�֐����ċN�Ăяo���Ŏ��s����֐�
void TaskManager::ChildTaskUpdate(Task* task, float elapsedTime)
{
	auto childList = task->GetTaskConnectInfo().GetChildList();
	auto it = childList->begin();
	while (it != childList->end())
	{
		// ���s���̃^�X�N��ݒ�
		m_currentTask = (*it);

		// Update�֐��̖߂�l��false�̏ꍇ�̓^�X�N���폜����
		if (!(*it)->Update(elapsedTime))
		{
			// �^�X�N�̍폜
			DeleteTask(*it);
			// �q���^�X�N���X�g����폜
			it = childList->erase(it);
			continue;
		}
		// �`�揇���Ǘ��e�[�u���ɓo�^
		m_ot.insert(*it);
		// �q���̃^�X�N�̍X�V���������s����
		ChildTaskUpdate(*it, elapsedTime);
		it++;
	}
}

