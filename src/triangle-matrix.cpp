// Bryn Mawr College, alinen, 2020
//

#include "AGL.h"
#include "AGLM.h"
#include <cmath>
#include <fstream>
#include <sstream>


const GLchar* vertexShader[] =
{
"#version 400\n"
"layout (location = 0) in vec3 VertexPosition;"
"layout (location = 1) in vec3 VertexColor;"
"out vec3 OutColor;"
"void main() {"
"  OutColor = VertexColor;"
"  gl_Position = vec4(VertexPosition, 1.0);"
"}"
};

const GLchar* fragmentShader[] =
{
"#version 400\n"
"in vec3 OutColor;"
"layout (location = 0) out vec4 FragColor;"
"void main() { FragColor = vec4(OutColor.rgb, 1.0); }"
};


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
   {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
   }
}

static void PrintShaderErrors(GLuint id, const std::string label)
{
   std::cerr << label << " failed\n";
   GLint logLen;
   glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
   if (logLen > 0)
   {
      char* log = (char*)malloc(logLen);
      GLsizei written;
      glGetShaderInfoLog(id, logLen, &written, log);
      std::cerr << "Shader log: " << log << std::endl;
      free(log);
   }
}

static std::string LoadShaderFromFile(const std::string& fileName)
{
   std::ifstream file(fileName);
   if (!file)
   {
      std::cout << "Cannot load file: " << fileName << std::endl;
      return "";
   }

   std::stringstream code;
   code << file.rdbuf();
   file.close();

   return code.str();
}

int main(int argc, char** argv)
{
   GLFWwindow* window;

   if (!glfwInit())
   {
      return -1;
   }

   // Explicitly ask for a 4.0 context 
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   /* Create a windowed mode window and its OpenGL context */
   window = glfwCreateWindow(500, 500, "Hello World", NULL, NULL);
   if (!window)
   {
      glfwTerminate();
      return -1;
   }

   // Make the window's context current 
   glfwMakeContextCurrent(window);

   // Tell GLFW to call our callback fun when a key is pressed
   glfwSetKeyCallback(window, key_callback);

#ifndef APPLE
   if (glewInit() != GLEW_OK)
   {
      return -1;
   }
#endif

   // Define a triangle - non-const so we can modify it!
   const float positions[] =
   {
       -0.9f, -0.9f, 0.0f,
        0.9f, -0.9f, 0.0f,
        0.0f,  0.9f, 0.0f
   };

   float dynamic_positions[] = // notice: not const!
   {
       -0.9f, -0.9f, 0.0f,
        0.9f, -0.9f, 0.0f,
        0.0f,  0.9f, 0.0f
   };

   const float colors[] =
   {
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
   };

   GLuint vboPosId;
   glGenBuffers(1, &vboPosId);
   glBindBuffer(GL_ARRAY_BUFFER, vboPosId);
   glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), dynamic_positions, GL_DYNAMIC_DRAW);

   GLuint vboColorId;
   glGenBuffers(1, &vboColorId);
   glBindBuffer(GL_ARRAY_BUFFER, vboColorId);
   glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colors, GL_STATIC_DRAW);

   GLuint vaoId;
   glGenVertexArrays(1, &vaoId);
   glBindVertexArray(vaoId);

   glEnableVertexAttribArray(0); // 0 -> Sending VertexPositions to array #0 in the active shader
   glBindBuffer(GL_ARRAY_BUFFER, vboPosId); // always bind before setting data
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

   glEnableVertexAttribArray(1); // 1 -> Sending VertexColors to array #1 in the active shader
   glBindBuffer(GL_ARRAY_BUFFER, vboColorId); // always bind before setting data
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

   GLint result;
   GLuint vshaderId = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vshaderId, 1, vertexShader, NULL);
   glCompileShader(vshaderId);
   glGetShaderiv(vshaderId, GL_COMPILE_STATUS, &result);
   if (result == GL_FALSE)
   {
      PrintShaderErrors(vshaderId, "Vertex shader");
      return -1;
   }

   GLuint fshaderId = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fshaderId, 1, fragmentShader, NULL);
   glCompileShader(fshaderId);
   glGetShaderiv(fshaderId, GL_COMPILE_STATUS, &result);
   if (result == GL_FALSE)
   {
      PrintShaderErrors(fshaderId, "Fragment shader");
      return -1;
   }

   GLuint shaderId = glCreateProgram();
   glAttachShader(shaderId, vshaderId);
   glAttachShader(shaderId, fshaderId);
   glLinkProgram(shaderId);
   glGetShaderiv(shaderId, GL_LINK_STATUS, &result);
   if (result == GL_FALSE)
   {
      PrintShaderErrors(shaderId, "Shader link");
      return -1;
   }

   glUseProgram(shaderId);
   GLuint timeParamId = glGetUniformLocation(shaderId, "time");
   glUniform1f(timeParamId, 0);

   glClearColor(0, 0, 0, 1);

   // Loop until the user closes the window 
   while (!glfwWindowShouldClose(window))
   {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers

      // Update positions
      float dx = 0.1 * sin(glfwGetTime());
      glm::mat4 T = glm::translate(glm::mat4(1.0), glm::vec3(dx, 0.0f, 0.0f));
      for (int i = 0; i < 9; i+=3)
      {
         glm::vec4 p(positions[i + 0], positions[i + 1], positions[i + 2], 1);
         p = T * p;
         dynamic_positions[i + 0] = p.x;
         dynamic_positions[i + 1] = p.y;
         dynamic_positions[i + 2] = p.z;
      }
      glBindBuffer(GL_ARRAY_BUFFER, vboPosId);
      glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), dynamic_positions, GL_DYNAMIC_DRAW);

      // Draw primitive
      glDrawArrays(GL_TRIANGLES, 0, 3);

      // Swap front and back buffers
      glfwSwapBuffers(window);

      // Poll for and process events
      glfwPollEvents();
   }

   glfwTerminate();
   return 0;
}

