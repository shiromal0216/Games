//--------------------------------------------------------------------------------------
// File: TaskManager.h
//
// �^�X�N���Ǘ�����N���X
//
// Date: 2023.3.20
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include <list>
#include <set>
#include <sstream>
#include <functional>

namespace Imase
{
	class TaskManager;
	class Task;

	//////////////////////////////////////////////////////////////////////////
	//																		//
	//	�^�X�N�A�����Ǘ��N���X											//
	//																		//
	//////////////////////////////////////////////////////////////////////////
	class TaskConnectInfo
	{
	private:
		// �^�X�N�Ǘ��I�u�W�F�N�g�ւ̃|�C���^
		TaskManager* m_taskManager;

		// �e�^�X�N�ւ̃|�C���^
		Task* m_parent;

		// �q���^�X�N�̃��X�g
		std::list<Task*> m_children;

	public:
		TaskConnectInfo() : m_taskManager{}, m_parent{} {}
		TaskManager* GetTaskManager() const { return m_taskManager; }
		Task* GetParent() const { return m_parent; }
		std::list<Task*>* GetChildList() { return &m_children; }
		void SetTaskManager(TaskManager* taskManager) { m_taskManager = taskManager; }
		void SetParent(Task* task) { m_parent = task; }
		void AddChild(Task* task) { m_children.push_back(task); }
		bool DeleteChildList(Task* task);
	};

	//////////////////////////////////////////////////////////////////////////
	//																		//
	//	�^�X�N�̊��N���X													//
	//																		//
	//////////////////////////////////////////////////////////////////////////
	class Task
	{
	private:
		// �`�揇���Ǘ��p(�������قǎ�O�ɕ`�悳���j
		int m_ot;

		// �^�X�N�A�����
		TaskConnectInfo m_connect;

		// �^�X�N�̖��O
		std::string m_name;

	public:

		TaskConnectInfo& GetTaskConnectInfo() { return m_connect; }

	public:

		// �R���X�g���N�^
		Task() : m_ot(0) {}

		// �f�X�g���N�^
		virtual ~Task() {}

		//////////////////////////////////////////
		//										//
		//	�I�[�o�[���C�h���Ďg���֐�			//
		//										//
		//////////////////////////////////////////

		// �X�V�֐��ifalse��Ԃ��ƃ^�X�N��������j
		virtual bool Update(float elapsedTime)
		{
			UNREFERENCED_PARAMETER(elapsedTime);
			return true;
		}

		// �`��֐�
		virtual void Render() {}

	public:

		// �^�X�N�}�l�[�W���[�̎擾�֐�
		TaskManager* GetTaskManager() const { return m_connect.GetTaskManager(); }

		// �e�^�X�N�̎擾�֐�
		Task* GetParent() const { return m_connect.GetParent(); }

		// �`�揇�ݒ�֐��iot�̒l��0����Ԏ�O�j
		void SetOt(int ot) { this->m_ot = ot; }

		// �`�揇�̎擾�֐�
		int GetOt() { return m_ot; }

		// �w�肵���^�X�N�̎q���ɂ���ύX����֐�
		void ChangeParent(Task* parent);

		// �^�X�N�ɖ��O��t����֐�
		void SetName(const std::string& name) { m_name = name; }

		// �^�X�N�̖��O���擾����֐�
		const std::string& GetName() { return m_name; }

		// �^�X�N�X�V�֐��i�q���̃^�X�N�܂Ń^�X�N�X�V�֐����Ăяo�����j
		void UpdateTasks(std::function<void(Task*)> func)
		{
			func(this);
			std::list<Task*>* children = GetTaskConnectInfo().GetChildList();
			for (const auto it : *children)
			{
				it->UpdateTasks(func);
			}
		}

	};

	//////////////////////////////////////////////////////////////////////////
	//																		//
	//	�^�X�N�Ǘ��N���X													//
	//																		//
	//////////////////////////////////////////////////////////////////////////
	class TaskManager
	{

	private:
#pragma region
		// �`�揇���Ǘ��e�[�u���ւ̓o�^�p��r�֐��iot�̒l���傫�������擪�ցj
		struct greater_ot : std::binary_function<Task*, Task*, bool>
		{
			bool operator()(Task* a, Task* b) const
			{
				return a->GetOt() > b->GetOt();
			}
		};

		// �`�揇���Ǘ��e�[�u��
		std::multiset<Task*, greater_ot> m_ot;

		// ���[�g�^�X�N
		Task* m_rootTask;

		// ���s���̃^�X�N
		Task* m_currentTask;

		// �^�X�N�̍폜�֐�
		void DeleteTask(Task* task);

		// �q���^�X�N�̍X�V�֐������s����֐�
		void ChildTaskUpdate(Task* task, float elapsedTime);

		// �^�X�N������
		uint64_t m_totalTaskCnt;

#pragma endregion

	public:

		// �R���X�g���N�^
		TaskManager() : m_rootTask{}, m_currentTask{}, m_totalTaskCnt{}
		{
			m_currentTask = m_rootTask = new Task();
			m_rootTask->SetName("RootTask");
		}

		// �f�X�g���N�^
		virtual ~TaskManager()
		{
			DeleteTask(m_rootTask);
		}

		// �X�V�֐�
		virtual void Update(float elapsedTime);

		// �`��֐�
		virtual void Render();

		// �^�X�N�̐����֐�
		template <class T, class... Args>
		T* AddTask(Args&&... args);

		// ���[�g�^�X�N���擾����֐�
		Task* GetRootTask() { return m_rootTask; }

	};

#pragma region
	// �^�X�N�̐����֐��i���s���̃^�X�N�̎q���ɂȂ�j
	template <class T, class... Args>
	T* TaskManager::AddTask(Args&&... args)
	{
		// �^�X�N�𐶐�����
		T* task = new T(std::forward<Args>(args)...);

		// �^�X�N�Ǘ��I�u�W�F�N�g��ݒ�
		task->GetTaskConnectInfo().SetTaskManager(this);

		// �e��ݒ�
		task->GetTaskConnectInfo().SetParent(m_currentTask);

		// �e�̎q�����X�g�ɒǉ�
		m_currentTask->GetTaskConnectInfo().AddChild(task);

		// �^�X�N�ɖ��O���t���Ă��Ȃ��ꍇ�͉��Ŗ��O��t����
		if (task->GetName().empty())
		{
			std::ostringstream ostr;
			ostr << "Task_" << m_totalTaskCnt;
			task->SetName(ostr.str());
		}

		// �^�X�N�����������Z
		m_totalTaskCnt++;

		return task;
	}
#pragma endregion

}

