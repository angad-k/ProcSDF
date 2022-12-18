// PREAMBLE

#version 330 core

uniform vec3 u_camera_origin;
uniform vec2 u_viewport_size;
uniform float u_focal_length;

out vec4 FragColor;

const bool DEBUG = false;

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

//vec3 get_color(vec3 position, int object_index)
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

// END OF STRUCTS

// SCATTER FNS

vec3 diffuse_scatter(in vec3 intersection, in vec3 normal)
{
    return intersection + random_in_hemisphere(normal, intersection);
}

// END OF SCATTER FNS
