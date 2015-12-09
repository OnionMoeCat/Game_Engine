return
{
	{
		builder = "MeshBuilder.exe",
		assets = {
			{
				source = "plane.meshraw",
				target = "plane.mesh",
			},
			{
				source = "floor.meshraw",
				target = "floor.mesh",
			},
			{
				source = "ball.meshraw",
				target = "ball.mesh",
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
				source = "default.effect.raw",
				target = "default.effect",
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
				source = "default.material.raw",
				target = "default.material",
			},
			{
				source = "red.material.raw",
				target = "red.material",
			},
			{
				source = "green.material.raw",
				target = "green.material",
			},
			{
				source = "transparent_03.material.raw",
				target = "transparent_03.material",
			},
			{
				source = "transparent_08.material.raw",
				target = "transparent_08.material",
			},
			{
				source = "eae6320.material.raw",
				target = "eae6320.material",
			},
			{
				source = "alpha.material.raw",
				target = "alpha.material",
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
				source = "brick.jpg",
				target = "brick.dds",
			},
			{
				source = "eae6320.png",
				target = "eae6320.dds",
			},
			{
				source = "alpha.png",
				target = "alpha.dds",
			},
		},
	},
}
