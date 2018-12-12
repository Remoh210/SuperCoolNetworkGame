#version 420 
// vertex01.glsl

//uniform mat4 MVP;		
uniform mat4 matModel;		// M
uniform mat4 matModelInvTrans;	// inverse(transpose(matModel))
uniform mat4 matView;		// V
uniform mat4 matProj;		// P

in vec3 vColour;		// rgb   	was "attribute"
in vec3 vPosition;		// xyz		was "attribute"
in vec3 vNormal;		// normal xyz
in vec4 vUV_x2;			// Texture coordinates (2 of them)

out vec3 color;			// exit to fragment
out vec4 vertPosWorld;	// "World space"
out vec3 vertNormal;	// "Model space"
out vec4 vertUV_x2;		// To the next shader stage


// Note, we're in the VERTEX shader now, baby!
uniform sampler2D texHeightMap;
uniform bool bUseHeightMap;			// "turn on" the vertex displacement
uniform float heightMapRatio;		// Increase the range of the displacement


void main()
{
	// Make a copy of the "model space" vertex
	vec3 posTemp = vPosition;
	
	// Apply vertex displacement?
	if ( bUseHeightMap )
	{
		// Note: I'm only sampling ONE of the colours, because it's black and white
		// Returns 0.0 to 1.0
		float height = texture( texHeightMap, vUV_x2.st ).r;
		
		height = height * heightMapRatio;
		
		posTemp.y = 0.0f;		// "Flatten" the mesh
		posTemp.y = height;		// Set the heigth
		// You could also "adjust" an existing mesh
	
	}//if ( bUseHeightMap )
	
	// Note these are 'backwards'
	mat4 MVP = matProj * matView * matModel;
    gl_Position = MVP * vec4(posTemp, 1.0);			// ON SCREEN
	
	vertPosWorld = matModel * vec4(posTemp, 1.0);
	
	// Transforms the normal into "world space"
	// Remove all scaling and transformation from model
	// Leaving ONLY rotation... 
	vertNormal = vec3( matModelInvTrans * vec4(normalize(vNormal),1.0f) );
	
    color = vColour;
	
	// Pass the texture coordinates out, unchanged.
	vertUV_x2.xy = vUV_x2.xy;
}



