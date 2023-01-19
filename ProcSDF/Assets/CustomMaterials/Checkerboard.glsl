// name Checkerboard
// float_params divide_by
scatter_info Checkerboard_scatter(in vec3 position, in vec3 normal, in vec3 r_in, in bool is_front_face, in vec3 color, in float divide_by)
{
  if(divide_by == 0.0)
  {
    divide_by = 0.01;
  }
  int x = int(r_in.x/divide_by);
  int y = int(r_in.y/divide_by);
  vec3 atten = vec3(1.0, 1.0, 1.0);
  if((x%2 == 0) && (y%2 == 0))
  {
    atten = vec3(0.0, 0.0, 0.0);
  }
  vec3 scattered = position + random_in_hemisphere(normal, position);
  //atten = color;
  return scatter_info(scattered, true, atten);
}