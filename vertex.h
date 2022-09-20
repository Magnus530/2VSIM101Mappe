#ifndef VERTEX_H
#define VERTEX_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>

class Vertex
{
   //! Overloaded ostream operator which writes all vertex data on an open textfile stream
   friend std::ostream& operator<< (std::ostream&, const Vertex&);

   //! Overloaded ostream operator which reads all vertex data from an open textfile stream
   friend std::istream& operator>> (std::istream&, Vertex&);

private:
//   float m_xyz[3];           // position i 3D
//   float m_normal[3];    // normal in 3D or rgb colors
//   float m_st[2];            // texture coordinates if used
//   float m_color[3];


public:
   glm::vec3 m_xyz;           // position i 3D
   glm::vec3 m_normal;    // normal in 3D or rgb colors
   glm::vec3 m_st;            // texture coordinates if used
   glm::vec3 m_color;

   Vertex();
   Vertex(float x, float y, float z, float r, float g, float b, float s, float t);
   Vertex(float x, float y, float z, float r, float g, float b, float s, float t, float fr, float fg, float fb);
   Vertex(QVector3D a, QVector3D b, QVector3D c);
   glm::vec3 getVertexXYZ();
   glm::vec3 getVertexNormal();
   glm::vec3 getVertexST();
   glm::vec3 getVertexColor();
};

#endif // VERTEX_H
