// PRIMITIVES

float Sphere(in vec3 position, in float radius)
{
    return length(position) - radius;
}

float Box(in vec3 position, in float dimension_x, in float dimension_y, in float dimension_z)
{
    vec3 dimension = vec3(dimension_x, dimension_y, dimension_z);
    vec3 half_dimension = dimension/2;
    vec3 dist = abs(position) - half_dimension;
    return length(max(dist,0.0)) + min(max(dist.x, max(dist.y, dist.z)), 0.0);
}

// torus with parameter representing the radius of the path of rotating circle and the radius of the rotating circle
float Torus(in vec3 position,in float parameter_x, in float parameter_y)
{
  vec2 parameter = vec2(parameter_x, parameter_y);
  vec2 dist = vec2(length(position.xz) - parameter.x, position.y);
  return length(dist) - parameter.y;
}

float BoxFrame(in vec3 position, in float dimension_x, in float dimension_y, in float dimension_z, in float frame_thickness)
{
   vec3 dimension = vec3(dimension_x, dimension_y, dimension_z);
   position = abs(position) - dimension;
   vec3 dist = abs(position + frame_thickness) - frame_thickness;
   return min(min(
      length(max(vec3(position.x, dist.y, dist.z), 0.0)) + min(max(position.x, max(dist.y, dist.z)), 0.0),
      length(max(vec3(dist.x, position.y, dist.z), 0.0)) + min(max(dist.x, max(position.y, dist.z)), 0.0)),
      length(max(vec3(dist.x , dist.y, position.z), 0.0)) + min(max(dist.x, max(dist.y, position.z)), 0.0));
}


// END OF PRIMITIVES