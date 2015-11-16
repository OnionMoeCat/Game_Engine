return
{
	{
		builder = "MeshBuilder.exe",
		assets = {
			{
				source = "box.mesh.raw",
				target = "box.mesh",
			},
			{
				source = "square.mesh.raw",
				target = "square.mesh",
			},
			{
				source = "ball.mesh.raw",
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
}
