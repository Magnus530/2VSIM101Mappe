#include "npc.h"

Npc::Npc()
{}

// Oppgave 7
Npc::Npc(QVector3D Pos)
{
    setPos(Pos);

    float mid = 0;

    mVertices.push_back(Vertex{mid - extent, mid - extent, mid + extent,0,1,0,0,0}); // 0 SW
    mVertices.push_back(Vertex{mid + extent, mid - extent, mid + extent,1,0,0,0,0}); // 1 SE
    mVertices.push_back(Vertex{mid, mid - extent, mid - extent,0,0,1,0,0}); // N
    mVertices.push_back(Vertex{mid, mid + extent, mid,1,1,0,0,0}); // Top

    mIndices =
    {
          0, 1, 2,
          0, 1, 3,
          1, 2, 3,
          2, 0, 3
    };
}

void Npc::init(GLint matrixUniform)
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

void Npc::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT,
    reinterpret_cast<const void*>(0));
}

float Npc::align(float x, float y)
{
    return sin(x) + cos(y);
}

float Npc::align2(float x)
{
    return (-1.1*pow(x, 2) + 5*x - 2.5);
}

void Npc::move(float dx, float dy, float dz, float dt) // gjenstår å finne en måte å bruke dt
{
    dt = 1.f;
    // Oppdatere posisjon
    mX += dx;
    mY += dy;

    float difference = (align(mX, mY)) - mZ;

    mZ = align(mX, mY);

    // Oppdatere posisjon mMatrix (endringer)
    mMatrix.translate(dx / dt, difference / dt, dy / dt);
}

void Npc::npcMove(float dt)
{
    int derivative = 10;

    if(x >= xmax)
    {
        rightDirection = false;
    }
    else if (x <= xmin)
    {
        rightDirection = true;
    }
//    std::cout << "x:" << x << "\n";
//    std::cout << "rightdir:" << rightDirection << "\n";

    if (path == 0)
    {
        if(rightDirection == true && x < xmax)
        {
            float y1 = (-1.1*pow(x, 2) + 5*x - 2.5);
            if(x == xmin)
            {
                move(x, 0, y1, dt);
            }
            float y2 = (-1.1*pow(x+h, 2) + 5*(x+h) - 2.5);
            move(h, 0, y2-y1, dt);
            x+=h;
        }

        if (rightDirection == false && x >= xmin)
        {
            x-=h;
            float y1 = (-1.1*pow(x, 2) + 5*x - 2.5);
            if(x >= xmax)
            {
                move(-x, 0, -y1, dt);
            }
            float y2 = (-1.1*pow(x+h, 2) + 5*(x+h) - 2.5);
            move(-h, 0, -(y2-y1), dt);
        }
    }
    else
    {
        if(rightDirection == true && x < xmax)
        {
            float y1 = (pow(x, 2) - 2*x + 1);
            if(x == xmin)
            {
                move(x, 0, y1, dt);
            }
            float y2 = (pow(x+h, 2) - 2*(x+h) + 1);
            move(h, 0, y2-y1, dt);
            x+=h;
        }

        if (rightDirection == false && x >= xmin)
        {
            x-=h;
            float y1 = (pow(x, 2) - 2*x + 1);
            if(x >= xmax)
            {
                move(-x, 0, -y1, dt);
            }
            float y2 = (pow(x+h, 2) - 2*(x+h) + 1);
            move(-h, 0, -(y2-y1), dt);
        }
    }
}

void Npc::setPath()
{
    if (path == 0)
    {
        path = 1;
    }
    else
    {
        path = 0;
    }
}

float Npc::getSpacing()
{
    return extent;
}

//    float xmin= 0.0f, xmax=5.0f, ymin= 0.0f, ymax=5.0f, h=0.1f;

//    for (auto x=xmin; x<xmax; x+=h)
//    {
//        float y1 = (-1.1*pow(x, 2) + 5*x - 2.5);
//        if(x == xmin)
//        {
//            npc->move(x, y1, 0);
//                render();
//            npc->draw();
//            std::cout << "in move:" << x << " " << y1 << "\n";
//        }
//        float y2 = (-1.1*pow(x+h, 2) + 5*(x+h) - 2.5);
//        npc->move(h, y2-y1, 0);
//            render();
//        npc->draw();
//            std::this_thread::sleep_for(std::chrono::milliseconds(100));

//        std::cout << "in loop:" << h << " " << y2-y1 << "\n";
//    }
