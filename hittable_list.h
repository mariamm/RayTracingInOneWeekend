#pragma once

#include "hittable.h"

#include <memory>
#include <vector>


class HittableList : public Hittable
{
    public: 
        HittableList(){}
        HittableList(std::shared_ptr<Hittable> object)
        {
            add(object);
        }

        void add(std::shared_ptr<Hittable> object) 
        {
            list.push_back(object);
        }
        void clear() 
        {
            list.clear();
        }

        virtual bool hit(const Ray& r, const double& min_t, const double& max_t, HitRecord& hitrecord) const override;
        virtual bool boundingBox(double time0, double time1, aabb& output_box) const override;

        std::vector<std::shared_ptr<Hittable>> list;
      
};

bool HittableList::hit(const Ray& r, const double& t_min, const double& t_max, HitRecord& rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : list) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool HittableList::boundingBox(double time0, double time1, aabb& output_box) const
{
    bool found_anything = false;
    output_box = aabb();
    for (const auto& object : list)
    {
        aabb temp;
        if(object->boundingBox(time0, time1, temp))
        {
            found_anything = true;
            output_box.surround(temp);
        }
    }
    return found_anything;
}
