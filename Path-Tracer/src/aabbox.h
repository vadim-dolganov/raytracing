#ifndef AABBOX_H
#define AABBOX_H

#include <cfloat>
#include <algorithm>

#include "vector.h"
#include "ray.h"

// Axis-aligned bounding box
struct AABBox {
    Vec bottomLeft;
    Vec topRight;

    AABBox (Vec bottomLeft_=Vec(), Vec topRight_=Vec()){
        bottomLeft=bottomLeft_, topRight=topRight_;
    }

    void expand(const AABBox &box) {
        if (box.bottomLeft.x < bottomLeft.x) bottomLeft.x = box.bottomLeft.x ;
        if (box.bottomLeft.y < bottomLeft.y) bottomLeft.y = box.bottomLeft.y;
        if (box.bottomLeft.z < bottomLeft.z) bottomLeft.z = box.bottomLeft.z;

        if (box.topRight.x > topRight.x) topRight.x = box.topRight.x;
        if (box.topRight.y > topRight.y) topRight.y = box.topRight.y;
        if (box.topRight.z > topRight.z) topRight.z = box.topRight.z ;
    }

    void expand(const Vec &vec) {
        if (vec.x < bottomLeft.x) bottomLeft.x = vec.x ;
        if (vec.y < bottomLeft.y) bottomLeft.y = vec.y;
        if (vec.z < bottomLeft.z) bottomLeft.z = vec.z;
    }

    // Возвращает самую длинную ось: 0, 1, 2 для x, y, z соответственно
    int get_longest_axis() {
        Vec diff = topRight - bottomLeft;
        if (diff.x > diff.y && diff.x > diff.z) return 0;
        if (diff.y > diff.x && diff.y > diff.z) return 1;
        return 2;
    }

    // Проверяем, пересекается ли луч с прямоугольником. Возвращает true/false и сохраняет расстояние в t
    bool intersection(const Ray &r, double &t) {
        double tx1 = (bottomLeft.x - r.origin.x)*r.direction_inv.x;
        double tx2 = (topRight.x - r.origin.x)*r.direction_inv.x;

        double tmin = std::min(tx1, tx2);
        double tmax = std::max(tx1, tx2);

        double ty1 = (bottomLeft.y - r.origin.y)*r.direction_inv.y;
        double ty2 = (topRight.y - r.origin.y)*r.direction_inv.y;

        tmin = std::max(tmin, std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));

        double tz1 = (bottomLeft.z - r.origin.z)*r.direction_inv.z;
        double tz2 = (topRight.z - r.origin.z)*r.direction_inv.z;

        tmin = std::max(tmin, std::min(tz1, tz2));
        tmax = std::min(tmax, std::max(tz1, tz2));
        t = tmin;

        return tmax >= tmin;
    }
};

#endif // AABBOX_H