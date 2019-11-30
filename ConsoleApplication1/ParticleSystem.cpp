#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Sprite* sprite, Vector3 position, DrawShape drawShape, EmitterShapeType emitterShapeType, DemoType type, int id, bool enabled)
{
	m_position = position;
	m_id = id;
	m_enabled = enabled;
	particleData.m_sprite = sprite;
	particleData.m_drawShape = drawShape;
	psData.emitterShapeType = emitterShapeType;
	m_defaultPosition = position;
	m_demoType = type;
}

ParticleSystem::~ParticleSystem()
{
	std::list<ParticleObject*>::iterator poIter = m_particleList.begin();
	while (!m_particleList.empty())
	{
		delete m_particleList.back();
		m_particleList.pop_back();
	}
	m_particleList.clear();

	while (!m_affectorList.empty())
	{
		delete m_affectorList.back();
		m_affectorList.pop_back();
	}
	m_affectorList.clear();
}

void ParticleSystem::Update(void)
{
	int particleNumCount = 0;
	UpdateEmitterShape();

	particleData.emissionData.emisionCount += particleData.emissionData.emissionRate;
	while (particleData.emissionData.emisionCount > particleData.emissionData.emmisionLimit)
	{
		while (particleNumCount < particleData.particleNum)
		{
			ParticleObject* newParticle = new ParticleObject(particleData.m_sprite);
			Vector3 pos = Vector3(0.0f, 0.0f, 0.0f);
			if (psData.emitterShape == NULL)
			{
				pos = m_position;
				checkRadius = 30.0f;
			}
			else
			{
				pos = psData.emitterShape->GetParticleEmissionPos(&m_position);
				newParticle->SetRotation(psData.emitterShape->GetAngle());

				if (psData.emitterShape->type == EmitterShapeType::NONE || psData.emitterShape->type == EmitterShapeType::POINT)
				{
					checkRadius = 30.0f;
				}
				else
				{
					if (psData.emitterShape->GetRadius() > 30.0f)
					{
						checkRadius = psData.emitterShape->GetRadius();
					}
					else
					{
						checkRadius = 30.0f;
					}
				}
			}
			newParticle->position = pos;
			newParticle->life = particleData.lifetime;
			newParticle->maxLife = particleData.lifetime;

			if (psData.motionType == MotionType::RANDOM)
			{
				newParticle->SetRandomVelocity();
			}
			else if (psData.motionType == MotionType::SPIRAL)
			{
				int numOfArms = psData.spiralData.numOfArms;
				float angleBetweenArms = psData.spiralData.angleBetweenArms;
				bool autoAngle = psData.spiralData.autoAngleBetweenArms;
				float stepSpeed = psData.spiralData.stepSpeed;
				newParticle->SetSpiralMotion(numOfArms, angleBetweenArms, autoAngle);
			}
			newParticle->maxSpeed = 1.0f;
			m_particleList.push_back(newParticle);
			particleNumCount++;
		}
		particleData.emissionData.emisionCount = 0.0f;
	}

	if (psData.newGravity->m_doEachUpdate == true)
	{
		psData.newGravity->UpdateAffector();
	}

	//Update 
	std::list<ParticleObject*>::iterator poIter = m_particleList.begin();
	while (poIter != m_particleList.end())
	{
		ParticleObject* po = *poIter;
		if (po->GetPos().x < 0 || po->GetPos().x > windowSize.windowWidth || po->GetPos().y < 0 || po->GetPos().y > windowSize.windowHeight)
		{
			poIter = m_particleList.erase(poIter);
			delete po;
		}
		else if (po->life <= 0)
		{
			poIter = m_particleList.erase(poIter);
			delete po;
		}
		else
		{
			//Traversing each particle affector
			std::list<ParticleAffector*>::iterator paIter = m_affectorList.begin();
			while (paIter != m_affectorList.end())
			{
				ParticleAffector* pa = *paIter;
				if (pa->m_enabled == true)
				{
					pa->AffectParticleUpdate(po);
				}
				paIter++;
			}

			po->Update();
			poIter++;
		}
	}
}

