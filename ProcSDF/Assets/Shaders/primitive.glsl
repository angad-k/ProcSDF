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

float RoundBox(in vec3 position, in float dimension_x, in float dimension_y, in float dimension_z, float radius)
{
    vec3 dimension = vec3(dimension_x, dimension_y, dimension_z);
    vec3 q = abs(position) - dimension;
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0) - radius;
}

float Cone( in vec3 position, in float p_sin, in float p_cos, float height )
{
     vec2 q = height*vec2(p_sin/p_cos,-1.0);
    
    vec2 w = vec2( length(position.xz), position.y );
    vec2 a = w - q*clamp( dot(w,q)/dot(q,q), 0.0, 1.0 );
    vec2 b = w - q*vec2( clamp( w.x/q.x, 0.0, 1.0 ), 1.0 );
    float k = sign( q.y );
    float d = min(dot( a, a ),dot(b, b));
    float s = max( k*(w.x*q.y-w.y*q.x),k*(w.y-q.y)  );
    return sqrt(d)*sign(s);
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

float Capsule( vec3 position, float point1_x, float point1_y, float point1_z, float point2_x, float point2_y, float point2_z, float radius )
{
    vec3 a = vec3(point1_x, point1_y, point1_z); 
    vec3 b = vec3(point2_x, point2_y, point2_z);
    vec3 pa = position - a, ba = b - a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h ) - radius;
}

float Cylinder(vec3 position, float point1_x, float point1_y, float point1_z, float point2_x, float point2_y, float point2_z, float radius)
{
    vec3 a = vec3(point1_x, point1_y, point1_z); 
    vec3 b = vec3(point2_x, point2_y, point2_z);
    vec3  ba = b - a;
    vec3  pa = position - a;
    float baba = dot(ba,ba);
    float paba = dot(pa,ba);
    float x = length(pa*baba-ba*paba) - radius*baba;
    float y = abs(paba-baba*0.5)-baba*0.5;
    float x2 = x*x;
    float y2 = y*y*baba;
    float d = (max(x,y)<0.0)?-min(x2,y2):(((x>0.0)?x2:0.0)+((y>0.0)?y2:0.0));
    return sign(d)*sqrt(abs(d))/baba;
}

// END OF PRIMITIVES