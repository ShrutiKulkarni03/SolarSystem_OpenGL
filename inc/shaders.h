#pragma once

//prototypes
void initPickingShader(void);
void initSceneLightingShader(void);
void initNoiseShader(void);
void initGodraysShader(void);
void initFinalPassForGodraysShader(void);
void initSkyboxShader(void);
void initCubemapCubeShader(void);
void initTextShader(void);
void initStarfieldShader(void);
//void initFullScreenQuad(void);

void uninitPickingShader(void);
void uninitSceneLightingShader(void);
void uninitNoiseShader(void);
void uninitGodraysShader(void);
void uninitFinalPassForGodraysShader(void);
void uninitSkyboxShader(void);
void uninitCubemapCubeShader(void);
void uninitTextShader(void);
void uninitStarfieldShader(void);
//void uninitFullScreenQuad(void);

//init
void initPickingShader(void)
{
	GLuint pickingVertexShaderObject;
	GLuint pickingFragmentShaderObject;

	/*****VERTEX SHADER*****/

	//create shader
	pickingVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//provide source code to vertex shader
	const GLchar* pickingVertexShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"in vec4 vPosition; \n" \
		"uniform mat4 u_projection_matrix; \n" \
		"uniform mat4 u_model_matrix; \n" \
		"uniform mat4 u_view_matrix; \n" \
		"void main(void) \n" \
		"{ \n" \
		"	gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition; \n" \
		"} \n";

	glShaderSource(pickingVertexShaderObject, 1, (const GLchar**)&pickingVertexShaderSourceCode, NULL);

	//compile shader
	glCompileShader(pickingVertexShaderObject);

	//vertex shader compilation error checking
	GLint infoLogLength = 0;
	GLint shaderCompiledStatus = 0;
	char* szBuffer = NULL;

	glGetShaderiv(pickingVertexShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(pickingVertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(pickingVertexShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nVertex Shader Compilation Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}

		}
	}


	/*****FRAGMENT SHADER*****/

	//create shader
	pickingFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	//provide source code to fragment shader
	const GLchar* pickingFragmentShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"uniform float u_drawIndex; \n" \
		"uniform float u_objectIndex; \n" \
		"out vec3 FragColor; \n" \
		"void main(void) \n" \
		"{ \n" \
		"	FragColor = vec3(u_objectIndex, u_drawIndex, gl_PrimitiveID + 1); \n" \
		"} \n";

	glShaderSource(pickingFragmentShaderObject, 1, (const GLchar**)&pickingFragmentShaderSourceCode, NULL);

	//compile shader
	glCompileShader(pickingFragmentShaderObject);

	//fragment shader compilation error checking
	infoLogLength = 0;
	shaderCompiledStatus = 0;
	szBuffer = NULL;

	glGetShaderiv(pickingFragmentShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(pickingFragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(pickingFragmentShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nFragment Shader Compilation Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);

			}
		}
	}


	/*****SHADER PROGRAM*****/

	//create
	pickingShaderProgramObject = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(pickingShaderProgramObject, pickingVertexShaderObject);

	//attach fragment shader to shader program
	glAttachShader(pickingShaderProgramObject, pickingFragmentShaderObject);

	//pre-linking binding
	glBindAttribLocation(pickingShaderProgramObject, SPK_ATTRIBUTE_POSITION, "vPosition");

	//link shader
	glLinkProgram(pickingShaderProgramObject);

	//shader linking error checking
	infoLogLength = 0;
	GLint shaderProgramLinkStatus;
	szBuffer = NULL;

	glGetProgramiv(pickingShaderProgramObject, GL_LINK_STATUS, &shaderProgramLinkStatus);

	if (shaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(pickingShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);

			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(pickingShaderProgramObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nShader Program Link Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}
		}
	}

	//get MVP uniform location
	modelMatrixUniformP = glGetUniformLocation(pickingShaderProgramObject, "u_model_matrix");
	viewMatrixUniformP = glGetUniformLocation(pickingShaderProgramObject, "u_view_matrix");
	projectionMatrixUniformP = glGetUniformLocation(pickingShaderProgramObject, "u_projection_matrix");
	drawIndexUniformP = glGetUniformLocation(pickingShaderProgramObject, "u_drawIndex");
	objectIndexUniformP = glGetUniformLocation(pickingShaderProgramObject, "u_objectIndex");

}

