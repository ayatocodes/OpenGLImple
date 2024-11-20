#version 330

layout(location = 0) in vec4 position; //location = 0 is specific attribute should match vertexattribpointer pos 0

void main()
{
   gl_Position = position; 
};