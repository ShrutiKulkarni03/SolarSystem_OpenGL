#include "inc/al.h"
#include "inc/alc.h"

#include "inc/vmath.h"
#include "inc/stack.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include<map>
#include<string>
#pragma once

#define WIN_WIDTH 1920
#define WIN_HEIGHT 1080
#define PI 3.14159265358979323846
#define NUM_STARS 8000

#define PICKING_FBO_WIDTH 1920
#define PICKING_FBO_HEIGHT 1080
#define STARFIELD_FBO_WIDTH 3000
#define STARFIELD_FBO_HEIGHT 3000

#define MERCURY_OBJECT_ID 11.0f
#define VENUS_OBJECT_ID 22.0f
#define EARTH_OBJECT_ID 33.0f
#define MARS_OBJECT_ID 44.0f
#define JUPITER_OBJECT_ID 55.0f
#define SATURN_OBJECT_ID 66.0f
#define URANUS_OBJECT_ID 77.0f
#define NEPTUNE_OBJECT_ID 88.0f
#define PLUTO_OBJECT_ID 99.0f

using namespace vmath;

//PROPERTIES OF VERTEX
enum
{
	SPK_ATTRIBUTE_POSITION = 0,
	SPK_ATTRIBUTE_COLOR,
	SPK_ATTRIBUTE_NORMAL,
	SPK_ATTRIBUTE_TEXCOORD,
};

//global variable declaration
DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
bool gbFullscreen = false;
HWND ghwnd = NULL;
bool gbActiveWindow = false;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
FILE* gpFile = NULL;

//OpenAL
ALCdevice* device = NULL;
ALCcontext* context = NULL;
unsigned int buffer;
unsigned int source;
ALsizei alSize, alFrequency;
ALenum alFormat;
ALvoid* alData;
ALboolean alLoop = AL_FALSE;

//text
struct Character {
	unsigned int textTextureID;     //ID handle of texture
	vec2 size;						//size of glyph
	vec2 bearing;					//offset from baseline to left/top of glyph
	unsigned int advance;			//horizontal offset to advance to next glyph
};

std::map<GLchar, Character> characters;

int gWidth, gHeight;

LPSTACK pStack = NULL;
GLuint frameCount = 0;   //2800
double currentTime = 0.0f;
bool bTextViewport = false;

//picking
bool LMBpressed = false;
bool RMBpressed = false;
float winXCoord;
float winYCoord;

//camera
GLfloat camXPos = 0.0f;
GLfloat camYPos = 0.0f;
GLfloat camZPos = 0.5f;   //init: 0.5f;

vec3 camPos = vec3(camXPos, camYPos, camZPos);
vec3 camTarget = vec3(0.0f, 0.0f, -1.0f);
vec3 camUpAxis = vec3(0.0f, 1.0f, 0.0f);
mat4 camera;

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = (float)WIN_WIDTH / 2.0f;
float lastY = (float)WIN_HEIGHT / 2.0f;
float fov = 50.0f;


//shader variables
//picking shader
GLuint pickingShaderProgramObject;

GLuint modelMatrixUniformP;
GLuint viewMatrixUniformP;
GLuint projectionMatrixUniformP;

GLuint drawIndexUniformP;
GLuint objectIndexUniformP;

//scene light shader
GLuint sceneShaderProgramObject;

GLuint modelMatrixUniformS;
GLuint viewMatrixUniformS;
GLuint projectionMatrixUniformS;

GLuint passUniformS;
GLuint lightUniformS;
GLuint textureSamplerUniformS;
GLuint lAUniformS;
GLuint lDUniformS;
GLuint lSUniformS;
GLuint positionalLightPositionUniformS;
GLuint pointLightPositionUniformS;
GLfloat kShininessUniformS;

GLuint lightConstantUniformS;
GLuint lightLinearUniformS;
GLuint lightQuadraticUniformS;

//noise shader
GLuint noiseShaderProgramObject;

GLuint modelMatrixUniformN;
GLuint viewMatrixUniformN;
GLuint projectionMatrixUniformN;

GLuint lAUniformN;
GLuint lDUniformN;
GLuint lSUniformN;
GLuint lightPositionUniformN;

GLuint kAUniformN;
GLuint kDUniformN;
GLuint kSUniformN;
GLfloat kShininessUniformN;

