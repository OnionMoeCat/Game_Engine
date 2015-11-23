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
				source = "floor.mesh.raw",
				target = "floor.mesh",
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
				source = "transparent_02.material.raw",
				target = "transparent_02.material",
			},
			{
				source = "transparent_08.material.raw",
				target = "transparent_08.material",
			},
		},
	},
}
