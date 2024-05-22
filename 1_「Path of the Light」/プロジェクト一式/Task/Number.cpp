#include "pch.h"
#include "Number.h"

using namespace DirectX;

Number::Number(
	std::unique_ptr<DirectX::SpriteBatch>* pSpriteBatch,
	ID3D11ShaderResourceView** pNumberSRV,
	uint32_t digit,
	float scale
)
	: m_pSpriteBatch(pSpriteBatch)
	, m_pNumberSRV(pNumberSRV)
	, m_digit(digit)
	, m_scale(scale)
	, m_number(0)
	, m_frame{}
{
	// �X�v���C�g�V�[�g�̍쐬
	m_sprites = std::make_unique<SpriteSheet>();
	m_sprites->Load(*m_pNumberSRV, L"Resources/Textures/number.txt");

	// �e�����̃X�v���C�g�t���[���ւ̃|�C���^���擾����
	for (size_t i = 0; i < 10; i++)
	{
		std::wostringstream oss;
		oss << "number_b_" << i;
		m_frame[i] = m_sprites->Find(oss.str().c_str());
		assert(m_frame[i] != 0);
	}
}

bool Number::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	return true;
}

void Number::Render()
{
	uint32_t number = m_number;
	SimpleMath::Vector2 pos = m_pos;

	// �����̃e�N�X�`���̕��ƍ������擾����
	float w = m_frame[0]->size.x;

	// �����̃Z���^�[������֕␳����i�����ł�texturePacker�Ή��̂��߁j
	pos.x += m_frame[0]->origin.x * m_scale;
	pos.y += m_frame[0]->origin.y * m_scale;

	// �����������\���ʒu���ړ�������
	pos.x += w * m_scale * (m_digit - 1);

	// ���ʌ����珇�Ԃɕ`�悷��
	while (number != 0)
	{
		uint32_t no = number % 10;
		number /= 10;
		m_sprites->Draw((*m_pSpriteBatch).get(), *m_frame[no], pos, Colors::White, 0.0f, m_scale);
		pos.x -= w * m_scale;
	}
}
