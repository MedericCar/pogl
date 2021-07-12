#pragma once

#include "matrix.hh"
#include "vector3.hh"

void frustum(mygl::matrix4& mat,const float& left,
    const float& right, const float& bottom,
    const float& top, const float& znear,const float& zfar);

void lookat(mygl::matrix4& mat, const float& eyeX,
    const float& eyeY, const float& eyeZ, const float& centerX,
    const float& centerY, const float& centerZ, float upX, float upY,
    float upZ);
