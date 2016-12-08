return
{
	{
		builder = "MeshBuilder.exe",
		assets = {
			{
				source = "cube.mesh.raw",
				target = "cube.mesh",
			},
		},
	},
	{
		builder = "ShaderBuilder.exe",
		dependencies = {
			"shaders.inc"
		},
		assets = {
			{
				source = "vertex.shader",
				target = "vertex.shader",
				optionalArguments = {
					"vertex"
				},
			},
			{
				source = "fragment.shader",
				target = "fragment.shader",
				optionalArguments = {
					"fragment"
				},
			},
			{
				source = "fragment_transparent.shader",
				target = "fragment_transparent.shader",
				optionalArguments = {
					"fragment"
				},
			},
		},
	},
	{
		builder = "EffectBuilder.exe",
		assets = {
			{
				source = "opaque.effect.raw",
				target = "opaque.effect",
			},
			{
				source = "transparent.effect.raw",
				target = "transparent.effect",
			},
		},
	},
	{
		builder = "MaterialBuilder.exe",
		assets = {
			{
				source = "opaque.material.raw",
				target = "opaque.material",
			},
			{
				source = "transparent.material.raw",
				target = "transparent.material",
			},
		},
	},
	{
		builder = "TextureBuilder.exe",
		assets = {
			{
				source = "default.jpg",
				target = "default.dds",
			},
		},
	},
}
