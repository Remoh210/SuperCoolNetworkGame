#version 420 
//fragment01.glsl

// In from a previous stage (vertex shader)
in vec3 color;			// in from the vertex shader
in vec4 vertPosWorld;
in vec3 vertNormal;		// "Model space" (only rotation)
in vec4 vertUV_x2;		// Texture coordinates

uniform vec4 objectDiffuse;		// becomes objectDiffuse in the shader
uniform vec4 objectSpecular;	// rgb + a, which is the power)

uniform vec3 eyeLocation;		// This is in "world space"

// Set this to true (1), and the vertex colour is used
uniform bool useVertexColour;

// If you want this, go ahead and use this.
// You would:
// - Make an "ambient" value that's diffuse * diffuseToAmbientRatio
// - Then, calculate the final colour with ONLY lighting.
// - If this value is LESS than the ambient, use the ambient.
uniform float diffuseToAmbientRatio;		// 0.2 

uniform bool bDontUseLighting;		

// If this is true, then we are using the smoke imposter
// (the shader will be changed a little)
uniform bool bSmokeImposter;


//vec4 gl_FragColor
out vec4 finalOutputColour;		// Any name, but must be vec4

struct sLight
{
	vec4 position;			
	vec4 diffuse;	
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
};

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

const int NUMBEROFLIGHTS = 10;
uniform sLight theLights[NUMBEROFLIGHTS];  	// 80 uniforms

// CAN'T put texture samplers into an array (sadly)
//uniform sampler textures[10];

uniform sampler2D texture00;
uniform sampler2D texture01;
uniform sampler2D texture02;
uniform sampler2D texture03;
uniform sampler2D texture04;
uniform sampler2D texture05;
uniform sampler2D texture06;
uniform sampler2D texture07;

// Cube map texture (NOT a sampler3D)
uniform samplerCube textureSkyBox;
uniform bool useSkyBoxTexture;

// 
uniform bool bAddReflect;		// Add reflection
uniform bool bAddRefract;		// Add refraction
uniform float refractionIndex;

// This is 4 x 2 floats or 8 floats
uniform vec4 texBlendWeights[2];	// x is 0, y is 1, z is 2

uniform float wholeObjectAlphaTransparency;


