#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "AllHeader.h"
#include "Sprite.h"
#include "GameObject.h"
#include "Vector3.h"
#include "bitmap.h"
#include "ParticleSystem.h"
#include "GLFW/glfw3.h"


class Application
{
private:
	GLFWwindow* window;
	ParticleSystem m_particleSystem1;
	ParticleSystem m_particleSystem2;
	
	std::string path;
    Sprite m_dummySprite;
	Sprite m_dummySprite1;
public:
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;

	bool play = true;

	std::list<ParticleSystem*> m_particleSystemList;
	ParticleSystem* curPS;

	enumBlend appBlendMode;

    Application(GLFWwindow* window, int WINDOW_WIDTH, int WINDOW_HEIGHT);
    virtual ~Application();

	virtual void Start();
    virtual void Update(float deltaTime);
	virtual void UpdateInput();
	virtual void MouseInput();
    virtual void Draw();
	void SetWindowSize(int width, int height);
};


#endif // APPLICATION_H_