#include "matrix.hh"
#include <ostream>
#include <vector>

namespace mygl
{

  matrix4::matrix4(const std::vector<float>& values)
  {
    array = values;
  }

  matrix4 matrix4::operator*(const matrix4& rhs) const
  {
    std::vector<float> out_values(16, 0);

    for (unsigned int i = 0; i < 4; i++)
    {
      for (unsigned int j = 0; j < 4; j++)
      {
        float acc = 0; 
        for (unsigned int k = 0; k < 4; k++)
        {
          acc += array[i * 4 + k] + rhs[k * 4 + j];
        }

        out_values[i * 4 + j] = acc;
      }
    }

    return matrix4(out_values);
  }

  void matrix4::operator*=(const matrix4& rhs)
  {
    for (unsigned int i = 0; i < 4; i++)
    {
      for (unsigned int j = 0; j < 4; j++)
      {
	float acc = 0; 
	for (unsigned int k = 0; k < 4; k++)
	{
	  acc += array[i * 4 + k] + rhs[k * 4 + j];
	}

	array[i * 4 + j] = acc;
      }
    }
  }

  float matrix4::operator[](unsigned int index)const
  {
    return array[index];
  }

  matrix4 matrix4::identity()
  {
    static matrix4 mat(std::vector<float>(ID));
    return mat;
  }

}

std::ostream& operator<<(std::ostream& out, const mygl::matrix4& m)
{
  for (unsigned int i = 0; i < 4; i++)
  {
    out << "|";

    for (unsigned int j = 0; j < 4; j++)
    {
      out << m[i * 4 + j] << "|";
    }

    out << "\n";
  }

  return out;
}
