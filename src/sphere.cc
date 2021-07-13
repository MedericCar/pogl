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
    std::vector<float> verts(lines * cols * 3);
    std::vector<unsigned int> indices((lines - 1) * (cols - 1) * 4);

    if (lines < 2 || cols < 3)
    {
        std::cerr << "Lines: " << lines << " | Cols: " << cols << std::endl
                    << "Lines must bigger than 1 and Cols bigger than 2"; 
        return {verts, indices};
    }

    /*double sectorStep = 2 * M_PI / sectors;
    double stackStep = M_PI / stacks;

    for (unsigned int i = 0; i <= stacks; i++)
    {
        double stackAngle = 2 * M_PI  / stackStep;
        double rcos = radius * std::cos(stackAngle);
        double z = radius * std::sin(stackAngle);

        for (unsigned int j = 0; j <= sectors; j++)
        {
            double sectorAngle = j * sectorStep;

            // vertex position (x, y, z)
            double x = rcos * std::cos(sectorAngle);// r * cos(u) * cos(v)
            double y = rcos * std::sin(sectorAngle);// r * cos(u) * sin(v)          
            
            verts.push_back(y);
            verts.push_back(z);
            verts.push_back(-x);
        }
        
    }
    //DO NOT FORGET TO ADD LAST TEXTURE COORDS   


    for (size_t i = 0; i < count; i++)
    {
        for (size_t j = 0; j < count; j++)
        {
        }        
    }*/

    float R = 1./(lines - 1);
    float S = 1./(cols - 1);
    //int r, s;


    /*sphere_vertices.resize(rings * sectors * 3);
    sphere_normals.resize(rings * sectors * 3);
    sphere_texcoords.resize(rings * sectors * 2);*/
    //std::vector<float>::iterator v = sphere_vertices.begin();
    //std::vector<GLfloat>::iterator n = sphere_normals.begin();
    //std::vector<GLfloat>::iterator t = sphere_texcoords.begin();
    unsigned int index = 0;
    for(unsigned int i = 0; i < lines; i++)
    {
        for(unsigned int j = 0; j < cols; j++)
        {
                double x = std::cos(2*M_PI * j * S) * std::sin( M_PI * i * R );
                double y = std::sin(-M_PI / 2 + M_PI * i * R);
                double z = std::sin(2*M_PI * j * S) * std::sin( M_PI * i * R );

                //*t++ = s*S;
                //*t++ = r*R;

                //std::cout << x << "," << y << "," << z << std::endl;

                /**v++*/verts[index++] = x * radius;
                /**v++*/verts[index++] = y * radius;
                /**v++*/verts[index++] = z * radius;

                //*n++ = x;
                //*n++ = y;
                //*n++ = z;
        }
    }

    //sphere_indices.resize(rings * sectors * 4);
    index = 0;

    //std:vector<unsigned int>::iterator it = sphere_indices.begin();

    for(unsigned int i = 0; i < lines -1; i++)
    {
        for(unsigned int j = 0; j < cols - 1; j++)
        {
                /**i++*/indices[index++] = i * cols + j;
                /**i++*/indices[index++] = i * cols + (j + 1);
                /**i++*/indices[index++] = (i + 1) * cols + (j + 1);
                /**i++*/indices[index++] = (i + 1) * cols + j;
        }
    }


    /*std::cout << "Actual size: " << verts.size() << std::endl;
    std::cout << "Expected size: " << lines * cols + 2 << std::endl;*/

    return {verts, indices};
}