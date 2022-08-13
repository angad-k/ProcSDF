vec3 ray_march(in vec3 ray_origin, in vec3 ray_direction, in int depth)
{
    float total_distance_traveled = 0.0;
    const int NUMBER_OF_STEPS = 16;
    const float MINIMUM_HIT_DISTANCE = 0.001;
    const float MAXIMUM_TRACE_DISTANCE = 1000.0;
    vec3 output_color = vec3(1.0);
    
   while(depth >= 0)
   {
        bool break_loop = false;
        if(depth == 0)
        {
            output_color *= 0.0;
            if(DEBUG)
            {
                output_color = vec3(1.0, 0.0, 0.0);
            }
            break;
        }

        for (int i = 0; i < NUMBER_OF_STEPS; ++i)
        {
            vec3 current_position = ray_origin + total_distance_traveled * ray_direction;

            closest_object_info closest = get_closest_object_info(current_position);

            if (closest.closest_distance < MINIMUM_HIT_DISTANCE)
            {
                vec3 normal = calculate_normal(current_position, closest.object_index);

                vec3 target = get_target_ray(current_position, closest.object_index, normal);

                output_color *= get_color(current_position, closest.object_index);
                ray_origin = current_position;
                ray_direction = target;
                break;
            }

            if (total_distance_traveled > MAXIMUM_TRACE_DISTANCE)
            {
                vec3 unit_direction = normalize(ray_direction);
                float t = 0.5*(unit_direction.y + 1.0);
                output_color *= (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
                if(DEBUG)
                {
                    return vec3(0.0, 1.0, 0.0);
                }
                break_loop = true;
                break;
            }
            total_distance_traveled += closest.closest_distance;
        }

        if(break_loop)
        {
            break;
        }
        depth--;
   }

    return output_color;
}