#version 410

in vec3 fragPos;
in vec3 normalPos;
in vec3 light_position_eye; 

//************//
//OBJECT COLOR//
//************//
uniform vec3 Ks;
uniform vec3 Kd;
uniform vec3 Ka;
uniform float specular_exponent;
uniform vec3 light;

//***********//
//LIGHT COLOR//
//***********//
uniform vec3 Ls; //white specular color
uniform vec3 Ld; // dull white diffuse light 
uniform vec3 La;  // grey ambient color


uniform vec3 camPos;

out vec4 fragment_color;

void main()
{    

    //ambient
    vec3 Ia = La * Ka;
    
	//diffuse
	vec3 norm = normalize(normalPos);
	vec3 lightDir = normalize(light - fragPos);
	float diff = max(dot(lightDir, norm), 0.0);
    vec3 Id = Ld * diff * Kd; 
    
    //Specular Intensity

	vec3 viewDir = normalize(camPos - fragPos);
	vec3 halfwayDir = normalize(light + viewDir);
	
	float spec = pow(max(dot(normalPos, halfwayDir), 0.0), 32);
	vec3 Is = specular_exponent * spec * Ks * Ls; 
    
    fragment_color = vec4(Id, 1.0);    
}