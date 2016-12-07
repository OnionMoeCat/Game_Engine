/*
	This is an example of a fragment shader
*/

// Platform-specific setup
#include "shaders.inc"

uniform float3 g_ambient_light;
uniform float3 g_light_position_world;
uniform sampler2D g_texture;
uniform sampler2D g_normal_map;

uniform float g_k0;
uniform float g_k1;
uniform float g_k2;
uniform float3 g_diffuse_light;
#if defined( EAE6320_PLATFORM_D3D )

// Entry Point
//============

void main(

	// Input
	//======

	// Whatever arbitrary data (i.e. everything excluding position) was output from the vertex shader
	// will be interpolated across the triangle and given as input to the fragment shader

	in float4 i_color : COLOR0,
	in float2 i_texcoords : TEXCOORD0,

	// Output
	//=======

	// Whatever color value is output from the fragment shader
	// will determine the color of the corresponding pixel on the screen
	out float4 o_color : COLOR0

	)

#elif defined( EAE6320_PLATFORM_GL )

// Input
//======

// Whatever arbitrary data (i.e. everything excluding position) was output from the vertex shader
// will be interpolated across the triangle and given as input to the fragment shader
layout( location = 0 ) in vec4 i_position_world;
layout( location = 1 ) in vec4 i_color;
layout( location = 2 ) in vec2 i_texcoords;
layout( location = 3 ) in mat3 i_TBN;

// Output
//=======

// Whatever color value is output from the fragment shader
// will determine the color of the corresponding pixel on the screen
out vec4 o_color;

// Entry Point
//============

void main()

#endif
{
	{
		// Calculate normal
		float3 normalWorld = tex2D(g_normal_map, i_texcoords).rgb;
		normalWorld = normalize(normalWorld * 2.0 - 1.0);
		normalWorld = normalize(Transform(normalWorld, i_TBN));

		// Calculate the diffuse light
		float3 lightVectorWorld = g_light_position_world - i_position_world.xyz;
		float distance = length(lightVectorWorld);
		float diffuseBrightness = clamp(dot(normalize(lightVectorWorld), normalize(normalWorld)) / (g_k0 + g_k1 * distance + g_k2 * distance * distance), 0, 1);
		//float diffuseBrightness = clamp(dot(normalize(lightVectorWorld), normalize(normalWorld)), 0, 1);
		float3 diffuseLight = g_diffuse_light * diffuseBrightness;
		
		// Calculate the ambient light
		float3 ambientLight = g_ambient_light;
		
		// Combine them
		float3 light = clamp(ambientLight + diffuseLight, float3(0, 0, 0), float3(1, 1, 1));

		// Set the fragment to the interpolated color that originated as per-vertex data
		// (where color is represented by 4 floats representing "RGBA" == "Red/Green/Blue/Alpha")
		o_color.rgb = i_color.rgb * light;
		o_color.a = i_color.a;
		o_color = o_color * tex2D(g_texture, i_texcoords);
	}
}
