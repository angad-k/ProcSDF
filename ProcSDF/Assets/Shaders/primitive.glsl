// PRIMITIVES

float sphere(in vec3 p, in vec3 c, float r)
{
    return length(p - c) - r;
}

// END OF PRIMITIVES