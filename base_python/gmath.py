import math
from display import *


  # IMPORANT NOTE

  # Ambient light is represeneted by a color value

  # Point light sources are 2D arrays of doubles.
  #      - The fist index (LOCATION) represents the vector to the light.
  #      - The second index (COLOR) represents the color.

  # Reflection constants (ka, kd, ks) are represened as arrays of
  # doubles (red, green, blue)

AMBIENT = 0
DIFFUSE = 1
SPECULAR = 2
LOCATION = 0
COLOR = 1
SPECULAR_EXP = 4

#lighting functions
def get_lighting(normal, view, ambient, light, areflect, dreflect, sreflect ):
    color = [0,0,0];
    ambient = [0,0,0];
    diffuse = [0,0,0];
    specular = [0,0,0];

    a = calculate_ambient(ambient, areflect);
    d = calculate_diffuse(light, dreflect, normal);
    s = calculate_specular(light, sreflect, view, normal);

    color[0] = int(a[0]) + int(d[0]) + int(s[0]);
    color[1] = int(a[1]) + int(d[1]) + int(s[1]);
    color[2] = int(a[2]) + int(d[2]) + int(s[2]);

    #color[0] = int(d[0])
    #color[1] = int(d[1])
    #color[2] = int(d[2])

    limit_color(color);

    return color;

def calculate_ambient(alight, areflect):
    a = [0, 0, 0];

    a[0] = alight[0] * areflect[0];
    a[1] = alight[1] * areflect[1];
    a[2] = alight[2] * areflect[2];

    return a;

def calculate_diffuse(light, dreflect, normal):
    d = [0, 0, 0];
    normalize(light[LOCATION]);
    normalize(normal);

    d[0] = light[COLOR][0] * dreflect[0] * dot_product(normal, light[LOCATION]);
    d[1] = light[COLOR][1] * dreflect[1] * dot_product(normal, light[LOCATION]);
    d[2] = light[COLOR][2] * dreflect[2] * dot_product(normal, light[LOCATION]);

    #print(d);
    print(int(d[0]), int(d[1]), int(d[2]))

    return d;

def calculate_specular(light, sreflect, view, normal):
    s = [0, 0, 0];
    normalize(light[LOCATION]);
    normalize(normal);

    dpnl = (dot_product(normal, light[LOCATION]));

    for i in range (0, 3):
        normal[i] = 2 * normal[i] * dpnl - light[LOCATION][i];

    s[0] = light[COLOR][0] * sreflect[0] * pow(dot_product(normal, view), SPECULAR_EXP);
    s[1] = light[COLOR][1] * sreflect[1] * pow(dot_product(normal, view), SPECULAR_EXP);
    s[2] = light[COLOR][2] * sreflect[2] * pow(dot_product(normal, view), SPECULAR_EXP);

    return s;

def limit_color(color):
    for i in range(0,3):
        if color[i] > 255:
            color[i] = 255;

#vector functions
#normalize vetor, should modify the parameter
def normalize(vector):
    magnitude = math.sqrt( vector[0] * vector[0] +
                           vector[1] * vector[1] +
                           vector[2] * vector[2])
    for i in range(3):
        vector[i] = vector[i] / magnitude

#Return the dot porduct of a . b
def dot_product(a, b):
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]

#Calculate the surface normal for the triangle whose first
#point is located at index i in polygons
def calculate_normal(polygons, i):

    A = [0, 0, 0]
    B = [0, 0, 0]
    N = [0, 0, 0]

    A[0] = polygons[i+1][0] - polygons[i][0]
    A[1] = polygons[i+1][1] - polygons[i][1]
    A[2] = polygons[i+1][2] - polygons[i][2]

    B[0] = polygons[i+2][0] - polygons[i][0]
    B[1] = polygons[i+2][1] - polygons[i][1]
    B[2] = polygons[i+2][2] - polygons[i][2]

    N[0] = A[1] * B[2] - A[2] * B[1]
    N[1] = A[2] * B[0] - A[0] * B[2]
    N[2] = A[0] * B[1] - A[1] * B[0]

    return N
