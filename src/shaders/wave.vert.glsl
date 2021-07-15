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

#define FREQ 2 * M_PI / 10
#define AMP  0.7
#define PHI  2 * M_PI * 2 / 10

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(modelTransposeInv) * aNormal;

    if (FragPos.z > 0)
    {
        //COMPUTING WAVES WITH SPHERICAL COORDINATES
        //Between [0; 2PI]
        float sphere_x_pos = atan(FragPos.x, FragPos.z) + M_PI;//sin(FragPos.x) + 0.5;

        //Between [-PI/2; PI/2]
        float sphere_x = atan(FragPos.x/ FragPos.z);//sin(FragPos.x) + 0.5;
        //Between [0; PI]
        float sphere_y = acos(FragPos.y);//-sin(FragPos.y) + 0.5;
        //coef = sphere_y / M_PI;

        vec2 dir = vec2(0, -1);
        float sphere_r = clamp(AMP * sin(dot(dir, vec2(sphere_x, sphere_y)) * FREQ + time * PHI), 0, AMP);

        coef = sphere_r;

        //CONVERTING TO CARTESIAN
        float cartes_x = sphere_r * sin(sphere_x/*_pos*/) * sin(sphere_y);
        float cartes_y = sphere_r * cos(sphere_y);
        float cartes_z = sphere_r * cos(sphere_x/*_pos*/) * sin(sphere_y);
        FragPos.xyz = vec3(cartes_x, cartes_y, cartes_z);
    }

    vec4 pos = projection * view * vec4(FragPos, 1.0);
    gl_Position = pos;
}
