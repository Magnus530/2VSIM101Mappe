#include "bsplinecurve.h"

BSplineCurve::BSplineCurve()
{


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
