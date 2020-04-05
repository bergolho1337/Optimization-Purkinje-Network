#include <cstdio>
#include <cstdlib>
#include <cmath>

using namespace std;

double prodScalar (double u[], double v[])
{
    double p_scalar = 0;
    for (int i = 0; i < 3; i++)
        p_scalar += u[i]*v[i];
    return p_scalar;
}

double calcNorm (double u[])
{
    return sqrt(pow(u[0],2) + pow(u[1],2) + pow(u[2],2));
}

int main ()
{
    double u[3], v[3];
    //u[0] = 0; u[1] = 0; u[2] = 1;
    //v[0] = 0; v[1] = 1; v[2] = 0;
    u[0] = 2; u[1] = -1; u[2] = 3;
    v[0] = -2; v[1] = 1; v[2] = 2;
    double p_scalar = prodScalar(u,v);
    double u_norm = calcNorm(u);
    double v_norm = calcNorm(v);
    double ang = acos(p_scalar / (u_norm*v_norm));
    printf("Produto escalar u.v = %.5lf\n",p_scalar);
    printf("Produto entre as normas |u|.|v| = %.5lf\n",u_norm*v_norm);
    printf("Angulo entre os dois vetores = %.5lf\n",ang * 180.0 / M_PI);
    return 0;
}