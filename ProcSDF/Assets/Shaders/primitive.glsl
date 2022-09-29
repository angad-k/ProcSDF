// PRIMITIVES

float Sphere(in vec3 position, in vec3 center, float radius)
{
    return length(position - center) - radius;
}

float Box(in vec3 position, in vec3 dimension)
{
    vec3 half_dimension = dimension/2;
    vec3 dist = abs(position) - half_dimension;
    return length(max(dist,0.0)) + min(max(dist.x, max(dist.y, dist.z)), 0.0);
}

// torus with parameter representing the radius of the path of rotating circle and the radius of the rotating circle
float Torus( vec3 position, vec2 parameter )
{
  vec2 dist = vec2(length(position.xz) - parameter.x, position.y);
  return length(dist) - parameter.y;
}

// END OF PRIMITIVES