#include "vertex.h"

std::ostream& operator<< (std::ostream& os, const Vertex& v)
{
  os << std::fixed;
  os << "(" << v.m_xyz[0] << ", " << v.m_xyz[1] << ", " << v.m_xyz[2] << ") ";
  os << "(" << v.m_normal[0] << ", " << v.m_normal[1] << ", " << v.m_normal[2] << ") ";
  os << "(" << v.m_st[0] << ", " << v.m_st[1] << ") ";
  return os;
}

std::istream& operator>> (std::istream& is, Vertex& v)
{
// Trenger fire temporære variabler som kun skal lese inn parenteser og komma
  char temp, temp2, temp3, temp4;
  is >> temp >> v.m_xyz[0] >> temp2 >> v.m_xyz[1] >> temp3 >> v.m_xyz[2] >> temp4;
  is >> temp >> v.m_normal[0] >> temp2 >> v.m_normal[1] >> temp3 >> v.m_normal[2] >> temp4;
  is >> temp >> v.m_st[0] >> temp2 >> v.m_st[1] >> temp3;
  return is;
}

Vertex::Vertex()
{}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float s, float t)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;

    m_normal[0] = r;
    m_normal[1] = g;
    m_normal[2] = b;

    m_st[0] = s;
    m_st[1] = t;
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float s, float t, float fr, float fg, float fb)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;

    m_normal[0] = r;
    m_normal[1] = g;
    m_normal[2] = b;

    m_st[0] = s;
    m_st[1] = t;

    m_color[0] = fr;
    m_color[1] = fg;
    m_color[2] = fb;
}

Vertex::Vertex(QVector3D a, QVector3D b, QVector3D c)
{
    m_xyz[0] = a.x();
    m_xyz[1] = a.y();
    m_xyz[2] = a.z();

    m_normal[0] = b.x();
    m_normal[1] = b.y();
    m_normal[2] = b.z();

    m_st[0] = c.x();
    m_st[1] = c.y();
}

glm::vec3 Vertex::getVertexXYZ()
{
    return m_xyz;
}

glm::vec3 Vertex::getVertexNormal()
{
    return m_normal;
}

glm::vec3 Vertex::getVertexST()
{
    return m_st;
}

glm::vec3 Vertex::getVertexColor()
{
    return m_color;
}