void ParticleSystem::Draw(void)
{
	if (psData.DrawReverse == false)
	{
		std::list<ParticleObject*>::iterator poIter = m_particleList.begin();
		while (poIter != m_particleList.end())
		{
			ParticleObject* po = *poIter;
			po->SetBlendMode(particleData.m_blendMode);

			DrawShape shape = particleData.m_drawShape;
			if (shape == DrawShape::RANDOM)
			{
				int randShapeNum = rand() % 5;
				shape = (DrawShape)randShapeNum;
			}

			po->SetShape(shape);
			po->Draw();
			poIter++;
		}
	}
	else if (psData.DrawReverse == true)
	{
		std::list<ParticleObject*>::reverse_iterator poIter = m_particleList.rbegin();
		while (poIter != m_particleList.rend())
		{
			ParticleObject* po = *poIter;
			po->SetBlendMode(particleData.m_blendMode);

			DrawShape shape = particleData.m_drawShape;
			if (shape == DrawShape::RANDOM)
			{
				int randShapeNum = rand() % 5;
				shape = (DrawShape)randShapeNum;
			}

			po->SetShape(shape);
			po->Draw();
			poIter++;
		}
	}
}

void ParticleSystem::InitializeSystem()
{
	InitializeEmitterShape();
	InitializeParticleAffector();
	UpdateEmitterShape();
	RunDemo(m_demoType);
}

void ParticleSystem::InitializeEmitterShape()
{
	psData.m_squareEmitter = SquareEmitter(10.0f);
	psData.m_circleEmitter = CircleEmitter(30.0f);
	psData.m_pointEmiter = PointEmitter();
	psData.m_ringEmitter = RingEmitter(100.0f, 105.0f);
}

void ParticleSystem::InitializeParticleAffector()
{
	psData.newColor = new ColorAffector(Color::Red, Color::Blue, true, true);
	m_affectorList.push_back(psData.newColor);

	float startScale = 10.0f;
	float endScale = 0.0f;
	psData.newScale = new ScaleAffector(startScale, endScale, true, true);
	m_affectorList.push_back(psData.newScale);

	float startMaxSpeed = 10.0f;
	float endMaxSpeed = 1.0f;
	psData.newSpeed = new SpeedAffector(startMaxSpeed, endMaxSpeed, true, true);
	m_affectorList.push_back(psData.newSpeed);

	psData.newGravity = new GravityAffector(Vector3(0.0f, 0.2f, 0.2f), true);
	m_affectorList.push_back(psData.newGravity);

	float rotSpeed = 0.01f;
	psData.newRotation = new RotateAffector(true, rotSpeed, false, true, false);
	m_affectorList.push_back(psData.newRotation);
}