void initSceneLightingShader(void)
{

	GLuint sceneVertexShaderObject;
	GLuint sceneFragmentShaderObject;

	/*****VERTEX SHADER*****/

	//create shader
	sceneVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//provide source code to vertex shader
	const GLchar* sceneVertexShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"in vec4 vPosition; \n" \
		"in vec4 vColor; \n" \
		"in vec2 vTexcoord; \n" \
		"in vec3 vNormal; \n" \
		"uniform vec4 u_positional_light_position; \n" \
		"uniform vec4 u_point_light_position; \n" \
		"uniform mat4 u_projection_matrix; \n" \
		"uniform mat4 u_model_matrix; \n" \
		"uniform mat4 u_view_matrix; \n" \
		"uniform int u_light; \n" \
		"out vec4 out_color; \n" \
		"out vec2 out_texcoord; \n" \
		"out vec3 transformed_normal; \n" \
		"out vec3 light_direction; \n" \
		"out vec3 view_vector; \n" \
		"out float distance; \n" \
		"void main(void) \n" \
		"{ \n" \
		"	vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition; \n" \
		"	transformed_normal = mat3(u_view_matrix * u_model_matrix) * vNormal; \n" \
		"	view_vector = -eye_coordinates.xyz; \n" \
		
		"	if(u_light == 0) \n" \
		"	{ \n" \
		"		light_direction = vec3(u_point_light_position - eye_coordinates); \n" \
		"		distance = length(u_point_light_position - vPosition);" \
		"	} \n" \
		"	else if(u_light == 1) \n" \
		"	{ \n" \
		"		light_direction = vec3(u_positional_light_position - eye_coordinates); \n" \
		"		distance = length(u_positional_light_position - vPosition);" \
		"	} \n" \
		
		"	gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition; \n" \
		"	out_color = vColor; \n" \
		"	out_texcoord = vTexcoord; \n" \
		"} \n";

	glShaderSource(sceneVertexShaderObject, 1, (const GLchar**)&sceneVertexShaderSourceCode, NULL);

	//compile shader
	glCompileShader(sceneVertexShaderObject);

	//vertex shader compilation error checking
	GLint infoLogLength = 0;
	GLint shaderCompiledStatus = 0;
	char* szBuffer = NULL;

	glGetShaderiv(sceneVertexShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(sceneVertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(sceneVertexShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nS: Vertex Shader Compilation Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}

		}
	}


	/*****FRAGMENT SHADER*****/

	//create shader
	sceneFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	//provide source code to fragment shader
	const GLchar* sceneFragmentShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"in vec4 out_color; \n" \
		"in vec3 transformed_normal; \n" \
		"in vec3 light_direction; \n" \
		"in vec3 view_vector; \n" \
		"in vec2 out_texcoord; \n" \
		"in float distance; \n" \
		"uniform int u_pass; \n" \
		"uniform sampler2D u_texture_sampler; \n" \
		"uniform vec3 u_lA; \n" \
		"uniform vec3 u_lD; \n" \
		"uniform vec3 u_lS; \n" \
		"uniform float u_materialShininess; \n" \
		"uniform vec3 u_light_target; \n" /*light direction*/ \
		"uniform float u_light_constant; \n" \
		"uniform float u_light_linear; \n" \
		"uniform float u_light_quadratic; \n" \
		"uniform int u_light; \n" \
		"out vec4 FragColor; \n" \
		"vec3 phong_ads_light; \n" \
		"void main(void) \n" \
		"{ \n" \
		"	if(u_pass == 1) \n" \
		"	{ \n" \
		"		vec4 texColor = texture(u_texture_sampler, out_texcoord); \n" \
		"		if(texColor.a < 0.1) \n" \
		"			discard; \n" \
		"		FragColor = out_color; \n" \
		"	} \n" \

		"	else \n" \
		"	{ \n" \
		"		vec4 texColor = texture(u_texture_sampler, out_texcoord); \n" \
		"		if(texColor.a < 0.1) \n" \
		"			discard; \n" \

		"		vec3 normalized_transformed_normals = normalize(transformed_normal); \n" \
		"		vec3 normalized_light_direction = normalize(light_direction); \n" \
		"		vec3 normalized_view_vector = normalize(view_vector); \n" \
		"		vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals); \n" \
	
		"		if(u_light == 0) \n" \
		"		{ \n" \
		"			vec3 ambient = u_lA * texColor.rgb; \n" \
		"			vec3 diffuse = u_lD * texColor.rgb * max(dot(normalized_light_direction, normalized_transformed_normals), 0.0); \n" \
		"			vec3 specular = u_lS * pow(max(dot(reflection_vector, normalized_view_vector), 0.0), u_materialShininess); \n" \
		"			float attenuation = 1.0 / (u_light_constant + u_light_linear * distance + u_light_quadratic * (distance * distance)); \n" \
		"			ambient *= attenuation; \n" \
		"			diffuse *= attenuation; \n" \
		"			specular *= attenuation; \n" \
		"			phong_ads_light = ambient + diffuse + specular; \n" \
		"			FragColor = vec4(phong_ads_light, 1.0f); \n" \
		"		} \n" \
		"		else if(u_light == 1) \n" \
		"		{ \n" \
		"			vec3 ambient = u_lA * texColor.rgb; \n" \
		"			vec3 diffuse = u_lD * texColor.rgb * max(dot(normalized_light_direction, normalized_transformed_normals), 0.0); \n" \
		"			vec3 specular = u_lS * pow(max(dot(reflection_vector, normalized_view_vector), 0.0), u_materialShininess); \n" \
		"			phong_ads_light = ambient + diffuse + specular; \n" \
		"			FragColor = vec4(phong_ads_light, 1.0f); \n" \
		"		} \n" \
		"	} \n" \
		"} \n";

	glShaderSource(sceneFragmentShaderObject, 1, (const GLchar**)&sceneFragmentShaderSourceCode, NULL);

	//compile shader
	glCompileShader(sceneFragmentShaderObject);

	//fragment shader compilation error checking
	infoLogLength = 0;
	shaderCompiledStatus = 0;
	szBuffer = NULL;

	glGetShaderiv(sceneFragmentShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(sceneFragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(sceneFragmentShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nS: Fragment Shader Compilation Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);

			}
		}
	}


	/*****SHADER PROGRAM*****/

	//create
	sceneShaderProgramObject = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(sceneShaderProgramObject, sceneVertexShaderObject);

	//attach fragment shader to shader program
	glAttachShader(sceneShaderProgramObject, sceneFragmentShaderObject);

	//pre-linking binding
	glBindAttribLocation(sceneShaderProgramObject, SPK_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(sceneShaderProgramObject, SPK_ATTRIBUTE_COLOR, "vColor");
	glBindAttribLocation(sceneShaderProgramObject, SPK_ATTRIBUTE_TEXCOORD, "vTexcoord");
	glBindAttribLocation(sceneShaderProgramObject, SPK_ATTRIBUTE_NORMAL, "vNormal");

	//link shader
	glLinkProgram(sceneShaderProgramObject);

	//shader linking error checking
	infoLogLength = 0;
	GLint shaderProgramLinkStatus;
	szBuffer = NULL;

	glGetProgramiv(sceneShaderProgramObject, GL_LINK_STATUS, &shaderProgramLinkStatus);

	if (shaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(sceneShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);

			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(sceneShaderProgramObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nS: Shader Program Link Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}
		}
	}

	//get MVP uniform location
	modelMatrixUniformS = glGetUniformLocation(sceneShaderProgramObject, "u_model_matrix");
	viewMatrixUniformS = glGetUniformLocation(sceneShaderProgramObject, "u_view_matrix");
	projectionMatrixUniformS = glGetUniformLocation(sceneShaderProgramObject, "u_projection_matrix");

	passUniformS = glGetUniformLocation(sceneShaderProgramObject, "u_pass");
	lightUniformS = glGetUniformLocation(sceneShaderProgramObject, "u_light");

	lAUniformS = glGetUniformLocation(sceneShaderProgramObject, "u_lA");
	lDUniformS = glGetUniformLocation(sceneShaderProgramObject, "u_lD");
	lSUniformS = glGetUniformLocation(sceneShaderProgramObject, "u_lS");

	positionalLightPositionUniformS = glGetUniformLocation(sceneShaderProgramObject, "u_positional_light_position");
	pointLightPositionUniformS = glGetUniformLocation(sceneShaderProgramObject, "u_point_light_position");
	lightConstantUniformS = glGetUniformLocation(sceneShaderProgramObject, "u_light_constant");
	lightLinearUniformS = glGetUniformLocation(sceneShaderProgramObject, "u_light_linear");
	lightQuadraticUniformS = glGetUniformLocation(sceneShaderProgramObject, "u_light_quadratic");

	kShininessUniformS = glGetUniformLocation(sceneShaderProgramObject, "u_materialShininess");
	textureSamplerUniformS = glGetUniformLocation(sceneShaderProgramObject, "u_texture_sampler");

}

