#include "systems/PerlinNoise.hpp"

PerlinNoise::PerlinNoise(){}
PerlinNoise::~PerlinNoise(){}

int PerlinNoise::noise2(int x, int y)
{
	int  yindex = (y + SEED) % 256;
	if (yindex < 0)
		yindex += 256;
	int  xindex = (HASH[yindex] + x) % 256;
	if (xindex < 0)
		xindex += 256;
	const int  result = HASH[xindex];
	return result;
}

double PerlinNoise::lin_inter(double x, double y, double s)
{
	return x + s * (y-x);
}
double PerlinNoise::smooth_inter(double x, double y, double s)
{
    return lin_inter( x, y, s * s * (3-2*s) );
}
double PerlinNoise::noise2d(double x, double y)
{
    const int  x_int = floor( x );
    const int  y_int = floor( y );
    const double  x_frac = x - x_int;
    const double  y_frac = y - y_int;
    const int  s = noise2( x_int, y_int );
    const int  t = noise2( x_int+1, y_int );
    const int  u = noise2( x_int, y_int+1 );
    const int  v = noise2( x_int+1, y_int+1 );
    const double  low = smooth_inter( s, t, x_frac );
    const double  high = smooth_inter( u, v, x_frac );
    const double  result = smooth_inter( low, high, y_frac );
    return result;
}
double PerlinNoise::perlin2d(double x, double y, double freq, int depth)
{
    double  xa = x*freq;
    double  ya = y*freq;
    double  amp = 1.0;
    double  fin = 0;
    double  div = 0.0;
    for (int i=0; i<depth; i++)
    {
        div += 256 * amp;
        fin += noise2d( xa, ya ) * amp;
        amp /= 2;
        xa *= 2;
        ya *= 2;
    }
    return fin/div;
}
