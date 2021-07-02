// clang-format off
//
// Created by goksu on 4/6/19.
//

#include <algorithm>
#include <vector>
#include "rasterizer.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>

rst::pos_buf_id rst::rasterizer::load_positions(const std::vector<Eigen::Vector3f> &positions)
{
    auto id = get_next_id();
    pos_buf.emplace(id, positions);

    return {id};
}

rst::ind_buf_id rst::rasterizer::load_indices(const std::vector<Eigen::Vector3i> &indices)
{
    auto id = get_next_id();
    ind_buf.emplace(id, indices);

    return {id};
}

rst::col_buf_id rst::rasterizer::load_colors(const std::vector<Eigen::Vector3f> &cols)
{
    auto id = get_next_id();
    col_buf.emplace(id, cols);

    return {id};
}

auto to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
    return Vector4f(v3.x(), v3.y(), v3.z(), w);
}


static bool insideTriangle(int x, int y, const Vector3f* _v)
{   
    // TODO : Implement this function to check if the point (x, y) is inside the triangle represented by _v[0], _v[1], _v[2]
    Vector3f P(x+0.5, y+0.5, 1.0);
    const Vector3f& A = _v[0];
    const Vector3f& B = _v[1];
    const Vector3f& C = _v[2];
    //
    Vector3f AB = B - A;
    Vector3f BC = C - B;
    Vector3f CA = A - C;
    //
    Vector3f AP = P - A;
    Vector3f BP = P - B;
    Vector3f CP = P - C;
    //
    float z1 = AB.cross(AP).z();
    float z2 = BC.cross(BP).z();
    float z3 = CA.cross(CP).z();
    //
    return (z1 > 0 && z2 >0 && z3 > 0) ||  (z1 < 0 && z2 <0 && z3 < 0);
}

static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Vector3f* v)
{
    float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
    float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
    float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
    return {c1,c2,c3};
}

void rst::rasterizer::draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, Primitive type)
{
    auto& buf = pos_buf[pos_buffer.pos_id];
    auto& ind = ind_buf[ind_buffer.ind_id];
    auto& col = col_buf[col_buffer.col_id];

    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;

    Eigen::Matrix4f mvp = projection * view * model;
    for (auto& i : ind)
    {
        Triangle t;
        Eigen::Vector4f v[] = {
                mvp * to_vec4(buf[i[0]], 1.0f),
                mvp * to_vec4(buf[i[1]], 1.0f),
                mvp * to_vec4(buf[i[2]], 1.0f)
        };
        //Homogeneous division
        for (auto& vec : v) {
            vec /= vec.w();
            // see the points
            // std::cout << vec.x() << "  " << vec.y() << "  " << vec.z() << std::endl;
        }
        
        //Viewport transformation
        for (auto & vert : v)
        {
            vert.x() = 0.5*width*(vert.x()+1.0);
            vert.y() = 0.5*height*(vert.y()+1.0);
            vert.z() = vert.z() * f1 + f2;
        }

        for (int i = 0; i < 3; ++i)
        {
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
        }

        auto col_x = col[i[0]];
        auto col_y = col[i[1]];
        auto col_z = col[i[2]];

        t.setColor(0, col_x[0], col_x[1], col_x[2]);
        t.setColor(1, col_y[0], col_y[1], col_y[2]);
        t.setColor(2, col_z[0], col_z[1], col_z[2]);

        // rasterize_triangle(t);
        rasterize_scanline(t);
    }
}