void initNoiseShader(void)
{
	GLuint noiseVertexShaderObject;
	GLuint noiseFragmentShaderObject;

	/*****VERTEX SHADER*****/

	//create shader
	noiseVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//provide source code to vertex shader
	const GLchar* noiseVertexShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"in vec4 vPosition; \n" \
		"in vec3 vNormal; \n" \

		"uniform mat4 u_projection_matrix; \n" \
		"uniform mat4 u_model_matrix; \n" \
		"uniform mat4 u_view_matrix; \n" \
		"uniform vec4 u_light_position; \n" \
		"uniform float u_scale; \n" \
		"uniform float u_vertexZAnim; \n" \

		"out vec3 transformed_normal; \n" \
		"out vec3 light_direction; \n" \
		"out vec3 view_vector; \n" \
		"out vec3 mc_position; \n" \
		"out float lightIntensity; \n" \

		"void main(void) \n" \
		"{ \n" \
		"	vec4 eye_coordinates = u_view_matrix * u_model_matrix * vPosition; \n" \
		/*"	mc_position = vPosition.xyz * u_scale; \n" \*/
		"	mc_position = vPosition.xyz * u_scale + vec3(0.0, 0.0, u_vertexZAnim); \n" \
		"	transformed_normal = mat3(u_view_matrix * u_model_matrix) * vNormal; \n" \
		"	light_direction = vec3(u_light_position - eye_coordinates); \n" \
		"	view_vector = -eye_coordinates.xyz; \n" \
		"	lightIntensity = dot(normalize(u_light_position.xyz - eye_coordinates.xyz), transformed_normal); \n" \
		"	lightIntensity *= 1.5; \n" \
		"	gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition; \n" \
		"} \n";


	glShaderSource(noiseVertexShaderObject, 1, (const GLchar**)&noiseVertexShaderSourceCode, NULL);

	//compile shader
	glCompileShader(noiseVertexShaderObject);

	//vertex shader compilation error checking
	GLint infoLogLength = 0;
	GLint shaderCompiledStatus = 0;
	char* szBuffer = NULL;

	glGetShaderiv(noiseVertexShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(noiseVertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(noiseVertexShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nNoise Vertex Shader Compilation Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}

		}
	}


	/*****FRAGMENT SHADER*****/

	//create shader
	noiseFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	//provide source code to fragment shader
	const GLchar* noiseFragmentShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"in vec3 transformed_normal; \n" \
		"in vec3 light_direction; \n" \
		"in vec3 view_vector; \n" \
		"in vec3 mc_position; \n" \
		"in float lightIntensity; \n" \

		"uniform vec3 u_lA; \n" \
		"uniform vec3 u_lD; \n" \
		"uniform vec3 u_lS; \n" \
		"uniform vec3 u_kA; \n" \
		"uniform vec3 u_kD; \n" \
		"uniform vec3 u_kS; \n" \
		"uniform float u_materialShininess; \n" \
		"uniform sampler3D u_noise; \n" \
		"uniform vec3 u_marbleColor; \n" \
		"uniform vec3 u_veinColor; \n" \
		"uniform float u_noiseScale; \n" \
		"uniform float u_tileFactor; \n" \
		"uniform float u_smoothFactor; \n" \

		"out vec4 FragColor; \n" \

		"vec3 phong_ads_light; \n" \

		"void main(void) \n" \
		"{ \n" \
		"	vec3 normalized_transformed_normals = normalize(transformed_normal); \n" \
		"	vec3 normalized_light_direction = normalize(light_direction); \n" \
		"	vec3 normalized_view_vector = normalize(view_vector); \n" \
		"	vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals); \n" \
		"	vec3 ambient = u_lA * u_kA; \n" \
		"	vec3 diffuse = u_lD * u_kD * max(dot(normalized_light_direction, normalized_transformed_normals), 0.0); \n" \
		"	vec3 specular = u_lS * u_kS * pow(max(dot(reflection_vector, normalized_view_vector), 0.0), u_materialShininess); \n" \
		"	phong_ads_light = ambient + diffuse + specular; \n" \

		"	vec4 noisevec = texture(u_noise, mc_position * u_noiseScale); \n" \
		"	float intensity = abs(noisevec[0] - 0.25) + abs(noisevec[1] - 0.125) + abs(noisevec[2] - 0.0625) + abs(noisevec[3] - 0.03125); \n" \

		/*"	float sineval = sin(mc_position.y * 6.0 + intensity * 12.0) * 0.5 + 0.5; \n" \*/
		"	float sineval = sin(mc_position.y * u_tileFactor + intensity * u_smoothFactor) * 0.5 + 0.5; \n" \

		"	vec3 color = mix(u_veinColor, u_marbleColor, sineval) * phong_ads_light; \n" \

		"	FragColor = vec4(color, 1.0); \n" \
		"} \n";

	glShaderSource(noiseFragmentShaderObject, 1, (const GLchar**)&noiseFragmentShaderSourceCode, NULL);

	//compile shader
	glCompileShader(noiseFragmentShaderObject);

	//fragment shader compilation error checking
	infoLogLength = 0;
	shaderCompiledStatus = 0;
	szBuffer = NULL;

	glGetShaderiv(noiseFragmentShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(noiseFragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(noiseFragmentShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nNoise Fragment Shader Compilation Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);

			}
		}
	}


	/*****SHADER PROGRAM*****/

	//create
	noiseShaderProgramObject = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(noiseShaderProgramObject, noiseVertexShaderObject);

	//attach fragment shader to shader program
	glAttachShader(noiseShaderProgramObject, noiseFragmentShaderObject);

	//pre-linking binding
	glBindAttribLocation(noiseShaderProgramObject, SPK_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(noiseShaderProgramObject, SPK_ATTRIBUTE_NORMAL, "vNormal");

	//link shader
	glLinkProgram(noiseShaderProgramObject);

	//shader linking error checking
	infoLogLength = 0;
	GLint shaderProgramLinkStatus;
	szBuffer = NULL;

	glGetProgramiv(noiseShaderProgramObject, GL_LINK_STATUS, &shaderProgramLinkStatus);

	if (shaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(noiseShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);

			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(noiseShaderProgramObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nNoise Shader Program Link Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}
		}
	}

	//get uniform location
	modelMatrixUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_model_matrix");
	viewMatrixUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_view_matrix");
	projectionMatrixUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_projection_matrix");

	lAUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_lA");
	lDUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_lD");
	lSUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_lS");
	lightPositionUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_light_position");

	kAUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_kA");
	kDUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_kD");
	kSUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_kS");
	kShininessUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_materialShininess");

	textureSampler3DNoiseUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_noise");
	veinColorUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_veinColor");
	marbleColorUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_marbleColor");
	noiseScaleUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_noiseScale");
	scaleUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_scale");
	tileFactorUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_tileFactor");
	smoothFactorUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_smoothFactor");
	vertexZAnimUniformN = glGetUniformLocation(noiseShaderProgramObject, "u_vertexZAnim");

}

