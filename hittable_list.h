#pragma once

#include "hittable.h"

#include <memory>
#include <vector>


class HittableList : public hittable
{
    public: 
        HittableList(){}
        HittableList(std::shared_ptr<hittable> object)
        {
            add(object);
        }

        void add(std::shared_ptr<hittable> object) 
        {
            list.push_back(object);
        }
        void clear() 
        {
            list.clear();
        }

        virtual bool hit(const ray& r, const double& min_t, const double& max_t, hit_record& hitrecord) const override;

        std::vector<std::shared_ptr<hittable>> list;
      
};

bool HittableList::hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const
{
    hit_record temp_rec;
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

