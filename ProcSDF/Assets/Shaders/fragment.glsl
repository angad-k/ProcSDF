#version 330 core

uniform vec3 cameraOrigin;
uniform vec2 viewportSize;
uniform float focalLength;

out vec4 FragColor;

const int SPHERE_0 = 0;
const int SPHERE_1 = 1;

int current_hit = -1;

// ------------------------- RANDOMIZATION CODE (Picked up from stackoverflow : https://stackoverflow.com/a/17479300) 
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


// ------------------- END OF RANDOMIZATION CODE

vec2 random2(in vec2 uv)
{
    float x = random(uv);
    float y = random(vec2(uv.x + 1, uv.y));
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

vec3 random_unit_vector(in vec2 uv)
{
    return normalize(random_in_unit_sphere(uv));
}

float distance_from_sphere(in vec3 p, in vec3 c, float r)
{
    return length(p - c) - r;
}

vec3 diffuse_scatter(in vec3 intersection, in vec3 normal)
{
    return intersection + normal + random_unit_vector(intersection.xy);
}

float map_the_world(in vec3 p)
{
    float sphere_0 = distance_from_sphere(p, vec3(0.0, 0.0, -1.0), 0.5);
    float sphere_1 = distance_from_sphere(p, vec3(0,-100.5,-1), 100);
    float min_dist = min(sphere_0, sphere_1);

    if(min_dist == sphere_0)
    {
        current_hit = SPHERE_0;
    }
    else if(min_dist == sphere_1)
    {
        current_hit = SPHERE_1;
    }
    
    return min_dist;
}

vec3 calculate_normal(in vec3 p)
{
    const vec3 small_step = vec3(0.001, 0.0, 0.0);
    float gradient_x = map_the_world(p + small_step.xyy) - map_the_world(p - small_step.xyy);
    float gradient_y = map_the_world(p + small_step.yxy) - map_the_world(p - small_step.yxy);
    float gradient_z = map_the_world(p + small_step.yyx) - map_the_world(p - small_step.yyx);
    vec3 normal = vec3(gradient_x, gradient_y, gradient_z);
    return normalize(normal);
}

vec3 ray_march(in vec3 ro, in vec3 rd, in int depth)
{
    float total_distance_traveled = 0.0;
    const int NUMBER_OF_STEPS = 16;
    const float MINIMUM_HIT_DISTANCE = 0.001;
    const float MAXIMUM_TRACE_DISTANCE = 1000.0;
    vec3 color = vec3(1.0);
    
    while(depth >= 0)
    {
        bool breakLoop = false;

        if(depth <= 0)
        {
            color *= 0.0;
            break;
        }

        for (int i = 0; i < NUMBER_OF_STEPS; ++i)
        {
            vec3 current_position = ro + total_distance_traveled * rd;

            float distance_to_closest = map_the_world(current_position);

            if (distance_to_closest < MINIMUM_HIT_DISTANCE)
            {
                vec3 normal = calculate_normal(current_position);

                vec3 target = vec3(0.0);
                
                switch(current_hit)
                {
                    case SPHERE_0 :
                        target = diffuse_scatter(current_position, normal);
                        break;
                    case SPHERE_1 :
                        target = diffuse_scatter(current_position, normal);
                        break;
                }

                color *= 0.5;
                ro = current_position;
                rd = target;
                break;
            }

            if (total_distance_traveled > MAXIMUM_TRACE_DISTANCE)
            {
                vec3 unitDirection = normalize(rd);
                float t = 0.5*(unitDirection.y + 1.0);
                color *= (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
                breakLoop = true;
                break;
            }
            total_distance_traveled += distance_to_closest;
        }
        
        if(breakLoop)
        {
            break;
        }
        depth--;
    }

    return color;
}

void main()
{
    int MAX_DEPTH = 50;
    int SAMPLES = 32;

    vec2 vSize = vec2(2.0*viewportSize.x/viewportSize.y, 2.0);

    vec3 horizontal = vec3(vSize.x, 0.0, 0.0);
    vec3 vertical = vec3(0.0, vSize.y, 0.0);
    vec3 lowerLeftCorner = cameraOrigin - horizontal/2 - vertical/2 - vec3(0, 0, focalLength);

    
    vec3 shaded_color = vec3(0.0);

    vec2 randomVal = random2(gl_FragCoord.xy);

    for(int i = 0; i < SAMPLES; i++)
    {
        vec2 uv = (gl_FragCoord.xy + randomVal)/viewportSize;
        randomVal = random2(randomVal);
        vec3 ro = cameraOrigin;
        vec3 rd = lowerLeftCorner + uv.x*horizontal + uv.y*vertical - cameraOrigin; 
        shaded_color += (ray_march(ro, rd, MAX_DEPTH)/SAMPLES);
    }
    FragColor = (vec4(shaded_color, 1.0));
}