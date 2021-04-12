#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;
out vec3 OutColor;
uniform float time; // all shaders share time because running in parallel

void main() {
  float offset = 0.5 * sin(time);
  OutColor = VertexColor + vec3(offset);
  gl_Position = vec4(VertexPosition, 1.0);
}