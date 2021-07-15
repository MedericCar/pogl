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

float radius_to_cube_l(float radius)
{
    return sqrt(pow(radius, 2) / 2);
}

vec3 get_face_normal(vec3 pos, float size)
{    
    vec3 a_pos = abs(pos);    
    if (a_pos.x > a_pos.y && a_pos.x > a_pos.z)
    {
        if(pos.x > 0)
        {
            Normal = mat3(modelTransposeInv) * vec3(1, 0, 0);
            return vec3(size, pos.y, pos.z);
        }
        Normal = mat3(modelTransposeInv) * vec3(-1, 0, 0);
        return vec3(-size, pos.y, pos.z);
    }
    else if (a_pos.y > a_pos.z)
    {
        if (pos.y > 0)
        {
            Normal = mat3(modelTransposeInv) * vec3(0, 1, 0);
            return vec3(pos.x, size, pos.z);
        }
        Normal = mat3(modelTransposeInv) * vec3(0, -1, 0);
        return vec3(pos.x, -size, pos.z);
    }
    else
    {
        if (pos.z > 0)
        {
            Normal = mat3(modelTransposeInv) * vec3(0, 0, 1);
            return vec3(pos.x, pos.y, size);
        }
        Normal = mat3(modelTransposeInv) * vec3(0, 0, -1);
        return vec3(pos.x, pos.y, -size);
    }
}

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    float radius = length(FragPos);

    //Let's modify radius depending on position
    vec3 norm = normalize(FragPos);
    float red_coef = round(max(abs(norm.x), max(abs(norm.y), abs(norm.z))) * 10) / 10;
    Normal = mat3(modelTransposeInv) * normalize(FragPos);
    vec3 mini_cube_pos = Normal * red_coef;


    float size = radius_to_cube_l(red_coef);
    vec3 cube = get_face_normal(mini_cube_pos, size);

    vec4 pos;
    if (time <= 10)
    {
        pos = projection * view * vec4(mix(FragPos, mini_cube_pos, time/10), 1.0);
    }
    else if (time <= 20)
    {
        pos = projection * view * vec4(mix(mini_cube_pos, cube, time/20), 1.0);
    }
    else
    {
        pos = projection * view * vec4(cube, 1.0);
    }
    gl_Position = pos;
}
