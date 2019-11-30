#include "GameObject.h"

GameObject::GameObject()
{
	m_ownSprite = true;
	m_sprites = new Sprite();
}

GameObject::GameObject(Sprite* sprite)
{
	m_sprites = sprite;
}

void GameObject::SetPos(Vector3 pos)
{
	m_position = pos;
}

const Vector3& GameObject::GetPos()
{
	return m_position;
}

void GameObject::SetRotation(float rotate)
{
	m_rotation = rotate;
}

const float& GameObject::GetRotation()
{
	return m_rotation;
}

void GameObject::SetScale(float scale)
{
	m_scale = scale;
}

const float& GameObject::GetScale()
{
	return m_scale;
}

void GameObject::SetSprite(Sprite* sprite)
{
	m_sprites = sprite;
}

Sprite* GameObject::GetSprite()
{
	return m_sprites;
}

void GameObject::SetColor(Color& color)
{
	m_color = color;
}

const Color& GameObject::GetColor()
{
	return m_color;
}

void GameObject::SetBlendMode(enumBlend blendMode)
{
	m_blendMode = blendMode;
}

const enumBlend& GameObject::GetBlendMode()
{
	return m_blendMode;
}

void GameObject::SetShape(DrawShape shape)
{
	m_shape = shape;
}

const DrawShape& GameObject::GetShape()
{
	return m_shape;
}

void GameObject::Draw()
{
	if (m_blendMode != enumBlend::NONE)
	{
		glEnable(GL_BLEND);

		if (m_blendMode == enumBlend::ADDITIVE)
		{
			//Without Alpha
			glBlendFunc(GL_ONE, GL_ONE);
		}
		else if (m_blendMode == enumBlend::ADDITIVE_ALPHA)
		{
			//With Alpha
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		}
		else if (m_blendMode == enumBlend::MULTIPLICATIVE)
		{
			//glBlendFunc(GL_DST_COLOR, GL_ONE);
			glBlendFunc(GL_DST_COLOR, GL_ZERO);
		}
		else if (m_blendMode == enumBlend::ALPHA)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}
	
	m_sprites->Draw(m_position.x, m_position.y, m_position.z, m_rotation, m_scale, m_shape, m_color);

	glDisable(GL_BLEND);
}

void GameObject::Update()
{
	
}
