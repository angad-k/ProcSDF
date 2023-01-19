// type primitive
// name Octahedron
// float_params s
float Octahedron( in vec3 p, in float s)
{
  p = abs(p);
  return (p.x+p.y+p.z-s)*0.57735027;
}