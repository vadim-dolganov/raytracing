#ifndef MATERIAL_H
#define MATERIAL_H

#include "../lib/rand48/erand48.h"
#include "vector.h"
#include "ray.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

enum MaterialType { DIFF, SPEC, EMIT };


class Material {

private:
	MaterialType m_type;
	Vec m_colour;
	Vec m_emission;


public:
	Material( MaterialType t=DIFF, Vec c=Vec(1,1,1), Vec e=Vec(0,0,0));
	MaterialType get_type() const;
	Vec get_colour() const;
	Vec getColourAt(double u, double v) const;
	Vec get_emission() const;
	Ray get_reflected_ray( const Ray &r, Vec &p, const Vec &n, unsigned short *Xi ) const;

};


#endif // MATERIAL_H