void initGodraysShader(void)
{
	GLuint godraysVertexShaderObject;
	GLuint godraysFragmentShaderObject;

	/*****VERTEX SHADER*****/

	//create shader
	godraysVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//provide source code to vertex shader
	const GLchar* godraysVertexShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"in vec4 vPosition; \n" \
		"in vec2 vTexcoord; \n" \
		"in vec3 vNormal; \n" \
		"out vec3 out_normal; \n" \
		"out vec2 out_texcoord; \n" \
		"void main(void) \n" \
		"{ \n" \
		"	out_normal = vNormal; \n" \
		"	out_texcoord = vTexcoord; \n" \
		"	gl_Position = vPosition; \n" \
		"} \n";

	glShaderSource(godraysVertexShaderObject, 1, (const GLchar**)&godraysVertexShaderSourceCode, NULL);

	//compile shader
	glCompileShader(godraysVertexShaderObject);

	//vertex shader compilation error checking
	GLint infoLogLength = 0;
	GLint shaderCompiledStatus = 0;
	char* szBuffer = NULL;

	glGetShaderiv(godraysVertexShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(godraysVertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(godraysVertexShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nG: Vertex Shader Compilation Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}

		}
	}


	/*****FRAGMENT SHADER*****/

	//create shader
	godraysFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	//provide source code to fragment shader
	const GLchar* godraysFragmentShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"in vec2 out_texcoord; \n" \
		"uniform mat4 u_projection_matrix; \n" \
		"uniform mat4 u_model_matrix; \n" \
		"uniform mat4 u_view_matrix; \n" \
		"uniform float u_exposure; \n" \
		"uniform float u_decay; \n" \
		"uniform float u_density; \n" \
		"uniform float u_weight; \n" \
		"uniform vec4 u_lightPosition; \n" \
		"uniform sampler2D u_occlusionTexture; \n" \
		"const int NUM_SAMPLES = 100; \n" \
		"out vec4 FragColor; \n" \
		"void main(void) \n" \
		"{ \n" \

		"	vec4 lightPos3D = mat4(u_projection_matrix * u_view_matrix * u_model_matrix) * u_lightPosition; \n" \
		"	lightPos3D /= lightPos3D.w; \n" \
		"	lightPos3D += vec4(1.0); \n" \
		"	lightPos3D *= 0.5; \n" \
		"	vec2 sunPosXY = lightPos3D.xy; \n" \

		"	vec2 deltaTexcoord = vec2(out_texcoord.st - sunPosXY); \n" \
		"	vec2 tex = out_texcoord.st; \n" \
		"	deltaTexcoord *= 1.0 / float(NUM_SAMPLES) * u_density; \n" \
		"	float illuminationDecay = 1.0; \n" \

		"	for(int i = 0; i < NUM_SAMPLES; i++) \n" \
		"	{ \n" \
		"		tex -= deltaTexcoord; \n" \
		"		vec4 sampled = texture(u_occlusionTexture, tex); \n" \
		"		sampled *= (illuminationDecay * u_weight); \n" \
		"		FragColor += sampled; \n" \
		"		illuminationDecay *= u_decay; \n" \
		"	} \n" \

		"	FragColor *= u_exposure; \n" \
		"	FragColor = vec4(FragColor.rgb, 1.0); \n" \
		"} \n";

	glShaderSource(godraysFragmentShaderObject, 1, (const GLchar**)&godraysFragmentShaderSourceCode, NULL);

	//compile shader
	glCompileShader(godraysFragmentShaderObject);

	//fragment shader compilation error checking
	infoLogLength = 0;
	shaderCompiledStatus = 0;
	szBuffer = NULL;

	glGetShaderiv(godraysFragmentShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(godraysFragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(godraysFragmentShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nG: Fragment Shader Compilation Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);

			}
		}
	}


	/*****SHADER PROGRAM*****/

	//create
	godraysShaderProgramObject = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(godraysShaderProgramObject, godraysVertexShaderObject);

	//attach fragment shader to shader program
	glAttachShader(godraysShaderProgramObject, godraysFragmentShaderObject);

	//pre-linking binding
	glBindAttribLocation(godraysShaderProgramObject, SPK_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(godraysShaderProgramObject, SPK_ATTRIBUTE_COLOR, "vColor");
	glBindAttribLocation(godraysShaderProgramObject, SPK_ATTRIBUTE_NORMAL, "vNormal");
	glBindAttribLocation(godraysShaderProgramObject, SPK_ATTRIBUTE_TEXCOORD, "vTexcoord");

	//link shader
	glLinkProgram(godraysShaderProgramObject);

	//shader linking error checking
	infoLogLength = 0;
	GLint shaderProgramLinkStatus;
	szBuffer = NULL;

	glGetProgramiv(godraysShaderProgramObject, GL_LINK_STATUS, &shaderProgramLinkStatus);

	if (shaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(godraysShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);

			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(godraysShaderProgramObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nG: Shader Program Link Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}
		}
	}

	//get MVP uniform location
	modelMatrixUniformG = glGetUniformLocation(godraysShaderProgramObject, "u_model_matrix");
	viewMatrixUniformG = glGetUniformLocation(godraysShaderProgramObject, "u_view_matrix");
	projectionMatrixUniformG = glGetUniformLocation(godraysShaderProgramObject, "u_projection_matrix");

	exposureUniformG = glGetUniformLocation(godraysShaderProgramObject, "u_exposure");
	decayUniformG = glGetUniformLocation(godraysShaderProgramObject, "u_decay");
	densityUniformG = glGetUniformLocation(godraysShaderProgramObject, "u_density");
	weightUniformG = glGetUniformLocation(godraysShaderProgramObject, "u_weight");
	lightPositionUniformG = glGetUniformLocation(godraysShaderProgramObject, "u_lightPosition");
	occlusionTextureSamplerUniformG = glGetUniformLocation(godraysShaderProgramObject, "u_occlusionTexture");

}

void initFinalPassForGodraysShader(void)
{
	GLuint finalPassVertexShaderObject;
	GLuint finalPassFragmentShaderObject;

	/*****VERTEX SHADER*****/

	//create shader
	finalPassVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//provide source code to vertex shader
	const GLchar* finalPassVertexShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"in vec4 vPosition; \n" \
		"in vec2 vTexcoord; \n" \
		"out vec2 out_texcoord; \n" \
		"void main(void) \n" \
		"{ \n" \
		"	out_texcoord = vTexcoord; \n" \
		"	gl_Position = vPosition; \n" \
		"} \n";

	glShaderSource(finalPassVertexShaderObject, 1, (const GLchar**)&finalPassVertexShaderSourceCode, NULL);

	//compile shader
	glCompileShader(finalPassVertexShaderObject);

	//vertex shader compilation error checking
	GLint infoLogLength = 0;
	GLint shaderCompiledStatus = 0;
	char* szBuffer = NULL;

	glGetShaderiv(finalPassVertexShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(finalPassVertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(finalPassVertexShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nG: Vertex Shader Compilation Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}

		}
	}


	/*****FRAGMENT SHADER*****/

	//create shader
	finalPassFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	//provide source code to fragment shader
	const GLchar* finalPassFragmentShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"in vec2 out_texcoord; \n" \
		"uniform sampler2D u_pass1Tex; \n" \
		"uniform sampler2D u_pass2Tex; \n" \
		"out vec4 FragColor; \n" \
		"void main(void) \n" \
		"{ \n" \
		"	vec4 result = texture(u_pass1Tex, out_texcoord) + texture(u_pass2Tex, out_texcoord);"
		"	FragColor = vec4(result.rgb, 1.0); \n" \
		"} \n";

	glShaderSource(finalPassFragmentShaderObject, 1, (const GLchar**)&finalPassFragmentShaderSourceCode, NULL);

	//compile shader
	glCompileShader(finalPassFragmentShaderObject);

	//fragment shader compilation error checking
	infoLogLength = 0;
	shaderCompiledStatus = 0;
	szBuffer = NULL;

	glGetShaderiv(finalPassFragmentShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(finalPassFragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(finalPassFragmentShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nG: Fragment Shader Compilation Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);

			}
		}
	}


	/*****SHADER PROGRAM*****/

	//create
	finalPassShaderProgramObject = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(finalPassShaderProgramObject, finalPassVertexShaderObject);

	//attach fragment shader to shader program
	glAttachShader(finalPassShaderProgramObject, finalPassFragmentShaderObject);

	//pre-linking binding
	glBindAttribLocation(finalPassShaderProgramObject, SPK_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(finalPassShaderProgramObject, SPK_ATTRIBUTE_COLOR, "vColor");
	glBindAttribLocation(finalPassShaderProgramObject, SPK_ATTRIBUTE_NORMAL, "vNormal");
	glBindAttribLocation(finalPassShaderProgramObject, SPK_ATTRIBUTE_TEXCOORD, "vTexcoord");

	//link shader
	glLinkProgram(finalPassShaderProgramObject);

	//shader linking error checking
	infoLogLength = 0;
	GLint shaderProgramLinkStatus;
	szBuffer = NULL;

	glGetProgramiv(finalPassShaderProgramObject, GL_LINK_STATUS, &shaderProgramLinkStatus);

	if (shaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(finalPassShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);

			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(finalPassShaderProgramObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nG: Shader Program Link Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}
		}
	}

	//get MVP uniform location
	pass1TextureSamplerUniformF = glGetUniformLocation(finalPassShaderProgramObject, "u_pass1Tex");
	pass2TextureSamplerUniformF = glGetUniformLocation(finalPassShaderProgramObject, "u_pass2Tex");

}

