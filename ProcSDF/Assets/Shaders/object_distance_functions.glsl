// OBJECT FNS

float object_1(vec3 position)
{
    return Torus(position, vec2(3.0,1.0));
}

float object_2(vec3 position)
{
    return Sphere(position, vec3(0,-100.5, -1.0), 100);
}

// END OF OBJECT FNS