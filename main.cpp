//SPDX-License-Identifier: BSD-3-Clause
//SPDX-FileCopyrightText: 2020 Lorenzo Cauli (lorecast162)

//global includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>

//local includes
#include <stb_image/stb_image.h>
#include <shader.h>
#include <camera.h>
#include <model.h>

unsigned int screenWidth = 1024;
unsigned int screenHeight = 768;
#define WINDOW_TITLE "Succhiapalle"

#define RESPATH "./res"

//vertices for our lil coote triangle👉👈
float vertices[] = {
	//big rect
	//x     y     z      r     g     b     s     t
//face3
	 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//face1
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//face4
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//face5                                              
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//face2
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//face6
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
	//small tri
//	 0.7f,  0.8f, 0.0f, 1.0f, 0.0f, 0.0f,
//	 0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
//	 0.8f,  0.7f, 0.0f, 0.0f, 0.0f, 1.0f,
};

unsigned int indices[] = {
	0,1,3,	//first  tri
	0,2,3,	//second tri

	4,5,7,	//first  tri
	4,6,7,	//second tri
	
	8,9,11,
	8,10,11,
	
	12,13,15,
	12,14,15,

	16,17,19,
	16,18,19,

	20,21,23,
	20,22,23,
	//4,5,6,  //third tri
};

float tex2Vertices[] = {
	 0.5f,  0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
};

unsigned int tex2Indexes[] = {
	0, 1, 3,
	0, 2, 3,
};

float tex3Vertices[] = {
	 0.5f,  -0.5f,  -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  -1.0f,  -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f,
    -0.5f,  -0.5f,  -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f,  -1.0f,  -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.5f,
};

unsigned int tex3Indexes[] = {
	0, 1, 3,
	0, 2, 3,
};

float crosshairVertices[] = {
    -0.01f,   0.05f,   0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.01f,  -0.05f,   0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.01f,   0.05f,   0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 0.01f,  -0.05f,   0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
} ;

unsigned int crosshairIndexes[] = {
	0, 1, 3,
	0, 2, 3,
};

float r = 0.0f;
float g = 0.0f;
float b = 0.0f;
float w = 1.0f;

float angle = 0.0f;
float scale = 0.4f;
float scaleInc = 0.004f;
float angleInc = 2.0f;

float projZ =0.0f;

//bool autoRotate = false;

float deltaTime = 0.0f;

uint64_t now;
uint64_t last;

uint32_t lastClock = -1;
unsigned int pastSeconds = 0;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void errorHandler(int ec, const char* msg);

float clamp(float d, float min, float max) {
    const float t = d < min ? min : d;
    return t > max ? max : t;
}

int main() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Error initializing SDL: \n%s\n", SDL_GetError());
		return -1;
	}
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		fprintf(stderr, "Error initializing SDL_Mixer: \n%s\n", SDL_GetError());
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	//create a screenWidthxscreenHeight window with title WINDOW_TITLE, then make it the current OGL context.
	//exit if errored
	SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!window) {
		fprintf(stderr, "Error creating SDL window: \n%s\n", SDL_GetError());
		return -2;
	}
	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (!context) {
		fprintf(stderr, "Error creating OpenGL context: \n%s\n", SDL_GetError());
		return -3;
	}

	SDL_GL_SetSwapInterval(1);

	//macOS required workaround
#if defined(__APPLE__) && defined(TARGET_OS_MAC)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

	//set up GLAD function loader
	if (!gladLoadGLLoader( (GLADloadproc)SDL_GL_GetProcAddress)) {
		fprintf(stderr, "Error initializing GLAD.\n");
		return -4;
	}

	//set viewport start coords and width,height.
	//in this case it'll have the whole window
	glViewport(0, 0, screenWidth, screenHeight);

