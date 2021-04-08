#version 400

in vec3 OutColor;
layout (location = 0) out vec4 FragColor;

void main() 
{ 
   FragColor = vec4(OutColor.rgb, 1.0);
}