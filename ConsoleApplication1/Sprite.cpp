#include "Sprite.h"


Sprite::Sprite()
{
    
}

Sprite::Sprite(const std::string& file)
{
    const char* texturePath = file.c_str();
	glGenTextures(1, &m_textureID);
	LoadTexture(texturePath);
}

Sprite::~Sprite()
{

}

void Sprite::LoadTexture(const char* path)
{
	CBitmap bitmap(path);

	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// bilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.GetWidth(), bitmap.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.GetBits());

	m_height = bitmap.GetHeight();
	m_width = bitmap.GetWidth();

}

void Sprite::Draw(float x, float y, float z, float rotation, float scale, DrawShape shape, Color& color)
{
	glPushMatrix();
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);

	if (shape == DrawShape::CIRCLE)
	{
		DrawCircle(x, y, z, rotation, scale, color);
	}
	else if (shape == DrawShape::TRIANGLE)
	{
		DrawTriangle(x, y, z, rotation, scale, color);
	}
	else if (shape == DrawShape::SQUARE)
	{
		DrawSquare(x, y, z, rotation, scale, color);
	}
	else if (shape == DrawShape::STRIPES)
	{
		DrawStripes(x, y, z, rotation, scale, color);
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
}

void Sprite::DrawSquare(float x, float y, float z, float rotation, float scale, Color& color)
{
	glColor4f(color.r, color.g, color.b, color.a);

	glTexCoord2f(0.0f, 0.0f);  glVertex3f(xCoord(-1.0f, -1.0f, rotation, scale, x), yCoord(-1.0f, -1.0f, rotation, scale, y), z);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(xCoord(-1.0f, 1.0f, rotation, scale, x), yCoord(-1.0f, 1.0f, rotation, scale, y), z);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(xCoord(1.0f, 1.0f, rotation, scale, x), yCoord(1.0f, 1.0f, rotation, scale, y), z);

	glTexCoord2f(1.0f, 1.0f);  glVertex3f(xCoord(1.0f, 1.0f, rotation, scale, x), yCoord(1.0f, 1.0f, rotation, scale, y), z);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(xCoord(1.0f, -1.0f, rotation, scale, x), yCoord(1.0f, -1.0f, rotation, scale, y), z);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(xCoord(-1.0f, -1.0f, rotation, scale, x), yCoord(-1.0f, -1.0f, rotation, scale, y), z);
}

void Sprite::DrawCircle(float x, float y, float z, float rotation, float scale, Color& color)
{
	float r = scale;
	float DEF_D = 1.0f;
	float k = 0.0f;

	glColor4f(color.r, color.g, color.b, color.a);
	for (k = 0; k <= 360; k += DEF_D)
	{
		glVertex2f(x, y);
		glVertex2f(x + cos(k) * r, y + sin(k) * r);
		glVertex2f(x + cos(k + DEF_D * 0.1) * r, y + sin(k + DEF_D * 0.1) * r);
	}
}

void Sprite::DrawTriangle(float x, float y, float z, float rotation, float scale, Color& color)
{
	glColor4f(color.r, color.g, color.b, color.a);

	glTexCoord2f(0.5f, 1.0f);  glVertex3f(xCoord(0.0f, -1.0f, rotation, scale, x), yCoord(0.0f, -1.0f, rotation, scale, y), z);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(xCoord(-1.0f, 4.0f, rotation, scale, x), yCoord(-1.0f, 4.0f, rotation, scale, y), z);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(xCoord(1.0f, 4.0f, rotation, scale, x), yCoord(1.0f, 4.0f, rotation, scale, y), z);
}

void Sprite::DrawStripes(float x, float y, float z, float rotation, float scale, Color& color)
{
	
	glColor4f(color.r, color.g, color.b, color.a);

	glTexCoord2f(0.0f, 0.0f);  glVertex3f(xCoord(-0.2f, -1.0f, rotation, scale, x), yCoord(-0.2f, -1.0f, rotation, scale, y), z);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(xCoord(-0.2f, 1.0f, rotation, scale, x), yCoord(-0.2f, 1.0f, rotation, scale, y), z);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(xCoord(0.2f, 1.0f, rotation, scale, x), yCoord(0.2f, 1.0f, rotation, scale, y), z);

	glTexCoord2f(1.0f, 1.0f);  glVertex3f(xCoord(0.2f, 1.0f, rotation, scale, x), yCoord(0.2f, 1.0f, rotation, scale, y), z);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(xCoord(0.2f, -1.0f, rotation, scale, x), yCoord(0.2f, -1.0f, rotation, scale, y), z);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(xCoord(-0.2f, -1.0f, rotation, scale, x), yCoord(-0.2f, -1.0f, rotation, scale, y), z);
}