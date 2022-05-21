#version 330 core

uniform vec3 lightPosition;

float random(in vec2 uv)
{
    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 random2(in vec2 uv)
{
    float x = random(uv);
    float y = random(vec2(uv.x*x, uv.y*x));
    return vec2(x, y);
}

float distance_from_sphere(in vec3 p, in vec3 c, float r)
{
    return length(p - c) - r;
}

float map_the_world(in vec3 p)
{
    //float displacement = sin(5.0 * p.x) * sin(5.0 * p.y) * sin(5.0 * p.z) * 0.25;
    float sphere_0 = distance_from_sphere(p, vec3(0.0, 0.0, -1.0), 0.5);
    float sphere_1 = distance_from_sphere(p, vec3(0,-100.5,-1), 100);
    return min(sphere_0, sphere_1);//+ displacement;
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
    const int NUMBER_OF_STEPS = 32;
    const float MINIMUM_HIT_DISTANCE = 0.0001;
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

                vec3 target = current_position + normal + random(current_position.xy);
                
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

out vec4 FragColor;
uniform vec3 cameraOrigin;
uniform vec2 viewportSize;
uniform float focalLength;

void main()
{
    int MAX_DEPTH = 50;
    int SAMPLES = 50;

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
    FragColor = vec4(shaded_color, 1.0);
}