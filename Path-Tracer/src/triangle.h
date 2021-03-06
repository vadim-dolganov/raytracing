#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <algorithm>
#include <cfloat>

#include "vector.h"
#include "aabbox.h"
#include "material.h"

struct Triangle {
    Vec v0, v1, v2;
    Vec e1, e2;
    Vec n, t0, t1, t2;
    Material *m;

    Triangle(Vec v0_, Vec v1_, Vec v2_, Vec t0_=Vec(), Vec t1_=Vec(), Vec t2_=Vec(), Material *m_=NULL){
        v0=v0_, v1=v1_, v2=v2_, e1=v1-v0, e2=v2-v0, n=e1.cross(e2).norm();
        t0=t0_, t1=t1_, t2=t2_;
        m=m_;
    }

    AABBox getBoundingBox(){
        Vec bottomLeft = Vec(
                std::min (std::min(v0.x, v1.x), v2.x ) ,
                std::min (std::min(v0.y, v1.y), v2.y ) ,
                std::min (std::min(v0.z, v1.z), v2.z )
        );
        Vec topRight = Vec(
                std::max (std::max(v0.x, v1.x), v2.x ) ,
                std::max (std::max(v0.y, v1.y), v2.y ) ,
                std::max (std::max(v0.z, v1.z), v2.z )
        );

        return AABBox(bottomLeft, topRight);
    }

    // Возвращает середину треугольника
    Vec getMidpoint(){
        return (v0 + v1 + v2)/3;
    }

    // Проверяет, пересекается ли луч с треугольником. Возвращает истину/ложь.
    bool intersect(Ray ray, double &t, double tmin, Vec &norm) const {

        double u, v, t_temp=0;

        Vec pvec = ray.direction.cross(e2);
        double det = e1.dot(pvec);
        if (det == 0) return false;
        double invDet = 1. / det;
        Vec tvec = ray.origin - v0;
        u = tvec.dot(pvec) * invDet;
        if (u < 0 || u > 1) return false;
        Vec qvec = tvec.cross(e1);
        v = ray.direction.dot(qvec) * invDet;
        if (v < 0 || u + v > 1) return false;
        t_temp = e2.dot(qvec) * invDet;
        if (t_temp < tmin) {
            if (t_temp > 1e-9 ){
                t = t_temp;
                norm = n;
                return true;
            }
        }
        return false;
    }

    Vec barycentric(Vec p){
        Vec v2_ = p - v0;
        double d00 = e1.dot(e1);
        double d01 = e1.dot(e2);
        double d11 = e2.dot(e2);
        double d20 = v2_.dot(e1);
        double d21 = v2_.dot(e2);
        double d = d00*d11 - d01*d01;
        double v = (d11*d20 - d01*d21) / d;
        double w = (d00*d21 - d01*d20) / d;
        double u = 1 - v - w;
        return Vec(u, v, w);
    }

    Vec getColourAt(Vec p){
        if(m==NULL) return Vec(1,0,1);

        Vec b = barycentric(p);
        Vec c = Vec();
        c = c + (t0 * b.x);
        c = c + (t1 * b.y);
        c = c + (t2 * b.z);

        return m->getColourAt(c.x, c.y);
    }

};

#endif // TRIANGLE_H