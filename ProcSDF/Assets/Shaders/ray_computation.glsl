closest_object_info get_closest_object_info(vec3 position)
{
    float dist_1 = object_1(position);
    float dist_2 = object_2(position);
    
    float min_dist = min(dist_1, dist_2);

    int object_index = 1;

    if(dist_1 == min_dist)
    {
        object_index = 1;
    }

    if(dist_2 == min_dist)
    {
        object_index = 2;
    }

    return closest_object_info(min_dist, object_index);
}

vec3 calculate_normal(vec3 position, int object_index)
{
    const vec3 small_step = vec3(0.001, 0.0, 0.0);
    vec3 normal = vec3(1.0, 1.0, 1.0);
    float g_x, g_y, g_z;
    switch(object_index)
    {
        case 1:
            g_x = object_1(position + small_step.xyy) - object_1(position - small_step.xyy);
            g_y = object_1(position + small_step.yxy) - object_1(position - small_step.yxy);
            g_z = object_1(position + small_step.yyx) - object_1(position - small_step.yyx);
            normal = normalize(vec3(g_x, g_y, g_z));
            break;
        case 2:
            g_x = object_2(position + small_step.xyy) - object_2(position - small_step.xyy);
            g_y = object_2(position + small_step.yxy) - object_2(position - small_step.yxy);
            g_z = object_2(position + small_step.yyx) - object_2(position - small_step.yyx);
            normal = normalize(vec3(g_x, g_y, g_z));
            break;
    }
    return normal;
}

vec3 get_target_ray(vec3 position, int object_index, vec3 normal)
{
    vec3 target = vec3(0.0, 0.0, 0.0);
    switch(object_index)
    {
        case 1:
            target = diffuse_scatter(position, normal);
            break;
        case 2:
            target = diffuse_scatter(position, normal);
            break;
    }
    return target;
}