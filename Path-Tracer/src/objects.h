#ifndef OBJECTS_H
#define OBJECTS_H

#include "vector.h"
#include "ray.h"
#include "triangle.h"
#include "kdtree.h"
#include "material.h"
#include "../lib/tiny_obj_loader/tiny_obj_loader.h"

struct ObjectIntersection {
	bool hit;	// было ли пересечение луча
	double u;	// Расстояние до пересечения луча
	Vec n;		// Нормаль пересекаемой грани
	Material m;	// Материал пересекаемой грани

	ObjectIntersection(bool hit_=false, double u_=0, Vec n_=Vec(), Material m_=Material());
};


class Object {

public:
	Vec m_p; // Position
	virtual ObjectIntersection get_intersection(const Ray &r) = 0;
};


class Sphere : public Object {

private:
	double m_r;	// Radius
	Material m_m;	// Material

public:
	Sphere(Vec p_, double r_, Material m_);	
	virtual double get_radius();
	virtual Material get_material();

	virtual ObjectIntersection get_intersection(const Ray &r);
};


class Mesh : public Object {

private:
	std::vector<tinyobj::shape_t> m_shapes;
    std::vector<tinyobj::material_t> m_materials;
    std::vector<Material> materials;
    std::vector<Triangle*> tris;
    Material m_m;	// Material
    KDNode *node;

public:
	Mesh(Vec p_, std::string file_path, Material m_);
	virtual ObjectIntersection get_intersection(const Ray &r);

};


#endif // OBJECTS_H