return
{
	{
		builder = "MeshBuilder.exe",
		assets = {
			{
				source = "square.mesh.raw",
				target = "square.mesh.raw",
			},
			{
				source = "triangle.mesh.raw",
				target = "triangle.mesh.raw",
			},
		},
	},
	{
		builder = "GenericBuilder.exe",
		assets = {
			{
				source = "vertex.shader",
				target = "vertex.shader",
			},
			{
				source = "fragment.shader",
				target = "fragment.shader",
			},
		},
	},
}
