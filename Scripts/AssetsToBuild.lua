return
{
	{
		builder = "MeshBuilder.exe",
		assets = {
			{
				source = "floor.mesh.raw",
				target = "floor.mesh",
			},
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
				source = "VertexShaderCode.shader",
				target = "VertexShaderCode.shader",
				optionalArguments = {
					"vertex"
				},
			},
			{
				source = "FragmentShaderCode.shader",
				target = "FragmentShaderCode.shader",
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
				source = "default.effect.raw",
				target = "default.effect",
			},
		},
	},
	{
		builder = "MaterialBuilder.exe",
		assets = {
			{
				source = "default.material.raw",
				target = "default.material",
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
			{
				source = "normal_map.png",
				target = "normal_map.dds",
			},
		},
	},
}
