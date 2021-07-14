//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        //  如果物体自发光(是光源)，求得光源的面积，pdf=1/S进行采样
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    // Gong: 随机生成一个浮点数 * 光源面积和 ???
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            // Gong: 仅对光源面积和大于p时进行采样 ???
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray& ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here

    //求入射光线wo的交点
    Intersection intersection = intersect(ray);
    Vector3f hitcolor = Vector3f(0);

    //判断是不是直接采样到光源了
    if (intersection.emit.norm() > 0)
        hitcolor = Vector3f(1);
    else if (intersection.happened)
    {
        //确定入射光线的方向、相交点坐标、交点法线
        Vector3f wo = normalize(-ray.direction);
        Vector3f p = intersection.coords;
        Vector3f N = normalize(intersection.normal);

        //对光源取一个采样点，确定采样点的方向、相交点坐标、交点法线
        // Gong: 如何采样？？？
        float pdf_light = 0.0f;
        Intersection inter;
        sampleLight(inter, pdf_light);     //光源内随机采样一个点
        Vector3f x = inter.coords;
        Vector3f ws = normalize(x - p);
        Vector3f NN = normalize(inter.normal);

        Vector3f L_dir = Vector3f(0.0f);

        //求直接光照
        if ((intersect(Ray(p, ws)).coords - x).norm() < 0.01) //判断灯光有没有遮挡
        {
            L_dir = inter.emit * intersection.m->eval(wo, ws, N) * dotProduct(ws, N) * dotProduct(-ws, NN)/ (((x - p).norm() * (x - p).norm()) * pdf_light);
        }

        //求间接光照
        Vector3f L_indir = Vector3f(0);
        float P_RR = get_random_float();
        if (P_RR < Scene::RussianRoulette)
        {
            //采样一个漫反射方向
            Vector3f wi = intersection.m->sample(wo, N);
            L_indir = castRay(Ray(p, wi), depth) * intersection.m->eval(wi, wo, N) * dotProduct(wi, N) / (intersection.m->pdf(wi, wo, N) * Scene::RussianRoulette);
        }
        hitcolor = L_indir + L_dir;
    }
    return hitcolor;
}
