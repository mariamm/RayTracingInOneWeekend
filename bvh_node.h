#pragma once

#include "hittable.h"
#include "hittable_list.h"
#include "ray.h"
#include "vec3.h"
#include "util.h"

#include <algorithm>
#include <vector>

using std::shared_ptr;
using std::make_shared;
using std::vector;

class BvhNode : public Hittable
{
public:

    BvhNode(){}
    BvhNode(const HittableList &hittableList, double time_0, double time_1) 
        : BvhNode(hittableList.list, 0, hittableList.list.size(), time_0, time_1)
    {

    }
    BvhNode(const vector<shared_ptr<Hittable>>& objectsList, size_t start, size_t end, double time_0, double time_1);

    virtual bool hit(const Ray& r, const double& min_t, const double& max_t, HitRecord& hitrecord) const override;
    virtual bool boundingBox(double time0, double time1, aabb& output_box) const override;

private:

    shared_ptr<Hittable> leftNode;
    shared_ptr<Hittable> rightNode;

    aabb bBox;
};

inline bool compareObjects(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b, int axis)
{
    aabb bboxA;
    aabb bboxB;
    if (!a->boundingBox(0, 0, bboxA) || !b->boundingBox(0, 0, bboxB))
    {
        std::cerr << "No bounding box in bvh_node constructor.\n";
        return false;
    }

    return bboxA.minimum()[axis] < bboxB.minimum()[axis];
}

bool box_x_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
{
    return compareObjects(a, b, 0);
}

bool box_y_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
{
    return compareObjects(a, b, 1);
}

bool box_z_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
{
    return compareObjects(a, b, 2);
}
BvhNode::BvhNode(const vector<shared_ptr<Hittable>>& objectsList, size_t start, size_t end, double time_0, double time_1)
{
    vector<shared_ptr<Hittable>> objects = objectsList;

    //choose compare function for a random axis
    int axis = random_int(0, 2);
    auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;

    size_t span = end - start;

    if (span == 1)
    {
        leftNode = rightNode = objects[start];
    }
    else if (span == 2)
    {
        if (objects[start] > objects[end - 1])
            swap(objects[start], objects[end - 1]);
        leftNode = objects[start];
        rightNode = objects[end - 1];
    }
    else
    {
        sort(objects.begin() + start, objects.begin() + end, comparator);
        size_t mid = start + span / 2;
        leftNode = make_shared<BvhNode>(objects, start, mid, time_0, time_1);
        rightNode = make_shared<BvhNode>(objects, mid, end, time_0, time_1);
    }

    aabb bboxA;
    aabb bboxB;
    if (!leftNode->boundingBox(time_0, time_1, bboxA) || !rightNode->boundingBox(time_0, time_1, bboxB))
    {
        std::cerr << "No bounding box in bvh_node constructor.\n";

    }

    bBox.surround(bboxA);
    bBox.surround(bboxB);
}
bool BvhNode::hit(const Ray& r, const double& min_t, const double& max_t, HitRecord& hitrecord) const
{
    if (!bBox.hit(r, min_t, max_t))
        return false;

    if(leftNode->hit(r, min_t, max_t, hitrecord))
        return true;

    if(rightNode->hit(r, min_t, max_t, hitrecord))
        return true;

    return false;
}

bool BvhNode::boundingBox(double time0, double time1, aabb& output_box) const
{
    output_box = bBox;
    return true;
}