void ParticleSystem::FireSparks()
{
	psData.emitterShapeType = EmitterShapeType::SQUARE;
	psData.m_squareEmitter.SetRadius(1000.0f);

	psData.newGravity->m_enabled = false;
	psData.newGravity->m_acceleration = Vector3(0.0f, 0.2f, 0.2f);
	psData.newGravity->m_useSine = false;
	psData.newGravity->m_useSineX = false;
	psData.newGravity->m_useSineY = false;
	psData.newGravity->m_doEachUpdate = true;
	psData.newGravity->xSineSpeed = 0.0f;
	psData.newGravity->ySineSpeed = 0.0f;

	psData.newRotation->m_enabled = true;
	psData.newRotation->m_DoRotation = true;
	psData.newRotation->m_rotateLeft = false;
	psData.newRotation->m_speed = 10.0f;
	psData.newRotation->m_customRotation = false;
	psData.newRotation->m_useSine = false;

	psData.newSpeed->m_enabled = true;
	psData.newSpeed->m_startSpeed = 10.0f;
	psData.newSpeed->m_endSpeed = 1.0f;
	psData.newSpeed->m_lerpSpeed = true;

	psData.newScale->m_enabled = true;
	psData.newScale->m_lerpScale = true;
	psData.newScale->m_startScale = 10.0f;
	psData.newScale->m_endScale = 0.0f;

	psData.newColor->m_enabled = true;
	psData.newColor->m_lerpColor = true;
	psData.newColor->m_StartColor = Color::Red;
	psData.newColor->m_EndColor = Color::Blue;

	psData.motionType = MotionType::SPIRAL;
	psData.spiralData.numOfArms = 4;
	psData.spiralData.autoAngleBetweenArms = true;
	psData.DrawReverse = true;

	*appblendMode = enumBlend::ADDITIVE_ALPHA;

	particleData.emissionData.emissionRate = 1.0f;
	particleData.emissionData.emmisionLimit = 1.0f;

	particleData.lifetime = 60.0f;
	particleData.particleNum = 20;

	particleData.m_drawShape = DrawShape::STRIPES;
}

void ParticleSystem::Spiral()
{
	psData.emitterShapeType = EmitterShapeType::SQUARE;
	psData.m_squareEmitter.SetRadius(10.0f);

	psData.newGravity->m_enabled = false;
	psData.newGravity->m_acceleration = Vector3(0.0f, 0.2f, 0.2f);
	psData.newGravity->m_useSine = false;
	psData.newGravity->m_useSineX = false;
	psData.newGravity->m_useSineY = false;
	psData.newGravity->m_doEachUpdate = true;
	psData.newGravity->xSineSpeed = 0.0f;
	psData.newGravity->ySineSpeed = 0.0f;

	psData.newRotation->m_enabled = true;
	psData.newRotation->m_DoRotation = true;
	psData.newRotation->m_rotateLeft = false;
	psData.newRotation->m_speed = 10.0f;
	psData.newRotation->m_customRotation = false;
	psData.newRotation->m_useSine = false;

	psData.newSpeed->m_enabled = true;
	psData.newSpeed->m_startSpeed = 10.0f;
	psData.newSpeed->m_endSpeed = 1.0f;
	psData.newSpeed->m_lerpSpeed = true;

	psData.newScale->m_enabled = true;
	psData.newScale->m_lerpScale = true;
	psData.newScale->m_startScale = 10.0f;
	psData.newScale->m_endScale = 0.0f;

	psData.newColor->m_enabled = true;
	psData.newColor->m_lerpColor = true;
	psData.newColor->m_StartColor = Color::Red;
	psData.newColor->m_EndColor = Color::Blue;

	psData.motionType = MotionType::SPIRAL;
	psData.spiralData.numOfArms = 4;
	psData.spiralData.autoAngleBetweenArms = true;
	psData.DrawReverse = true;

	*appblendMode = enumBlend::ADDITIVE_ALPHA;

	particleData.emissionData.emissionRate = 1.0f;
	particleData.emissionData.emmisionLimit = 1.0f;

	particleData.lifetime = 60.0f;
	particleData.particleNum = 20;

	particleData.m_drawShape = DrawShape::STRIPES;
}

