#include "Constants.h"
#include <cmath>

double INTERSECTION_TOLERANCE = 1e-9;  // How far away a double has to be from 0 to NOT be seen as 0.
Vector3 CROSSPRODUCT_DUMMY = Vector3(0.0072, 1.0, 0.0034);  // Taken from Suffern listing 17.5.

bool isBasicallyZero(double x) { return std::abs(x) < INTERSECTION_TOLERANCE; }
bool isNegativeOrAcne(double t) { return t < INTERSECTION_TOLERANCE; }