#include <SDL2/SDL.h>
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <fstream>
#include <filesystem>

// see
// https://docs.gl
// https://www.youtube.com/c/MikeShah

// Globals
int gScreenHeight = 600;
int gScreenWidth = 800;

SDL_Window *gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext;
bool gQuit = false;

// VAO
GLuint gVertexArrayObject = 0;
// VBO
GLuint gVertexBufferObject = 0;
GLuint gVertexBufferObject2 = 0;
// Program Object (for shaders) ie graphics pipeline
GLuint gGraphicsPipelineShaderProgram = 0;


std::string LoadShaderAsString(const std::string& relativePath) {
  // program directory
  auto path = std::filesystem::canonical("/proc/self/exe").parent_path();
  auto projectDir = path / "..";
  auto filepath = std::filesystem::canonical(projectDir / relativePath);
  std::string result = "";
  std::ifstream file(filepath.c_str(), std::ifstream::in);

  if (!file.is_open()) {
    std::cout << "can't open file \"" << filepath << "\"" << std::endl;
    return result;
  }
  for (std::string line; std::getline(file, line); ) {
    result += line + "\n";
  }
  file.close();
  return result;
}

void GetOpenGLVersionInfo() {
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

}

void InitializeProgram() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    puts("SDL2 could not initialize video subsystem");
    exit(1);
  }
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6); // 1 for mac?
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  // SDL_GL_CONTEXT_PROFILE_ES
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 2);
  int flags = SDL_WINDOW_OPENGL;
  gGraphicsApplicationWindow = SDL_CreateWindow("OpenGL Window", 0, 0,
    gScreenWidth, gScreenHeight, flags);
  if (gGraphicsApplicationWindow == nullptr) {
    puts("SDL_CreateWindow error!");
    exit(2);
  }
  gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);
  if (gOpenGLContext == nullptr) {
    std::cout << "OpenGL context not available" << std::endl;
    exit(3);
  }
  if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
    std::cout << "glad was not initialized" << std:: endl;
    exit(4);
  }
  GetOpenGLVersionInfo();
}

void VertexSpecification() {
  const std::vector<GLfloat> vertexData {
    // x|r   y|g   z|b
    -0.8f, -0.8f, 0.0f,  // vertex 1
    1.0f, 0.0f, 0.0f,    // red
    0.8f,  -0.8f, 0.0f,  // vertex 2
    0.8f, 1.0f, 0.0f,    // green
    0.0f, 0.8f, 0.0f,    // vertex 3
    0.0f, 0.0f, 1.0f     // blue
  };

  // generate VBA
  glGenVertexArrays(1, &gVertexArrayObject);
  glBindVertexArray(gVertexArrayObject);

  // start generating VBOs
  // position
  glGenBuffers(1, &gVertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER,
               vertexData.size() * sizeof(GLfloat),
               vertexData.data(),
               GL_STATIC_DRAW);

  GLsizei stride = sizeof(GLfloat) * 6;
  glEnableVertexAttribArray(0); // 0 is the position x,y,z
  glVertexAttribPointer(0, // index
                        3, // size (3, ie x,y,z)
                        GL_FLOAT, // type
                        GL_FALSE, // normalized? True could work
                        stride, // space in between attributes
                        (void*)0); // offset?
  glEnableVertexAttribArray(1); // 1 is the color r,g,b
  glVertexAttribPointer(1, // index
                        3, // size (3, ie r,g,b)
                        GL_FLOAT, // type
                        GL_FALSE, // normalized? True could work
                        stride, // space in between attributes
                        (GLvoid*) (sizeof(GLfloat) *3)); // offset?

}

