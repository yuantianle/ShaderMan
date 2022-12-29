#version 330 core
in vec2 TexCoords;
in vec2 TexCoordsCrack;
in vec2 TexCoordsCrack_for_Parallel;
in vec3 vs_position;

out vec4 FragColor;

uniform sampler2D CrackInfoTexture;

uniform vec3 LightPos0;

uniform float uAmbient;    //0~1
uniform float uDiffusion;  //0~1
uniform float uSpecular;   //0~1
uniform float uShininess;  //1~100
uniform float uRoughness;  //0~1 
//uniform float uCrackDepth; //0~500
//uniform float uCrackWidth;

float max_depth = 100;
float max_width = 100;

in VS_OUT {
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

float ThresholdFunc(float k);
float CalRol(vec2 texCoording);
float calcDerivative(vec2 texCoording);

float orenNayar(vec3 N, vec3 L, vec3 V, float NdotL, float NdotV, float Roughness)
{
    float PI = 3.141592;
    float theta_r = acos(NdotV);
    float theta_i = acos(NdotL);

    float alpha = max(theta_r, theta_i);
    float beta = min(theta_r, theta_i);

    float r2 = Roughness * Roughness;
    //float A = 1.0 - 0.5*(r2/(r2+0.33));
    //float B = 0.45*(r2/(r2+0.09));
    //float C = sin(alpha) * tan(beta);
    //
    //float gamma = dot(V - N * NdotV, L - N * NdotL); 
    //
    //return NdotL * (A + (B  * C * max(0.0, gamma)));
    float gamma = dot(V - N * NdotV, L - N * NdotL); 
    float C1 = 1.0 - 0.5*(r2/(r2+0.33));
    float C2 = 0.45*(r2/(r2+0.09));
    if (gamma >= 0) C2 = C2*sin(alpha);
    else  C2 = C2*sin(alpha - pow((2.0*beta/PI),3));
    float C3 = 0.125*(r2/(r2+0.09))*pow(((4.0*alpha*beta)/(PI*PI)),2);
    float twoBetaPi = 2.0 * beta/PI; 
    float L2 = 0.17 * (r2/(r2 + 0.13)) * (1.0 - gamma * twoBetaPi * twoBetaPi);
    return NdotL*(C1 + gamma*C2*tan(beta)) + (1.0-abs(gamma))*C3*tan(0.5*(alpha+beta) + L2);
}

float CookTorrance(float NdotL, float NdotV, float NdotH, float VdotH, float Roughness)
{
	Roughness *= 3.0f;
	// compute the geometric term
	float G1 = (2.0f * NdotH * NdotV) / VdotH;
	float G2 = (2.0f * NdotH * NdotL) / VdotH;
	float G = min(1.0f, max(0.0f, min(G1,G2)));

	// we set fresnel term == 1
	float F = 1.0; //Roughness2 + (1.0F - Roughness2) * Pow(1.0f - NdotV, 5.0f);

	// compute the roughness term
	float R_2 = Roughness * Roughness;
	float NDotH_2 = NdotH * NdotH;
	float A = 1.0f / (4.0f * R_2 * NDotH_2 * NDotH_2);
	float B = exp(-(1.0f - NDotH_2)/(R_2 * NDotH_2));
	float R = A * B;

	return ((G*F*R)/(NdotL*NdotV));
}

float Transparency(float tao, float NdotL, float NdotV)
{
	return exp(-1.0*tao*(1.0/NdotV+1.0/NdotL));
}

float Daust_coefficient(float g, float NdotL, float NdotV)
{
	float g2 = g*g;
	float theta_r = acos(NdotV);
    float theta_i = acos(NdotL);
	return ((1-g2)/pow((1+g2-2*g*cos(theta_r+theta_i)),1.5)) * (NdotL/(NdotL+ NdotV));
}

//////////////////////////////////////////////////////
// Implements Parallax Mapping technique
// Returns modified texture coordinates, and last used depth
vec2 parallaxMapping(in vec3 VertextoEye, in vec2 texCoord, out float parallaxHeight)
{
  	// Variables that control parallax occlusion mapping quality
	float heightScale = texture(CrackInfoTexture, texCoord).x; //maximum depth of the illusion
	const float minLayers = 20.0f;
    const float maxLayers = 100.0f;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0f, 0.0f, 1.0f), VertextoEye))); //dynamically choose the number of layers based on the angle to the surface
	float layerDepth = 1.0f / numLayers;  //mix(v1, v2, a) = v1 * (1-a) + v2 * a
	float currentLayerDepth = 0.0f;
	
	// Remove the z division if you want less aberated results 
	vec2 S = VertextoEye.xy / VertextoEye.z * heightScale; //(GET the S vector)
    vec2 deltaUVs = S / numLayers;
	
	vec2 UVs = TexCoords;
    float currentDepthMapValue;
    if (texture(CrackInfoTexture, UVs).a == 0) currentDepthMapValue = 1.0f - 1.0f;
	else currentDepthMapValue = texture(CrackInfoTexture, UVs).x * (1-texture(CrackInfoTexture, UVs).z);
	
	// Loop till the point on the heightmap is "hit"
	while(currentLayerDepth < currentDepthMapValue)
    {
        UVs -= deltaUVs;
        if (texture(CrackInfoTexture, UVs).a == 0) currentDepthMapValue = 1.0f - 1.0f;
        else currentDepthMapValue = texture(CrackInfoTexture, UVs).x*(1-texture(CrackInfoTexture, UVs).z);
        currentLayerDepth += layerDepth;
    }
	
	// Apply Occlusion (interpolation with prev value)
	vec2 prevTexCoords = UVs + deltaUVs;  //previous terxture coordinates
    
	float afterDepth  = currentDepthMapValue - currentLayerDepth; // heights for linear interpolation
    float beforeDepth;
    if (texture(CrackInfoTexture, prevTexCoords).a == 0) beforeDepth = 1.0f - 1.0f - currentLayerDepth + layerDepth;
	else beforeDepth = texture(CrackInfoTexture, prevTexCoords).x*(1-texture(CrackInfoTexture, prevTexCoords).z) - currentLayerDepth + layerDepth;
	
    float weight = afterDepth / (afterDepth - beforeDepth);  //proportions for linear interpolation
	UVs = prevTexCoords * weight + UVs * (1.0f - weight);  //interpolation of terture coordinates
	
	parallaxHeight = currentLayerDepth + beforeDepth * weight + afterDepth * (1.0 - weight); //interpolation of depth values

	return UVs;
}