GLuint textureSampler3DNoiseUniformN;
GLuint marbleColorUniformN;
GLuint veinColorUniformN;
GLuint noiseScaleUniformN;
GLuint scaleUniformN;
GLuint tileFactorUniformN;
GLuint smoothFactorUniformN;
GLuint vertexZAnimUniformN;


//starfield shader
GLuint starfieldShaderProgramObject;

GLuint projectionMatrixUniformSF;
GLuint textureSamplerUniformSF;
GLuint timeUniformSF;

//godrays shader
GLuint godraysShaderProgramObject;

GLuint modelMatrixUniformG;
GLuint viewMatrixUniformG;
GLuint projectionMatrixUniformG;

GLuint occlusionTextureSamplerUniformG;
GLuint densityUniformG;
GLuint weightUniformG;
GLuint decayUniformG;
GLuint exposureUniformG;
GLuint lightPositionUniformG;
GLuint numSamplesUniformG;

//final pass for godrays shader
GLuint finalPassShaderProgramObject;

GLuint pass1TextureSamplerUniformF;
GLuint pass2TextureSamplerUniformF;

//skybox shader
GLuint skyboxShaderProgramObject;

GLuint projectionMatrixUniform;
GLuint viewMatrixUniform;
GLuint textureSamplerUniform;

//cube for cubemap shader
GLuint cubeShaderProgramObject;

GLuint modelMatrixUniformC;
GLuint viewMatrixUniformC;
GLuint projectionMatrixUniformC;
//GLuint mvpMatrixUniformC;
GLuint textureSamplerUniformC;

//text shader
GLuint textShaderProgramObject;

GLuint textSamplerUniform;
GLuint textColorUniform;
GLuint mvpMatrixUniform;

//fullscreen quad shader
GLuint fullscreenQuadShaderProgramObject;

//perspective projection matrix
mat4 perspectiveProjectionMatrix;   //4x4 matrix

//scene variables
GLuint fbo_pass1;
GLuint silhouetteTexture_pass1;
GLuint rbo_pass1;

GLuint fbo_pass2;
GLuint colorTexture_pass2;
GLuint rbo_pass2;

GLuint fbo_godrays;
GLuint colorBuffer;

//fbo for cubemap

GLuint fbo_starfield;
GLuint starfieldTextureColorBuffer;
GLuint rbo_starfield;

//vao for square
GLuint vao_star;
GLuint vbo_star;

//vao for text
GLuint vao_text;
GLuint vbo_text;

//vao for cube
GLuint vao_cube;
GLuint vbo_position_cube;
GLuint vbo_texture_cube;

//vao for square
GLuint vao_square;
GLuint vbo_position_square;
GLuint vbo_texture_square;
GLuint vbo_normal_square;

//vao for skybox
GLuint vao_skybox;
GLuint vbo_skybox;

//vao mercury ellipse
GLuint vao_mercury_ellipse;
GLuint vbo_position_mercury_ellipse;
GLuint mercury_ellipse_num_vertices = 0;
struct vector* mercuryEllipse;
GLfloat mercuryOrbitRadius = 0.2f;
GLfloat mercuryXLengthConstant = 0.1f;

//vao venus ellipse
GLuint vao_venus_ellipse;
GLuint vbo_position_venus_ellipse;
GLuint venus_ellipse_num_vertices = 0;
struct vector* venusEllipse;
GLfloat venusOrbitRadius = 0.3f;
GLfloat venusXLengthConstant = 0.15f;

//vao earth ellipse
GLuint vao_earth_ellipse;
GLuint vbo_position_earth_ellipse;
GLuint earth_ellipse_num_vertices = 0;
struct vector* earthEllipse;
GLfloat earthOrbitRadius = 0.4f;
GLfloat earthXLengthConstant = 0.2f;

//vao mars ellipse
GLuint vao_mars_ellipse;
GLuint vbo_position_mars_ellipse;
GLuint mars_ellipse_num_vertices = 0;
struct vector* marsEllipse;
GLfloat marsOrbitRadius = 0.5f;
GLfloat marsXLengthConstant = 0.25f;

//vao jupiter ellipse
GLuint vao_jupiter_ellipse;
GLuint vbo_position_jupiter_ellipse;
GLuint jupiter_ellipse_num_vertices = 0;
struct vector* jupiterEllipse;
GLfloat jupiterOrbitRadius = 0.8f;
GLfloat jupiterXLengthConstant = 0.4f;

