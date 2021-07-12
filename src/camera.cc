#include "camera.hh"
#include "matrix.hh"
#include <vector>

void frustum(mygl::matrix4& mat, const float& left,
    const float& right, const float& bottom,
    const float& top, const float& znear, const float& zfar)
{
  std::vector<float> look_values(16, 0);
  look_values[0] = (2 * znear) / (right - left);
  look_values[2] = (right + left) / (right - left);
  look_values[5] = (2 * znear) / (top - bottom);
  look_values[6] = (top + bottom) / (top - bottom);
  look_values[10] = (zfar + znear) / (zfar - znear);
  look_values[11] = -(2 * zfar * znear) / (zfar - znear);
  look_values[14] = -1;

  mygl::matrix4 persp_mat(look_values);

  mat *= persp_mat;
}

void lookat(mygl::matrix4& mat, const float& eyeX,
    const float& eyeY, const float& eyeZ, const float& centerX,
    const float& centerY, const float& centerZ, float upX, float upY,
    float upZ)
{
  Vector3 f = Vector3(centerX - eyeX, centerY - eyeY, centerZ - eyeZ)
          .get_normalized();
  Vector3 up_norm = Vector3(upX, upY, upZ).get_normalized();
  Vector3 s = f * up_norm;
  Vector3 u = s.get_normalized() * f;

  std::vector<float> look_values(16, 0);
  look_values[0] = s.x;
  look_values[1] = s.y;
  look_values[2] = s.z;

  look_values[4] = u.x;
  look_values[5] = u.y;
  look_values[6] = u.z;

  look_values[8] = -f.x;
  look_values[9] = -f.y;
  look_values[10] = -f.z;

  look_values[15] = 1;

  mygl::matrix4 look_mat(look_values);
  mat *= look_mat;

  std::vector<float> translate_values(16, 0);
  translate_values[0] = 1;
  translate_values[3] = eyeX;

  translate_values[5] = 1;
  translate_values[7] = eyeY;

  translate_values[10] = 1;
  translate_values[11] = eyeZ;

  translate_values[15] = 1;

  mat = mygl::matrix4(translate_values) * mat;
}