//Screen space rasterization
void rst::rasterizer::rasterize_triangle(const Triangle& t) {
    auto v = t.toVector4();
    for(auto& vv:v)
    {
        std::cout << vv.x() << "  " << vv.y() << "  " << vv.z() << "  " << vv.w() << std::endl;
    }

    // TODO : Find out the bounding box of current triangle.
    float minx=0, maxx=0, miny=0, maxy=0;
    // serch 3 vertex points
    for(size_t i = 0; i < 3; ++i)
    {
        const Vector3f& pcur = t.v[i];
        if(i==0)
        {
            minx = maxx = pcur.x();
            miny = maxy = pcur.y();
            continue;
        }
        minx = pcur.x() < minx ? pcur.x():minx;
        miny = pcur.y() < miny ? pcur.y():miny;
        maxx = pcur.x() > maxx ? pcur.x():maxx;
        maxy = pcur.y() > maxy ? pcur.y():maxy;
    }
    // iterate through the pixel and find if the current pixel is inside the triangle
    for(int x = static_cast<int>(minx); x < maxx; ++x)
    {
        for(int y = static_cast<int>(miny); y < maxy; ++y)
        {
            // // not inside 
            // if(!insideTriangle(x,y,t.v)) continue;
            // //
            // // If so, use the following code to get the interpolated z value.
            // //auto[alpha, beta, gamma] = computeBarycentric2D(x, y, t.v);
            // //float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
            // //float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
            // //z_interpolated *= w_reciprocal;
            // auto[alpha, beta, gamma] = computeBarycentric2D(x, y, t.v);
            // float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
            // float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
            // z_interpolated *= w_reciprocal;

            // // TODO : set the current pixel (use the set_pixel function) to the color of the triangle (use getColor function) if it should be painted.
            // int buf_index = get_index(x,y);     
            // if(z_interpolated >= depth_buf[buf_index]) continue;
            // depth_buf[buf_index] = z_interpolated;
            // set_pixel(Vector3f(x,y,1),t.getColor());

            // MSAA
            int count = 0;
            if(insideTriangle(x + 0.25f,y + 0.25f,t.v)){
                MSAA(x, y, 0.25f, 0.25f, t, count);
            }
            if(insideTriangle(x + 0.75f,y + 0.25f,t.v)){
                MSAA(x, y, 0.75f, 0.25f, t, count);
            }
            if(insideTriangle(x + 0.75f,y + 0.75f,t.v)){
                MSAA(x, y, 0.75f, 0.75f, t, count);
            }
            if(insideTriangle(x + 0.25f,y + 0.75f,t.v)){
                MSAA(x, y, 0.25f, 0.75f, t, count);
            }
            //
            if(count!=0)
                set_pixel(Vector3f(x,y,1),t.getColor()*count / 4.0);
        }
    }
}

void rst::rasterizer::set_model(const Eigen::Matrix4f& m)
{
    model = m;
}

void rst::rasterizer::set_view(const Eigen::Matrix4f& v)
{
    view = v;
}

void rst::rasterizer::set_projection(const Eigen::Matrix4f& p)
{
    projection = p;
}

void rst::rasterizer::clear(rst::Buffers buff)
{
    if ((buff & rst::Buffers::Color) == rst::Buffers::Color)
    {
        std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f{0, 0, 0});
    }
    if ((buff & rst::Buffers::Depth) == rst::Buffers::Depth)
    {
        std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity());
    }
}

rst::rasterizer::rasterizer(int w, int h) : width(w), height(h)
{
    frame_buf.resize(w * h);
    depth_buf.resize(w * h);
}

int rst::rasterizer::get_index(int x, int y)
{
    return (height-1-y)*width + x;
}

void rst::rasterizer::set_pixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    auto ind = (height-1-point.y())*width + point.x();
    frame_buf[ind] = color;

}

// clang-format on

void rst::rasterizer::rasterize_scanline(const Triangle& t)
{
    Eigen::Vector3f arr[] = {t.v[0],t.v[1],t.v[2]};
    if (arr[0].y() > arr[1].y()) {
        Vector3f tmp = arr[0];
        arr[0] = arr[1];
        arr[1] = tmp;
    }
    if (arr[1].y() > arr[2].y()) {
        Vector3f tmp = arr[1];
        arr[1] = arr[2];
        arr[2] = tmp;
    }
    if (arr[0].y() > arr[1].y()) {
        Vector3f tmp = arr[0];
        arr[0] = arr[1];
        arr[1] = tmp;
    }
    //需要按照不同三角形分别处理
    if (arr[0].y() == arr[1].y())
    {
        update_top({arr[1],arr[0],arr[2]}, t);
    }
    else if (arr[1].y() == arr[2].y())
    {
        update_bottom({arr[1],arr[2],arr[0]}, t);
    }
    else
    {   
        float weight = (arr[2].y() - arr[1].y()) / (arr[2].y() - arr[0].y());
        float pos_x;
        pos_x = arr[2].x() - (arr[2].x() - arr[0].x()) * weight;
        Eigen::Vector3f new_p = { pos_x,arr[1].y(),1.f };
        update_top({ arr[1],new_p,arr[2] },t);
        update_bottom({ arr[1],new_p,arr[0] }, t);
    }
}

