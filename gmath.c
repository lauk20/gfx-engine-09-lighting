
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"


/*============================================
  IMPORANT NOTE

  Ambient light is represeneted by a color value

  Point light sources are 2D arrays of doubles.
       - The fist index (LOCATION) represents the vector to the light.
       - The second index (COLOR) represents the color.

  Reflection constants (ka, kd, ks) are represened as arrays of
  doubles (red, green, blue)
  ============================================*/


//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  color a;
  color d;
  color s;

  a = calculate_ambient(alight, areflect);
  d = calculate_diffuse(light, dreflect, normal);
  s = calculate_specular(light, sreflect, view, normal);

  float r = a.red + d.red + s.red;
  float g = a.green + d.green + s.green;
  float b = a.blue + d.blue + s.blue;

  if (r < 0){
    i.red = 0;
  } else if (r > 255){
    i.red = 255;
  } else {
    i.red = r;
  }

  if (g < 0){
    i.green = 0;
  } else if (g > 255){
    i.green = 255;
  } else {
    i.green = g;
  }

  if (b < 0){
    i.blue = 0;
  } else if (b > 255){
    i.blue = 255;
  } else {
    i.blue = b;
  }

  //i.red = d.red;
  //i.green = d.green;
  //i.blue = d.blue;

  limit_color(&i);

  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;

  a.red = alight.red * areflect[0];
  a.green = alight.green * areflect[1];
  a.blue = alight.blue * areflect[2];

  //printf("%d %d %d\n", a.red, a.green, a.blue);

  //limit_color(&a);

  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  d.red = 0;
  d.green = 0;
  d.blue = 0;

  //double l[3] = {light[LOCATION][0] -}
  normalize(normal);
  normalize(light[LOCATION]);


  float r = (light[COLOR][RED] * dreflect[0] * (dot_product(normal, light[LOCATION])));
  float g = (light[COLOR][GREEN] * dreflect[1] * (dot_product(normal, light[LOCATION])));
  float b = (light[COLOR][BLUE] * dreflect[2] * (dot_product(normal, light[LOCATION])));

  if (r < 0){
    d.red = 0;
  } else {
    d.red = r;
  }

  if (g < 0){
    d.green = 0;
  } else {
    d.green = g;
  }

  if (b < 0){
    d.blue = 0;
  } else {
    d.blue = b;
  }

  //printf("%d %d %d\n", d.red, d.green, d.blue);

  //limit_color(&d);

  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;

  normalize(normal);
  normalize(light[LOCATION]);

  //long nl = dot_product(normal, light[LOCATION]);

  float r = light[COLOR][RED] * sreflect[0] * pow((dot_product(normal, light[LOCATION])), SPECULAR_EXP);
  float g = light[COLOR][GREEN] * sreflect[1] * pow((dot_product(normal, light[LOCATION])), SPECULAR_EXP);
  float b = light[COLOR][BLUE] * sreflect[2] * pow((dot_product(normal, light[LOCATION])), SPECULAR_EXP);

  if (r < 0){
    s.red = 0;
  } else if (r > 255){
    s.red = 255;
  } else {
    s.red = r;
  }

  if (g < 0){
    s.green = 0;
  } else if (g > 255){
    s.green = 255;
  } else {
    s.green = g;
  }

  if (b < 0){
    s.blue = 0;
  } else if (b > 255){
    s.blue = 255;
  } else {
    s.blue = b;
  }

  return s;
}

//limit each component of c to a max of 255
void limit_color( color * c ) {
  if (c->red > 255){
    c->red = 255;
  }

  if (c->red < 0){
    c->red = 0;
  }

  if (c->green > 255){
    c->green = 255;
  }

  if (c->green < 0){
    c->green = 0;
  }

  if (c->blue > 255){
    c->blue = 255;
  }

  if (c->blue < 0){
    c->blue = 0;
  }
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double magnitude;
  magnitude = sqrt( vector[0] * vector[0] +
                    vector[1] * vector[1] +
                    vector[2] * vector[2] );
  int i;
  for (i=0; i<3; i++) {
    vector[i] = vector[i] / magnitude;
  }
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


//Calculate the surface normal for the triangle whose first
//point is located at index i in polygons
double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