//vao saturn ellipse
GLuint vao_saturn_ellipse;
GLuint vbo_position_saturn_ellipse;
GLuint saturn_ellipse_num_vertices = 0;
struct vector* saturnEllipse;
GLfloat saturnOrbitRadius = 1.0f;
GLfloat saturnXLengthConstant = 0.5f;

//vao saturn ring ellipse
GLuint vao_saturnRing_ellipse;
GLuint vbo_position_saturnRing_ellipse;
GLuint saturnRing_ellipse_num_vertices = 0;
struct vector* saturnRingEllipse;
GLfloat saturnRingOrbitRadius = 0.8f;
GLfloat saturnRingXLengthConstant = 0.0f;

//vao uranus ellipse
GLuint vao_uranus_ellipse;
GLuint vbo_position_uranus_ellipse;
GLuint uranus_ellipse_num_vertices = 0;
struct vector* uranusEllipse;
GLfloat uranusOrbitRadius = 1.2f;
GLfloat uranusXLengthConstant = 0.6f;

//vao neptune ellipse
GLuint vao_neptune_ellipse;
GLuint vbo_position_neptune_ellipse;
GLuint neptune_ellipse_num_vertices = 0;
struct vector* neptuneEllipse;
GLfloat neptuneOrbitRadius = 1.4f;
GLfloat neptuneXLengthConstant = 0.7f;

//vao pluto ellipse
GLuint vao_pluto_ellipse;
GLuint vbo_position_pluto_ellipse;
GLuint pluto_ellipse_num_vertices = 0;
struct vector* plutoEllipse;
GLfloat plutoOrbitRadius = 1.6f;
GLfloat plutoXLengthConstant = 0.8f;



//update variables
GLfloat sunPosX = 0.0f;
GLfloat sunPosY = 0.0f;

//NOISE
GLfloat vertexScaleFactor;
GLfloat noiseScaleFactor;
GLfloat tilingFactor;
GLfloat smoothingFactor;
GLfloat vertexZAnim = 0.0f;

GLfloat mercuryRevAngle = 0.0f;
GLfloat mercuryRotAngle = 0.0f;
GLfloat venusRevAngle = 0.0f;
GLfloat venusRotAngle = 0.0f;
GLfloat earthRevAngle = 0.0f;
GLfloat earthRotAngle = 0.0f;
GLfloat earthMoonRevAngle = 0.0f;
GLfloat earthMoonRotAngle = 0.0f;
GLfloat marsRevAngle = 0.0f;
GLfloat marsRotAngle = 0.0f;
GLfloat marsMoonRevAngle = 0.0f;
GLfloat marsMoonRotAngle = 0.0f;
GLfloat jupiterRevAngle = 0.0f;
GLfloat jupiterRotAngle = 0.0f;
GLfloat jupiterMoonRevAngle = 0.0f;
GLfloat jupiterMoonRotAngle = 0.0f;
GLfloat saturnRevAngle = 0.0f;
GLfloat saturnRotAngle = 0.0f;
GLfloat saturnMoonRevAngle = 0.0f;
GLfloat saturnMoonRotAngle = 0.0f;
GLfloat uranusRevAngle = 0.0f;
GLfloat uranusRotAngle = 0.0f;
GLfloat uranusMoonRevAngle = 0.0f;
GLfloat uranusMoonRotAngle = 0.0f;
GLfloat neptuneRevAngle = 0.0f;
GLfloat neptuneRotAngle = 0.0f;
GLfloat neptuneMoonRevAngle = 0.0f;
GLfloat neptuneMoonRotAngle = 0.0f;
GLfloat plutoRevAngle = 0.0f;
GLfloat plutoRotAngle = 0.0f;

//texture variables
GLuint textureID;
GLuint noise3DTexture;
GLuint star_texture;

GLuint cubemapTextureID;
GLuint cubemap_texture;

GLuint sun_texture;
GLuint mercury_texture;
GLuint venus_texture;
GLuint earth_texture;
GLuint earthMoon_texture;
GLuint mars_texture;
GLuint marsMoon_texture;
GLuint jupiter_texture;
GLuint jupiterMoon_texture;
GLuint saturn_texture;
GLuint saturn_ring_texture;
GLuint saturnMoon_texture;
GLuint uranus_texture;
GLuint uranusMoon_texture;
GLuint neptune_texture;
GLuint neptuneMoon_texture;
GLuint pluto_texture;

