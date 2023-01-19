// name Stripes
// float_params multiply_by roughness
scatter_info Stripes_scatter(in vec3 position, in vec3 normal, in vec3 r_in, in bool is_front_face, in vec3 color, in float multiply_by, in float roughness)
{
    vec3 atten = color;
    vec3 scattered = position + random_in_hemisphere(normal, position);
    vec3 reflected = reflect(normalize(r_in), normal) + roughness*random_in_unit_sphere(random_val);
    reflected = normalize(reflected);
    float ref_length = length(reflected);
    float q = reflected.z/ref_length;
    int p = int(q*multiply_by);
    if(p%2==0)
    {
        atten = vec3(0.0, 0.0, 0.0);
    }
    return scatter_info(scattered, true, atten);

}