void initSkyboxShader(void)
{
	GLuint skyboxVertexShaderObject;
	GLuint skyboxFragmentShaderObject;

	/*****VERTEX SHADER*****/

//create shader
	skyboxVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//provide source code to vertex shader
	const GLchar* skyboxVertexShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"in vec3 vPosition; \n" \
		"uniform mat4 u_projectionMatrix; \n" \
		"uniform mat4 u_viewMatrix; \n" \
		"out vec3 out_texcoord; \n" \
		"void main(void) \n" \
		"{ \n" \
		"	out_texcoord = vPosition; \n" \
		"	vec4 pos = u_projectionMatrix * u_viewMatrix * vec4(vPosition, 1.0); \n" \
		/*"	vec4 pos = vec4(vPosition, 1.0); \n" \*/
		"	gl_Position = pos.xyww; \n" \
		"} \n";

	glShaderSource(skyboxVertexShaderObject, 1, (const GLchar**)&skyboxVertexShaderSourceCode, NULL);

	//compile shader
	glCompileShader(skyboxVertexShaderObject);

	//vertex shader compilation error checking
	GLint infoLogLength = 0;
	GLint shaderCompiledStatus = 0;
	char* szBuffer = NULL;

	glGetShaderiv(skyboxVertexShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(skyboxVertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(skyboxVertexShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nVertex Shader Compilation Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}

		}
	}


	/*****FRAGMENT SHADER*****/

	//create shader
	skyboxFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	//provide source code to fragment shader
	const GLchar* skyboxFragmentShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"in vec3 out_texcoord; \n" \
		"uniform samplerCube u_texture_sampler; \n" \
		"out vec4 FragColor; \n" \
		"void main(void) \n" \
		"{ \n" \
		"	FragColor = texture(u_texture_sampler, out_texcoord); \n" \
		/*"	FragColor = vec4(0.0, 1.0, 1.0, 1.0); \n" \*/
		"} \n";

	glShaderSource(skyboxFragmentShaderObject, 1, (const GLchar**)&skyboxFragmentShaderSourceCode, NULL);

	//compile shader
	glCompileShader(skyboxFragmentShaderObject);

	//fragment shader compilation error checking
	infoLogLength = 0;
	shaderCompiledStatus = 0;
	szBuffer = NULL;

	glGetShaderiv(skyboxFragmentShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(skyboxFragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(skyboxFragmentShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nFragment Shader Compilation Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);

			}
		}
	}


	/*****SHADER PROGRAM*****/

	//create
	skyboxShaderProgramObject = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(skyboxShaderProgramObject, skyboxVertexShaderObject);

	//attach fragment shader to shader program
	glAttachShader(skyboxShaderProgramObject, skyboxFragmentShaderObject);

	//pre-linking binding
	glBindAttribLocation(skyboxShaderProgramObject, SPK_ATTRIBUTE_POSITION, "vPosition");
	//glBindAttribLocation(shaderProgramObject, SPK_ATTRIBUTE_TEXCOORD, "vTexcoord");

	//link shader
	glLinkProgram(skyboxShaderProgramObject);

	//shader linking error checking
	infoLogLength = 0;
	GLint shaderProgramLinkStatus;
	szBuffer = NULL;

	glGetProgramiv(skyboxShaderProgramObject, GL_LINK_STATUS, &shaderProgramLinkStatus);

	if (shaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(skyboxShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);

			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(skyboxShaderProgramObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nShader Program Link Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}
		}
	}

	//get MVP uniform location
	projectionMatrixUniform = glGetUniformLocation(skyboxShaderProgramObject, "u_projectionMatrix");
	viewMatrixUniform = glGetUniformLocation(skyboxShaderProgramObject, "u_viewMatrix");
	textureSamplerUniform = glGetUniformLocation(skyboxShaderProgramObject, "u_texture_sampler");



}

void initCubemapCubeShader(void)
{
	GLuint cubeVertexShaderObject;
	GLuint cubeFragmentShaderObject;

	/*****VERTEX SHADER CUBE*****/

	//create shader
	cubeVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//provide source code to vertex shader
	const GLchar* cubeVertexShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"in vec4 vPosition; \n" \
		"in vec2 vTexcoord; \n" \
		"uniform mat4 u_mMatrix; \n" \
		"uniform mat4 u_vMatrix; \n" \
		"uniform mat4 u_pMatrix; \n" \
		"out vec2 out_texcoord; \n" \
		"void main(void) \n" \
		"{ \n" \
		"	gl_Position = u_pMatrix * u_vMatrix * u_mMatrix * vPosition; \n" \
		"	out_texcoord = vTexcoord; \n" \
		"} \n";

	glShaderSource(cubeVertexShaderObject, 1, (const GLchar**)&cubeVertexShaderSourceCode, NULL);

	//compile shader
	glCompileShader(cubeVertexShaderObject);

	//vertex shader compilation error checking
	GLint infoLogLength = 0;
	GLint shaderCompiledStatus = 0;
	char* szBuffer = NULL;

	glGetShaderiv(cubeVertexShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(cubeVertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(cubeVertexShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nVertex Shader Compilation Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}

		}
	}


	/*****FRAGMENT SHADER CUBE*****/

	//create shader
	cubeFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	//provide source code to fragment shader
	const GLchar* cubeFragmentShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"in vec2 out_texcoord; \n" \
		"uniform sampler2D u_texture_sampler; \n" \
		"out vec4 FragColor; \n" \
		"void main(void) \n" \
		"{ \n" \
		"	FragColor = texture(u_texture_sampler, out_texcoord); \n" \
		"} \n";

	glShaderSource(cubeFragmentShaderObject, 1, (const GLchar**)&cubeFragmentShaderSourceCode, NULL);

	//compile shader
	glCompileShader(cubeFragmentShaderObject);

	//fragment shader compilation error checking
	infoLogLength = 0;
	shaderCompiledStatus = 0;
	szBuffer = NULL;

	glGetShaderiv(cubeFragmentShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(cubeFragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(cubeFragmentShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nFragment Shader Compilation Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);

			}
		}
	}


	/*****SHADER PROGRAM CUBE*****/

	//create
	cubeShaderProgramObject = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(cubeShaderProgramObject, cubeVertexShaderObject);

	//attach fragment shader to shader program
	glAttachShader(cubeShaderProgramObject, cubeFragmentShaderObject);

	//pre-linking binding
	glBindAttribLocation(cubeShaderProgramObject, SPK_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(cubeShaderProgramObject, SPK_ATTRIBUTE_TEXCOORD, "vTexcoord");

	//link shader
	glLinkProgram(cubeShaderProgramObject);

	//shader linking error checking
	infoLogLength = 0;
	GLint shaderProgramLinkStatus;
	szBuffer = NULL;

	glGetProgramiv(cubeShaderProgramObject, GL_LINK_STATUS, &shaderProgramLinkStatus);

	if (shaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(cubeShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);

			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(cubeShaderProgramObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nShader Program Link Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}
		}
	}

	//get MVP uniform location
	modelMatrixUniformC = glGetUniformLocation(cubeShaderProgramObject, "u_mMatrix");
	viewMatrixUniformC = glGetUniformLocation(cubeShaderProgramObject, "u_vMatrix");
	projectionMatrixUniformC = glGetUniformLocation(cubeShaderProgramObject, "u_pMatrix");
	textureSamplerUniformC = glGetUniformLocation(cubeShaderProgramObject, "u_texture_sampler");

}

