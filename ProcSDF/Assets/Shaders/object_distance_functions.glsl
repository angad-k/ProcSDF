// OBJECT FNS

float object_1(vec3 position)
{
    return BoxFrame(position, vec3(2.0,2.0,2.0), 0.2);
}

float object_2(vec3 position)
{
    return Sphere(position, vec3(0,-100.5, -1.0), 100);
}

// END OF OBJECT FNS