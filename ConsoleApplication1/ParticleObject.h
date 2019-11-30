#pragma once
#include "AllHeader.h"
#include "Sprite.h"
#include "Transformation.h"
#include "Vector3.h"
#include "Color.h"
#include "Rendering.h"
#include "bitmap.h"
#include "GameObject.h"

class ParticleObject : public GameObject
{
public:
	Vector3 velocity;
	Vector3 acceleration;
	Vector3 position;
	float maxSpeed;
	float life;
	float maxLife;
	static float step;
	static float stepSpeed;

	ParticleObject(Sprite* sprite) :GameObject(sprite) {/*SetSprite(sprite);*/ }
	ParticleObject() {}

	void SetRandomVelocity()
	{
		float angle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / TWO_PI);
		float triVelX = sin(angle);
		float triVelY = cos(angle);
		velocity = Vector3(triVelX, triVelY, 0.0);
	}

	void SetSpiralMotion(int numOfArms, float angleInbetween, bool autoAngleBetweenArms)
	{
		int randAngle = (rand() % numOfArms);
		float angleSteps;
		if (autoAngleBetweenArms == true)
		{
			angleSteps = randAngle * (TWO_PI / numOfArms);
		}
		else
		{
			float angleBetween = angleInbetween / 360.0f;
			angleSteps = randAngle * (angleBetween * TWO_PI);
		}
		float angle = angleSteps + step;
		float triVelX = sin(angle);
		float triVelY = cos(angle);
		velocity = Vector3(triVelX, triVelY, 0.0);
		step += stepSpeed / 100.0f;
	}

	virtual void Update() override
	{
		velocity += acceleration;
		velocity.SetMagnitude(maxSpeed);
		position += velocity;
		acceleration = Vector3(0.0f, 0.0f, 0.0f);
		SetPos(position);

		//decrease the life of particle every update
		life--;
	}
};