//////////////////////////////////////////////////////
// Implements self-shadowing technique - hard or soft shadows
// Returns shadow factor
float parallaxSoftShadowMultiplier(in vec3 L, in vec2 initialTexCoord,
                                       in float initialHeight)
{
   float u_shadowMultiplier = 1;

   const float minLayers = 15;
   const float maxLayers = 100;

   // calculate lighting only for surface oriented to the light source
   if(dot(vec3(0, 0, 1), L) > 0)
   {
      // calculate initial parameters
      float numSamplesUnderSurface = 0;
      u_shadowMultiplier = 0;
      float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0, 0, 1), L)));  //get the maximum layers will the light have 
      float layerHeight = initialHeight / numLayers;
      vec2 texStep = max_depth * L.xy / L.z / numLayers;

      // current parameters
      float currentLayerHeight = initialHeight - layerHeight;
      vec2 currentTextureCoords = initialTexCoord + texStep;
      float heightFromTexture;
      if (texture(CrackInfoTexture, currentTextureCoords).a == 0) heightFromTexture = 1-1;
      else {
            heightFromTexture =  texture(CrackInfoTexture, currentTextureCoords).x*(1-texture(CrackInfoTexture, currentTextureCoords).z); //1 - texture(textureK, currentTextureCoords).r;
            //if (texture(CrackInfoTexture, currentTextureCoords).z*texture(CrackInfoTexture, currentTextureCoords).x < 0.5) 
            //       heightFromTexture = mix( 1 - texture(CrackInfoTexture, currentTextureCoords).z*texture(CrackInfoTexture, currentTextureCoords).x, texture(CrackInfoTexture, currentTextureCoords).z*texture(CrackInfoTexture, currentTextureCoords).x,uCrackDepth);
	  }
      int stepIndex = 1;

      // while point is below depth 0.0 )
      while(currentLayerHeight > 0)
      {
         // if point is under the surface
         if(heightFromTexture < currentLayerHeight)
         {
            // calculate partial shadowing factor
            numSamplesUnderSurface += 1;
            float newShadowMultiplier = (currentLayerHeight - heightFromTexture) *
                                             (1.0 - stepIndex / numLayers);
            u_shadowMultiplier = max(u_shadowMultiplier, newShadowMultiplier);
         }

         // offset to the next layer
         stepIndex += 1;
         currentLayerHeight -= layerHeight;
         currentTextureCoords += texStep;
         if (texture(CrackInfoTexture, currentTextureCoords).a == 0) heightFromTexture = 1-1;
         else heightFromTexture = texture(CrackInfoTexture, currentTextureCoords).x*(1-texture(CrackInfoTexture, currentTextureCoords).z);
      }

      // Shadowing factor should be 1 if there were no points under the surface
      if(numSamplesUnderSurface < 1)
      {
         u_shadowMultiplier = 1;
      }
      else
      {
         u_shadowMultiplier = 1.0 - u_shadowMultiplier;
      }
   }
   return u_shadowMultiplier;
}

