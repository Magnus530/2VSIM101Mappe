#include "bsplinecurve.h"

BSplineCurve::BSplineCurve(GLuint shaderNum, GLuint id)
{
    mTexId=id;
    mShaderNum=shaderNum;
    for(float t=0; t<tMax; t+=dt)
    {
        Vertex v;
        v.m_xyz=EvalutaeBSpline(t);
        v.m_normal={0,0,1};
        mVertices.push_back(v);
    }
    int j=0;
    for (GLuint i=0; i < mVertices.size(); i++)
    {
        mIndices.push_back(i);
    }
    setPos({0,0,0});




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
void BSplineCurve::update()
{
    //Må gå igjennom for alle t[tMin] til t[max] med funksjonen EvaluateBSpilne
    //Finne en måte å tegne dette på
    //ha en funksjon som får in data fra objectene
    //Oppdaterer og skriver inn verdier for tengingen av kurven

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
    a.reserve(d+1);
    for(int j=0; j<=d;j++)
    {
        a[d-j] = c[my-j];
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
