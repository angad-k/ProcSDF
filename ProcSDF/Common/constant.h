#pragma once
#include <string>
#include<vector>

#include "ImGui/imgui.h"

namespace sdf {
	const std::string SHADER_FOLDER_PATH = "Assets/Shaders/";
	const std::string SHADER_FILE_EXTENSION = ".glsl";
	const std::string VERTEX_SHADER_PATH = "Assets/Shaders/vertex.glsl";
	const std::string TRANSLATION_NODE = "Translation";
	const std::string ROTATION_X_NODE = "Rotation_X";
	const std::string ROTATION_Y_NODE = "Rotation_Y";
	const std::string ROTATION_Z_NODE = "Rotation_Z";
}

namespace shader_generation {
	// change the order of files according to the order of insertion in the generated fragment shader file.
	const std::vector <std::string> shader_files{ "header", "primitive", "footer" };
	const std::string POSITION = "position";
	const std::string POSITION_OPERATION = "position_operation";
	const std::string RETURN = "return $;\n";
	const std::string SWITCH_STATEMENT = "\nswitch(object_index)\n{\n$\n}\n";
	const std::string FLOAT = "float ";
	const std::string VEC3 = "vec3($, $, $)";
	namespace object_function {
		const std::string FUNCTION_TEMPLATE = "\nfloat $(vec3 position)\n{\n#\n}\n";
		const std::string INITIALIZATION = "\nmat3 rotation_transform_x = mat3(1.0);\nmat3 rotation_transform_y = mat3(1.0);\nmat3 rotation_transform_z = mat3(1.0);\nvec3 position_dup = position;\n";
		const std::string TRANSLATION_TRANSFORM_APPLICATION = "\position = position - $;\n";
		const std::string ROTATION_TRANSFORM_INIT_X = "\nrotation_transform_x = mat3(1.0);\nrotation_transform_x[1][1] = rotation_transform_x[2][2] = cos(radians(float($)));\nrotation_transform_x[1][2] = -sin(radians(float($)));\nrotation_transform_x[2][1] = -rotation_transform_x[1][2];\n";
		const std::string ROTATION_TRANSFORM_INIT_Y = "\nrotation_transform_y = mat3(1.0);\nrotation_transform_y[0][0] = rotation_transform_y[2][2] = cos(radians(float($)));\nrotation_transform_y[2][0] = -sin(radians(float($)));\nrotation_transform_y[0][2] = -rotation_transform_y[2][0];\n";
		const std::string ROTATION_TRANSFORM_INIT_Z = "\nrotation_transform_z = mat3(1.0);\nrotation_transform_z[0][0] = rotation_transform_z[1][1] = cos(radians(float($)));\nrotation_transform_z[0][1] = -sin(radians(float($)));\nrotation_transform_z[1][0] = -rotation_transform_z[0][1];\n";
		const std::string ROTATION_TRANSFORM_APPLICATION = "\nposition = rotation_transform_$ * position;\n";
		const std::string POSITION_RESTORATION = "\nposition = position_dup;\n";
		const std::string DISTANCE_STORAGE = "\nfloat $ = $(position);\n";
		const std::string ROTATION = "rotation_transform";
	}
	namespace closest_object_info {
		const std::string FUNCTION_TEMPLATE = "\nclosest_object_info get_closest_object_info(vec3 position)\n{\n$\nreturn closest_object_info(min_dist, object_index);\n}\n";
		const std::string DISTANCE_COMPUTATION = "\nfloat dist_$ = object_$(position);\n";
		const std::string VARIABLE_INITIALIZATION = "\nfloat min_dist = 3e+38;\nint object_index = 1;\n";
		const std::string MIN_DIST_COMPUTATION = "\nmin_dist = min(min_dist, dist_$);\n";
		const std::string CONDITIONAL_OBJECT_INDEX_COMPUTATION = "\nif(dist_$ == min_dist)\n{\nobject_index = $;\n}\n";
	}

	namespace uniform {
		const std::string UNIFORM_FLOAT = "\nuniform float $;\n";
		const std::string UNIFORM_VEC3 = "\nuniform vec3 $;\n";
		const std::string UNIFORM_VEC2 = "\nuniform vec2 $;\n";
	}

	namespace calculate_normal {
		const std::string FUNCTION_TEMPLATE = "\nvec3 calculate_normal(vec3 position, int object_index)\n{\nconst vec3 small_step = vec3(0.001, 0.0, 0.0);\nvec3 normal = vec3(1.0, 1.0, 1.0);\nfloat g_x, g_y, g_z;\n$\nreturn normal;\n}\n";
		const std::string CASE_STATEMENT = "case $:\ng_x = object_$(position + small_step.xyy) - object_$(position - small_step.xyy);\ng_y = object_$(position + small_step.yxy) - object_$(position - small_step.yxy);\ng_z = object_$(position + small_step.yyx) - object_$(position - small_step.yyx);\nnormal = normalize(vec3(g_x, g_y, g_z));\nbreak;\n";
		const int FREQUENCY = 7;
	}

