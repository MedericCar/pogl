#version 450

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


float interpolate(float a0, float a1, float w)
{
    w = clamp(w, 0, 1);
    return (1 - w) * a0 + a1 * w;
    //return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
    //return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
}

vec2 randomGrad(float ix, float iy)
{
    /*float x = *ix;
    float y = *iy;*/
    float val = time * 5 * sin(ix * 1654 + iy * 1998 + 7518) *
        cos(ix * 2317 * iy * 2132 + 975);

    /**ix = cos(val);
    *iy = sin(val);*/

    return vec2(cos(val), sin(val));
}

float dotGridGradient(int ix, int iy, float x, float y)
{
    vec2 grad = randomGrad(ix, iy);

    float ix_grad = grad.x;
    float iy_grad = grad.y;    

    float dx = x - ix;
    float dy = y - iy;

    return dx * ix_grad + dy * iy_grad;
}

float perlin(float x, float y)
{
    int x0 = int(x);
    int x1 = x0 + 1;

    int y0 = int(y);
    int y1 = y0 + 1;

    float sx = x - x0;
    float sy = y - y0;

    float n0, n1, ix0, ix1, value;

    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = interpolate(n0, n1, sx);

    value = interpolate(ix0, ix1, sy);
    return value;
}

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(modelTransposeInv) * aNormal;

    vec4 pos = projection * view * vec4(FragPos, 1.0);
    //float coef = perlin(pos.x, pos.y);
    coef = clamp(perlin(perlin(pos.x, pos.y), pos.z), 0, 1);

    pos.x += /*pos.x */ coef;
    pos.y += /*pos.y */ coef;
    //pos.z += /*pos.z */ coef;
    gl_Position = pos;
}
