#include <algorithm>
#include <cassert>
#include <map>
#include "BVH.hpp"

BVHAccel::BVHAccel(std::vector<Object *> p, int maxPrimsInNode,
                   SplitMethod splitMethod)
    : maxPrimsInNode(std::min(255, maxPrimsInNode)), splitMethod(splitMethod),
      primitives(std::move(p))
{
    time_t start, stop;
    time(&start);
    if (primitives.empty())
        return;

    root = recursiveBuild(primitives);

    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff / 3600;
    int mins = ((int)diff / 60) - (hrs * 60);
    int secs = (int)diff - (hrs * 3600) - (mins * 60);

    printf(
        "\rBVH Generation complete: \nTime Taken: %i hrs, %i mins, %i secs\n\n",
        hrs, mins, secs);
}

BVHBuildNode *BVHAccel::recursiveBuild(std::vector<Object *> objects)
{
    BVHBuildNode *node = new BVHBuildNode();

    // Compute bounds of all primitives in BVH node
    Bounds3 bounds; // 这个和MeshTriangle中的bounding_box一致 就是求所有格网点构成的最大包围和
    for (int i = 0; i < objects.size(); ++i)
        bounds = Union(bounds, objects[i]->getBounds());

    if (objects.size() == 1)
    {
        // Create leaf _BVHBuildNode_
        node->bounds = objects[0]->getBounds();
        node->object = objects[0];
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }
    else if (objects.size() == 2)
    {
        node->left = recursiveBuild(std::vector{objects[0]});
        node->right = recursiveBuild(std::vector{objects[1]});

        node->bounds = Union(node->left->bounds, node->right->bounds);
        return node;
    }
    else
    {
        Bounds3 centroidBounds;
        for (int i = 0; i < objects.size(); ++i) // 确定划分的分界点位
            centroidBounds =
                Union(centroidBounds, objects[i]->getBounds().Centroid());

        std::vector<Object*> leftshapes;
        std::vector<Object*> rightshapes;

        switch (splitMethod)
        {
            case SplitMethod::NAIVE:
            {
                int dim = centroidBounds.maxExtent(); // 确定当前范围最大的一维 用作划分objects
                switch (dim)
                { // 按照Bounds的中心进行排序
                case 0:
                    std::sort(objects.begin(), objects.end(), [](auto f1, auto f2)
                            { return f1->getBounds().Centroid().x <
                                    f2->getBounds().Centroid().x; });
                    break;
                case 1:
                    std::sort(objects.begin(), objects.end(), [](auto f1, auto f2)
                            { return f1->getBounds().Centroid().y <
                                    f2->getBounds().Centroid().y; });
                    break;
                case 2:
                    std::sort(objects.begin(), objects.end(), [](auto f1, auto f2)
                            { return f1->getBounds().Centroid().z <
                                    f2->getBounds().Centroid().z; });
                    break;
                }

                auto beginning = objects.begin();
                auto middling = objects.begin() + (objects.size() / 2);
                auto ending = objects.end();

                leftshapes = std::vector<Object *>(beginning, middling);
                rightshapes = std::vector<Object *>(middling, ending);

                break;
            }
            case SplitMethod::SAH:
            {
                Bounds3 nBounds;
                for (int i = 0; i < objects.size(); ++i)
                    nBounds = Union(nBounds, objects[i]->getBounds());
                float nArea = centroidBounds.SurfaceArea();

                int minCostCoor = 0;
                int mincostIndex = 0;
                float minCost = std::numeric_limits<float>::infinity();
                std::map<int, std::map<int, int>> indexMap;
                for(int i = 0; i < 3; i++)
                {
                    int bucketCount = 12;
                    std::vector<Bounds3> boundsBuckets;
                    std::vector<int> countBucket;
                    for(int j = 0; j < bucketCount; j++)
                    {
                        boundsBuckets.push_back(Bounds3());
                        countBucket.push_back(0);
                    }

                    std::map<int, int> objMap;

                    for(int j = 0; j < objects.size(); j++)
                    {
                        Vector3f tmp = centroidBounds.Offset(objects[j]->getBounds().Centroid());
                        int bid =  bucketCount * tmp[i];
                        if(bid > bucketCount - 1)
                        {
                            bid = bucketCount - 1;
                        }
                        Bounds3 b = boundsBuckets[bid];
                        b = Union(b, objects[j]->getBounds().Centroid());
                        boundsBuckets[bid] = b;
                        countBucket[bid] = countBucket[bid] + 1;
                        objMap.insert(std::make_pair(j, bid));
                    }

                    indexMap.insert(std::make_pair(i, objMap));

                    for(int j = 1; j < boundsBuckets.size(); j++)
                    {
                        Bounds3 A;
                        Bounds3 B;
                        int countA = 0;
                        int countB = 0;
                        for(int k = 0; k < j; k++)
                        {
                            A = Union(A, boundsBuckets[k]);
                            countA += countBucket[k];
                        }

                        for(int k = j; k < boundsBuckets.size(); k++)
                        {
                            B = Union(B, boundsBuckets[k]);
                            countB += countBucket[k];
                        }

                        float cost = 1 + (countA * A.SurfaceArea() + countB * B.SurfaceArea()) / nArea;

                        if(cost < minCost)
                        {
                            minCost = cost;
                            mincostIndex = j;
                            minCostCoor = i;
                        }
                    }
                }

                for(int i = 0; i < objects.size(); i++)
                {
                    if(indexMap[minCostCoor][i] < mincostIndex)
                    {
                        leftshapes.push_back(objects[i]);
                    }
                    else
                    {
                        rightshapes.push_back(objects[i]);
                    }
                }
                break;
            }
            
            default:
                break;
        }
        assert(objects.size() == (leftshapes.size() + rightshapes.size()));

        node->left = recursiveBuild(leftshapes);
        node->right = recursiveBuild(rightshapes);

        node->bounds = Union(node->left->bounds, node->right->bounds);
    }

    return node;
}

Intersection BVHAccel::Intersect(const Ray &ray) const
{
    Intersection isect;
    if (!root)
        return isect;
    isect = BVHAccel::getIntersection(root, ray);
    return isect;
}

Intersection BVHAccel::getIntersection(BVHBuildNode *node, const Ray &ray) const
{
    // TODO Traverse the BVH to find intersection
    Vector3f invDir = Vector3f{1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z};
    std::array<int, 3> dirIsNeg = {ray.direction.x > 0, ray.direction.y > 0, ray.direction.z > 0};

    // 先判断与当前的包围和节点是否相交 不相交则返回 相交则递归对当前节点的左右节点求交
    if (!node->bounds.IntersectP(ray, invDir, dirIsNeg))
    {
        return {};
    }

    if (node->left == nullptr && node->right == nullptr)
    {
        // 如果是叶子节点中的BVH相交，调用BVH节点Node中的物体进行求交，计算光线与物体的交点hitPoint
        return node->object->getIntersection(ray);
    }

    Intersection h1 = getIntersection(node->left, ray);
    Intersection h2 = getIntersection(node->right, ray);

    return h1.distance < h2.distance ? h1 : h2;
}