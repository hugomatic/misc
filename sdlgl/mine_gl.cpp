#include <SDL2/SDL.h>
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <fstream>
#include <filesystem>
#include <tuple>

#include "mine.h"

// see
// https://docs.gl
// https://www.youtube.com/c/MikeShah

// Globals
map_t gMap;
layout_t gLayout {rows, cols, -0.9, 0.9, 0.04, -0.04, 0.001, -0.001};

int gScreenWidth = 1280;
int gScreenHeight = 1280;

int gTriangleCount = 6;

SDL_Window *gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext;
bool gQuit = false;

// VAO
GLuint gVertexArrayObject = 0;
// VBO
GLuint gVertexBufferObject = 0;
GLuint gVertexBufferObject2 = 0;
// IBO store vertex indices when drawing
GLuint gIndexBufferObject = 0;


// Program Object (for shaders) ie graphics pipeline
GLuint gGraphicsPipelineShaderProgram = 0;


// error handling
static void GLClearAllErrors() {
  while(glGetError() != GL_NO_ERROR) {

  }
}

// true if we have an error
static bool GLCheckErrorStatus(const char* function, int line) {
  while(GLenum error = glGetError()) {
    std::cout << "OpenGL Error: " << error
              << "\tline: " << line
              << "\tfunction: " << function << std::endl;
    return true;
  }
  return false;
}

