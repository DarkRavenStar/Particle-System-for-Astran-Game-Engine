#ifndef SPRITE_H_
#define SPRITE_H_

#include "AllHeader.h"
#include "Vector3.h"
#include "Transformation.h"
#include "Color.h"
#include "Rendering.h"
#include "bitmap.h"



class Sprite
{
private:
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_textureID;
    Vector4 color1 = { 0.5f, 0.2f, 1.0f, 1.0f };
    Vector4 color2 = { 1.0f, 0.5f, 1.0f, 1.0f };

	void LoadTexture(const char* path);
	void DrawSquare(float x, float y, float z, float rotation, float scale, Color& color);
	void DrawCircle(float x, float y, float z, float rotation, float scale, Color& color);
	void DrawTriangle(float x, float y, float z, float rotation, float scale, Color& color);
	void DrawStripes(float x, float y, float z, float rotation, float scale, Color& color);
public:
    Sprite();
    Sprite(const std::string& file);
    ~Sprite();
	void Draw(float x, float y, float z, float rotation, float scale, DrawShape shape, Color& color = Color::White);
};




#endif // SPRITE_H_
