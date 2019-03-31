#include "QuadricSurface.h"

class Ellipsoid : public QuadricSurface
{
    public:
    // a, b, and c for the equation of an ellipsoid
    double a, b, c;

    Ellipsoid(const dvec3 & position, const color & mat, double a, double b, double c);
    Ellipsoid(const dvec3 & position, const Material & mat, double a, double b, double c);
};
