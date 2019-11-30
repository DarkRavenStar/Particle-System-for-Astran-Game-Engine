#pragma once
#include "AllHeader.h"
#include "Sprite.h"
#include "Transformation.h"
#include "Vector3.h"
#include "Color.h"
#include "Rendering.h"
#include "Rendering.h"
#include "bitmap.h"

class GameObject
{
private:
	bool m_ownSprite = false;
	Sprite* m_sprites = NULL;
	Vector3 m_position;
	float m_rotation;
	float m_scale;
	Color m_color;
	enumBlend m_blendMode;
	DrawShape m_shape;

public:
	GameObject();
	GameObject(Sprite* sprite);

	void SetPos(Vector3 pos);
	const Vector3& GetPos();

	void SetRotation(float rotate);
	const float& GetRotation();
	
	void SetScale(float scale);
	const float& GetScale();
	
	void SetSprite(Sprite* sprite);
	Sprite* GetSprite();
	
	void SetColor(Color& color);
	const Color& GetColor();
	
	void SetBlendMode(enumBlend blendMode);
	const enumBlend& GetBlendMode();
	
	void SetShape(DrawShape shape);
	const DrawShape& GetShape();
	
	void Draw();
	virtual void Update();
};