// stone/mxrock0b >> stone/mxrock0 >> assault_rock/ground_c07a
// stone/mxrock2b >> assault_rock/ground_c07a
// stone/mxrock1b >> terrain/grass_ml03cmp
// stone/mxrock4b >> assault_rock/ground_c09a

///////////////// Trenchtoast ////////////////
//////////////////////////////////////////////

textures/nerve-guf/trenchterra1_0
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/assault_rock/ground_c07a.tga
		rgbGen vertex
		tcmod scale 0.1 0.1
	}
}

textures/nerve-guf/trenchterra1_1
{
	surfaceparm gravelsteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/terrain/grass_ml03cmp.tga
		rgbGen vertex
		tcmod scale 0.1 0.1 
	}	
}

textures/nerve-guf/trenchterra1_2
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/assault_rock/ground_c07a.tga
		tcmod scale 0.05 0.05 
		rgbGen vertex
	}
}

textures/nerve-guf/trenchterra1_3
{
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/terrain/mountain_m09.tga
		tcmod scale 0.05 0.05 
		rgbGen vertex
	}
}

textures/nerve-guf/trenchterra1_4
{
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/assault_rock/ground_c09a.tga
		tcmod scale 0.1 0.1 
		rgbGen vertex
	}
}

textures/nerve-guf/trenchterra1_0to1
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/assault_rock/ground_c07a.tga	
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.1 0.1 
	}
	{
		map textures/terrain/grass_ml03cmp.tga
		tcmod scale 0.1 0.1
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/nerve-guf/trenchterra1_0to2
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/assault_rock/ground_c07a.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.1 0.1
	}
	{
		map textures/assault_rock/ground_c07a.tga
		tcmod scale 0.05 0.05 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/nerve-guf/trenchterra1_0to3
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/assault_rock/ground_c07a.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.1 0.1
	}
	{
		map textures/terrain/mountain_m09.tga
		tcmod scale 0.05 0.05 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/nerve-guf/trenchterra1_0to4
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/assault_rock/ground_c07a.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.1 0.1
	}
	{
		map textures/assault_rock/ground_c09a.tga
		tcmod scale 0.1 0.1 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/nerve-guf/trenchterra1_1to2
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/terrain/grass_ml03cmp.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.1 0.1 
	}
	{
		map textures/assault_rock/ground_c07a.tga
		tcmod scale 0.05 0.05 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/nerve-guf/trenchterra1_1to3
{
	surfaceparm gravelsteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/terrain/grass_ml03cmp.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.1 0.1 
	}
	{
		map textures/terrain/mountain_m09.tga
		tcmod scale 0.05 0.05 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/nerve-guf/trenchterra1_1to4
{
	surfaceparm gravelsteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/terrain/grass_ml03cmp.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.1 0.1 
	}
	{
		map textures/assault_rock/ground_c09a.tga
		tcmod scale 0.1 0.1 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/nerve-guf/trenchterra1_2to3
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/assault_rock/ground_c07a.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.05 0.05 
	}
	{
		map textures/terrain/mountain_m09.tga
		tcmod scale 0.05 0.05 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/nerve-guf/trenchterra1_2to4
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/assault_rock/ground_c07a.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.05 0.05 
	}
	{
		map textures/assault_rock/ground_c09a.tga
		tcmod scale 0.1 0.1 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/nerve-guf/trenchterra1_3to4
{
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/terrain/mountain_m09.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.05 0.05 
	}
	{
		map textures/assault_rock/ground_c09a.tga
		tcmod scale 0.1 0.1 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
