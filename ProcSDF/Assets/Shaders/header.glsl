// PREAMBLE

#version 330 core

uniform vec3 u_camera_origin;
uniform vec2 u_viewport_size;
uniform float u_focal_length;

uniform vec3 u_r_horizon_bottom_color;
uniform vec3 u_r_horizon_top_color;

uniform int u_r_Max_Depth;
uniform int u_r_Samples;
uniform int u_r_Number_of_steps;
uniform int u_r_Maximum_Trace_Distance;

out vec4 FragColor;

uniform bool DEBUG;
uniform bool DEBUG_DEPTH;
uniform bool DEBUG_MAX_TRACE;
uniform bool DEBUG_STEPS_END;

vec2 random_val = vec2(0.0);
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

vec3 reflect(in vec3 v, in vec3 normal) {
    return v - 2*dot(v,normal)*normal;
}

vec3 refract(in vec3 v, in vec3 n, in float etai_over_etat) {
    float cos_theta = min(dot(-v, n), 1.0);
    vec3 r_out_perp =  etai_over_etat * (v + cos_theta*n);
    float r_out_perp_length = length(r_out_perp);
    vec3 r_out_parallel = -sqrt(abs(1.0 - r_out_perp_length*r_out_perp_length)) * n;
    return r_out_perp + r_out_parallel;
}

float reflectance(float cosine, float ref_idx) 
{
    // Use Schlick's approximation for reflectance.
    float r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

vec3 get_color(int object_index);
//{
//    return vec3(1.0, 0, 0.0);
//}

// END OF UTILS

// OPERATIONS

float Intersection(float dist1, float dist2) 
{
    return max(dist1, dist2);
}

float Union(float dist1, float dist2)
{
    return min(dist1, dist2);
}

// END OF OPERATIONS

// STRUCTS

struct closest_object_info
{
    float closest_distance;
    int object_index;
};

struct scatter_info
{
    vec3 scattered_ray;
    bool is_scattered;
    vec3 attenuation;
};

// END OF STRUCTS

// SCATTER FNS

scatter_info diffuse_scatter(in vec3 intersection, in vec3 normal, in int object_index)
{
    vec3 scattered = intersection + random_in_hemisphere(normal, intersection);
    return scatter_info(scattered, true, get_color(object_index));
}

scatter_info metallic_scatter(in vec3 intersection, in vec3 normal, in int object_index, in vec3 r_in, float roughness)
{
    if(roughness > 1.0)
    {
        roughness = 1.0;
    }
    vec3 reflected = reflect(normalize(r_in), normal) + roughness*random_in_unit_sphere(random_val);
    return scatter_info(reflected, (dot(reflected, normal) > 0), get_color(object_index));
}

scatter_info dielectric_scatter(in vec3 intersection, in vec3 normal, in int object_index, in vec3 r_in, bool is_front_face, float ior, float roughness)
{
     if(roughness > 1.0)
     {
        roughness = 1.0;
     }
     //is_front_face = !is_front_face;
     float refraction_ratio = is_front_face ? (1.0/ior) : ior;

     vec3 unit_direction = normalize(r_in);
     float cos_theta = min(dot(-unit_direction, normal), 1.0);
     float sin_theta = sqrt(1.0 - cos_theta*cos_theta);

     bool cannot_refract = refraction_ratio * sin_theta > 1.0;
     vec3 direction;

     if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random(random_val))
        direction = reflect(unit_direction, normal);
     else
        direction = refract(unit_direction, normal, refraction_ratio);

     return scatter_info(direction + roughness*random_in_unit_sphere(random_val), true, get_color(object_index));
}

// END OF SCATTER FNS
