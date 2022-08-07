// PREAMBLE

#version 330 core

uniform vec3 u_camera_origin;
uniform vec2 u_viewport_size;
uniform float u_focal_length;

out vec4 FragColor;

const bool DEBUG = true;

// END OF PREAMBLE

// RANDOMIZATION CODE (Picked up from stackoverflow : https://stackoverflow.com/a/17479300) 
// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint hash( uint x ) {
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}

// Compound versions of the hashing algorithm I whipped together.
uint hash( uvec2 v ) { return hash( v.x ^ hash(v.y)                         ); }
uint hash( uvec3 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z)             ); }
uint hash( uvec4 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w) ); }

// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float floatConstruct( uint m ) {
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0

    float  f = uintBitsToFloat( m );       // Range [1:2]
    return f - 1.0;                        // Range [0:1]
}

// Pseudo-random value in half-open range [0:1].
float random( float x ) { return floatConstruct(hash(floatBitsToUint(x))); }
float random( vec2  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
float random( vec3  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
float random( vec4  v ) { return floatConstruct(hash(floatBitsToUint(v))); }

// END OF RANDOMIZATION CODE

// UTILS

vec2 random2(in vec2 uv)
{
    float x = random(uv);
    float y = random(-normalize(uv)*x);
    //float y = random(vec2(uv.x + 1, uv.y));
    return vec2(x, y);
}

vec3 random3(in vec2 uv)
{
    float x = random(uv);
    float y = random(vec2(uv.x + 1, uv.y));
    float z = random(vec2(uv.x - 1, uv.y));
    return vec3(x, y, z);
}

vec3 random_in_unit_sphere(in vec2 uv)
{
    vec3 randomVec = random3(uv);
    randomVec = randomVec * 2.0 - 1.0;
    return randomVec;
}

vec3 random_in_hemisphere(in vec3 normal, in vec3 intersection) {
    vec3 in_unit_sphere = random_in_unit_sphere(intersection.xy);
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

// END OF UTILS

// PRIMITIVES

float sphere(in vec3 p, in vec3 c, float r)
{
    return length(p - c) - r;
}

// END OF PRIMITIVES

// STRUCTS

struct closest_object_info
{
    float closest_distance;
    int object_index;
};

// END OF STRUCTS

// SCATTER FNS

vec3 diffuse_scatter(in vec3 intersection, in vec3 normal)
{
    return intersection + random_in_hemisphere(normal, intersection);
}

// END OF SCATTER FNS

// OBJECT FNS

float object_1(vec3 position)
{
    return sphere(position, vec3(0.0, 0.0, -1.0), 0.5);
}

float object_2(vec3 position)
{
    return 100000.0;
}

// END OF OBJECT FNS


closest_object_info get_closest_object_info(vec3 position)
{
    float dist_1 = object_1(position);
    float dist_2 = object_2(position);
    
    float min_dist = dist_1;

    int object_index = 1;

    /*if(dist_1 == min_dist)
    {
        object_index = 1;
    }

    if(dist_2 == min_dist)
    {
        object_index = 2;
    }*/

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

vec3 get_color(vec3 position, int object_index)
{
    return vec3(0, 1.0, 0.0);
}

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