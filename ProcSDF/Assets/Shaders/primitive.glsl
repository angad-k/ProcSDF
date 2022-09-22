// PRIMITIVES

float sphere(in vec3 position, in vec3 center, float radius){
    return length(position - center) - radius;
}

float box(in vec3 position, in vec3 dimension) {
    vec3 half_dims = dimension / 2;
    vec3 dist = abs(position) - half_dims;
    return length(max(dist, 0.0)) + min(max(dist.x, max(dist.y, dist.z)), 0.0);
}

// END OF PRIMITIVES