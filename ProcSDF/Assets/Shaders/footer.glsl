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

void main()
{
    int MAX_DEPTH = 50;
    int SAMPLES = 10;

    vec2 v_size = vec2(2.0*u_viewport_size.x/u_viewport_size.y, 2.0);

    vec3 horizontal = vec3(v_size.x, 0.0, 0.0);
    vec3 vertical = vec3(0.0, v_size.y, 0.0);
    vec3 lower_left_corner = u_camera_origin - horizontal/2 - vertical/2 - vec3(0, 0, u_focal_length);

    vec3 output_color = vec3(0.0);

    vec2 random_val = random2(gl_FragCoord.xy);

    for(int i = 0; i < SAMPLES; i++)
    {
        vec2 uv = (gl_FragCoord.xy + random_val)/u_viewport_size;
        random_val = random2(random_val);
        vec3 ray_origin = u_camera_origin;
        vec3 ray_direction = lower_left_corner + uv.x*horizontal + uv.y*vertical - u_camera_origin; 
        output_color += (ray_march(ray_origin, ray_direction, MAX_DEPTH)/SAMPLES);
    }
    FragColor = (vec4(output_color, 1.0));
}