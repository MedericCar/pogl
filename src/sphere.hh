#pragma once

#include <vector>
#include "vector3.hh"

class Sphere
{
public:
    Sphere(Point3& center, float radius);
    
    //Returns vertices and indices
    std::pair<std::vector<float>, std::vector<unsigned int>> generate_vertices(unsigned int stacks, unsigned int sectors);

    std::vector<float>& get_normals() { return normals; }

    ~Sphere() = default;
private:
    Point3 center;
    float radius;

    std::vector<float> normals;
};