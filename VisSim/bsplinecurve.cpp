#include "bsplinecurve.h"

BSplineCurve::BSplineCurve()
{
//    n=c.size();
//    if(n<2)
//        return;
//    for(float t=0; t<tMax; t+=dt)
//    {
//        Vertex v;
//        v.m_xyz=EvalutaeBSpline(t);
//        v.m_normal={0,0,1};
//        mVertices.push_back(v);
//    }
//    for (GLuint i=0; i < mVertices.size()-1; i++)
//    {
//        mIndices.push_back(i);
//        mIndices.push_back(i+1);
//    }
    setPos({0,0,0});
//    std::cout<<"\n mVertices size: "<<mVertices.size();
//    for(int i=0; i<mVertices.size();i++)
//    {
//        glm::vec3 temp =mVertices[i].getVertexXYZ();
//        std::cout<<"Vertex: "<<i<<" x: " << temp.x<< ", y: "<<temp.y<<", z: "<<temp.z<<"\n";

//    }




}

void BSplineCurve::init(GLint matrixUniform)
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

    // 1st attribute buffer : Position
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)NULL);
    glEnableVertexAttribArray(0);

    // 2nd atrribute buffer : Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_st) );
    glEnableVertexAttribArray(2);

    // 4rth attribute buffer : Color
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_color) );
    glEnableVertexAttribArray(3);

    //Index buffer binding so that its easier to create objects without duplicate vertices.
    glGenBuffers( 1, &mIBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW );
    glBindVertexArray(0);


}
void BSplineCurve::firstUpdate(glm::vec3 controlpoint)
{
    std::cout<<"\n \n First controlPoint: x: "<<controlpoint.x<<" y: "<<controlpoint.y<<" z: "<<controlpoint.z<<"\n";
    bHasBeinUpdatedOnce=true;
    float cT = tMin;
    for(int i=0; i<d+1;i++)
    {
        c.push_back(controlpoint);
        t.push_back(cT);
    }
    cT++;
    for(int i=0;i<c.size();i++)
    {
        t.push_back(cT);
    }
    tMax=cT;
    n=c.size();
    calculateBSpline();
    std::cout<<"\n I HAVE BEN UPDATER\n";
}
void BSplineCurve::update(glm::vec3 controlpoint)
{
    if(!bHasBeinUpdatedOnce)
        return;
    std::cout<<"\n Has Bein updated again \n";
    c.push_back(controlpoint);
    t.push_back(t.back());
    tMax++;
    n=c.size();
    for(int i=0; i<d+1;i++)
    {
        t[t.size()-(1+i)]=tMax;
    }
    calculateBSpline();
}

int BSplineCurve::findKnotInterval(float x)
{
    int my = n-1;
    while(x<t[my] && my>d)
        my--;
    return my;

}

glm::vec3 BSplineCurve::EvalutaeBSpline(float x)
{
    int my =findKnotInterval(x);
    std::vector<glm::vec3>a;
//    a.reserve(d+1);
    glm::vec3 tempVec = glm::vec3{0,0,0};

    for (int i = 0; i < d+1; i++)
    {
        a.push_back(tempVec);
    }

    for(int j=0; j<=d;j++)
    {
        a[d-j]=c[my-j];
    }
    for(int k=d; k>0; k--)
    {
        int j =my-k;
        for(int i=0;i<k;i++)
        {
            j++;
            float w=(x-t[j])/(t[j+k]-t[j]);
            a[i] = a[i]*(1-w)+a[i+1]*w;
        }
    }
    return a[0];
}
void BSplineCurve::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_LINES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());

}

void BSplineCurve::calculateBSpline()
{
    mVertices.clear();
    mIndices.clear();
    for(float t=0; t<tMax; t+=dt)
        {
            Vertex v;
            v.m_xyz=EvalutaeBSpline(t);
            v.m_normal={1,0,0};
            std::cout<<"\n vertex: x: "<<v.m_xyz.x<<"y: "<<v.m_xyz.y<<"z: "<<v.m_xyz.z<<"\n";
            mVertices.push_back(v);
        }
        for (GLuint i=0; i < mVertices.size()-1; i++)
        {
            mIndices.push_back(i);
            mIndices.push_back(i+1);
        }
    std::cout<<"\n Vertex size: "<<mVertices.size();


}
