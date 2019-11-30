#pragma once
#include "AllHeader.h"
#include "Sprite.h"
#include "Transformation.h"
#include "Vector3.h"
#include "Color.h"
#include "Rendering.h"
#include "bitmap.h"

enum class EmitterShapeType
{
	NONE,
	SQUARE,
	CIRCLE,
	POINT,
	RING,
};

class EmitterShape
{
protected:
	float m_angle;
public:
	EmitterShapeType type;
	EmitterShape() {}

	virtual void SetRadius(float radius) {}
	virtual float GetRadius() { return 0.0f; }
	virtual Vector3 GetParticleEmissionPos(Vector3* particleSystemPos) = 0;
	virtual float GetAngle() = 0;
};

class CircleEmitter : public EmitterShape
{
private:
	float m_radius;
public:
	CircleEmitter()
	{
		m_radius = 1.0f;
		type = EmitterShapeType::CIRCLE;
	}
	
	CircleEmitter(float radius)
	{
		m_radius = radius;
		type = EmitterShapeType::CIRCLE;
	}

	virtual void SetRadius(float radius) override { m_radius = radius; }
	virtual float GetRadius() override { return m_radius; }

	virtual Vector3 GetParticleEmissionPos(Vector3* particleSystemPos) override
	{
		m_angle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / TWO_PI);
		float ranRadius = -m_radius + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (m_radius * 2.0f));
		float triVelX = sin(m_angle);
		float triVelY = cos(m_angle);
		Vector3 pos = *particleSystemPos + Vector3(ranRadius * triVelX, ranRadius * triVelY, 0.0f);
		return pos;
	}

	virtual float GetAngle() override
	{
		return m_angle;
	}
};

class SquareEmitter : public EmitterShape
{
private:
	float m_radius;
public:
	SquareEmitter()
	{
		m_radius = 1.0f;
		type = EmitterShapeType::SQUARE;
	}
	
	SquareEmitter(float radius)
	{
		m_radius = radius;
		type = EmitterShapeType::SQUARE;
	}

	virtual void SetRadius(float radius) override { m_radius = radius; }
	virtual float GetRadius() override { return m_radius; }

	virtual Vector3 GetParticleEmissionPos(Vector3* particleSystemPos) override
	{
		float xranRadius = -m_radius + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (m_radius * 2.0f));
		float yranRadius = -m_radius + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (m_radius * 2.0f));
		Vector3 pos = *particleSystemPos + Vector3(xranRadius, yranRadius, 0.0f);

		float correction = 90.0f / 360.0f * TWO_PI;
		m_angle = correction + atan2(pos.y, pos.x);
		return pos;
	}

	virtual float GetAngle() override
	{
		return m_angle;
	}
};

class TriangleEmitter : public EmitterShape
{
private:
public:
	TriangleEmitter() {}
	
	virtual void SetRadius(float radius) override
	{
		//m_radius = radius;
		type = EmitterShapeType::CIRCLE;
	}

	virtual float GetRadius() override
	{
		//return m_radius;
	}

	virtual float GetAngle() override
	{
		return m_angle;
	}
};

class PointEmitter : public EmitterShape
{
private:
public:
	PointEmitter() { type = EmitterShapeType::POINT; }
	virtual void SetRadius(float radius) override {}
	virtual float GetRadius() override {return 0.0f;}
	virtual Vector3 GetParticleEmissionPos(Vector3* particleSystemPos) override { return *particleSystemPos; }
	virtual float GetAngle() override {return 0.0f;}
};

class RingEmitter : public EmitterShape
{
private:
	float m_innerRadius;
	float m_outerRadius;
public:
	RingEmitter() { type = EmitterShapeType::RING; }
	RingEmitter(float innerRadius, float outerRadius)
	{
		type = EmitterShapeType::RING;
		m_innerRadius = innerRadius;
		m_outerRadius = outerRadius;
	}

	virtual float GetOuterRadius() { return m_outerRadius; }
	virtual float GetInnerRadius() { return m_innerRadius; }

	virtual float GetRadius() override { return m_outerRadius; }

	virtual void SetRadius(float innerRadius, float outerRadius)
	{
		m_innerRadius = innerRadius;
		m_outerRadius = outerRadius;
	}

	virtual Vector3 GetParticleEmissionPos(Vector3* particleSystemPos) override
	{
		//Use M_Angle just for interesting cross affect
		m_angle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / TWO_PI);
		float ranRadius = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / ((m_outerRadius - m_innerRadius)));
		float triVelX = sin(m_angle);
		float triVelY = cos(m_angle);
		Vector3 angleVector = Vector3((m_innerRadius + ranRadius) * triVelX, (m_innerRadius + ranRadius) * triVelY, 0.0f);
		Vector3 pos = *particleSystemPos + angleVector;

		float correction = 90.0f / 360.0f * TWO_PI;
		//m_angle = correction + atan2(angleVector.y, angleVector.x);

		return pos;
	}

	virtual float GetAngle() override
	{
		return m_angle;
	}
};