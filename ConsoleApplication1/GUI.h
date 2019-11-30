#pragma once
#include "AllHeader.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL2_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"
#include "nuklear_glfw_gl2.h"
#include "Sprite.h"
#include "Transformation.h"
#include "Vector3.h"
#include "Color.h"
#include "Rendering.h"
#include "bitmap.h"
#include "ParticleObject.h"
#include "ParticleAffector.h"
#include "EmitterShape.h"

class GUI
{
protected:
	struct nk_context *ctx;
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;
	ParticleSystem* selectedPS;
	Application* selectedApp;

public:
	GUI() {}
	GUI(GLFWwindow* window, int width, int height)
	{
		/* GUI */
		ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS);
		{struct nk_font_atlas *atlas;
		nk_glfw3_font_stash_begin(&atlas);
		nk_glfw3_font_stash_end(); }
		WINDOW_WIDTH = width;
		WINDOW_HEIGHT = height;
	}
	~GUI()
	{
		nk_glfw3_shutdown();
	}

	void SetWindowSize(int width, int height)
	{
		WINDOW_WIDTH = width;
		WINDOW_HEIGHT = height;
	}
	void SetCurrentParticleSystem(ParticleSystem* ps) { selectedPS = ps; }
	void SetCurrentApplication(Application* app) { selectedApp = app; }
	void DrawGUI()
	{
		nk_glfw3_new_frame();

		/* GUI */
		if (nk_begin(ctx, "Particle System", nk_rect(0, 0, WINDOW_WIDTH * 0.25, WINDOW_HEIGHT),
			NK_WINDOW_BORDER | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
		{
			const struct nk_input *in = &ctx->input;
			struct nk_rect bounds = nk_widget_bounds(ctx);
			RunPlayAppGUI(in, bounds);
			RunningEnablingParticleSystemGUI();
			RunBlendingModeGUI();

			nk_layout_row_dynamic(ctx, 1, 1);
			nk_layout_row_dynamic(ctx, 30, 1);
			nk_label(ctx, ParticleSystemName().c_str(), NK_TEXT_CENTERED);

			if (selectedPS != NULL)
			{
				static ParticleSystemData* psd = selectedPS->GetParticleSystemData();
				static ParticleData* pd = selectedPS->GetParticleData();
				RunDemo();
				RunParticleBaseProperties(psd, pd);
				RunDrawShapeGUI(psd, pd);
				RunMotionTypeGUI(psd, pd);
				RunEmitterShapeGUI(psd, pd);
				RunAffectorList();
			}

			nk_layout_row_dynamic(ctx, 30, 1);
			nk_layout_row_dynamic(ctx, 30, 1);
			nk_layout_row_dynamic(ctx, 30, 1);
			nk_layout_row_dynamic(ctx, 30, 1);
			nk_layout_row_dynamic(ctx, 30, 1);
			nk_layout_row_dynamic(ctx, 30, 1);
			nk_layout_row_dynamic(ctx, 30, 1);
			nk_layout_row_dynamic(ctx, 30, 1);
			nk_layout_row_dynamic(ctx, 30, 1);
		}
		nk_end(ctx);

		nk_glfw3_render(NK_ANTI_ALIASING_ON);
	}

private:
	static std::string Name(int id)
	{
		static std::string name = "Particle System: ";
		std::stringstream sstm;
		sstm << name << id;
		return sstm.str();
	}
	
	std::string ParticleSystemName()
	{
		static std::string name = "Particle System: ";
		std::stringstream sstm;

		if (selectedPS != NULL)
		{
			int id = selectedPS->GetID();
			sstm << name << id;
		}
		else if (selectedPS == NULL)
		{
			sstm << name;
		}

		return sstm.str();
	}

	void RunParticleBaseProperties(ParticleSystemData* psd, ParticleData* pd)
	{
		nk_layout_row_dynamic(ctx, 30, 1);
		int m_drawReverse = (int)psd->DrawReverse;
		nk_checkbox_label(ctx, "Draw Latest Particle First?", &m_drawReverse);
		psd->DrawReverse = (bool)m_drawReverse;

		nk_layout_row_dynamic(ctx, 30, 1);
		nk_property_int(ctx, "Num of Particle Per Cycle:", 0, &pd->particleNum, 100, 1, 1);

		nk_layout_row_dynamic(ctx, 30, 1);
		nk_property_float(ctx, "Particle Lifetime:", 0, &pd->lifetime, 1000.0f, 1, 1);

		nk_layout_row_dynamic(ctx, 30, 1);
		nk_property_float(ctx, "Emission Rate:", 0, &pd->emissionData.emissionRate, 100.0f, 0.1f, 0.2f);

		nk_layout_row_dynamic(ctx, 30, 1);
		nk_property_float(ctx, "Emission Limit:", 0, &pd->emissionData.emmisionLimit, 100.0f, 0.1f, 0.2f);
	}

	void RunPlayAppGUI(const struct nk_input *in, struct nk_rect bounds)
	{
		static int playInt = 0;
		playInt = (int)selectedApp->play;
		nk_layout_row_dynamic(ctx, 30, 1);
		nk_selectable_label(ctx, "PLAY / PAUSE", NK_TEXT_CENTERED, &playInt);
		selectedApp->play = (bool)playInt;

		nk_layout_row_dynamic(ctx, 1, 1);
		if (nk_tree_push(ctx, NK_TREE_NODE, "Instruction", NK_MINIMIZED))
		{
			nk_layout_row_dynamic(ctx, 60, 1);
			nk_label_wrap(ctx, "1. To edit a particle system, click on it.");
			nk_label_wrap(ctx, "2. You can pick a demo and run it but can never edit its default properties.");
			nk_label_wrap(ctx, "3. So if you click run again, the property panel will revert back to the default properties");
			nk_label_wrap(ctx, "4. You can edit any of the particle properties in the property panel but it will not be saved");
			nk_label_wrap(ctx, "5. The setup in the property panel will reset into demo data if demo is clicked to run.");
			nk_tree_pop(ctx);
		}
		nk_layout_row_dynamic(ctx, 1, 1);
	}

	void RunningEnablingParticleSystemGUI()
	{
		nk_layout_row_dynamic(ctx, 15, 1);
		nk_label(ctx, "Enable/Disable Particle System:", NK_TEXT_CENTERED);

		nk_layout_row_dynamic(ctx, 30, 2);
		std::list<ParticleSystem*>::iterator psIter = selectedApp->m_particleSystemList.begin();
		for (psIter; psIter != selectedApp->m_particleSystemList.end(); psIter++)
		{
			static int selectedParticleSystem = 0;
			ParticleSystem* ps = *psIter;
			selectedParticleSystem = (int)ps->GetEnable();
			nk_selectable_label(ctx, Name(ps->GetID()).c_str(), NK_TEXT_CENTERED, &selectedParticleSystem);
			ps->SetEnable((bool)selectedParticleSystem);
		}
	}

	void RunBlendingModeGUI()
	{
		nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 2);
		nk_layout_row_push(ctx, 0.3f);
		nk_label(ctx, "Blending:", NK_TEXT_LEFT);
		static const char* blendMode[] = { "None","Additive","Additive_Alpha","Multiplicative","Alpha" };
		static int curBlendOption;

		if (selectedApp->appBlendMode == enumBlend::NONE)
		{
			curBlendOption = 0;
		}
		else if (selectedApp->appBlendMode == enumBlend::ADDITIVE)
		{
			curBlendOption = 1;
		}
		else if (selectedApp->appBlendMode == enumBlend::ADDITIVE_ALPHA)
		{
			curBlendOption = 2;
		}
		else if (selectedApp->appBlendMode == enumBlend::MULTIPLICATIVE)
		{
			curBlendOption = 3;
		}
		else if (selectedApp->appBlendMode == enumBlend::ALPHA)
		{
			curBlendOption = 4;
		}

		nk_layout_row_push(ctx, 0.6f);
		curBlendOption = nk_combo(ctx, blendMode, NK_LEN(blendMode), curBlendOption, 25, nk_vec2(200, 200));
		selectedApp->appBlendMode = (enumBlend)curBlendOption;
		nk_layout_row_end(ctx);
	}

	void RunDrawShapeGUI(ParticleSystemData* psd, ParticleData* pd)
	{
		nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 2);
		nk_layout_row_push(ctx, 0.4f);
		nk_label(ctx, "Draw Shape:", NK_TEXT_LEFT);
		static const char* drawShapeTypeOption[] = { "Triangle","Circle","Square","Stripes","Random" };
		static int curShapeType;

		if (pd->m_drawShape == DrawShape::TRIANGLE)
		{
			curShapeType = 0;
		}
		else if (pd->m_drawShape == DrawShape::CIRCLE)
		{
			curShapeType = 1;
		}
		else if (pd->m_drawShape == DrawShape::SQUARE)
		{
			curShapeType = 2;
		}
		else if (pd->m_drawShape == DrawShape::STRIPES)
		{
			curShapeType = 3;
		}
		else if (pd->m_drawShape == DrawShape::RANDOM)
		{
			curShapeType = 4;
		}

		nk_layout_row_push(ctx, 0.6f);
		curShapeType = nk_combo(ctx, drawShapeTypeOption, NK_LEN(drawShapeTypeOption), curShapeType, 25, nk_vec2(200, 200));
		pd->m_drawShape = (DrawShape)curShapeType;
		nk_layout_row_end(ctx);
	}

	void RunEmitterShapeGUI(ParticleSystemData* psd, ParticleData* pd)
	{
		nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 2);
		nk_layout_row_push(ctx, 0.4f);
		nk_label(ctx, "Emitter Shape:", NK_TEXT_LEFT);
		static const char* emitterShapeTypeOption[] = { "None","Square","Circle","Point","Ring" };
		static int curEmitterType;

		if (psd->emitterShapeType == EmitterShapeType::NONE)
		{
			curEmitterType = 0;
		}
		else if (psd->emitterShapeType == EmitterShapeType::SQUARE)
		{
			curEmitterType = 1;
		}
		else if (psd->emitterShapeType == EmitterShapeType::CIRCLE)
		{
			curEmitterType = 2;
		}
		else if (psd->emitterShapeType == EmitterShapeType::POINT)
		{
			curEmitterType = 3;
		}
		else if (psd->emitterShapeType == EmitterShapeType::RING)
		{
			curEmitterType = 4;
		}
		nk_layout_row_push(ctx, 0.6f);
		curEmitterType = nk_combo(ctx, emitterShapeTypeOption, NK_LEN(emitterShapeTypeOption), curEmitterType, 25, nk_vec2(200, 200));
		psd->emitterShapeType = (EmitterShapeType)curEmitterType;
		nk_layout_row_end(ctx);

		if (psd->emitterShapeType == EmitterShapeType::SQUARE || psd->emitterShapeType == EmitterShapeType::CIRCLE)
		{
			if (psd->emitterShape != NULL)
			{
				nk_layout_row_dynamic(ctx, 30, 1);
				float radius = psd->emitterShape->GetRadius();
				nk_property_float(ctx, "Radius: ", 0, &radius, 1000.0f, 0.1f, 0.2f);
				psd->emitterShape->SetRadius(radius);
			}
		}
		else if (psd->emitterShapeType == EmitterShapeType::RING)
		{

			RingEmitter* ring = &psd->m_ringEmitter;

			if (ring != NULL)
			{
				float innerRadius = ring->GetInnerRadius();
				float outerRadius = ring->GetOuterRadius();
				nk_layout_row_dynamic(ctx, 30, 1);
				nk_property_float(ctx, "Inner Radius: ", 0, &innerRadius, outerRadius, 0.1f, 0.2f);
				nk_layout_row_dynamic(ctx, 30, 1);
				nk_property_float(ctx, "Outer Radius: ", innerRadius, &outerRadius, 1000.0f, 0.1f, 0.2f);
				ring->SetRadius(innerRadius, outerRadius);
			}
		}
	}

	void RunMotionTypeGUI(ParticleSystemData* psd, ParticleData* pd)
	{
		nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 2);
		nk_layout_row_push(ctx, 0.4f);
		nk_label(ctx, "MotionType:", NK_TEXT_LEFT);
		static const char* motionTypeOption[] = { "None","Spiral","Random" };
		static int curMotionType;

		if (psd->motionType == MotionType::NONE)
		{
			curMotionType = 0;
		}
		else if (psd->motionType == MotionType::SPIRAL)
		{
			curMotionType = 1;
		}
		else if (psd->motionType == MotionType::RANDOM)
		{
			curMotionType = 2;
		}

		nk_layout_row_push(ctx, 0.6f);
		curMotionType = nk_combo(ctx, motionTypeOption, NK_LEN(motionTypeOption), curMotionType, 25, nk_vec2(200, 200));
		psd->motionType = (MotionType)curMotionType;
		nk_layout_row_end(ctx);

		if (psd->motionType == MotionType::SPIRAL)
		{
			SpiralData* sd = &psd->spiralData;

			nk_layout_row_dynamic(ctx, 30, 1);
			nk_property_int(ctx, "Number Arms: ", 1, &sd->numOfArms, 10, 1, 1);

			nk_layout_row_dynamic(ctx, 30, 1);
			int autoAngle = (int)sd->autoAngleBetweenArms;
			nk_checkbox_label(ctx, "Auto Angle Between Arms", &autoAngle);
			sd->autoAngleBetweenArms = (bool)autoAngle;

			nk_property_float(ctx, "Spiral Speed: ", 0, &ParticleObject::stepSpeed, 10.0f, 0.01f, 0.02f);

			if (sd->autoAngleBetweenArms == false)
			{
				nk_layout_row_dynamic(ctx, 30, 1);
				nk_property_float(ctx, "Angle Betweeen Arms: ", 0, &sd->angleBetweenArms, 360.0f, 0.1f, 0.2f);
			}
		}
	}

	void RunAffectorList()
	{
		static std::list<ParticleAffector*>* list = selectedPS->GetAffectorList();
		nk_layout_row_dynamic(ctx, 5, 1);
		nk_layout_row_dynamic(ctx, 30, 1);
		nk_label(ctx, "Affectors", NK_TEXT_CENTERED);

		static GravityAffector* ga;
		static ColorAffector* ca;
		static SpeedAffector* sp;
		static ScaleAffector* sca;
		static RotateAffector* ra;

		static bool runOnce = false;
		if (runOnce == false)
		{
			std::list<ParticleAffector*>::iterator psIter = list->begin();
			for (psIter; psIter != list->end(); psIter++)
			{

				ParticleAffector* pa = *psIter;

				if (pa->m_type == AffectorType::GRAVITY)
				{
					ga = (GravityAffector*)pa;
				}
				else if (pa->m_type == AffectorType::COLOR)
				{
					ca = (ColorAffector*)pa;
				}
				else if (pa->m_type == AffectorType::SPEED)
				{
					sp = (SpeedAffector*)pa;
				}
				else if (pa->m_type == AffectorType::SCALE)
				{
					sca = (ScaleAffector*)pa;
				}
				else if (pa->m_type == AffectorType::ROTATION)
				{
					ra = (RotateAffector*)pa;
				}
			}
			runOnce = true;
		}

		RunGravityAffectorGUI(ga);
		RunColorAffectorGUI(ca);
		RunSpeedAffectorGUI(sp);
		RunScaleAffectorGUI(sca);
		RunRotationAffectorGUI(ra);
	}

	void RunGravityAffectorGUI(GravityAffector* ga)
	{
		if (ga != NULL)
		{
			if (nk_tree_push(ctx, NK_TREE_NODE, "Gravity", NK_MINIMIZED))
			{
				nk_layout_row_dynamic(ctx, 30, 1);
				int m_enabled = (int)ga->m_enabled;
				nk_checkbox_label(ctx, "Enable/Disable Gravity Affector", &m_enabled);
				ga->m_enabled = (bool)m_enabled;

				if (m_enabled == true)
				{
					nk_layout_row_dynamic(ctx, 30, 2);
					int m_useSine = (int)ga->m_useSine;
					nk_checkbox_label(ctx, "Use Sine", &m_useSine);
					ga->m_useSine = (bool)m_useSine;

					int m_doEachUpdate = (int)ga->m_doEachUpdate;
					nk_checkbox_label(ctx, "Run Per Update", &m_doEachUpdate);
					ga->m_doEachUpdate = (bool)m_doEachUpdate;

					if (m_useSine == true)
					{
						nk_layout_row_dynamic(ctx, 30, 1);
						int m_useSineX = (int)ga->m_useSineX;
						nk_checkbox_label(ctx, "Use SineX", &m_useSineX);
						ga->m_useSineX = (bool)m_useSineX;
						nk_property_float(ctx, "#Sine X Speed:", -10.0f, &ga->xSineSpeed, 10.0f, 0.01f, 0.02f);


						nk_layout_row_dynamic(ctx, 30, 1);
						int m_useSineY = (int)ga->m_useSineY;
						nk_checkbox_label(ctx, "Use SineX", &m_useSineY);
						ga->m_useSineY = (bool)m_useSineY;
						nk_property_float(ctx, "#Sine Y Speed:", -10.0f, &ga->ySineSpeed, 10.0f, 0.01f, 0.02f);
					}

					nk_layout_row_dynamic(ctx, 30, 1);
					nk_property_float(ctx, "#X:", -10.0f, &ga->m_acceleration.x, 10.0f, 0.01f, 0.02f);
					nk_property_float(ctx, "#Y:", -10.0f, &ga->m_acceleration.y, 10.0f, 0.01f, 0.02f);
				}

				nk_tree_pop(ctx);
			}
		}
	}

	void RunColorAffectorGUI(ColorAffector* ca)
	{
		if (ca != NULL)
		{
			if (nk_tree_push(ctx, NK_TREE_NODE, "Color", NK_MINIMIZED))
			{
				nk_layout_row_dynamic(ctx, 30, 1);
				int m_enabled = (int)ca->m_enabled;
				nk_checkbox_label(ctx, "Enable/Disable Color Affector", &m_enabled);
				ca->m_enabled = (bool)m_enabled;
				static Color AlphaRed = Color(1.0f, 0.2f, 0.2f, 1.0f);
				static Color AlphaBlue = Color(0.2f, 0.2f, 1.0f, 1.0f);

				if (m_enabled == true)
				{

					int lerpColor = (int)ca->m_lerpColor;
					nk_checkbox_label(ctx, "Lerp Color", &lerpColor);
					ca->m_lerpColor = (bool)lerpColor;

					nk_layout_row_dynamic(ctx, 30, 1);
					nk_label(ctx, "Default Color to choose from: ", NK_TEXT_LEFT);
					nk_layout_row_dynamic(ctx, 30, 2);
					if (nk_button_label(ctx, "RED"))
					{
						ca->m_StartColor = Color::Red;
					}
					if (nk_button_label(ctx, "BLUE"))
					{
						ca->m_StartColor = Color::Blue;
					}
					if (nk_button_label(ctx, "ALPHA RED"))
					{
						ca->m_StartColor = AlphaRed;
					}
					if (nk_button_label(ctx, "ALPHA BLUE"))
					{
						ca->m_StartColor = AlphaBlue;
					}

					nk_layout_row_dynamic(ctx, 30, 1);
					nk_label(ctx, "Start Color: ", NK_TEXT_LEFT);

					static struct nk_colorf color1;
					color1.r = ca->m_StartColor.r;
					color1.g = ca->m_StartColor.g;
					color1.b = ca->m_StartColor.b;
					color1.a = ca->m_StartColor.a;


					if (nk_combo_begin_color(ctx, nk_rgb_cf(color1), nk_vec2(200, 400)))
					{
						nk_layout_row_dynamic(ctx, 120, 1);
						color1 = nk_color_picker(ctx, color1, NK_RGBA);
						nk_layout_row_dynamic(ctx, 25, 1);
						color1.r = nk_propertyf(ctx, "#R:", 0, color1.r, 1.0f, 0.01f, 0.005f);
						color1.g = nk_propertyf(ctx, "#G:", 0, color1.g, 1.0f, 0.01f, 0.005f);
						color1.b = nk_propertyf(ctx, "#B:", 0, color1.b, 1.0f, 0.01f, 0.005f);
						color1.a = nk_propertyf(ctx, "#A:", 0, color1.a, 1.0f, 0.01f, 0.005f);
						nk_combo_end(ctx);
					}

					ca->m_StartColor.r = color1.r;
					ca->m_StartColor.g = color1.g;
					ca->m_StartColor.b = color1.b;
					ca->m_StartColor.a = color1.a;

					
					nk_layout_row_dynamic(ctx, 30, 1);
					nk_label(ctx, "Default Color to choose from: ", NK_TEXT_LEFT);
					nk_layout_row_dynamic(ctx, 30, 2);
					if (nk_button_label(ctx, "RED"))
					{
						ca->m_EndColor = Color::Red;
					}
					if (nk_button_label(ctx, "BLUE"))
					{
						ca->m_EndColor = Color::Blue;
					}
					if (nk_button_label(ctx, "ALPHA RED"))
					{
						ca->m_EndColor = AlphaRed;
					}
					if (nk_button_label(ctx, "ALPHA BLUE"))
					{
						ca->m_EndColor = AlphaBlue;
					}
					
					nk_layout_row_dynamic(ctx, 30, 1);
					nk_label(ctx, "End Color: ", NK_TEXT_LEFT);

					static struct nk_colorf color2;
					color2.r = ca->m_EndColor.r;
					color2.g = ca->m_EndColor.g;
					color2.b = ca->m_EndColor.b;
					color2.a = ca->m_EndColor.a;


					if (nk_combo_begin_color(ctx, nk_rgb_cf(color2), nk_vec2(200, 400)))
					{
						nk_layout_row_dynamic(ctx, 120, 1);
						color2 = nk_color_picker(ctx, color2, NK_RGBA);
						nk_layout_row_dynamic(ctx, 25, 1);
						color2.r = nk_propertyf(ctx, "#R:", 0, color2.r, 1.0f, 0.01f, 0.005f);
						color2.g = nk_propertyf(ctx, "#G:", 0, color2.g, 1.0f, 0.01f, 0.005f);
						color2.b = nk_propertyf(ctx, "#B:", 0, color2.b, 1.0f, 0.01f, 0.005f);
						color2.a = nk_propertyf(ctx, "#A:", 0, color2.a, 1.0f, 0.01f, 0.005f);
						nk_combo_end(ctx);
					}

					ca->m_EndColor.r = color2.r;
					ca->m_EndColor.g = color2.g;
					ca->m_EndColor.b = color2.b;
					ca->m_EndColor.a = color2.a;
				}
				nk_tree_pop(ctx);
			}
		}
	}

	void RunSpeedAffectorGUI(SpeedAffector* sp)
	{
		if (sp != NULL)
		{
			if (nk_tree_push(ctx, NK_TREE_NODE, "Speed", NK_MINIMIZED))
			{
				nk_layout_row_dynamic(ctx, 30, 1);
				int m_enabled = (int)sp->m_enabled;
				nk_checkbox_label(ctx, "Enable/Disable Speed Affector", &m_enabled);
				sp->m_enabled = (bool)m_enabled;

				if (m_enabled == true)
				{
					sp->m_lerpSpeed;
					int lerpSpeed = (int)sp->m_lerpSpeed;
					nk_checkbox_label(ctx, "Lerp Speed", &lerpSpeed);
					sp->m_lerpSpeed = (bool)lerpSpeed;

					nk_property_float(ctx, "#StartSpeed:", 0.0f, &sp->m_startSpeed, 100.0f, 0.1f, 0.2f);
					nk_property_float(ctx, "#EndSpeed:", 0.0f, &sp->m_endSpeed, 100.0f, 0.1f, 0.2f);
				}
				nk_tree_pop(ctx);
			}
		}
	}

	void RunScaleAffectorGUI(ScaleAffector* sca)
	{
		if (sca != NULL)
		{
			if (nk_tree_push(ctx, NK_TREE_NODE, "Scale", NK_MINIMIZED))
			{
				nk_layout_row_dynamic(ctx, 30, 1);
				int m_enabled = (int)sca->m_enabled;
				nk_checkbox_label(ctx, "Enable/Disable Scale Affector", &m_enabled);
				sca->m_enabled = (bool)m_enabled;

				if (m_enabled == true)
				{
					sca->m_lerpScale;
					int lerpScale = (int)sca->m_lerpScale;
					nk_checkbox_label(ctx, "Lerp Scale", &lerpScale);
					sca->m_lerpScale = (bool)lerpScale;

					nk_property_float(ctx, "#StartScale:", 0.0f, &sca->m_startScale, 100.0f, 0.1f, 0.2f);
					nk_property_float(ctx, "#EndScale:", 0.0f, &sca->m_endScale, 100.0f, 0.1f, 0.2f);
				}
				nk_tree_pop(ctx);
			}
		}
	}

	void RunRotationAffectorGUI(RotateAffector* ra)
	{
		if (ra != NULL)
		{
			if (nk_tree_push(ctx, NK_TREE_NODE, "Rotation", NK_MINIMIZED))
			{
				nk_layout_row_dynamic(ctx, 30, 1);
				int m_enabled = (int)ra->m_enabled;
				nk_checkbox_label(ctx, "Enable/Disable Rotation Affector", &m_enabled);
				ra->m_enabled = (bool)m_enabled;

				if (m_enabled == true)
				{
					int doRotation = (int)ra->m_DoRotation;
					nk_checkbox_label(ctx, "Do Rotation", &doRotation);
					ra->m_DoRotation = (bool)doRotation;

					int customRotation = (int)ra->m_customRotation;
					nk_checkbox_label(ctx, "RotationFollowDirection", &customRotation);
					ra->m_customRotation = (bool)customRotation;

					if (customRotation == true)
					{
						int rotateLeft = (int)ra->m_rotateLeft;
						nk_checkbox_label(ctx, "Rotate Left", &rotateLeft);
						ra->m_rotateLeft = (bool)rotateLeft;

						int useSine = (int)ra->m_useSine;
						nk_checkbox_label(ctx, "Use Sine", &useSine);
						ra->m_useSine = (bool)useSine;

						nk_property_float(ctx, "#Rot Speed:", 0.0f, &ra->m_speed, 100.0f, 0.1f, 0.2f);
					}
				}
				nk_tree_pop(ctx);
			}
		}
	}

	void RunDemo()
	{
		nk_layout_row_dynamic(ctx, 30, 1);
		nk_label(ctx, "Run Particle System Demo:", NK_TEXT_CENTERED);
		static const char* demoMode[] = { "FIRE_WAVE", "SPIRAL", "FIRE_SPARKS", "FIRE", "FIRE_FAN", "FALLING_SQUARE", "BOKEH", "FLOWER_TRIANGLE", "FIRE_FLY" };
		static int demoOption;
		demoOption = selectedPS->GetDemoType();
		nk_layout_row_dynamic(ctx, 30, 1);
		demoOption = nk_combo(ctx, demoMode, NK_LEN(demoMode), demoOption, 25, nk_vec2(200, 200));
		selectedPS->SetDemoType((DemoType)demoOption);

		nk_layout_row_dynamic(ctx, 30, 1);
		if (nk_button_label(ctx, "RunDemo"))
		{
			selectedPS->RunDemo(selectedPS->GetDemoType());
		}
		nk_layout_row_dynamic(ctx, 15, 1);
	}
};