//solar system orbits slight tilting variables
GLfloat orbitsXRot = 30.0f;
GLfloat orbitsZRot = 0.0f;


//1st translate animation
bool isMercuryInOrbit = false;
GLfloat mercuryXPos = 0.0f;
GLfloat mercuryYPos = 0.0f;
GLfloat mercuryZPos = 0.0f;

bool isVenusInOrbit = false;
GLfloat venusXPos = 0.0f;
GLfloat venusYPos = 0.0f;
GLfloat venusZPos = 0.0f;

bool isEarthInOrbit = false;
GLfloat earthXPos = 0.0f;
GLfloat earthYPos = 0.0f;
GLfloat earthZPos = 0.0f;

bool isMarsInOrbit = false;
GLfloat marsXPos = 0.0f;
GLfloat marsYPos = 0.0f;
GLfloat marsZPos = 0.0f;

bool isJupiterInOrbit = false;
GLfloat jupiterXPos = 0.0f;
GLfloat jupiterYPos = 0.0f;
GLfloat jupiterZPos = 0.0f;

bool isSaturnInOrbit = false;
GLfloat saturnXPos = 0.0f;
GLfloat saturnYPos = 0.0f;
GLfloat saturnZPos = 0.0f;

bool isUranusInOrbit = false;
GLfloat uranusXPos = 0.0f;
GLfloat uranusYPos = 0.0f;
GLfloat uranusZPos = 0.0f;

bool isNeptuneInOrbit = false;
GLfloat neptuneXPos = 0.0f;
GLfloat neptuneYPos = 0.0f;
GLfloat neptuneZPos = 0.0f;

bool isPlutoInOrbit = false;
GLfloat plutoXPos = 0.0f;
GLfloat plutoYPos = 0.0f;
GLfloat plutoZPos = 0.0f;

//focused planets after click
bool isMercuryClicked = false;
GLfloat focusedMercuryXPos = 0.0f;
GLfloat focusedMercuryYPos = 0.0f;
GLfloat focusedMercuryZPos = 0.0f;

bool isVenusClicked = false;
GLfloat focusedVenusXPos = 0.0f;
GLfloat focusedVenusYPos = 0.0f;
GLfloat focusedVenusZPos = 0.0f;

bool isEarthClicked = false;
GLfloat focusedEarthXPos = 0.0f;
GLfloat focusedEarthYPos = 0.0f;
GLfloat focusedEarthZPos = 0.0f;

bool isMarsClicked = false;
GLfloat focusedMarsXPos = 0.0f;
GLfloat focusedMarsYPos = 0.0f;
GLfloat focusedMarsZPos = 0.0f;

bool isJupiterClicked = false;
GLfloat focusedJupiterXPos = 0.0f;
GLfloat focusedJupiterYPos = 0.0f;
GLfloat focusedJupiterZPos = 0.0f;

bool isSaturnClicked = false;
GLfloat focusedSaturnXPos = 0.0f;
GLfloat focusedSaturnYPos = 0.0f;
GLfloat focusedSaturnZPos = 0.0f;

bool isUranusClicked = false;
GLfloat focusedUranusXPos = 0.0f;
GLfloat focusedUranusYPos = 0.0f;
GLfloat focusedUranusZPos = 0.0f;

bool isNeptuneClicked = false;
GLfloat focusedNeptuneXPos = 0.0f;
GLfloat focusedNeptuneYPos = 0.0f;
GLfloat focusedNeptuneZPos = 0.0f;

bool isPlutoClicked = false;
GLfloat focusedPlutoXPos = 0.0f;
GLfloat focusedPlutoYPos = 0.0f;
GLfloat focusedPlutoZPos = 0.0f;

//random number generator
static unsigned int seed = 0x13371337;

static inline float random_float()
{
	float res;
	unsigned int tmp;

	seed *= 16807;

	tmp = seed ^ (seed >> 4) ^ (seed << 15);

	*((unsigned int*)&res) = (tmp >> 9) | 0x3F800000;

	return(res - 1.0f);
}

GLfloat tempx = -2.0f;
GLfloat angleS = 0.0f;
float t;

