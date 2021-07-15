#version 450
#define M_PI 3.1415926535897932384626433832795

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out float coef;
  
uniform mat4 model;
uniform mat4 modelTransposeInv;
uniform mat4 projection;
uniform mat4 view;
uniform float time;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(modelTransposeInv) * aNormal;

    vec4 pos = projection * view * vec4(FragPos, 1.0);

    float maxi = exp(exp(4.46));
    coef = exp(exp((sin(pos.x + time) / 2 + 0.5) * 4.46)) / maxi;
    coef += exp(exp((sin(pos.x + time + M_PI / 4) / 2 + 0.5) * 4.46)) / maxi;
    coef += exp(exp((sin(pos.x - time + M_PI / 2) / 2 + 0.5) * 4.46)) / maxi;
    coef += exp(exp((sin(pos.x - time + 3 * M_PI / 4) / 2 + 0.5) * 4.46)) / maxi;


    coef += exp(exp((sin(pos.y - time * 2) / 2 + 0.5) * 4.46)) / maxi;
    coef += exp(exp((sin(pos.y - time * 2 + M_PI / 4) / 2 + 0.5) * 4.46)) / maxi;
    coef += exp(exp((sin(pos.y + time * 2 + M_PI / 2) / 2 + 0.5) * 4.46)) / maxi;
    coef += exp(exp((sin(pos.y + time * 2 + 3 * M_PI / 4) / 2 + 0.5) * 4.46)) / maxi;
    gl_Position = pos;
}
