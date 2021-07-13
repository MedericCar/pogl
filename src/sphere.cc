#include "sphere.hh"


Sphere::Sphere(Point3& center, float radius)
: center(center),
  radius(radius)
{    
}

/*
** x: left (->)
** y: up
** z: away from you
*/
std::pair<std::vector<float>, std::vector<unsigned int>> Sphere::generate_vertices(unsigned int lines, unsigned int cols)
{
    std::vector<float> verts{};//(lines * cols * 3);
    std::vector<unsigned int> indices{};//((lines - 1) * (cols - 1) * 4);

    if (lines < 2 || cols < 3)
    {
        std::cerr << "Lines: " << lines << " | Cols: " << cols << std::endl
                    << "Lines must bigger than 1 and Cols bigger than 2"; 
        return {verts, indices};
    }

    //Top vertex
    verts.push_back(0);
    verts.push_back(radius);
    verts.push_back(0);

    for (unsigned int i = 0; i < lines - 1; ++i)
    {
        double r = M_PI * (i + 1) / lines;
        for (unsigned int j = 0; j < cols; ++j)
        {
            double s = 2 *  M_PI * j / cols;

            double x = std::sin(r) * std::cos(s);
            double y = std::cos(r);
            double z = std::sin(r) * std::sin(r);

            verts.push_back(x);
            verts.push_back(y);
            verts.push_back(z);
        }
        
    }    

    //Bottom vertex
    verts.push_back(0);
    verts.push_back(-radius);
    verts.push_back(0);

    //Top and Bot
    for (unsigned int i = 0; i < cols; ++i)
    {
        unsigned int i0 = i + 1;
        unsigned int i1 = (i + 1) % cols + 1;
        indices.push_back(0);
        indices.push_back(i1);
        indices.push_back(i0);

        i0 = i + cols * (lines - 2) + 1;
        i1 = (i + 1) % cols + cols * (lines - 2) + 1;
        indices.push_back(verts.size() / 3 - 1);
        indices.push_back(i1);
        indices.push_back(i0);
    }

    for (int j = 0; j < lines - 2; j++)
    {
        unsigned int j0 = j * cols + 1;
        unsigned int j1 = (j + 1) * cols + 1;
        for (int i = 0; i < cols; i++)
        {
            unsigned int i0 = j0 + i;
            unsigned int i1 = j0 + (i + 1) % cols;
            unsigned int i2 = j1 + (i + 1) % cols;
            unsigned int i3 = j1 + i;
            

            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i2);

            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i3);
        }
    }

    return {verts, indices};
}