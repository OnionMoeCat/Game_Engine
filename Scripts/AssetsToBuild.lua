return
{
	{
		builder = "MeshBuilder.exe",
		assets = {
			{
				source = "square.mesh.raw",
				target = "square.mesh",
			},
			{
				source = "triangle.mesh.raw",
				target = "triangle.mesh",
			},
		},
	},
	{
		builder = "ShaderBuilder.exe",
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
}
