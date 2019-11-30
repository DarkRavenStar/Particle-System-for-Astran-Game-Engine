#include "Application.h"

Application::Application(GLFWwindow* win, int WIDTH, int HEIGHT)
{
	WINDOW_WIDTH = WIDTH;
	WINDOW_HEIGHT = HEIGHT;
	window = win;
}

Application::~Application()
{
	curPS = NULL;
	m_particleSystemList.clear();
}

void Application::Start()
{
	m_dummySprite = Sprite();
	m_dummySprite1 = Sprite();

	float posStepX = float(WINDOW_WIDTH) / 10.0f;
	float halfHeight = float(WINDOW_HEIGHT) / 2.0f;
	float center = (float(WINDOW_WIDTH) * 0.3f) + ((float(WINDOW_WIDTH) - (float(WINDOW_WIDTH) * 0.3f)) / 2.0f);

	Vector3 centerPosition = Vector3(center, halfHeight, 0.0f);

	Vector3 position = Vector3(center - posStepX, halfHeight, 0.0f);
	m_particleSystem1 = ParticleSystem(&m_dummySprite, position, DrawShape::STRIPES, EmitterShapeType::SQUARE, DemoType::SPIRAL, 0, true);
	m_particleSystem1.SetDefaultPos(centerPosition);
	m_particleSystem1.SetCurrentAppBlendMode(&appBlendMode);
	m_particleSystem1.InitializeSystem();
	m_particleSystemList.push_back(&m_particleSystem1);

	position = Vector3(center + posStepX, halfHeight, 0.0f);
	m_particleSystem2 = ParticleSystem(&m_dummySprite1, position, DrawShape::TRIANGLE, EmitterShapeType::CIRCLE, DemoType::FIRE_FLY, 1, false);
	m_particleSystem2.SetDefaultPos(centerPosition);
	m_particleSystem2.SetCurrentAppBlendMode(&appBlendMode);
	m_particleSystem2.InitializeSystem();
	m_particleSystemList.push_back(&m_particleSystem2);
	std::srand(std::time(NULL));
}

void Application::Update(float deltaTime)
{
	if (curPS != NULL)
	{
		if (curPS->GetEnable() == false)
		{
			curPS = NULL;
		}
	}

	std::list<ParticleSystem*>::iterator psIter = m_particleSystemList.begin();
	while (psIter != m_particleSystemList.end())
	{
		ParticleSystem* ps = *psIter;

		if (ps->GetEnable() == true)
		{
			float x = ps->GetPos().x;
			float y = ps->GetPos().y;

			float halfHeight = float(WINDOW_HEIGHT) / 2.0f;
			float halfWidth = float(WINDOW_WIDTH) / 2.0f;
			float center = (float(WINDOW_WIDTH) * 0.3f) + ((float(WINDOW_WIDTH) - (float(WINDOW_WIDTH) * 0.3f)) / 2.0f);

			if (ps->GetPos().x < WINDOW_WIDTH * 0.3 ||
				ps->GetPos().x > WINDOW_WIDTH ||
				ps->GetPos().y < 0 ||
				ps->GetPos().y > WINDOW_HEIGHT)
			{
				ps->SetPos(Vector3(center, halfHeight, 0.0f));
			}

			ps->SetWindowWidthHeight(WINDOW_WIDTH, WINDOW_HEIGHT);
			ps->Update();
		}

		psIter++;
	}
}

void Application::UpdateInput()
{
	MouseInput();
}

void Application::MouseInput()
{
	static double xpos, ypos;
	int leftMousebutton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
	glfwGetCursorPos(window, &xpos, &ypos);
	Vector3 target = Vector3(xpos, WINDOW_HEIGHT - ypos, 0.0);

	static bool targetReached = false;

	if (leftMousebutton == GLFW_PRESS)
	{
		if (targetReached == false)
		{
			if (xpos > WINDOW_WIDTH * 0.3f)
			{
				curPS = NULL;
			}

			std::list<ParticleSystem*>::iterator psIter = m_particleSystemList.begin();
			while (psIter != m_particleSystemList.end())
			{
				ParticleSystem* ps = *psIter;
				if (ps->GetEnable() == true)
				{
					targetReached = ps->CheckPosition(target);

					if (targetReached == true)
					{
						curPS = ps;
						break;
					}
				}
				psIter++;
			}

			

			if (curPS == NULL) {}
		}
	}

	if (targetReached == true && curPS != NULL)
	{
		glfwGetCursorPos(window, &xpos, &ypos);
		Vector3 pos = Vector3(xpos, WINDOW_HEIGHT - ypos, 0.0);
		curPS->SetPos(pos);
	}

	if (leftMousebutton == GLFW_RELEASE)
	{
		targetReached = false;
	}

}

void Application::Draw()
{
	if (appBlendMode == enumBlend::ALPHA)
	{
		glClearColor(0.2f, 0.2f, 0.2f, 0.5f);
	}
	if (appBlendMode == enumBlend::MULTIPLICATIVE)
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	}
	if (appBlendMode == enumBlend::NONE || appBlendMode == enumBlend::ADDITIVE || appBlendMode == enumBlend::ADDITIVE_ALPHA)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::list<ParticleSystem*>::iterator psIter = m_particleSystemList.begin();
	while (psIter != m_particleSystemList.end())
	{
		ParticleSystem* ps = *psIter;
		if (ps->GetEnable() == true)
		{
			ps->SetBlendMode(appBlendMode);
			ps->Draw();
		}
		psIter++;
	}
}

void Application::SetWindowSize(int width, int height)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
}