float CalRol_forderive(vec2 texCoording)  //Gray body part
{
    float rol_0 = 0.35;
    float PI = 3.1415926;
        
    float L = texture(CrackInfoTexture, texCoording).x * max_depth;
    float R = 0.5 * texture(CrackInfoTexture, texCoording).y * max_width;
    if (L < 1.5*R) return 1.0;  //45 degree
    else
    {
        float x = texture(CrackInfoTexture, texCoording).z*texture(CrackInfoTexture, texCoording).x* max_depth;
        float theta1 = (PI/2.0)-atan((R*L+R*x)/(L*L-L*x))-atan((R*L-R*x)/(L*L-L*x));
        float theta2 = PI/2.0;
        float sin_theta1 = sin(theta1);
        float sin_theta2 = sin(theta2);
        float F = sin_theta2*sin_theta2-sin_theta1*sin_theta1;
        float G = (R*R)/((2*R*sqrt(R*R+L*L))+(R*R));
        
        float top = (1-rol_0)*(1+rol_0*(G-F));
        float bot = 1-rol_0*(1-G);
        float epsilon = top/bot;
        float rol = 1- epsilon;

        return 1.0/rol_0*rol;
    }
}

float calcDerivative(vec2 texCoording){
    float delta = 1.0 / 10000000;
    float num = ((CalRol_forderive(texCoording) + delta) - (CalRol_forderive(texCoording) - delta)) / (2 * delta);
    return num;
}

float CalRol(vec2 texCoording)
{
    float rol_0 = 0.35;
    float PI = 3.1415926;
        
    float L = texture(CrackInfoTexture, texCoording).x * max_depth;
    float R = 0.5 * texture(CrackInfoTexture, texCoording).y*max_width;//*0.4
    if (L < 1.5*R) return 1.0;  //45 degree
    else
    {
        float x = texture(CrackInfoTexture, texCoording).z*texture(CrackInfoTexture, texCoording).x* max_depth;
        float theta1 = (PI/2.0)-atan((R*L+R*x)/(L*L-L*x))-atan((R*L-R*x)/(L*L-L*x));
        float theta2 = PI/2.0;
        float sin_theta1 = sin(theta1);
        float sin_theta2 = sin(theta2);
        float F = sin_theta2*sin_theta2-sin_theta1*sin_theta1;
        float G = (R*R)/((2*R*sqrt(R*R+L*L))+(R*R));
        
        float top = (1-rol_0)*(1+rol_0*(G-F));
        float bot = 1-rol_0*(1-G);
        float epsilon = top/bot;
        float rol = 1- epsilon;
        if (calcDerivative(texCoording) < 0)
            return 1;
        else
            return 1.0/rol_0*rol;
        //return 1.0/rol_0*rol;
    }
}

float ThresholdFunc(float k)
{
    float add_parameter = 0.5;
    k = k + 0.5;
    if (k > 1.0)
        return 1.0;
    else return k;
}


//void main()
//{
//    //if (texture(CrackInfoTexture,TexCoordsCrack).a == 0) FragColor = vec4(0,0,0,1);
//    //else FragColor = vec4(vec3(1-texture(CrackInfoTexture,TexCoordsCrack).x+texture(CrackInfoTexture,TexCoordsCrack).x*texture(CrackInfoTexture,TexCoordsCrack).z),1.0);
//    FragColor = vec4(1,0,0,1);
//}


void main()
{
//==========================ParallaxMapping===========================    

	vec3 VertextoLight = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);	
    vec3 VertextoEye = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
//--------------------------------------------------------------------------
    float parallaxHeight; //for shadow
	vec2 UVs = parallaxMapping(VertextoEye, TexCoordsCrack, parallaxHeight);
	
	//if(UVs.x > 1.0 || UVs.y > 1.0 || UVs.x < 0.0 || UVs.y < 0.0)
    //discard;

//====================================================================