void VertexSpecification_using_2_arrays() {
  // lives on the cpu
  const std::vector<GLfloat> vertexPosition {
    // x    y    z
    -0.8f, -0.8f, 0.0f, // vertex 1
    0.8f,  -0.8f, 0.0f, // vertex 2
    0.0f, 0.8f, 0.0f    // vertex 3
  };

  const std::vector<GLfloat> vertexColor {
    // r    g    b
    1.0f, 0.0f, 0.0f, // red
    0.8f, 1.0f, 0.0f, // green
    0.0f, 0.0f, 1.0f  // blue
  };


  // generate VBA
  glGenVertexArrays(1, &gVertexArrayObject);
  glBindVertexArray(gVertexArrayObject);

  // start generating VBOs
  // position
  glGenBuffers(1, &gVertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER,
               vertexPosition.size() * sizeof(GLfloat),
               vertexPosition.data(),
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0); // 0 is the position (only one attribute)
  glVertexAttribPointer(0, // index
                        3, // size (3, ie x,y,z)
                        GL_FLOAT, // type
                        GL_FALSE, // normalized? True could work
                        0, // space in between attributes
                        (void*)0); // offset?

  // color
  glGenBuffers(1, &gVertexBufferObject2);
  glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject2);
  glBufferData(GL_ARRAY_BUFFER,
               vertexColor.size() * sizeof(GLfloat),
               vertexColor.data(),
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1,
                        3, // r,g,b
                        GL_FLOAT,
                        GL_FALSE,
                        0,
                        (void*)0);


  // cleanup
  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

GLuint CompileShader(GLuint type, const std::string &source){
  GLuint shaderObject = 0;
  if (type == GL_VERTEX_SHADER) {
    shaderObject = glCreateShader(GL_VERTEX_SHADER);
  }
  else if (type == GL_FRAGMENT_SHADER) {
    shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
  }
  const char* src = source.c_str();
  glShaderSource(shaderObject, 1, &src, nullptr);
  glCompileShader(shaderObject);
  // retrieve result
  int result;
  glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
    char *errorMessages = new char[length];
    glGetShaderInfoLog(shaderObject, length, &length, errorMessages);
    if (type == GL_VERTEX_SHADER) {
      std::cout << "ERROR: GL_VERTEX_SHADER compilation failed!\n" << errorMessages << "\n";
    } else if (type == GL_FRAGMENT_SHADER) {
      std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed\n" << errorMessages << "\n";
    }
    delete[] errorMessages;
  }
  return shaderObject;
}

GLuint CreateShaderProgram(const std::string &vertexShaderSource,
                         const std::string &fragmentShaderSource) {
  GLuint programObject = glCreateProgram();
  GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
  GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  glAttachShader(programObject, myVertexShader);
  glAttachShader(programObject, myFragmentShader);
  glLinkProgram(programObject);

  // validate the program
  glValidateProgram(programObject);
  // glDetachShader, glDeleteShader
  return programObject;
}

void CreateGraphicsPipeline() {
  std::string vertexShaderSource = LoadShaderAsString("./shaders/vert.glsl");
  std::string fragmentShaderSource = LoadShaderAsString("./shaders/frag.glsl");
  gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource,
    fragmentShaderSource);
}

void Input() {
  SDL_Event e;
  while (SDL_PollEvent(&e) !=0) {
    if (e.type == SDL_QUIT) {
      std::cout << "Goodbye!" << std::endl;
      gQuit = true;
    }
  }
}

void PreDraw() {
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glViewport(0,0, gScreenWidth, gScreenHeight);
  glClearColor(1.f, 1.f, 0.f, 1.f);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glUseProgram(gGraphicsPipelineShaderProgram);
}

void Draw() {
  glBindVertexArray(gVertexArrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glUseProgram(0);
}

void MainLoop() {
  while(!gQuit) {
    Input();
    PreDraw();
    Draw();
    SDL_GL_SwapWindow(gGraphicsApplicationWindow);
  }
}

void Cleanup() {
  SDL_DestroyWindow(gGraphicsApplicationWindow);
  SDL_Quit();
}


int main() {
  InitializeProgram();
  VertexSpecification();
  CreateGraphicsPipeline();
  MainLoop();
  Cleanup();
  return 0;
}
