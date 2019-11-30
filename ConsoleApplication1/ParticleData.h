#pragma once
#include "AllHeader.h"
#include "Sprite.h"
#include "Transformation.h"
#include "Vector3.h"
#include "Color.h"
#include "Rendering.h"
#include "bitmap.h"
#include "ParticleObject.h"
#include "ParticleAffector.h"
#include "EmitterShape.h"

class WindowSize
{
public:
	int windowWidth; //done
	int windowHeight; //done
};

class EmissionData
{
public:
	float emissionRate = 1.0f; //done
	float emmisionLimit = 1.0f; //done
	float emisionCount = 0.0f;
};

class ParticleData
{
public:
	int particleNum = 20; //done
	float lifetime = 60.0f; //done

	EmissionData emissionData; //done
	Sprite* m_sprite; //done
	enumBlend m_blendMode; //done
	DrawShape m_drawShape; //done
};

enum class MotionType
{
	NONE, //done
	SPIRAL, //done
	RANDOM, //done
};

class SpiralData
{
public:
	int numOfArms = 4; //done
	float angleBetweenArms = 45.0f; //done
	bool autoAngleBetweenArms = true; //done
	float stepSpeed;
};

class ParticleSystemData
{
public:
	bool DrawReverse = false;;
	MotionType motionType = MotionType::SPIRAL; //done
	SpiralData spiralData; //done
	EmitterShape* emitterShape = NULL; //done
	EmitterShapeType emitterShapeType; //done

	SquareEmitter m_squareEmitter; //done
	CircleEmitter m_circleEmitter; //done
	PointEmitter m_pointEmiter; //done
	RingEmitter m_ringEmitter; //done

	ColorAffector* newColor;
	ScaleAffector* newScale;
	SpeedAffector* newSpeed;
	GravityAffector* newGravity;
	RotateAffector* newRotation;
};