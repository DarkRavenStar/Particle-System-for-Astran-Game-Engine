#pragma once
#include "AllHeader.h"
#include "Sprite.h"
#include "Transformation.h"
#include "Vector3.h"
#include "Color.h"
#include "Rendering.h"
#include "ParticleObject.h"
#include "bitmap.h"

enum AffectorType
{
	GRAVITY,
	COLOR,
	ROTATION,
	SPEED,
	SCALE
};

class ParticleAffector
{
public:
	bool m_enabled = true;
	AffectorType m_type;
	ParticleAffector(){}
	virtual void AffectParticleUpdate(ParticleObject* po) = 0;
};

class ColorAffector : public ParticleAffector
{
private:
	Color m_Color;
public:
	Color m_StartColor;
	Color m_EndColor;
	bool m_lerpColor;

	ColorAffector(Color StartColor, Color EndColor, bool lerpColor, bool enabled)
	{
		m_StartColor = StartColor;
		m_EndColor = EndColor;
		m_lerpColor = lerpColor;
		m_type = AffectorType::COLOR;
		m_enabled = enabled;
	}

	// Inherited via ParticleAffector
	virtual void AffectParticleUpdate(ParticleObject* po) override
	{
		float time = 1.0f - (po->life / po->maxLife);
		//float time = po->life / po->maxLife;
		if (m_lerpColor == true)
		{
			m_Color = Color::LerpRGB(m_StartColor, m_EndColor, time);
		}
		else
		{
			m_Color = m_StartColor;
		}
		po->SetColor(m_Color);
	}
};

class RotateAffector : public ParticleAffector
{
private:
	float m_angle = 0.0f;
public:
	bool m_DoRotation;
	float m_speed = 1.0f;
	bool m_customRotation = false;
	bool m_rotateLeft = false;
	bool m_useSine = false;

	RotateAffector(bool DoRotation, float speed, bool customRotation, bool enabled, bool rotateLeft)
	{
		m_DoRotation = DoRotation;
		m_speed = speed;
		m_customRotation = customRotation;
		m_type = AffectorType::ROTATION;
		m_enabled = enabled;
		m_rotateLeft = rotateLeft;
	}
	// Inherited via ParticleAffector
	
	virtual void AffectParticleUpdate(ParticleObject* po) override
	{
		if (m_DoRotation == true)
		{
			if (m_customRotation == true)
			{
				float speed = m_speed / 1000.0f;

				if (m_rotateLeft == false)
				{
					m_angle += speed;

					if (m_angle >= 2 * PI)
					{
						m_angle = 0.0f;
					}
				}
				else if (m_rotateLeft == true)
				{
					m_angle -= speed;
					
					if (m_angle <= 0.0f)
					{
						m_angle = 2 * PI;
					}
				}

				
			}
			if (m_customRotation == false)
			{
				Vector3 angleVector = po->velocity;
				float correction = 90.0f / 360.0f * TWO_PI;
				m_angle = correction + atan2(angleVector.y, angleVector.x);
			}

			if (m_useSine == true)
			{
				po->SetRotation(sin(m_angle));
			}
			else if (m_useSine == false)
			{
				po->SetRotation(m_angle);
			}

			
		}
	}
};

class GravityAffector : public ParticleAffector
{
private:
	float x = 0.0f;
	float y = 0.0f;
	float xSine = 0.0f;
	float ySine = 0.0f;
public:
	Vector3 m_acceleration;
	bool m_useSine = false;
	bool m_useSineX = false;
	bool m_useSineY = false;
	bool m_doEachUpdate = true;
	float xSineSpeed = 0.0f;
	float ySineSpeed = 0.0f;

	GravityAffector()
	{
		m_acceleration = Vector3(0.0f, -1.0f, 0.0f);
		m_type = AffectorType::GRAVITY;
		m_enabled = true;
	}

	GravityAffector(Vector3 accel, bool enabled)
	{
		m_acceleration = accel;
		m_type = AffectorType::GRAVITY;
		m_enabled = enabled;
	}

	virtual void UpdateAffector()
	{
		if (m_useSineX == true)
		{
			xSine += xSineSpeed / 10.0f;
		}

		if (m_useSineY == true)
		{
			ySine += ySineSpeed / 10.0f;
		}
	}

	// Inherited via ParticleAffector
	virtual void AffectParticleUpdate(ParticleObject* po) override
	{
		if (m_useSine == true)
		{
			if (m_doEachUpdate == false)
			{
				UpdateAffector();
			}
			if (m_useSineX == true)
			{
				x = sin(xSine);
				m_acceleration.x = x;
			}

			if (m_useSineY == true)
			{
				y = sin(ySine);
				m_acceleration.y = y;
			}
		}
		else if (m_useSine == false)
		{
			x = 0.0f;
			y = 0.0f;
			xSine = 0.0f;
			ySine = 0.0f;
		}

		po->acceleration = m_acceleration;
	}
};

class SpeedAffector : public ParticleAffector
{
private:
	float m_speed;
public:
	float m_startSpeed;
	float m_endSpeed;
	bool m_lerpSpeed;

	SpeedAffector()
	{
		m_startSpeed = 0.0f;
		m_type = AffectorType::SPEED;
		m_enabled = true;
	}

	SpeedAffector(float startSpeed, float endSpeed, bool lerpSpeed, bool enabled)
	{
		m_startSpeed = startSpeed;
		m_endSpeed = endSpeed;
		m_lerpSpeed = lerpSpeed;
		m_type = AffectorType::SPEED;
		m_enabled = enabled;
	}

	// Inherited via ParticleAffector
	virtual void AffectParticleUpdate(ParticleObject* po) override
	{
		if (m_lerpSpeed == true)
		{
			float time = 1.0f - (po->life / po->maxLife);
			m_speed = Lerp(m_startSpeed, m_endSpeed, time);
		}
		else if(m_lerpSpeed == false)
		{
			m_speed = m_startSpeed;
		}
		
		po->maxSpeed = m_speed;
	}
};

class ScaleAffector : public ParticleAffector
{
private:
	float m_scale;
public:
	float m_startScale;
	float m_endScale;
	bool m_lerpScale;

	ScaleAffector(float startScale, float endScale, bool lerpScale, bool enabled)
	{
		m_startScale = startScale;
		m_endScale = endScale;
		m_lerpScale = lerpScale;
		m_type = AffectorType::SCALE;
		m_enabled = enabled;
	}

	// Inherited via ParticleAffector
	virtual void AffectParticleUpdate(ParticleObject* po) override
	{
		if (m_lerpScale == true)
		{
			float time = 1.0f - (po->life / po->maxLife);
			m_scale = Lerp(m_startScale, m_endScale, time);
		}
		else
		{
			m_scale = m_startScale;
		}
		po->SetScale(m_scale);
	}
};
