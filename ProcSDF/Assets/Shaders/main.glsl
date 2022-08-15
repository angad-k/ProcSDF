void main()
{
    int MAX_DEPTH = 50;
    int SAMPLES = 10;

    vec2 v_size = vec2(2.0*u_viewport_size.x/u_viewport_size.y, 2.0);

    vec3 horizontal = vec3(v_size.x, 0.0, 0.0);
    vec3 vertical = vec3(0.0, v_size.y, 0.0);
    vec3 lower_left_corner = u_camera_origin - horizontal/2 - vertical/2 - vec3(0, 0, u_focal_length);

    vec3 output_color = vec3(0.0);

    vec2 random_val = random2(gl_FragCoord.xy);

    for(int i = 0; i < SAMPLES; i++)
    {
        vec2 uv = (gl_FragCoord.xy + random_val)/u_viewport_size;
        random_val = random2(random_val);
        vec3 ray_origin = u_camera_origin;
        vec3 ray_direction = lower_left_corner + uv.x*horizontal + uv.y*vertical - u_camera_origin; 
        output_color += (ray_march(ray_origin, ray_direction, MAX_DEPTH)/SAMPLES);
    }
    FragColor = (vec4(output_color, 1.0));
}