#pragma once

struct PixelInfo {
	float ObjectID;
	float DrawID;
	float PrimID;

	/*PixelInfo()
	{
		ObjectID = 0.0f;
		DrawID = 0.0f;
		PrimID = 0.0f;
	}*/
};

PixelInfo ReadPixel(unsigned int, unsigned int);

GLuint m_fbo;
GLuint m_pickingTexture;
GLuint m_depthTexture;