void ParticleSystem::FireWave()
{
	psData.emitterShapeType = EmitterShapeType::POINT;

	psData.newGravity->m_enabled = true;
	psData.newGravity->m_acceleration = Vector3(0.0f, 1.0f, 0.0f);
	psData.newGravity->m_useSine = true;
	psData.newGravity->m_useSineX = true;
	psData.newGravity->m_useSineY = false;
	psData.newGravity->m_doEachUpdate = true;
	psData.newGravity->xSineSpeed = 1.0f;
	psData.newGravity->ySineSpeed = 0.0f;

	psData.newRotation->m_enabled = true;
	psData.newRotation->m_DoRotation = true;
	psData.newRotation->m_rotateLeft = false;
	psData.newRotation->m_speed = 10.0f;
	psData.newRotation->m_customRotation = false;
	psData.newRotation->m_useSine = false;

	psData.newSpeed->m_enabled = true;
	psData.newSpeed->m_startSpeed = 10.0f;
	psData.newSpeed->m_endSpeed = 1.0f;
	psData.newSpeed->m_lerpSpeed = true;

	psData.newScale->m_enabled = true;
	psData.newScale->m_lerpScale = true;
	psData.newScale->m_startScale = 10.0f;
	psData.newScale->m_endScale = 0.0f;

	psData.newColor->m_enabled = true;
	psData.newColor->m_lerpColor = true;
	psData.newColor->m_StartColor = Color::Red;
	psData.newColor->m_EndColor = Color::Blue;

	psData.motionType = MotionType::RANDOM;
	psData.DrawReverse = true;

	*appblendMode = enumBlend::ADDITIVE_ALPHA;

	particleData.emissionData.emissionRate = 1.0f;
	particleData.emissionData.emmisionLimit = 1.0f;

	particleData.lifetime = 60.0f;
	particleData.particleNum = 10;

	particleData.m_drawShape = DrawShape::STRIPES;
}

void ParticleSystem::Fire()
{
	psData.emitterShapeType = EmitterShapeType::POINT;

	psData.newGravity->m_enabled = true;
	psData.newGravity->m_acceleration = Vector3(0.0f, 1.0f, 0.0f);
	psData.newGravity->m_useSine = false;
	psData.newGravity->m_useSineX = false;
	psData.newGravity->m_useSineY = false;
	psData.newGravity->m_doEachUpdate = true;
	psData.newGravity->xSineSpeed = 0.0f;
	psData.newGravity->ySineSpeed = 0.0f;

	psData.newRotation->m_enabled = true;
	psData.newRotation->m_DoRotation = true;
	psData.newRotation->m_rotateLeft = false;
	psData.newRotation->m_speed = 10.0f;
	psData.newRotation->m_customRotation = false;
	psData.newRotation->m_useSine = false;

	psData.newSpeed->m_enabled = true;
	psData.newSpeed->m_startSpeed = 10.0f;
	psData.newSpeed->m_endSpeed = 1.0f;
	psData.newSpeed->m_lerpSpeed = true;

	psData.newScale->m_enabled = true;
	psData.newScale->m_lerpScale = true;
	psData.newScale->m_startScale = 10.0f;
	psData.newScale->m_endScale = 0.0f;

	psData.newColor->m_enabled = true;
	psData.newColor->m_lerpColor = true;
	psData.newColor->m_StartColor = Color::Red;
	psData.newColor->m_EndColor = Color::Blue;

	psData.motionType = MotionType::RANDOM;
	psData.DrawReverse = true;

	*appblendMode = enumBlend::ADDITIVE_ALPHA;

	particleData.emissionData.emissionRate = 1.0f;
	particleData.emissionData.emmisionLimit = 1.0f;

	particleData.lifetime = 60.0f;
	particleData.particleNum = 10;

	particleData.m_drawShape = DrawShape::STRIPES;
}