//	SDL_SetRelativeMouseMode(SDL_TRUE);

	//generate vertex buffer object and vertex array object
	unsigned int VBO;
	unsigned int VAO;

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	//generate element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	//bind VAO to vertex arrays.
	glBindVertexArray(VAO);

	//bind VBO buffer to the GL_ARRAY_BUFFER, which we'll use as vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//transfer verts array into VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//bind EBO to GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//transfer indices to element buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set vertex coords position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//set vertex color position attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//set vertex texture position attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//generate vertex buffer object and vertex array object
	unsigned int VBO2;
	unsigned int VAO2;

	glGenBuffers(1, &VBO2);
	glGenVertexArrays(1, &VAO2);

	//generate element buffer object
	unsigned int EBO2;
	glGenBuffers(1, &EBO2);

	//bind VAO2 to vertex arrays.
	glBindVertexArray(VAO2);

	//bind VBO2 buffer to the GL_ARRAY_BUFFER, which we'll use as vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);

	//transfer verts array into VBO2 buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex2Vertices), tex2Vertices, GL_STATIC_DRAW);

	//bind EBO2 to GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);

	//transfer indices to element buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tex2Indexes), tex2Indexes, GL_STATIC_DRAW);

	//set vertex coords position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//set vertex color position attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//set vertex texture position attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//generate vertex buffer object and vertex array object
	unsigned int VBO3;
	unsigned int VAO3;

	glGenBuffers(1, &VBO3);
	glGenVertexArrays(1, &VAO3);

	//generate element buffer object
	unsigned int EBO3;
	glGenBuffers(1, &EBO3);

	//bind VAO3 to vertex arrays.
	glBindVertexArray(VAO3);

	//bind VBO3 buffer to the GL_ARRAY_BUFFER, which we'll use as vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);

	//transfer verts array into VBO3 buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex3Vertices), tex3Vertices, GL_STATIC_DRAW);

	//bind EBO3 to GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);

	//transfer indices to element buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tex3Indexes), tex3Indexes, GL_STATIC_DRAW);

	//set vertex coords position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//set vertex color position attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//set vertex texture position attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//generate vertex buffer object and vertex array object
	unsigned int VBO4;
	unsigned int VAO4;

	glGenBuffers(1, &VBO4);
	glGenVertexArrays(1, &VAO4);

	//generate element buffer object
	unsigned int EBO4;
	glGenBuffers(1, &EBO4);

	//bind VAO3 to vertex arrays.
	glBindVertexArray(VAO4);

	//bind VBO3 buffer to the GL_ARRAY_BUFFER, which we'll use as vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO4);

	//transfer verts array into VBO3 buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

	//bind EBO3 to GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);

	//transfer indices to element buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(crosshairIndexes), crosshairIndexes, GL_STATIC_DRAW);

	//set vertex coords position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//set vertex color position attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//set vertex texture position attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

    //create shader program to render poly
	Shader myShader(RESPATH"/shaders/vertex/simple.vs", RESPATH"/shaders/fragment/simple.fs");
	Shader myShader2(RESPATH"/shaders/vertex/crosshair.vs", RESPATH"/shaders/fragment/crosshair.fs");
	Shader modelShader(RESPATH"/shaders/vertex/model.vs", RESPATH"/shaders/fragment/model.fs");
	//Shader myShader2(RESPATH"/shaders/vertex/simple.vs", RESPATH"/shaders/fragment/simple.fs");

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	int w,h,nChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char *textureData = stbi_load(RESPATH"/textures/sburnias.png", &w, &h, &nChannels, 0);
	if (textureData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		fprintf(stderr, "Texture load error.\n");
	}
	stbi_image_free(textureData);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	textureData = stbi_load(RESPATH"/textures/risi.jpg", &w, &h, &nChannels, 0);
	if (textureData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		fprintf(stderr, "Texture load error.\n");
	}
	stbi_image_free(textureData);

	unsigned int texture3;
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	textureData = stbi_load(RESPATH"/textures/caca.png", &w, &h, &nChannels, 0);
	if (textureData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		fprintf(stderr, "Texture load error.\n");
	}
	stbi_image_free(textureData);

	unsigned int texture4;
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	textureData = stbi_load(RESPATH"/textures/cock.png", &w, &h, &nChannels, 0);
	if (textureData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		fprintf(stderr, "Texture load error.\n");
	}
	stbi_image_free(textureData);

	unsigned int texture5;
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	textureData = stbi_load(RESPATH"/textures/crosshair.png", &w, &h, &nChannels, 0);
	if (textureData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		fprintf(stderr, "Texture load error.\n");
	}
	stbi_image_free(textureData);

	myShader.use();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Mix_Music* music = Mix_LoadMUS(RESPATH"/music/bgm.mp3");
	if (!music) {
		fprintf(stderr, "Music load error: \n%s\n", SDL_GetError());
	}

	Mix_PlayMusic(music, -1);
	Mix_VolumeMusic(20);

	Mix_Chunk* sample = Mix_LoadWAV(RESPATH"/sfx/shoot.ogg");
	Mix_Chunk* succ = Mix_LoadWAV(RESPATH"/sfx/succ.ogg");
	
	SDL_Event event;

	bool shouldClose = false;

	now = SDL_GetPerformanceCounter();
	last = 0;
	deltaTime = 0;

	Model chick(RESPATH"/models/chick.dae");

	bool mouseGrab = false;

	//main render loop
	while (!shouldClose) {

		last = now;
		now = SDL_GetPerformanceCounter();

		deltaTime = ((now - last)*1000 / (double)SDL_GetPerformanceFrequency() );
		deltaTime /= 1000;

		uint32_t flags = SDL_GetWindowFlags(window);

		//clear to dark aqua-ish color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					shouldClose = true;
					break;
				case SDL_MOUSEMOTION:
					if (flags & SDL_WINDOW_INPUT_FOCUS)
						if (mouseGrab)
							camera.ProcessMouseMovement(event.motion.xrel, event.motion.yrel * -1);
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
						shouldClose = true;
					else if (event.key.keysym.sym == SDLK_m) {
						if (mouseGrab) {
							mouseGrab = false;
						}
						else {
							mouseGrab = true;
						}

					}
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT && flags & SDL_WINDOW_INPUT_FOCUS)
						if (mouseGrab)
							Mix_PlayChannel(-1, sample, 0);
					break;
			}
		}
		if (mouseGrab) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}
		else {
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}

		const uint8_t* state = SDL_GetKeyboardState(NULL);

		if (mouseGrab) {
			if (state[SDL_SCANCODE_W]) {
				camera.ProcessKeyboard(FORWARD, deltaTime);
			}
			else if (state[SDL_SCANCODE_S]) {
				camera.ProcessKeyboard(BACKWARD, deltaTime);
			}
			if (state[SDL_SCANCODE_A]) {
				camera.ProcessKeyboard(LEFT, deltaTime);
			}
			else if (state[SDL_SCANCODE_D]) {
				camera.ProcessKeyboard(RIGHT, deltaTime);
			}
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, texture5);

		myShader.use();

		glm::mat4 trans;

