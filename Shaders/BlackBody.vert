#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormal;


out vec2 TexCoords;
out vec2 TexCoordsCrack;
out vec2 TexCoordsCrack_for_Parallel;
out vec3 vs_position;

uniform vec3 LightPos0;
out VS_OUT {
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
    //==============================Tangent matrix===================================//
	vec3 aNormal = mat3(transpose(inverse(ViewMatrix*ModelMatrix))) *aNormal;
	vec3 y = vec3 (0.,1.,0.);
	float d = dot( y, aNormal );
	vec3 aBitangent = normalize( y - d*aNormal );
	vec3 aTangent = normalize( cross(y,aNormal) );
	
	vec3 T   = normalize(aTangent);
    vec3 B   = normalize(aBitangent);
    vec3 N   = normalize(aNormal);
    mat3 TBN = transpose(mat3(T, B, N));

	vs_out.TangentLightPos = TBN * LightPos0;
    vs_out.TangentViewPos  = TBN * vec3(0., 0., 3);
    vs_out.TangentFragPos  = TBN * aPos;
//===============================================================================//

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix *vec4(aPos.x, aPos.y, aPos.z, 1.0); 
    TexCoords = vec2(aTexCoords.x, aTexCoords.y);
    TexCoordsCrack = vec2(aTexCoords.x, 1-aTexCoords.y);
    TexCoordsCrack_for_Parallel = vec2(1-aTexCoords.x, 1-aTexCoords.y);
    vs_position = vec4(ModelMatrix*ViewMatrix * vec4(aPos, 1.f)).xyz;
}