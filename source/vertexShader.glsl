#version 330 core

//input
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal; //modelspace normal

//output
out vec2 UV;
out vec3 fragmentPosition;
out vec3 Normal; //viewspace normal


//uniform
uniform mat4 MVP;
uniform mat4 MV;

void main()
{
    // Output vertex position
    gl_Position = MVP * vec4(position, 1.0); //screenspace position
    UV = uv; //outputting the texture coordina

    fragmentPosition = vec3(MV* vec4(position, 1.0));
    Normal = mat3(transpose(inverse(MV))) * normal;
}