void initTextShader(void)
{
	GLuint textVertexShaderObject;
	GLuint textFragmentShaderObject;

	/*****VERTEX SHADER*****/

	//create shader
	textVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//provide source code to vertex shader
	const GLchar* textVertexShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"in vec4 vPosition; \n" \
		"out vec2 texcoords; \n" \
		"uniform mat4 u_mvpMatrix; \n" \
		"void main(void) \n" \
		"{ \n" \
		"	gl_Position = u_mvpMatrix * vec4(vPosition.xy, 0.0, 1.0); \n" \
		"	texcoords = vPosition.zw; \n" \
		"} \n";

	glShaderSource(textVertexShaderObject, 1, (const GLchar**)&textVertexShaderSourceCode, NULL);

	//compile shader
	glCompileShader(textVertexShaderObject);

	//vertex shader compilation error checking
	GLint infoLogLength = 0;
	GLint shaderCompiledStatus = 0;
	char* szBuffer = NULL;

	glGetShaderiv(textVertexShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(textVertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(textVertexShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nVertex Shader Compilation Log : %s\n", szBuffer);
				fflush(gpFile);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}

		}
	}


	/*****FRAGMENT SHADER*****/

	//create shader
	textFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	//provide source code to fragment shader
	const GLchar* textFragmentShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"in vec2 texcoords; \n" \
		"out vec4 FragColor; \n" \
		"uniform sampler2D text; \n" \
		"uniform vec4 textColor; \n" \
		"void main(void) \n" \
		"{ \n" \
		"	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texcoords).r); \n" \
		"	FragColor = textColor * sampled; \n" \
		"} \n";

	glShaderSource(textFragmentShaderObject, 1, (const GLchar**)&textFragmentShaderSourceCode, NULL);

	//compile shader
	glCompileShader(textFragmentShaderObject);

	//fragment shader compilation error checking
	infoLogLength = 0;
	shaderCompiledStatus = 0;
	szBuffer = NULL;

	glGetShaderiv(textFragmentShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(textFragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(textFragmentShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nFragment Shader Compilation Log : %s\n", szBuffer);
				fflush(gpFile);
				free(szBuffer);
				DestroyWindow(ghwnd);

			}
		}
	}


	/*****SHADER PROGRAM*****/

	//create
	textShaderProgramObject = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(textShaderProgramObject, textVertexShaderObject);

	//attach fragment shader to shader program
	glAttachShader(textShaderProgramObject, textFragmentShaderObject);

	//pre-linking binding
	glBindAttribLocation(textShaderProgramObject, SPK_ATTRIBUTE_POSITION, "vPosition");

	//link shader
	glLinkProgram(textShaderProgramObject);

	//shader linking error checking
	infoLogLength = 0;
	GLint shaderProgramLinkStatus;
	szBuffer = NULL;

	glGetProgramiv(textShaderProgramObject, GL_LINK_STATUS, &shaderProgramLinkStatus);

	if (shaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(textShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);

			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(textShaderProgramObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nShader Program Link Log : %s\n", szBuffer);
				fflush(gpFile);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}
		}
	}

	//get MVP uniform location
	mvpMatrixUniform = glGetUniformLocation(textShaderProgramObject, "u_mvpMatrix");
	textSamplerUniform = glGetUniformLocation(textShaderProgramObject, "text");
	textColorUniform = glGetUniformLocation(textShaderProgramObject, "textColor");

	glGenVertexArrays(1, &vao_text);
	glBindVertexArray(vao_text);

	glGenBuffers(1, &vbo_text);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_text);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(SPK_ATTRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
	glEnableVertexAttribArray(SPK_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void initStarfieldShader(void)
{
	GLuint starfieldVertexShaderObject;
	GLuint starfieldFragmentShaderObject;

	/*****VERTEX SHADER*****/

	//create shader
	starfieldVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//provide source code to vertex shader
	const GLchar* starfieldVertexShaderSourceCode =
		"#version 450 core \n" \

		"in vec4 vPosition; \n" \
		"in vec4 vColor; \n" \

		"uniform float time; \n" \
		"uniform mat4 u_projMatrix; \n" \

		"flat out vec4 out_color; \n" \

		"void main(void) \n" \
		"{ \n" \
		"	vec4 newVertex = vPosition; \n" \

		"	newVertex.z += time; \n" \
		"	newVertex.z = fract(newVertex.z); \n" \

		"	float size = (40.0 * newVertex.z * newVertex.z); \n" \

		"	out_color = smoothstep(1.0, 7.0, size) * vColor; \n" \

		"	newVertex.z = (999.9 * newVertex.z) - 1000.0; \n" \
		"	gl_Position = u_projMatrix * newVertex; \n" \
		"	gl_PointSize = size; \n" \

		"} \n";

	glShaderSource(starfieldVertexShaderObject, 1, (const GLchar**)&starfieldVertexShaderSourceCode, NULL);

	//compile shader
	glCompileShader(starfieldVertexShaderObject);

	//vertex shader compilation error checking
	GLint infoLogLength = 0;
	GLint shaderCompiledStatus = 0;
	char* szBuffer = NULL;

	glGetShaderiv(starfieldVertexShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(starfieldVertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(starfieldVertexShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nVertex Shader Compilation Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}

		}
	}


	/*****FRAGMENT SHADER*****/

	//create shader
	starfieldFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	//provide source code to fragment shader
	const GLchar* starfieldFragmentShaderSourceCode =
		"#version 450 core \n" \
		"\n" \
		"flat in vec4 out_color; \n" \

		"uniform sampler2D u_star_texture_sampler; \n" \

		"out vec4 FragColor; \n" \

		"void main(void) \n" \
		"{ \n" \
		"	vec4 texColor = texture(u_star_texture_sampler, gl_PointCoord); \n" \
		"	if(texColor.a < 0.5) \n" \
		"		discard; \n" \
		"	FragColor = out_color * texColor; \n" \
		"} \n";

	glShaderSource(starfieldFragmentShaderObject, 1, (const GLchar**)&starfieldFragmentShaderSourceCode, NULL);

	//compile shader
	glCompileShader(starfieldFragmentShaderObject);

	//fragment shader compilation error checking
	infoLogLength = 0;
	shaderCompiledStatus = 0;
	szBuffer = NULL;

	glGetShaderiv(starfieldFragmentShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);

	if (shaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(starfieldFragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);
			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(starfieldFragmentShaderObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nFragment Shader Compilation Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);

			}
		}
	}


	/*****SHADER PROGRAM*****/

	//create
	starfieldShaderProgramObject = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(starfieldShaderProgramObject, starfieldVertexShaderObject);

	//attach fragment shader to shader program
	glAttachShader(starfieldShaderProgramObject, starfieldFragmentShaderObject);

	//pre-linking binding
	glBindAttribLocation(starfieldShaderProgramObject, SPK_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(starfieldShaderProgramObject, SPK_ATTRIBUTE_COLOR, "vColor");
	//glBindAttribLocation(shaderProgramObject, SPK_ATTRIBUTE_TEXCOORD, "vTexcoord");

	//link shader
	glLinkProgram(starfieldShaderProgramObject);

	//shader linking error checking
	infoLogLength = 0;
	GLint shaderProgramLinkStatus;
	szBuffer = NULL;

	glGetProgramiv(starfieldShaderProgramObject, GL_LINK_STATUS, &shaderProgramLinkStatus);

	if (shaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(starfieldShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szBuffer = (char*)malloc(infoLogLength);

			if (szBuffer != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(starfieldShaderProgramObject, infoLogLength, &written, szBuffer);
				fprintf(gpFile, "\nShader Program Link Log : %s\n", szBuffer);
				free(szBuffer);
				DestroyWindow(ghwnd);
			}
		}
	}

	//get MVP uniform location
	projectionMatrixUniformSF = glGetUniformLocation(starfieldShaderProgramObject, "u_projMatrix");
	textureSamplerUniformSF = glGetUniformLocation(starfieldShaderProgramObject, "u_star_texture_sampler");
	timeUniformSF = glGetUniformLocation(starfieldShaderProgramObject, "time");

}

//void initFullScreenQuad(void)
//{
//	GLuint fullscreenQuadVertexShaderObject;
//	GLuint fullscreenQuadFragmentShaderObject;
//
//	/*****VERTEX SHADER*****/
//
//	//create shader
//	fullscreenQuadVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
//
//	//provide source code to vertex shader
//	const GLchar* fullscreenQuadVertexShaderSourceCode =
//		"#version 450 core \n" \
//		"\n" \
//		"in vec4 vPosition; \n" \
//		"in vec4 vColor; \n" \
//		"out vec4 out_color; \n" \
//		"void main(void) \n" \
//		"{ \n" \
//		"	out_color = vColor; \n" \
//		"	gl_Position = vPosition; \n" \
//		"} \n";
//
//	glShaderSource(fullscreenQuadVertexShaderObject, 1, (const GLchar**)&fullscreenQuadVertexShaderSourceCode, NULL);
//
//	//compile shader
//	glCompileShader(fullscreenQuadVertexShaderObject);
//
//	//vertex shader compilation error checking
//	GLint infoLogLength = 0;
//	GLint shaderCompiledStatus = 0;
//	char* szBuffer = NULL;
//
//	glGetShaderiv(fullscreenQuadVertexShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);
//
//	if (shaderCompiledStatus == GL_FALSE)
//	{
//		glGetShaderiv(fullscreenQuadVertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
//
//		if (infoLogLength > 0)
//		{
//			szBuffer = (char*)malloc(infoLogLength);
//			if (szBuffer != NULL)
//			{
//				GLsizei written;
//				glGetShaderInfoLog(fullscreenQuadVertexShaderObject, infoLogLength, &written, szBuffer);
//				fprintf(gpFile, "\nG: Vertex Shader Compilation Log : %s\n", szBuffer);
//				free(szBuffer);
//				DestroyWindow(ghwnd);
//			}
//
//		}
//	}
//
//
//	/*****FRAGMENT SHADER*****/
//
//	//create shader
//	fullscreenQuadFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
//
//	//provide source code to fragment shader
//	const GLchar* fullscreenQuadFragmentShaderSourceCode =
//		"#version 450 core \n" \
//		"\n" \
//		"in vec4 out_color; \n" \
//		"uniform sampler2D u_pass1Tex; \n" \
//		"uniform sampler2D u_pass2Tex; \n" \
//		"out vec4 FragColor; \n" \
//		"void main(void) \n" \
//		"{ \n" \
//		"	FragColor = out_color; \n" \
//		"} \n";
//
//	glShaderSource(fullscreenQuadFragmentShaderObject, 1, (const GLchar**)&fullscreenQuadFragmentShaderSourceCode, NULL);
//
//	//compile shader
//	glCompileShader(fullscreenQuadFragmentShaderObject);
//
//	//fragment shader compilation error checking
//	infoLogLength = 0;
//	shaderCompiledStatus = 0;
//	szBuffer = NULL;
//
//	glGetShaderiv(fullscreenQuadFragmentShaderObject, GL_COMPILE_STATUS, &shaderCompiledStatus);
//
//	if (shaderCompiledStatus == GL_FALSE)
//	{
//		glGetShaderiv(fullscreenQuadFragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
//
//		if (infoLogLength > 0)
//		{
//			szBuffer = (char*)malloc(infoLogLength);
//			if (szBuffer != NULL)
//			{
//				GLsizei written;
//				glGetShaderInfoLog(fullscreenQuadFragmentShaderObject, infoLogLength, &written, szBuffer);
//				fprintf(gpFile, "\nG: Fragment Shader Compilation Log : %s\n", szBuffer);
//				free(szBuffer);
//				DestroyWindow(ghwnd);
//
//			}
//		}
//	}
//
//
//	/*****SHADER PROGRAM*****/
//
//	//create
//	fullscreenQuadShaderProgramObject = glCreateProgram();
//
//	//attach vertex shader to shader program
//	glAttachShader(fullscreenQuadShaderProgramObject, fullscreenQuadVertexShaderObject);
//
//	//attach fragment shader to shader program
//	glAttachShader(fullscreenQuadShaderProgramObject, fullscreenQuadFragmentShaderObject);
//
//	//pre-linking binding
//	glBindAttribLocation(fullscreenQuadShaderProgramObject, SPK_ATTRIBUTE_POSITION, "vPosition");
//	glBindAttribLocation(fullscreenQuadShaderProgramObject, SPK_ATTRIBUTE_COLOR, "vColor");
//	glBindAttribLocation(fullscreenQuadShaderProgramObject, SPK_ATTRIBUTE_NORMAL, "vNormal");
//	glBindAttribLocation(fullscreenQuadShaderProgramObject, SPK_ATTRIBUTE_TEXCOORD, "vTexcoord");
//
//	//link shader
//	glLinkProgram(fullscreenQuadShaderProgramObject);
//
//	//shader linking error checking
//	infoLogLength = 0;
//	GLint shaderProgramLinkStatus;
//	szBuffer = NULL;
//
//	glGetProgramiv(fullscreenQuadShaderProgramObject, GL_LINK_STATUS, &shaderProgramLinkStatus);
//
//	if (shaderProgramLinkStatus == GL_FALSE)
//	{
//		glGetProgramiv(fullscreenQuadShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
//
//		if (infoLogLength > 0)
//		{
//			szBuffer = (char*)malloc(infoLogLength);
//
//			if (szBuffer != NULL)
//			{
//				GLsizei written;
//				glGetProgramInfoLog(fullscreenQuadShaderProgramObject, infoLogLength, &written, szBuffer);
//				fprintf(gpFile, "\nG: Shader Program Link Log : %s\n", szBuffer);
//				free(szBuffer);
//				DestroyWindow(ghwnd);
//			}
//		}
//	}
//
//}


//uninit
void uninitPickingShader(void)
{
	if (pickingShaderProgramObject)
	{
		glUseProgram(pickingShaderProgramObject);
		GLsizei shaderCount;
		glGetProgramiv(pickingShaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);

		GLuint* pShaders = NULL;
		pShaders = (GLuint*)malloc(sizeof(GLuint) * shaderCount);

		glGetAttachedShaders(pickingShaderProgramObject, shaderCount, &shaderCount, pShaders);

		for (GLsizei i = 0; i < shaderCount; i++)
		{
			glDetachShader(pickingShaderProgramObject, pShaders[i]);
			glDeleteShader(pShaders[i]);
			pShaders[i] = 0;
		}
		free(pShaders);

		glDeleteProgram(pickingShaderProgramObject);
		pickingShaderProgramObject = 0;
		glUseProgram(0);

	}
}

void uninitSceneLightingShader(void)
{
	if (sceneShaderProgramObject)
	{
		glUseProgram(sceneShaderProgramObject);
		GLsizei shaderCount;
		glGetProgramiv(sceneShaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);

		GLuint* pShaders = NULL;
		pShaders = (GLuint*)malloc(sizeof(GLuint) * shaderCount);

		glGetAttachedShaders(sceneShaderProgramObject, shaderCount, &shaderCount, pShaders);

		for (GLsizei i = 0; i < shaderCount; i++)
		{
			glDetachShader(sceneShaderProgramObject, pShaders[i]);
			glDeleteShader(pShaders[i]);
			pShaders[i] = 0;
		}
		free(pShaders);

		glDeleteProgram(sceneShaderProgramObject);
		sceneShaderProgramObject = 0;
		glUseProgram(0);

	}
}

void uninitNoiseShader(void)
{
	if (noiseShaderProgramObject)
	{
		glUseProgram(noiseShaderProgramObject);
		GLsizei shaderCount;
		glGetProgramiv(noiseShaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);

		GLuint* pShaders = NULL;
		pShaders = (GLuint*)malloc(sizeof(GLuint) * shaderCount);

		glGetAttachedShaders(noiseShaderProgramObject, shaderCount, &shaderCount, pShaders);

		for (GLsizei i = 0; i < shaderCount; i++)
		{
			glDetachShader(noiseShaderProgramObject, pShaders[i]);
			glDeleteShader(pShaders[i]);
			pShaders[i] = 0;
		}
		free(pShaders);

		glDeleteProgram(noiseShaderProgramObject);
		noiseShaderProgramObject = 0;
		glUseProgram(0);

	}
}

void uninitGodraysShader(void)
{
	if (godraysShaderProgramObject)
	{
		glUseProgram(godraysShaderProgramObject);
		GLsizei shaderCount;
		glGetProgramiv(godraysShaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);

		GLuint* pShaders = NULL;
		pShaders = (GLuint*)malloc(sizeof(GLuint) * shaderCount);

		glGetAttachedShaders(godraysShaderProgramObject, shaderCount, &shaderCount, pShaders);

		for (GLsizei i = 0; i < shaderCount; i++)
		{
			glDetachShader(godraysShaderProgramObject, pShaders[i]);
			glDeleteShader(pShaders[i]);
			pShaders[i] = 0;
		}
		free(pShaders);

		glDeleteProgram(godraysShaderProgramObject);
		godraysShaderProgramObject = 0;
		glUseProgram(0);

	}
}

void uninitFinalPassForGodraysShader(void)
{
	if (finalPassShaderProgramObject)
	{
		glUseProgram(finalPassShaderProgramObject);
		GLsizei shaderCount;
		glGetProgramiv(finalPassShaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);

		GLuint* pShaders = NULL;
		pShaders = (GLuint*)malloc(sizeof(GLuint) * shaderCount);

		glGetAttachedShaders(finalPassShaderProgramObject, shaderCount, &shaderCount, pShaders);

		for (GLsizei i = 0; i < shaderCount; i++)
		{
			glDetachShader(finalPassShaderProgramObject, pShaders[i]);
			glDeleteShader(pShaders[i]);
			pShaders[i] = 0;
		}
		free(pShaders);

		glDeleteProgram(finalPassShaderProgramObject);
		finalPassShaderProgramObject = 0;
		glUseProgram(0);

	}

}

void uninitSkyboxShader(void)
{
	if (skyboxShaderProgramObject)
	{
		glUseProgram(skyboxShaderProgramObject);
		GLsizei shaderCount;
		glGetProgramiv(skyboxShaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);

		GLuint* pShaders = NULL;
		pShaders = (GLuint*)malloc(sizeof(GLuint) * shaderCount);

		glGetAttachedShaders(skyboxShaderProgramObject, shaderCount, &shaderCount, pShaders);

		for (GLsizei i = 0; i < shaderCount; i++)
		{
			glDetachShader(skyboxShaderProgramObject, pShaders[i]);
			glDeleteShader(pShaders[i]);
			pShaders[i] = 0;
		}
		free(pShaders);

		glDeleteProgram(skyboxShaderProgramObject);
		skyboxShaderProgramObject = 0;
		glUseProgram(0);

	}
}

void uninitCubemapCubeShader(void)
{
	if (cubeShaderProgramObject)
	{
		glUseProgram(cubeShaderProgramObject);
		GLsizei shaderCount;
		glGetProgramiv(cubeShaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);

		GLuint* pShaders = NULL;
		pShaders = (GLuint*)malloc(sizeof(GLuint) * shaderCount);

		glGetAttachedShaders(cubeShaderProgramObject, shaderCount, &shaderCount, pShaders);

		for (GLsizei i = 0; i < shaderCount; i++)
		{
			glDetachShader(cubeShaderProgramObject, pShaders[i]);
			glDeleteShader(pShaders[i]);
			pShaders[i] = 0;
		}
		free(pShaders);

		glDeleteProgram(cubeShaderProgramObject);
		cubeShaderProgramObject = 0;
		glUseProgram(0);

	}
}

void uninitTextShader(void)
{
	/*****SAFE SHADER CLEAN-UP*****/

	if (textShaderProgramObject)
	{
		glUseProgram(textShaderProgramObject);
		GLsizei shaderCount;
		glGetProgramiv(textShaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);

		GLuint* pShaders = NULL;
		pShaders = (GLuint*)malloc(sizeof(GLuint) * shaderCount);

		glGetAttachedShaders(textShaderProgramObject, shaderCount, &shaderCount, pShaders);

		for (GLsizei i = 0; i < shaderCount; i++)
		{
			glDetachShader(textShaderProgramObject, pShaders[i]);
			glDeleteShader(pShaders[i]);
			pShaders[i] = 0;
		}
		free(pShaders);

		glDeleteProgram(textShaderProgramObject);
		textShaderProgramObject = 0;
		glUseProgram(0);

	}
}

void uninitStarfieldShader(void)
{
	if (starfieldShaderProgramObject)
	{
		glUseProgram(starfieldShaderProgramObject);
		GLsizei shaderCount;
		glGetProgramiv(starfieldShaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);

		GLuint* pShaders = NULL;
		pShaders = (GLuint*)malloc(sizeof(GLuint) * shaderCount);

		glGetAttachedShaders(starfieldShaderProgramObject, shaderCount, &shaderCount, pShaders);

		for (GLsizei i = 0; i < shaderCount; i++)
		{
			glDetachShader(starfieldShaderProgramObject, pShaders[i]);
			glDeleteShader(pShaders[i]);
			pShaders[i] = 0;
		}
		free(pShaders);

		glDeleteProgram(starfieldShaderProgramObject);
		starfieldShaderProgramObject = 0;
		glUseProgram(0);

	}

}

//void uninitFullScreenQuad(void)
//{
//	/*****SAFE SHADER CLEAN-UP*****/
//
//	if (fullscreenQuadShaderProgramObject)
//	{
//		glUseProgram(fullscreenQuadShaderProgramObject);
//		GLsizei shaderCount;
//		glGetProgramiv(fullscreenQuadShaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);
//
//		GLuint* pShaders = NULL;
//		pShaders = (GLuint*)malloc(sizeof(GLuint) * shaderCount);
//
//		glGetAttachedShaders(fullscreenQuadShaderProgramObject, shaderCount, &shaderCount, pShaders);
//
//		for (GLsizei i = 0; i < shaderCount; i++)
//		{
//			glDetachShader(fullscreenQuadShaderProgramObject, pShaders[i]);
//			glDeleteShader(pShaders[i]);
//			pShaders[i] = 0;
//		}
//		free(pShaders);
//
//		glDeleteProgram(fullscreenQuadShaderProgramObject);
//		fullscreenQuadShaderProgramObject = 0;
//		glUseProgram(0);
//
//	}
//}