void main()
{

	vec4 materialDiffuse = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	vec4 materialSpecular = objectSpecular;

	// is this the skybox texture?
	if (useSkyBoxTexture == true)
	{
		//finalOutputColour.rgb = vec3(1.0f, 0.0f, 0.0f);

		// Note for cube maps, the texture coordinates are 3D
		// (so here we are using the normal on the surface 
		//  of the sphere, like a "ray cast" really)
		vec3 skyPixelColour = texture( textureSkyBox, vertNormal.xyz ).rgb;
		
		finalOutputColour.rgb = skyPixelColour;
		finalOutputColour.a = 1.0f;
		return;
	}
	

	

	// Reflection and refraction if you want to use these later...
	vec3 rgbReflect = vec3(0.0f,0.0f,0.0f);
	vec3 rgbRefract = vec3(0.0f,0.0f,0.0f);
	
	// Add a FULLY reflective object 
	if ( bAddReflect )
	{
		// Calculate ray from vertex to eye
		vec3 viewVector = eyeLocation.xyz - vertPosWorld.xyz;
		viewVector = normalize(viewVector); 
		
		// Calcualte the reflection vector
		vec3 vReflect = reflect(viewVector, vertNormal.xyz);
		
		rgbReflect = texture( textureSkyBox, vReflect ).rgb;
	}//if ( bAddReflect )
	
	if (bAddRefract)
	{
		// Calculate ray from vertex to eye
		vec3 viewVector = eyeLocation.xyz - vertPosWorld.xyz;
		viewVector = normalize(viewVector); 
		
		// Calcualte the reflection vector
		vec3 vRefract = refract(viewVector, vertNormal.xyz, 1.10);
		
		rgbRefract = texture( textureSkyBox, vRefract ).rgb;
	}//if (bAddRefract)	
	
	
	// You could add the reflection and refraction colours to your
	// object's colour, but here I'm going to combine and exit early.
//	if ( bAddReflect || bAddRefract )
//	{
//		float amountOfReflect = 0.5f;
//		float amountOfRefract = 0.5f;
//	
//		finalOutputColour.rgb =   (rgbReflect * amountOfReflect)
//		                        + (rgbRefract * amountOfRefract);
//		finalOutputColour.a = 1.0f;	
//		return;	
//	}
	
	

	if ( useVertexColour )
	{
		//gl_FragColor = vec4(color, 1.0);
		materialDiffuse = vec4( color, 1.0f );
	}
	else
	{
		//gl_FragColor = vec4(objectColour, 1.0);
//		materialDiffuse = objectDiffuse;

		vec4 tex0Col = texture( texture00, vertUV_x2.st ).rgba;
		vec4 tex1Col = texture( texture01, vertUV_x2.st ).rgba;
		vec4 tex2Col = texture( texture02, vertUV_x2.st ).rgba;
		vec4 tex3Col = texture( texture03, vertUV_x2.st ).rgba;
		vec4 tex4Col = texture( texture04, vertUV_x2.st ).rgba;
		vec4 tex5Col = texture( texture05, vertUV_x2.st ).rgba;
		vec4 tex6Col = texture( texture06, vertUV_x2.st ).rgba;
		vec4 tex7Col = texture( texture07, vertUV_x2.st ).rgba;
		
		materialDiffuse =  objectDiffuse
						  + (tex0Col * texBlendWeights[0].x) 	 // 0
		                  + (tex1Col * texBlendWeights[0].y)     // 1
						  + (tex2Col * texBlendWeights[0].z)     // 2
		                  + (tex3Col * texBlendWeights[0].w)     // 3
		                  + (tex4Col * texBlendWeights[1].x)     // 4
		                  + (tex5Col * texBlendWeights[1].y)     // 5
		                  + (tex6Col * texBlendWeights[1].z)     // 6
		                  + (tex7Col * texBlendWeights[1].w);    // 7
		
	}//if ( useVertexColour )
	
	// Is this being lit? 
	if ( bDontUseLighting )
	{
		// Just exit early
		finalOutputColour = objectDiffuse;
		return;
	}
	
	
//theLights[0].position;
//theLights[0].diffuseColour;
//theLights[0].atten;

	vec3 norm = normalize(vertNormal);
	vec4 finalObjectColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{	
		// ********************************************************
		// is light "on"
		if ( theLights[index].param2.x == 0.0f )
		{	// it's off
			continue;
		}
		
		// Cast to an int (note with c'tor)
		int intLightType = int(theLights[index].param1.x);
		
		// We will do the directional light here... 
		// (BEFORE the attenuation, since sunlight has no attenuation, really)
		if ( intLightType == DIRECTIONAL_LIGHT_TYPE )		// = 2
		{
			// This is supposed to simulate sunlight. 
			// SO: 
			// -- There's ONLY direction, no position
			// -- Almost always, there's only 1 of these in a scene
			// Cheapest light to calculate. 

			vec3 lightContrib = theLights[index].diffuse.rgb;
			
			// Get the dot product of the light and normalize
			float dotProduct = dot( -theLights[index].direction.xyz,  
									   normalize(norm.xyz) );	// -1 to 1

			dotProduct = max( 0.0f, dotProduct );		// 0 to 1
			
			lightContrib *= dotProduct;		
			
			finalObjectColour.rgb += (materialDiffuse.rgb * theLights[index].diffuse.rgb * lightContrib); 
									 //+ (materialSpecular.rgb * lightSpecularContrib.rgb);
			// NOTE: There isn't any attenuation, like with sunlight.
			// (This is part of the reason directional lights are fast to calculate)

			// TODO: Still need to do specular, but this gives you an idea
			finalOutputColour.rgb = finalObjectColour.rgb;
			finalOutputColour.a = wholeObjectAlphaTransparency;

			return;		
		}
		
		// Assume it's a point light 
		// intLightType = 0
		
		// Contribution for this light
		vec3 vLightToVertex = theLights[index].position.xyz - vertPosWorld.xyz;
		float distanceToLight = length(vLightToVertex);	
		vec3 lightVector = normalize(vLightToVertex);
		float dotProduct = dot(lightVector, norm);	 
		
		dotProduct = max( 0.0f, dotProduct );	
		
		vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;
			

		// Specular 
		vec3 lightSpecularContrib = vec3(0.0f);
			
		vec3 reflectVector = reflect( -lightVector, normalize(norm) );

		// Get eye or view vector
		// The location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertPosWorld.xyz);

		// To simplify, we are NOT using the light specular value, just the object’s.
		float objectSpecularPower = objectSpecular.w; 
		
		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
			                   * objectSpecular.rgb;	//* theLights[lightIndex].Specular.rgb
					   
		// Attenuation
		float attenuation = 1.0f / 
				( theLights[index].atten.x + 										
				  theLights[index].atten.y * distanceToLight +						
				  theLights[index].atten.z * distanceToLight*distanceToLight );  	
				  
		// total light contribution is Diffuse + Specular
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;
		
		
		// But is it a spot light
		if ( intLightType == SPOT_LIGHT_TYPE )		// = 1
		{	
		

			// Yes, it's a spotlight
			// Calcualate light vector (light to vertex, in world)
			vec3 vertexToLight = vertPosWorld.xyz - theLights[index].position.xyz;

			vertexToLight = normalize(vertexToLight);

			float currentLightRayAngle
					= dot( vertexToLight.xyz, theLights[index].direction.xyz );
					
			currentLightRayAngle = max(0.0f, currentLightRayAngle);

			//vec4 param1;	
			// x = lightType, y = inner angle, z = outer angle, w = TBD

			// Is this inside the cone? 
			float outerConeAngleCos = cos(radians(theLights[index].param1.z));
			float innerConeAngleCos = cos(radians(theLights[index].param1.y));
							
			// Is it completely outside of the spot?
			if ( currentLightRayAngle < outerConeAngleCos )
			{
				// Nope. so it's in the dark
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if ( currentLightRayAngle < innerConeAngleCos )
			{
				// Angle is between the inner and outer cone
				// (this is called the penumbra of the spot light, by the way)
				// 
				// This blends the brightness from full brightness, near the inner cone
				//	to black, near the outter cone
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
									  (innerConeAngleCos - outerConeAngleCos);
									  
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}
						
		}// if ( intLightType == 1 )
		
		
					
		finalObjectColour.rgb += (materialDiffuse.rgb * lightDiffuseContrib.rgb) 
		                         + (materialSpecular.rgb * lightSpecularContrib.rgb);
		// ********************************************************
	}//for(intindex=0...
	
	finalOutputColour.rgb = finalObjectColour.rgb;
	finalOutputColour.a = wholeObjectAlphaTransparency;
	
	// Brigher for the dim projector
	finalOutputColour.rgb *= 1.25f;
	
	
	
	// Add any reflection or refraction 
	if ( bAddRefract )
	{
		float amountToAdd = 0.25f;
		// Drop the amount of current colour by a little bit...
		finalOutputColour *= ( 1.0f - amountToAdd );
		// ... and add the refractive colour		
		finalOutputColour.rgb += ( amountToAdd * rgbRefract );
	}
	if ( bAddReflect )
	{
		float amountToAdd = 0.25f;
		// Drop the amount of current colour by a little bit...
		finalOutputColour *= ( 1.0f - amountToAdd );
		// ... and add the reflective colour
		finalOutputColour.rgb += ( amountToAdd * rgbReflect );
	}
	
	
	// Smoke imposter
	if ( bSmokeImposter ) 
	{
		// Discard anything that's "black enough"
		vec4 tex0Col = texture( texture00, vertUV_x2.st ).rgba;	// Smoke
		
		// Pick any pixel to check
		if ( tex0Col.r < 0.25f )
		{
			// Don't draw it
			// Literally Doesn't Draw the pixel
			discard;
		}	
		// OK, I'm drawing the smoke...
		// The darker it is, the more transparent it should be
		// Colour is from 0 to 1  (white = 1)
		// Alpha goes from 0 to 1, too (oh my goodness)
		finalOutputColour.a = tex0Col.r;
	}
	
	
	// Make the colour "black"
//	finalOutputColour.rgb *= 0.001f;
	
	// How far away from the centre of the circle am I?
	// ( 0.5, 0.5) 
	
//	vec2 cirCentre = vec2( 0.5f, 0.5f );
//	float cirRadius = 0.3f;	
//	
//	if ( distance(cirCentre, vertUV_x2.st ) >= cirRadius )
//	{
//		// Outside the circle
//		finalOutputColour.rgb += vec3(0,1,0);		// Green
//	}
//	else 
//	{
//		// Inside the circle
//		finalOutputColour.rgb += vec3(1,0,0);		// Red
//	}
	
	
	
	// Step
//	if ( vertUV_x2.s <= 0.25 )			// s == u
//	{
//		finalOutputColour.rgb += vec3(1,0,0);		// Red
//	}
//	else 
//	{
//		finalOutputColour.rgb += vec3(0,1,0);		// green
//	}

//	finalOutputColour.r += mix( 0.0f, 1.0f, vertUV_x2.s );	// u
//	finalOutputColour.g += mix( 1.0f, 0.0f, vertUV_x2.s );	// u
//	CameraSpeed = mix( MaxSpeed, 0.0f, HowFarIntoTheGreyAreaWeAre );	// u
	
//	finalOutputColour.r += smoothstep( 0.0f, 1.0f, vertUV_x2.s );	// u
//	finalOutputColour.g += smoothstep( 1.0f, 0.0f, vertUV_x2.s );	// u
	
	
	// EVERYTHING is 50% transparent
	finalOutputColour.a = wholeObjectAlphaTransparency;
	

}