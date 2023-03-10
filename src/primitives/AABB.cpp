#include "AABB.h"

AABB::AABB(const Vector3& minimalCorner, const Vector3& maximalCorner)
    : minimalCorner(minimalCorner), maximalCorner(maximalCorner) {}


double AABB::intersect(const Ray &ray) const {
    // Strategy (no idea if this is the canonical way to do this):
    //  1. Accumulate the "biggest smallest" and "smallest biggest" for three dimensions;
    //     whenever you encounter a singularity, check for bounds.
    //  2. If the biggest smallest t is smaller than the smallest biggest t, it is the intersecting t.
    //     Otherwise, you have left one of the three slabs before having entered all three.

    // Accumulation
    double biggestSmallestT = -1e99;
    double smallestBiggestT = +1e99;

    if (isBasicallyZero(ray.d.x)) {
        if (ray.p0.x < this->minimalCorner.x || ray.p0.x > this->maximalCorner.x)
            return -1;
    } else {
        // p(t) = p0 + td   =>   p_x(t) = p0_x + t d_x   =>   t = (p_x(t) - p0_x)/d_x
        double tx1 = (this->minimalCorner.x - ray.p0.x)/ray.d.x;
        double tx2 = (this->maximalCorner.x - ray.p0.x)/ray.d.x;

        biggestSmallestT = std::max(biggestSmallestT, std::min(tx1,tx2));
        smallestBiggestT = std::min(smallestBiggestT, std::max(tx1,tx2));
    }

    if (isBasicallyZero(ray.d.y)) {
        if (ray.p0.y < this->minimalCorner.y || ray.p0.y > this->maximalCorner.y)
            return -1;
    } else {
        double ty1 = (this->minimalCorner.y - ray.p0.y)/ray.d.y;
        double ty2 = (this->maximalCorner.y - ray.p0.y)/ray.d.y;

        biggestSmallestT = std::max(biggestSmallestT, std::min(ty1,ty2));
        smallestBiggestT = std::min(smallestBiggestT, std::max(ty1,ty2));
    }

    if (isBasicallyZero(ray.d.z)) {
        if (ray.p0.z < this->minimalCorner.z || ray.p0.z > this->maximalCorner.z)
            return -1;
    } else {
        double tz1 = (this->minimalCorner.z - ray.p0.z)/ray.d.z;
        double tz2 = (this->maximalCorner.z - ray.p0.z)/ray.d.z;

        biggestSmallestT = std::max(biggestSmallestT, std::min(tz1,tz2));
        smallestBiggestT = std::min(smallestBiggestT, std::max(tz1,tz2));
    }

    // Final decision
    if (biggestSmallestT < smallestBiggestT) {
        return biggestSmallestT;
    } else {
        return -1;
    }
}

/**
 * Imagine for a moment that all points p are on the outside of one of the six faces.
 * Every such point lies between the extrema of two dimensions, and is either smaller than the minimum or bigger than the maximum of the third dimension.
 * There is a complication: rays that come from inside the AABB will have a point p that lies just inside the walls, not outside.
 * Currently, this is solved by adding acne margin.
 */
Vector3 AABB::outwardNormal(const Vector3& p) const {
    if (isNegativeOrAcne(p.x - this->minimalCorner.x))  // Assume y and z are within bounds.
        return Vector3(-1, 0, 0);

    if (isNegativeOrAcne(this->maximalCorner.x - p.x))  // Idem.
        return Vector3(+1, 0, 0);

    if (isNegativeOrAcne(p.y - this->minimalCorner.y))  // Assume x and z are within bounds.
        return Vector3(0, -1, 0);

    if (isNegativeOrAcne(this->maximalCorner.y - p.y))
        return Vector3(0, +1, 0);

    if (isNegativeOrAcne(p.z - this->minimalCorner.z))
        return Vector3(0, 0, -1);

    if (isNegativeOrAcne(this->maximalCorner.z - p.z))
        return Vector3(0, 0, +1);

    // Should never occur.
    return Vector3(0, 0, 0);
}

double AABB::randomPoint(RNG &rng, Vector3 &result) const {
    double deltaX = this->maximalCorner.x - this->minimalCorner.x;
    double deltaY = this->maximalCorner.y - this->minimalCorner.y;
    double deltaZ = this->maximalCorner.z - this->minimalCorner.z;

    double area1 = deltaX * deltaY;
    double area2 = deltaY * deltaZ;
    double area3 = deltaZ * deltaX;

    double totalArea = 2*area1 + 2*area2 + 2*area3;
    double sampleWhichSide = totalArea * rng.rand01();

    double p1 = rng.rand01();
    double p2 = rng.rand01();

    // The following cascade does one-sided checks which are two-sided exactly because they cascade.
    if (sampleWhichSide < area1) {
        result = Vector3(
                this->minimalCorner.x + p1*deltaX,
                this->minimalCorner.y + p2*deltaY,
                this->minimalCorner.z);
    } else if (sampleWhichSide < 2*area1) {  // The "else" means that sampleWhichSide > area1.
        result = Vector3(
                this->minimalCorner.x + p1*deltaX,
                this->minimalCorner.y + p2*deltaY,
                this->maximalCorner.z);
    } else if (sampleWhichSide < 2*area1 + area2) {
        result = Vector3(
                this->minimalCorner.x,
                this->minimalCorner.y + p1*deltaY,
                this->minimalCorner.z + p2*deltaZ);
    } else if (sampleWhichSide < 2*area1 + 2*area2) {
        result = Vector3(
                this->maximalCorner.x,
                this->minimalCorner.y + p1*deltaY,
                this->minimalCorner.z + p2*deltaZ);
    } else if (sampleWhichSide < 2*area1 + 2*area2 + area3) {
        result = Vector3(
                this->minimalCorner.x + p1*deltaX,
                this->minimalCorner.y,
                this->minimalCorner.z + p2*deltaZ);
    } else {
        result = Vector3(
                this->minimalCorner.x + p1*deltaX,
                this->maximalCorner.y,
                this->minimalCorner.z + p2*deltaZ);
    }

    return 1.0/totalArea;
}