//cube
		trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, glm::radians(angle), glm::vec3(1.0, 0.0, 0.0));
		trans = glm::rotate(trans, glm::radians(angle * 0.3f), glm::vec3(0.0, 1.0, 0.0));
		trans = glm::rotate(trans, glm::radians(angle + 2.0f), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(scale, scale, scale));

		glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		myShader.setMat4("transform", trans);
		myShader.setMat4("model", model);
		myShader.setMat4("view", view);
		myShader.setMat4("projection", projection);

		myShader.setInt("mytexture", 0);

		//draw triangle
		myShader.use();

		//get `coolColor` uniform location to inject r,g,b,w vars into
		//int offsetLocation = glGetUniformLocation(shaderProgram, "coolColor");
		//glUniform4f(offsetLocation, r, g, b, w);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

//cock
		modelShader.use();

		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, -1.0f));
		trans = glm::scale(trans, glm::vec3(0.02f, 0.02f, 0.02f));
		trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		modelShader.setMat4("transform", trans);

		modelShader.setMat4("model", model);
		modelShader.setMat4("view", view);
		modelShader.setMat4("projection", projection);

		chick.Draw(modelShader);

//skybox
		myShader.use();
		myShader.setMat4("model", model);
		myShader.setMat4("view", view);
		myShader.setMat4("projection", projection);

		trans = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 20.0f, 20.0f));
		myShader.setMat4("transform", trans);
		myShader.setInt("mytexture", 3);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


//bottom thing
		myShader.setMat4("transform", glm::mat4(1.0f));

		myShader.setInt("mytexture", 1);

		myShader.use();

		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

//top sign thing		
		trans = glm::mat4(1.0f);
		//trans = glm::rotate(trans, (float)sin(/*glfwGetTime()*/1), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
		trans = glm::rotate(trans, sin(glm::radians(angle)), glm::vec3(0.0, 0.0, 1.0));
		myShader.setMat4("transform", trans);

		myShader.setInt("mytexture", 2);

		myShader.use();

		glBindVertexArray(VAO3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		

//crosshair
		//myShader2.setMat4("transform", glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
		myShader2.use();

		myShader2.setMat4("transform", glm::mat4(1.0f));
		myShader2.setInt("mytexture", 4);

		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glm::mat4 tr2 = glm::mat4(1.0f);
		tr2 = glm::scale(tr2, glm::vec3(1.0f, (float) screenWidth / screenHeight, 1.0f ));
		tr2 = glm::rotate(tr2, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		myShader2.setMat4("transform", tr2);
		myShader2.setInt("mytexture", 4);

		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//swap buffers to present to screen
		//glfwSwapBuffers(window);
		SDL_GL_SwapWindow(window);
		
		angle += angleInc;
		//scale += scaleInc;
		//if (scale >= 1.3f || scale <= 0.4f) scaleInc *= -1.0f;

		if (rand() % 256 == 0)
			Mix_PlayChannel(-1, succ, 0);

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);

	glDeleteVertexArrays(1, &VAO3);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &EBO3);

	glDeleteVertexArrays(1, &VAO4);
	glDeleteBuffers(1, &VBO4);
	glDeleteBuffers(1, &EBO4);

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);

	Mix_FreeMusic(music);
	Mix_CloseAudio();

	SDL_Quit();
	return 0;
}