// error codes are in hex
// https://www.khronos.org/opengl/wiki/OpenGL_Error#Meaning_of_errors
#define GLCHECK(x) do{GLClearAllErrors(); x; GLCheckErrorStatus(#x, __LINE__); }while(0);


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
  gGraphicsApplicationWindow = SDL_CreateWindow("Mine sweeper", 0, 0,
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

/*
void VertexSpecification() {
  const std::vector<GLfloat> vertexData {
    // x|r   y|g   z|b
    // 0 vertex
    -0.5f, -0.5f, 0.0f,  // left vertex
     1.0f,  0.0f, 0.0f,  // red
    // 1 vertex
     0.5f, -0.5f, 0.0f,  // right vertex
     0.0f,  1.0f, 0.0f,  // green
    // 2 vertex
    -0.5f,  0.5f, 0.0f,  // top vertex
     0.0f,  0.0f, 1.0f,  // blue
    // second triangle
    // 3 vertex
     0.5f,  0.5f, 0.0f,  // top right
     1.0f,  0.0f, 0.0f,  // red
  };

  const std::vector<GLuint> indexData {2,0,1,3,2,1};

}
*/


/*

void VertexSpecification() {

  std::vector<GLfloat> vertexData;
  std::vector<GLuint> indexData;

  size_t i=0;
  size_t quadCount = 0;
  for (size_t r=0; r < rows; ++r) {
    for (size_t c=0; c < cols; ++c) {

      double tx, ty, z, bx, by;
      z = 0;
      gLayout.topLeft(r, c, tx, ty);
      gLayout.bottomRight(r, c, bx, by);

      char v = gMap[r][c];
      double r, g, b;
      bool gameOver = false;
      gLayout.color(v,gameOver, r, g, b);
      // left vertex
      vertexData.push_back(tx);
      vertexData.push_back(by);
      vertexData.push_back(z);
      // red
      vertexData.push_back(r);
      vertexData.push_back(g);
      vertexData.push_back(b);
      // right vertex
      vertexData.push_back(bx);
      vertexData.push_back(by);
      vertexData.push_back(z);
      // green
      vertexData.push_back(0.0);
      vertexData.push_back(1.0);
      vertexData.push_back(0.0);
      // top vertex
      vertexData.push_back(tx);
      vertexData.push_back(ty);
      vertexData.push_back(z);
      // blue
      vertexData.push_back(0.0);
      vertexData.push_back(0.0);
      vertexData.push_back(1.0);
      // top right vertex
      vertexData.push_back(bx);
      vertexData.push_back(ty);
      vertexData.push_back(z);
      // red
      vertexData.push_back(r);
      vertexData.push_back(g);
      vertexData.push_back(b);

      // indices
      indexData.push_back(4 * quadCount + 2);
      indexData.push_back(4 * quadCount + 0);
      indexData.push_back(4 * quadCount + 1);
      indexData.push_back(4 * quadCount + 3);
      indexData.push_back(4 * quadCount + 2);
      indexData.push_back(4 * quadCount + 1);

      quadCount ++;
    }
  }


*/

void UpdateVertexData(map_t const& map,
                      std::vector<GLfloat> &vertexData,
                      std::vector<GLuint> &indexData)
{
  size_t quadCount = 0;
  for (size_t r=0; r < rows; ++r) {
    for (size_t c=0; c < cols; ++c) {

      double tx, ty, z, bx, by;
      z = 0;
      gLayout.topLeft(r, c, tx, ty);
      gLayout.bottomRight(r, c, bx, by);

      char v = gMap[r][c];
      double r, g, b;
      bool gameOver = false;
      gLayout.color(v,gameOver, r, g, b);
      // left vertex
      vertexData[quadCount * 24 + 0] = tx;
      vertexData[quadCount * 24 + 1] = by;
      vertexData[quadCount * 24 + 2] = z;
      // red
      vertexData[quadCount * 24 + 3] = r;
      vertexData[quadCount * 24 + 4] = g;
      vertexData[quadCount * 24 + 5] = b;
      // right vertex
      vertexData[quadCount * 24 + 6] = bx;
      vertexData[quadCount * 24 + 7] = by;
      vertexData[quadCount * 24 + 8] = z;
      // green
      vertexData[quadCount * 24 + 9] = 0.0;
      vertexData[quadCount * 24 + 10] = 1.0;
      vertexData[quadCount * 24 + 11] = 0.0;
      // top vertex
      vertexData[quadCount * 24 + 12] = tx;
      vertexData[quadCount * 24 + 13] = ty;
      vertexData[quadCount * 24 + 14] = z;
      // blue
      vertexData[quadCount * 24 + 15] = 0.0;
      vertexData[quadCount * 24 + 16] = 0.0;
      vertexData[quadCount * 24 + 17] = 1.0;
      // top right vertex
      vertexData[quadCount * 24 + 18] = bx;
      vertexData[quadCount * 24 + 19] = ty;
      vertexData[quadCount * 24 + 20] = z;
      // red
      vertexData[quadCount * 24 + 21] = r;
      vertexData[quadCount * 24 + 22] = g;
      vertexData[quadCount * 24 + 23] = b;

      // indices
      indexData[quadCount * 6 + 0] = 4 * quadCount + 2;
      indexData[quadCount * 6 + 1] = 4 * quadCount + 0;
      indexData[quadCount * 6 + 2] = 4 * quadCount + 1;
      indexData[quadCount * 6 + 3] = 4 * quadCount + 3;
      indexData[quadCount * 6 + 4] = 4 * quadCount + 2;
      indexData[quadCount * 6 + 5] = 4 * quadCount + 1;

      quadCount ++;
    }
  }
}

void VertexSpecification() {
  std::vector<GLfloat> vertexData;
  std::vector<GLuint> indexData;
  // 4 vertex per square with 6 data: x,y,z,r,g,b
  vertexData.resize(cols * rows * (4 * 6));
  // 2 triangles per square
  indexData.resize(cols * rows * 6);

  // fill the triangle and color  data from the mine map
  UpdateVertexData(gMap, vertexData, indexData);

  // glDraw needs to know how many triangles to draw later
  gTriangleCount = vertexData.size() / 4;

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
  // setup Index Buffer Object (IBO or EBO)
  glGenBuffers(1, &gIndexBufferObject);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint),
               indexData.data(), GL_STATIC_DRAW);

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
    if (e.type == SDL_MOUSEBUTTONDOWN ) {
      int x, y;
      Uint32 buttons = SDL_GetMouseState(&x, &y);
      if ((buttons & SDL_BUTTON_LMASK) != 0) {
        double fx = 2 * double(x) / gScreenWidth - 1;
        double fy = -2 * double(y) / gScreenHeight + 1;
        size_t row = 0, col = 0;
        bool hit = gLayout.inside(fx, fy, row, col);
        std::cout << "left click! ["
                  << x << ", " << y
                  << "] [" << fx << ", " << fy << "] hit: " << hit;
        if (hit) {
          std::cout << " row/col [" << row << ", " << col << "]";
        }
        std::cout << std::endl;
        // bool hit = (fx > -0.5 ) && (fx < 0.5) && (fy > -0.5 && fy < 0.5);
        // std::cout << "left click! " << fx << ", " << fy << "hit: " << hit << std::endl;
      }
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
  glDrawElements(GL_TRIANGLES, gTriangleCount, GL_UNSIGNED_INT, 0);
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

  initMap(gMap, 120);
  auto [gameOver, changes] = click(gMap, 39, 39);
  for(auto [r, c, v]: changes) {
    gMap[r][c] = v;
  }
  std::cout << gMap;
  layout_t layout {rows, cols, -0.9, 0.9, 0.04, -0.04, 0.001, -0.001};
  std::cout << "\n\n";
  double x, y;
  layout.topLeft(0,0, x,y);
  std::cout << "grid[0][0] top left [" << x << "," << y << "]" << std::endl;
  layout.bottomRight(0,0, x,y);
  std::cout << "grid[0][0] bottom left [" << x << "," << y << "]" << std::endl;
  layout.topLeft(rows-1,cols-1, x,y);
  std::cout << "grid[" << (rows-1) << "][" << cols -1
            <<  "] top left [" << x << "," << y << "]" << std::endl;
  layout.bottomRight(rows-1, cols - 1 , x,y);
  std::cout << "grid[-1][-1] bottom left [" << x << "," << y << "]" << std::endl;

  size_t r,c;
  bool b = layout.inside(0.7, -0.7, r, c);
  std::cout << "[0.7, -0.7] inside " << b << " row " << r << ", col " << c << std::endl;

  std::cout << "\n";
  InitializeProgram();
  VertexSpecification();
  CreateGraphicsPipeline();
  MainLoop();
  Cleanup();
  return 0;
}
