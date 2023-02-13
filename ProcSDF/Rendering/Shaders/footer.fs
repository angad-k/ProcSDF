R"(
vec3 ray_march(in vec3 ray_origin, in vec3 ray_direction, in int depth)
{
    float total_distance_traveled = 0.0;
    int NUMBER_OF_STEPS = u_r_Number_of_steps;
    const float MINIMUM_HIT_DISTANCE = 0.001;
    float MAXIMUM_TRACE_DISTANCE = u_r_Maximum_Trace_Distance;
    vec3 output_color = vec3(1.0);
    
   while(depth >= 0)
   {
        bool break_loop = false;
        if(depth == 0)
        {
            //output_color *= 0.0;
            if(DEBUG && DEBUG_DEPTH)
            {
                return DEBUG_DEPTH_COL;
            }
            break;
        }
        int i;
        for (i = 0; i < NUMBER_OF_STEPS; ++i)
        {
            vec3 current_position = ray_origin + total_distance_traveled * ray_direction;

            closest_object_info closest = get_closest_object_info(current_position);

            if (closest.closest_distance < MINIMUM_HIT_DISTANCE)
            {
                vec3 normal = calculate_normal(current_position, closest.object_index);
                float prev_dist = get_distance_from(ray_origin, closest.object_index);
                bool is_mat_light = is_light(closest.object_index);
                
                if(is_mat_light)
                {
                    output_color *= get_color(closest.object_index);
                    break_loop = true;
                    break;
                }
                
                bool is_front_face = (prev_dist > 0.0);
                if(abs(prev_dist)<MINIMUM_HIT_DISTANCE)
                {
                    is_front_face = !is_front_face;
                }

                scatter_info info = get_target_ray(current_position, closest.object_index, normal, ray_direction, is_front_face);
                output_color *= info.attenuation;
                vec3 target = info.scattered_ray;
                
                if(!info.is_scattered)
                {
                    output_color = vec3(0.0, 0.0, 0.0);
                    break_loop = true;
                    break;
                }

                ray_origin = current_position;
                ray_direction = target;
                break;
            }

            if (total_distance_traveled > MAXIMUM_TRACE_DISTANCE)
            {
                vec3 unit_direction = normalize(ray_direction);
                float t = 0.5*(unit_direction.y + 1.0);
                output_color *= t*u_r_horizon_top_color + (1.0-t)*u_r_horizon_bottom_color;
                if(DEBUG && DEBUG_MAX_TRACE)
                {
                    return DEBUG_MAX_TRACE_COL;
                }
                break_loop = true;
                break;
            }
            total_distance_traveled += closest.closest_distance;
        }

        if(i == NUMBER_OF_STEPS && DEBUG && DEBUG_STEPS_END)
        {
            return DEBUG_STEPS_END_COL;
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
    int MAX_DEPTH = u_r_Max_Depth;
    int SAMPLES = u_r_Samples;

    vec2 v_size = vec2(2.0*u_viewport_size.x/u_viewport_size.y, 2.0);

    vec3 horizontal = vec3(v_size.x, 0.0, 0.0);
    vec3 vertical = vec3(0.0, v_size.y, 0.0);
    vec3 lower_left_corner = u_camera_origin - horizontal/2 - vertical/2 - vec3(0, 0, u_focal_length);

    vec3 output_color = vec3(0.0);

    random_val = random2(gl_FragCoord.xy);

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
)"