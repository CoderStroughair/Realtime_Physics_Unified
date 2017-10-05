#ifndef DEFINES_H
#define DEFINES_H

#define GOLD	glm::vec3(247.0f / 255.0f, 194.0f / 255.0f, 87.0f / 255.0f)
#define BROWN	glm::vec3(51.0f / 255.0f, 25.0f / 255.0f, 0.0f / 255.0f)
#define BLACK	glm::vec3(0.0f, 0.0f, 0.0f)
#define WHITE	glm::vec3(1.0f, 1.0f, 1.0f)
#define CYAN	glm::vec3(0.0f, 1.0f, 1.0f)
#define MAGENTA	glm::vec3(1.0f, 0.0f, 1.0f)
#define YELLOW	glm::vec3(1.0f, 1.0f, 0.0f)
#define BLUE	glm::vec3(0.0f, 0.0f, 1.0f)
#define RED		glm::vec3(1.0f, 0.0f, 0.0f)
#define GREY	glm::vec3(0.7f, 0.7f, 0.7f)
#define GREEN	glm::vec3(0.0f, 1.0f, 0.0f)
#define PURPLE	glm::vec3(102.0f/255.0f, 0.0, 204.0f/255.0f)

/*----------------------------------------------------------------------------
							SHADER VARIABLES
----------------------------------------------------------------------------*/
#define NOTEXTURE_VERT			"../Shaders/noTextureShader.vert"
#define NOTEXTURE_FRAG			"../Shaders/noTextureShader.frag"
#define BEAR_FRAG				"../Shaders/bearShader.frag"
#define BLACKBEAR_FRAG			"../Shaders/blackBearShader.frag"
#define VARIABLE_FRAG			"../Shaders/variableShader.frag"
#define CONE_FRAG				"../Shaders/coneLightShader.frag"

#define TEXTURE_VERT			"../Shaders/textureVertexShader.txt"
#define TEXTURE_FRAG			"../Shaders/textureFragmentShader.txt"

#define NORMAL_VERT				"../Shaders/normalisedVertexShader.txt"
#define NORMAL_FRAG				"../Shaders/normalisedFragmentShader.txt"

#define MIRROR_VERT				"../Shaders/mirrorVertexShader.txt"
#define MIRROR_FRAG				"../Shaders/mirrorFragmentShader.txt"

#define TEST_VERT				"../Shaders/testVertex.txt"
#define TEST_FRAG				"../Shaders/testFragment.txt"

#define SKY_VERT				"../Shaders/skyboxVertexShader.txt"
#define SKY_FRAG				"../Shaders/skyboxFragmentShader.txt"

#define REFLECTIVE_FRAG			"../Shaders/reflectiveFragmentShader.txt"
#define FRESNEL_FRAG			"../Shaders/fresnelFragmentShader.txt"
#define REFLECTIVE_GLOBAL_FRAG	"../Shaders/reflectiveGlobalShader.frag"

#define SIMPLE_VERT				"../Shaders/simple.vert"
#define SIMPLE_FRAG				"../Shaders/simple.frag"
/*----------------------------------------------------------------------------
						MESH AND TEXTURE VARIABLES
----------------------------------------------------------------------------*/
#define MONKEYHEAD_MESH			"../Meshes/suzanne.obj"
#define SPHERE_MESH				"../Meshes/polysphere.obj"
#define GEM_MESH				"../Meshes/gem.obj"
#define WALL_MESH				"../Meshes/wall.obj"
#define BEAR_MESH				"../Meshes/bear.obj"
#define LAMP_MESH				"../Meshes/lamp.obj"
#define TORCH_MESH				"../Meshes/torch.obj"
#define SIGN_MESH				"../Meshes/sign.obj"
#define BUDDHA_MESH				"../Meshes/buddha.dae"
#define PARTICLE_MESH			"../Meshes/particle.dae"
#define CUBE_MESH				"../Meshes/cube.obj"

#define BEAR_TEXTURE			"../Textures/bear.tga"
#define REFLECT_TEXTURE			"../Textures/reflection.png"
#define REFRACT_TEXTURE			"../Textures/refraction.png"
#define NORMAL_TEXTURE			"../Textures/normalmapped.png"
#define BRICK_TEXTURE			"../Textures/brickwall.jpg"

#define BRICK_NORMAL			"../Textures/brickwall_normal.jpg"
#define WOOD_NORMAL				"../Textures/woodnormal.jpg"
#define STEEL_NORMAL			"../Textures/steelFloor_normal.jpg"
/*--------------------------------------------------------------------------*/

#endif