void ParticleSystem::FireFan()
{
	psData.emitterShapeType = EmitterShapeType::SQUARE;
	psData.m_squareEmitter.SetRadius(10.0f);

	psData.newGravity->m_enabled = false;
	psData.newGravity->m_acceleration = Vector3(1.0f, -1.0f, 0.0f);
	psData.newGravity->m_useSine = false;
	psData.newGravity->m_useSineX = false;
	psData.newGravity->m_useSineY = false;
	psData.newGravity->m_doEachUpdate = true;
	psData.newGravity->xSineSpeed = 0.0f;
	psData.newGravity->ySineSpeed = 0.0f;

	psData.newRotation->m_enabled = true;
	psData.newRotation->m_DoRotation = true;
	psData.newRotation->m_rotateLeft = false;
	psData.newRotation->m_speed = 10.0f;
	psData.newRotation->m_customRotation = true;
	psData.newRotation->m_useSine = false;

	psData.newSpeed->m_enabled = true;
	psData.newSpeed->m_startSpeed = 10.0f;
	psData.newSpeed->m_endSpeed = 1.0f;
	psData.newSpeed->m_lerpSpeed = true;

	psData.newScale->m_enabled = true;
	psData.newScale->m_lerpScale = true;
	psData.newScale->m_startScale = 10.0f;
	psData.newScale->m_endScale = 0.0f;


	static Color AlphaRed = Color(1.0f, 0.2f, 0.2f, 1.0f);
	static Color AlphaBlue = Color(0.2f, 0.2f, 1.0f, 1.0f);

	psData.newColor->m_enabled = true;
	psData.newColor->m_lerpColor = true;
	psData.newColor->m_StartColor = AlphaRed;
	psData.newColor->m_EndColor = AlphaBlue;

	psData.motionType = MotionType::SPIRAL;
	psData.spiralData.numOfArms = 8;
	psData.spiralData.autoAngleBetweenArms = false;
	psData.spiralData.angleBetweenArms = 30.0f;
	psData.DrawReverse = true;

	*appblendMode = enumBlend::ADDITIVE_ALPHA;

	particleData.emissionData.emissionRate = 1.0f;
	particleData.emissionData.emmisionLimit = 1.0f;

	particleData.lifetime = 60.0f;
	particleData.particleNum = 10;

	particleData.m_drawShape = DrawShape::SQUARE;
}

void ParticleSystem::FallingSquare()
{
	psData.emitterShapeType = EmitterShapeType::SQUARE;
	psData.m_squareEmitter.SetRadius(50.0f);

	psData.newGravity->m_enabled = true;
	psData.newGravity->m_acceleration = Vector3(1.0f, -1.0f, 0.0f);
	psData.newGravity->m_useSine = false;
	psData.newGravity->m_useSineX = false;
	psData.newGravity->m_useSineY = false;
	psData.newGravity->m_doEachUpdate = true;
	psData.newGravity->xSineSpeed = 0.0f;
	psData.newGravity->ySineSpeed = 0.0f;

	psData.newRotation->m_enabled = true;
	psData.newRotation->m_DoRotation = true;
	psData.newRotation->m_rotateLeft = false;
	psData.newRotation->m_speed = 10.0f;
	psData.newRotation->m_customRotation = true;
	psData.newRotation->m_useSine = false;

	psData.newSpeed->m_enabled = true;
	psData.newSpeed->m_startSpeed = 10.0f;
	psData.newSpeed->m_endSpeed = 1.0f;
	psData.newSpeed->m_lerpSpeed = true;

	psData.newScale->m_enabled = true;
	psData.newScale->m_lerpScale = true;
	psData.newScale->m_startScale = 10.0f;
	psData.newScale->m_endScale = 20.0f;

	psData.newColor->m_enabled = true;
	psData.newColor->m_lerpColor = true;
	psData.newColor->m_StartColor = Color::Red;
	psData.newColor->m_EndColor = Color::Blue;

	psData.motionType = MotionType::RANDOM;
	psData.DrawReverse = true;

	*appblendMode = enumBlend::ADDITIVE_ALPHA;

	particleData.emissionData.emissionRate = 1.0f;
	particleData.emissionData.emmisionLimit = 1.0f;

	particleData.lifetime = 60.0f;
	particleData.particleNum = 1;

	particleData.m_drawShape = DrawShape::SQUARE;
}