	namespace get_distance_from {
		const std::string FUNCTION_TEMPLATE = "\nfloat get_distance_from(vec3 position, int object_index)\n{\nfloat d = 0.0;\n$\nreturn d;\n}\n";
		const std::string CASE_STATEMENT = "case $:\nd = object_$(position);\nbreak;\n";
		const int FREQUENCY = 2;
	}

	namespace is_light {
		const std::string FUNCTION_TEMPLATE = "\nbool is_light(int object_index)\n{\nbool res = false;\n$\nreturn res;\n}\n";
		const std::string CASE_STATEMENT = "case $:\nres = $;\nbreak;\n";
	}
	
	namespace target_ray {
		const std::string FUNCTION_TEMPLATE = "\scatter_info get_target_ray(vec3 position, int object_index, vec3 normal, vec3 r_in, bool is_front_face)\n{\nscatter_info target = scatter_info(vec3(0.0,0.0,0.0), true, vec3(1.0, 1.0, 1.0));\n$\nreturn target;\n}\n";
		const std::string CASE_STATEMENT = "\ncase $:\ntarget = $;\nbreak;\n";
	}

	namespace scatter_calls {
		const std::string DIFFUSE = "diffuse_scatter(position, normal, object_index)";
		const std::string METTALIC = "metallic_scatter(position, normal, object_index, r_in, $)";
		const std::string DIELECTRIC = "dielectric_scatter(position, normal, object_index, r_in, is_front_face, $)";
		const std::string CUSTOM = "$_scatter(position, normal, r_in, is_front_face, $)";
		const std::string LIGHT = "scatter_info(vec3(0.0, 0.0, 0.0), true, vec3(0.0, 0.0, 0.0))"; //light's scatter will never be used.
	}

	namespace get_color {
		const std::string FUNCTION_TEMPLATE = "\nvec3 get_color(int object_index)\n{\n$\nreturn vec3(1.0, 0, 0.0);\n}\n";
		const std::string CASE_STATEMENT = "\ncase $:\nreturn $;\nbreak;\n";
	}
}

namespace save_project {
	const std::string NODE_LINK = "nodeLink";
	const std::string NODE_NAME = "nodeName";
	const std::string INPUT_IDS = "inputIDs";
	const std::string OUTPUT_IDS = "outputIDS";
	const std::string INPUT_FLOAT3 = "inputFloat3";
	const std::string INPUT_FLOAT = "inputFloat";
	const std::string PROCSDF_EXTENSION = ".procsdf";
	const std::string CAMERA_SETTING = "cameraSetting";
	const std::string CAMERA_ORIGIN = "cameraOrigin";
	const std::string CAMERA_FOCAL_LENGTH = "focalLength";
	const std::string NODE_ID = "nodeID";
	const std::string SDF_FILE_PATH = (std::string)ROOT_DIR"Assets\\CustomSDFs";
}

namespace node_name {
	const std::string FINAL_NODE = "Final node";
	const std::string OBJECT_NODE = "Object node";
	const std::string INTERSECTION_NODE = "Intersection";
	const std::string UNION_NODE = "Union";
	const std::string SPHERE_NODE = "Sphere";
	const std::string BOX_NODE = "Box";
	const std::string TORUS_NODE = "Torus";
	const std::string BOX_FRAME_NODE = "BoxFrame";
	const std::string TRANSLATION_NODE = "Translation";
	const std::string ROTATIONX_NODE = "Rotation_X";
	const std::string ROTATIONY_NODE = "Rotation_Y";
	const std::string ROTATIONZ_NODE = "Rotation_Z";
}

namespace material_type {
	const std::string DIFFUSE = "Diffuse";
	const std::string METAL = "Metal";
	const std::string DIELECTRIC = "Dielectric";
	const std::string CUSTOM = "Custom";
	const std::string LIGHT = "Light";
}

namespace imgui_colors {
	const int BLACK = IM_COL32(0, 0, 0, 255);
	const int RED = IM_COL32(255, 50, 50, 255);
	const int GREEN = IM_COL32(80, 190, 50, 255);
	const int BLUE = IM_COL32(50, 50, 255, 255);
	const int PURPLE = IM_COL32(160, 32, 240, 255);
	const int ORANGE = IM_COL32(255, 115, 29, 255);
	const int CYAN = IM_COL32(55, 170, 156, 255);
	const int GREY = IM_COL32(57, 62, 70, 255);
	const int TRANFSFORM = ORANGE;
	const int PRIMITIVE = RED;
	const int FINAL = GREEN;
	const int OPERATION = BLUE;
	const int OBJECT = PURPLE;
	const int CUSTOM_NODE = CYAN;
}

namespace filter {
	const std::string PNG = "PNG images (*.png)\0 * .png\0";
	const std::string PROCSDF = "ProcSDF Node Space (*.procsdf)\0*.procsdf\0";
}