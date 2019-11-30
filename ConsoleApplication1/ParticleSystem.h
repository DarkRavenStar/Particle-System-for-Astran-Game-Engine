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
#include "ParticleData.h"

enum DemoType
{
	FIRE_WAVE,
	SPIRAL,
	FIRE_SPARKS,
	FIRE,
	FIRE_FAN,
	FALLING_SQUARE,
	BOKEH,
	FLOWER_TRIANGLE,
	FIRE_FLY,
	NO_DEMO
};

class ParticleSystem
{
private:
	bool m_enabled;
	int m_id;
	float radius = 30.0f;
	float checkRadius = 30.0f;
	enumBlend* appblendMode;
	DemoType m_demoType;
	std::list<ParticleObject*> m_particleList;
	std::list<ParticleAffector*> m_affectorList;

	ParticleSystemData psData;
	ParticleData particleData;
	WindowSize windowSize;
	Vector3 m_position;
	Vector3 m_defaultPosition;

	void UpdateEmitterShape();
	void InitializeEmitterShape();
	void InitializeParticleAffector();

	void FireSparks();
	void Spiral();
	void FireWave();
	void Fire();
	void FireFan();
	void FallingSquare();
	void Bokeh();
	void FlowerTriangle();
	void FireFly();

public:
	ParticleSystem() {}
	ParticleSystem(Sprite* sprite, Vector3 position, DrawShape drawShape, EmitterShapeType emitterShapeType, DemoType type, int id, bool enabled);
	virtual ~ParticleSystem();

	virtual void Update(void);
	void Draw(void);
	void InitializeSystem();

	void SetPos(Vector3 pos) { m_position = pos; }
	const Vector3& GetPos() { return m_position; }
	
	void SetDefaultPos(Vector3 pos) { m_defaultPosition = pos; }
	const Vector3& GetDefaultPos() { return m_defaultPosition; }
	
	const float& GetRadius() { return radius; }

	void SetWindowWidthHeight(int width, int height) { windowSize.windowWidth = width; windowSize.windowHeight = height; }
	WindowSize* GetWindowWidthHeight() { return &windowSize; }

	bool CheckPosition(Vector3 input);

	void SetID(int id) { m_id = id; }
	int& GetID() { return m_id; }

	void SetEnable(bool enable) { m_enabled = enable; }
	bool& GetEnable() { return m_enabled; }

	void SetBlendMode(enumBlend blendMode) { particleData.m_blendMode = blendMode; }

	ParticleData* GetParticleData() { return &particleData;  }
	ParticleSystemData* GetParticleSystemData() { return &psData; }

	std::list<ParticleAffector*>* GetAffectorList() { return &m_affectorList; }

	void RunDemo(DemoType type);

	void SetCurrentAppBlendMode(enumBlend* blendMode) { appblendMode = blendMode; }
	enumBlend* GetCurrentApp() { return appblendMode; }

	void SetDemoType(DemoType demotype) { m_demoType = demotype; }
	DemoType& GetDemoType() { return m_demoType; }
};