void ParticleSystem::Bokeh()
{
	psData.emitterShapeType = EmitterShapeType::SQUARE;
	psData.m_squareEmitter.SetRadius(100.0f);
	
	psData.newGravity->m_enabled = false;
	psData.newGravity->m_acceleration = Vector3(0.0f, 0.0f, 0.0f);
	psData.newGravity->m_useSine = false;
	psData.newGravity->m_useSineX = false;
	psData.newGravity->m_useSineY = false;
	psData.newGravity->m_doEachUpdate = true;
	psData.newGravity->xSineSpeed = 0.0f;
	psData.newGravity->ySineSpeed = 0.0f;
	
	psData.newRotation->m_enabled = true;
	psData.newRotation->m_DoRotation = true;
	psData.newRotation->m_rotateLeft = false;
	psData.newRotation->m_speed = 10.0f;
	psData.newRotation->m_customRotation = true;
	psData.newRotation->m_useSine = false;

	psData.newSpeed->m_enabled = true;
	psData.newSpeed->m_startSpeed = 2.0f;
	psData.newSpeed->m_endSpeed = 0.0f;
	psData.newSpeed->m_lerpSpeed = true;
	
	psData.newScale->m_enabled = true;
	psData.newScale->m_lerpScale = false;
	psData.newScale->m_startScale = 10.0f;
	psData.newScale->m_endScale = 20.0f;

	psData.newColor->m_enabled = true;
	psData.newColor->m_lerpColor = true;
	psData.newColor->m_StartColor = Color::Red;
	psData.newColor->m_EndColor = Color::Blue;

	psData.motionType = MotionType::RANDOM;
	psData.DrawReverse = true;

	*appblendMode = enumBlend::ADDITIVE_ALPHA;
	
	particleData.emissionData.emissionRate = 1.0f;
	particleData.emissionData.emmisionLimit = 1.0f;
	
	particleData.lifetime = 100.0f;
	particleData.particleNum = 1;

	particleData.m_drawShape = DrawShape::SQUARE;
}

void ParticleSystem::FlowerTriangle()
{
	psData.emitterShapeType = EmitterShapeType::SQUARE;
	psData.m_squareEmitter.SetRadius(50.0f);

	psData.newGravity->m_enabled = false;
	psData.newGravity->m_acceleration = Vector3(0.0f, 0.0f, 0.0f);
	psData.newGravity->m_useSine = false;
	psData.newGravity->m_useSineX = false;
	psData.newGravity->m_useSineY = false;
	psData.newGravity->m_doEachUpdate = true;
	psData.newGravity->xSineSpeed = 0.0f;
	psData.newGravity->ySineSpeed = 0.0f;

	psData.newRotation->m_enabled = true;
	psData.newRotation->m_DoRotation = true;
	psData.newRotation->m_rotateLeft = false;
	psData.newRotation->m_speed = 10.0f;
	psData.newRotation->m_customRotation = true;
	psData.newRotation->m_useSine = false;

	psData.newSpeed->m_enabled = true;
	psData.newSpeed->m_startSpeed = 5.0f;
	psData.newSpeed->m_endSpeed = 1.0f;
	psData.newSpeed->m_lerpSpeed = false;

	psData.newScale->m_enabled = true;
	psData.newScale->m_lerpScale = true;
	psData.newScale->m_startScale = 10.0f;
	psData.newScale->m_endScale = 1.0f;

	psData.newColor->m_enabled = true;
	psData.newColor->m_lerpColor = true;
	psData.newColor->m_StartColor = Color::Red;
	psData.newColor->m_EndColor = Color::Blue;
	
	psData.motionType = MotionType::RANDOM;
	psData.DrawReverse = false;

	*appblendMode = enumBlend::ADDITIVE_ALPHA;

	particleData.emissionData.emissionRate = 1.0f;
	particleData.emissionData.emmisionLimit = 1.0f;
	
	particleData.lifetime = 60.0f;
	particleData.particleNum = 20;

	particleData.m_drawShape = DrawShape::TRIANGLE;
}