//-----------------------------Hightmap light-----------------------------
    vec3 normal;
    const vec2 size = vec2(2.0,0.0);
    const ivec3 off = ivec3(-1,0,1);
    
    vec4 wave = texture(CrackInfoTexture, TexCoordsCrack);
    float s11 = (1-wave.x+wave.z*wave.x);
    vec4 wave01 = textureOffset(CrackInfoTexture, TexCoordsCrack, off.xy);
    vec4 wave02 = textureOffset(CrackInfoTexture, TexCoordsCrack, off.zy);
    vec4 wave03 = textureOffset(CrackInfoTexture, TexCoordsCrack, off.yx);
    vec4 wave04 = textureOffset(CrackInfoTexture, TexCoordsCrack, off.yz);
    float s01 = (1 - wave01.x + wave01.z * wave01.x);
    float s21 = (1 - wave02.x + wave02.z * wave02.x);
    float s10 = (1 - wave03.x + wave03.z * wave03.x);
    float s12 = (1 - wave04.x + wave04.z * wave04.x);
    float mult = 5;  // the depth 2.3
    vec3 va = normalize(vec3(size.xy,mult*(s21-s01)));
    vec3 vb = normalize(vec3(size.yx,mult*(s12-s10)));
    normal = cross(va,vb);
    

    vec4 F = vec4(0.886, 0.819, 0.749,1);

//--------------------------------------------------------------------------

    //Ambient light
	vec3  AmbientLight = uAmbient*vec3(1.0f, 1.0f, 1.0f);                            //The strength of the light
    
    //Diffuse light
	vec3  DiffuseColor = vec3(1.0,1.0,1.0);//mix(vec3(0.85f, 0.55f, 0.38f),vec3(0.123,0.83,0.51),uRoughness);                           //White light
    //vec3 VertextoLight = normalize(LightPos0 - vs_position);
    //float Diffuse = max(dot(normal, VertextoLight),0.);
    //vec3  DiffuseFinal = uDiffusion * DiffuseColor * Diffuse;

    //==================OrenNayar Lighting===================//
    //vec3 VertextoLight = normalize(LightPos0 - vs_position);
    //vec3 VertextoEye = normalize(vec3(0,0,25) - vs_position);
    float NdotL = clamp(dot(normal, VertextoLight), 0.0, 1.0);//max(dot(normal, VertextoLight),0.);
    float NdotV = clamp(dot(normal, VertextoEye), 0.0, 1.0);//max(dot(normal, VertextoEye),0.);
    float Diffuse = orenNayar(normal, VertextoLight, VertextoEye, NdotL, NdotV, uRoughness);
    vec3 DiffuseFinal = uDiffusion * DiffuseColor * Diffuse;


    //Specular light
	//vec3  LightToPosDirVec = normalize(LightPos0 - vs_position);
	//vec3  ReflectDirVec = normalize(reflect(LightToPosDirVec, normal));
	//vec3  PosToViewDirVec = normalize(vs_position - vec3(0,0,20));//CameraPos
	//float SpecularConstant = pow(max(dot(PosToViewDirVec, ReflectDirVec), 0), uShininess); //50 is the strength of the light
	//vec3  SpecularFinal = uSpecular * vec3(1.f, 1.0f, 1.0f) * SpecularConstant;

    vec3 SpecularColor = vec3(1.f, 1.0f, 1.0f);
    float Specular = 0.;
    //vec3 VertextoEye = normalize(vec3(0,0,20) - vs_position);
    if (dot(normal,VertextoLight) > 0.)
    {
        vec3 ref = normalize(2. * normal * dot(normal,VertextoLight) - VertextoLight);
		Specular = pow(max(dot(VertextoEye,ref), 0.), uShininess);
    }
    vec3  SpecularFinal = uSpecular * SpecularColor * Specular;


    float dc = max(0.0, dot(VertextoLight, normal));
	//float shadowMultiplier = ShadowCalc(UVs, VertextoLight);//dc > 0.0 ? ShadowCalc(UVs, VertextoLight) : 0.0;
    float shadowMultiplier = dc > 0.0 ? parallaxSoftShadowMultiplier(VertextoLight, UVs, parallaxHeight - 0.05) : 0.0;

    vec4 ligheffect;
    if (texture(CrackInfoTexture,UVs).a == 0) ligheffect = vec4((SpecularFinal + AmbientLight + DiffuseFinal).xyz, 1.0);//* pow(shadowMultiplier,4)
    else ligheffect = vec4((SpecularFinal + AmbientLight + DiffuseFinal).xyz, 1.0)* pow(shadowMultiplier,4);//
   
    
//-----------------------------------------------
    if (texture(CrackInfoTexture,TexCoords) != vec4(1,1,1,1) ) 
    {
        float k = pow(CalRol(TexCoords),0.7);
        F = vec4(F.xyz*k,1.0);
    }
    FragColor = vec4((F * ligheffect).xyz,1.0);

}