void rst::rasterizer::update_top(const std::vector<Eigen::Vector3f> new_t, const Triangle& t)
{

    Eigen::Vector3f left = new_t[0].x() < new_t[1].x() ? new_t[0]: new_t[1];
    Eigen::Vector3f right = new_t[0].x() > new_t[1].x() ? new_t[0] : new_t[1];
    float ratio_left = (new_t[2].x() - left.x()) / (new_t[2].y() - left.y());
    float ratio_right = (new_t[2].x() - right.x()) / (new_t[2].y() - right.y());
    float l_step = 1.f * ratio_left;
    float r_step = 1.f * ratio_right;
    float start = left.x();
    float end = right.x();
    for (int y = new_t[0].y(); y <= new_t[2].y(); ++y)
    {
        if (start > end)
            break;
        for (int x = start; x <= end; ++x)
        {
            auto v = t.toVector4();
            auto p = computeBarycentric2D(x, y, t.v);
            // float alpha = p.x();
            // float beta = p.y();
            // float gamma = p.z();
            float alpha,beta,gamma;
            std::tie(alpha, beta, gamma) = p;

            float w_reciprocal = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
            float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
            z_interpolated *= w_reciprocal;
            if (z_interpolated < depth_buf[get_index(x, y)])
            {
                depth_buf[get_index(x, y)] = z_interpolated;
                //Vector3f color = alpha * t.color[0]/ v[0].w() + beta * t.color[1] / v[1].w() + gamma * t.color[2] / v[2].w();
                //color *= w_reciprocal;
                Vector3f color = alpha * t.color[0] + beta * t.color[1] + gamma * t.color[2];
                frame_buf[get_index(x, y)] = color * 255;
            }
        }
        start += l_step;
        end += r_step;
    }
}

void rst::rasterizer::update_bottom(const std::vector<Eigen::Vector3f> new_t, const Triangle& t)
{
    Eigen::Vector3f left = new_t[0].x() < new_t[1].x() ? new_t[0] : new_t[1];
    Eigen::Vector3f right = new_t[0].x() > new_t[1].x() ? new_t[0] : new_t[1];
    float ratio_left = (new_t[2].x() - left.x()) / (new_t[2].y() - left.y());
    float ratio_right = (new_t[2].x() - right.x()) / (new_t[2].y() - right.y());
    float l_step = 1.f * ratio_left;
    float r_step = 1.f * ratio_right;
    float start = left.x();
    float end = right.x();
    for (int y = new_t[0].y(); y >= new_t[2].y(); --y)
    {
        if (start > end)
            break;
        for (int x = start; x <= end; ++x)
        {
            auto v = t.toVector4();
            auto p = computeBarycentric2D(x, y, t.v);
            // float alpha = p.x();
            // float beta = p.y();
            // float gamma = p.z();
            float alpha,beta,gamma;
            std::tie(alpha, beta, gamma) = p;
            
            float w_reciprocal = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
            float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
            z_interpolated *= w_reciprocal;
            if (z_interpolated < depth_buf[get_index(x, y)])
            {
                depth_buf[get_index(x, y)] = z_interpolated;
                //Vector3f color = alpha * t.color[0]/ v[0].w() + beta * t.color[1] / v[1].w() + gamma * t.color[2] / v[2].w();
                //color *= w_reciprocal;
                Vector3f color = alpha * t.color[0] + beta * t.color[1] + gamma * t.color[2];
                frame_buf[get_index(x, y)] = color * 255;
            }
        }
        start -= l_step;
        end -= r_step;
    }
}

void rst::rasterizer::MSAA(float x, float y,float dx, float dy, const Triangle& t, int& count)
{
    auto v = t.toVector4();
    auto p = computeBarycentric2D(x+dx, y+dy, t.v);
    // float alpha = p.x();
    // float beta = p.y();
    // float gamma = p.z();
    float alpha,beta,gamma;
    std::tie(alpha, beta, gamma) = p;
            
    float w_reciprocal = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
    float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
    z_interpolated *= w_reciprocal;

    if(z_interpolated < depth_buf[get_index(x, y)])
    {
        depth_buf[get_index(x, y)] = z_interpolated;
        count++;
    }
}