#pragma once

#include <iterator>
#include <ostream>
#include <vector>

#define ID {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1}

namespace mygl
{

class matrix4
{
  public:

    matrix4(const std::vector<float>& values);

    matrix4 operator*(const matrix4& r) const;

    void operator*=(const matrix4& rhs);

    float operator[](unsigned int index) const;

    static matrix4 identity();

  private:
    std::vector<float> array;
};

}

std::ostream& operator<<(std::ostream& out, const mygl::matrix4& m);
