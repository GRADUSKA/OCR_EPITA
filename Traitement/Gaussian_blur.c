#include <stdio.h>
#include <math.h>
#include <use.h>

double gaussian( double x, double mu, double sigma )
{
    const double a = ( x - mu ) / sigma;
    return exp( -0.5 * a * a );
}