void ParticleSystem::FireFly()
{
	psData.emitterShapeType = EmitterShapeType::SQUARE;
	psData.m_squareEmitter.SetRadius(1000.0f);

	psData.newGravity->m_enabled = false;
	psData.newGravity->m_acceleration = Vector3(0.0f, 0.2f, 0.2f);
	psData.newGravity->m_useSine = false;
	psData.newGravity->m_useSineX = false;
	psData.newGravity->m_useSineY = false;
	psData.newGravity->m_doEachUpdate = true;
	psData.newGravity->xSineSpeed = 0.0f;
	psData.newGravity->ySineSpeed = 0.0f;

	psData.newRotation->m_enabled = true;
	psData.newRotation->m_DoRotation = true;
	psData.newRotation->m_rotateLeft = false;
	psData.newRotation->m_speed = 10.0f;
	psData.newRotation->m_customRotation = false;
	psData.newRotation->m_useSine = false;

	psData.newSpeed->m_enabled = true;
	psData.newSpeed->m_startSpeed = 4.0f;
	psData.newSpeed->m_endSpeed = 1.0f;
	psData.newSpeed->m_lerpSpeed = true;

	psData.newScale->m_enabled = true;
	psData.newScale->m_lerpScale = true;
	psData.newScale->m_startScale = 1.8f;
	psData.newScale->m_endScale = 0.0f;

	psData.newColor->m_enabled = true;
	psData.newColor->m_lerpColor = true;
	psData.newColor->m_StartColor = Color::Red;
	psData.newColor->m_EndColor = Color::Blue;

	psData.motionType = MotionType::SPIRAL;
	psData.spiralData.numOfArms = 4;
	psData.spiralData.autoAngleBetweenArms = true;
	psData.DrawReverse = true;

	*appblendMode = enumBlend::ADDITIVE_ALPHA;

	particleData.emissionData.emissionRate = 1.0f;
	particleData.emissionData.emmisionLimit = 1.0f;

	particleData.lifetime = 60.0f;
	particleData.particleNum = 20;

	particleData.m_drawShape = DrawShape::CIRCLE;
}

void ParticleSystem::UpdateEmitterShape()
{
	if (psData.emitterShapeType == EmitterShapeType::NONE)
	{
		psData.emitterShape = NULL;
	}
	else if (psData.emitterShapeType == EmitterShapeType::CIRCLE)
	{
		psData.emitterShape = &psData.m_circleEmitter;
	}
	else if (psData.emitterShapeType == EmitterShapeType::SQUARE)
	{
		psData.emitterShape = &psData.m_squareEmitter;
	}
	else if (psData.emitterShapeType == EmitterShapeType::POINT)
	{
		psData.emitterShape = &psData.m_pointEmiter;
	}
	else if (psData.emitterShapeType == EmitterShapeType::RING)
	{
		psData.emitterShape = &psData.m_ringEmitter;
	}
}

bool ParticleSystem::CheckPosition(Vector3 input)
{
	Vector3 distVector = input - m_position;
	float distance = distVector.GetMagnitude();

	if (distance < checkRadius)
	{
		return true;
	}

	return false;
}

void ParticleSystem::RunDemo(DemoType type)
{
	if (type == DemoType::FLOWER_TRIANGLE)
	{
		FlowerTriangle();
	}
	else if (type == DemoType::FALLING_SQUARE)
	{
		FallingSquare();
	}
	else if (type == DemoType::BOKEH)
	{
		Bokeh();
	}
	else if (type == DemoType::FIRE_FAN)
	{
		FireFan();
	}
	else if (type == DemoType::FIRE)
	{
		Fire();
	}
	else if (type == DemoType::SPIRAL)
	{
		Spiral();
	}
	else if (type == DemoType::FIRE_SPARKS)
	{
		FireSparks();
	}
	else if (type == DemoType::FIRE_WAVE)
	{
		FireWave();
	}
	else if (type == DemoType::FIRE_FLY)
	{
		FireFly();
	}
}
