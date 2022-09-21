#include "tetrahedron.h"

Tetrahedron::Tetrahedron()
{}

Tetrahedron::Tetrahedron(QVector3D middlePoint)
{
    float spacing = 0.2;
    float tetraHeight = 0;

    float difference = (align(middlePoint.x(), middlePoint.y())) - middlePoint.z();
    mPos = {middlePoint.x(), difference, middlePoint.y()};

    difference = (align(middlePoint.x() - spacing, middlePoint.y() - spacing)) + tetraHeight;
    mVertices.push_back(Vertex{middlePoint.x() - spacing, difference,middlePoint.y() - spacing,1,0,0,0,0}); // SW 0

    difference = (align(middlePoint.x() + spacing, middlePoint.y() - spacing)) + tetraHeight;
    mVertices.push_back(Vertex{middlePoint.x() + spacing, difference,middlePoint.y() - spacing,1,0,0,0,0}); // SE 1

    difference = (align(middlePoint.x(), middlePoint.y() + spacing)) + tetraHeight;
    mVertices.push_back(Vertex{middlePoint.x(), difference,middlePoint.y() + spacing,1,0,0,0,0}); // NM 2

    difference = (align(middlePoint.x(), middlePoint.y())) + spacing;
    mVertices.push_back(Vertex{middlePoint.x(), difference, middlePoint.y(),1,1,0,0,0}); // Top 3

    mIndices =
    {
        0, 1, 2,

        0, 1, 3,
        1, 2, 3,
        2, 0, 3
    };

//    Vertex A {-0.5,-0.5,0.5,0,1,0};
//    Vertex B {0.5,-0.5,0.5,1,0,0};
//    Vertex C {0,0.5,0,0,0,1};
//    Vertex D {0,-0.5,-0.5,1,1,0};

//    mVertices.push_back(Vertex{A});
//    mVertices.push_back(Vertex{B});
//    mVertices.push_back(Vertex{C});
//    mVertices.push_back(Vertex{D});

//    mIndices.push_back(0);
//    mIndices.push_back(1);
//    mIndices.push_back(2);
//    mIndices.push_back(3);
}

Tetrahedron::Tetrahedron(std::string filnavn)
{
    readFile(filnavn);
}

Tetrahedron::~Tetrahedron()
{}

void Tetrahedron::readFile(std::string filnavn)
{
    std::ifstream inn;
       inn.open(filnavn.c_str());

       if (inn.is_open()) {
           int ni, nv, indeks;
           inn >> ni;
           mIndices.reserve(ni);
           inn >> nv;
           mVertices.reserve(nv);

           for (int i=0; i<ni; i++) { inn >> indeks; mIndices.push_back(indeks); }
           Vertex vertex;
           for (int i=0; i<nv; i++) { inn >> vertex; mVertices.push_back(vertex); }

           inn.close();
       }
}

void Tetrahedron::writeFile(std::string filnavn)
{

}

void Tetrahedron::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint),
    mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Tetrahedron::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
//    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT,
    reinterpret_cast<const void*>(0));
}

float Tetrahedron::align(float x, float y)
{
//    return pow((4*x+y), 3);
    return sin(x) + cos(y);
}

void Tetrahedron::move(float dx, float dy, float dz, float dt)
{
    // Oppdatere posisjon
    mX += dx;
    mY += dy;

    float difference = align(mX, mY) - mZ;

    mZ = align(mX, mY);

    // Oppdatere mMatrix (endringer)
    mMatrix.translate(dx, dy, difference);

    mPos = {mX, mY, mZ};
//    std::cout << "position:" << mPos.x() << " " << mPos.y() << " " << mPos.z() << "\n";
}

QVector3D Tetrahedron::getPlayerPos()
{
    return mPos;
}

//Bruk formelen for funksjonen (flaten) direkte slik at tetraederobjektet følger flaten ved interaktivitet.
