fleshimpactsmokepuff
{
	nofog
	cull none
	entityMergable		// allow all the sprites to be merged together
	{
		map gfx/misc/smokepuffflesh.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen		vertex
		alphaGen	vertex
	}
}



powerbar
{
	nopicmip
	nocompress
	nomipmap
	{
		map ui_mp/assets/hudpower.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
sprintbar
{
	nopicmip
	nocompress
	nomipmap
	{
		clampmap ui_mp/assets/hudsprint.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

console-16bit
{
	nopicmip
	nocompress

	{
		map ui_mp/assets/SMOKE-16bit.tga
		rgbGen const ( 0.3 0.3 0.3 )
		blendfunc GL_ONE GL_ZERO
		tcmod scale 1.5 .75
		tcmod scroll 0.025 .05
	}
	{
		map ui_mp/assets/SMOKE-16bit.tga
		blendfunc GL_ONE GL_ONE
		rgbGen const ( 0.2 0.2 0.2 )
		tcmod scale .5 .25
		tcmod scroll 0 .05 
	}
}

// saturates color band in 16-bit instead of using bottom 1/4
console2-16bit
{
	nopicmip
	nocompress
	{
		blendfunc blend
		map ui_mp/assets/wolficonback4.tga	// the bg logo that goes over the console and under the text
	}
}

textures/liquids/water_beach_nerve
{
	qer_editorimage textures/liquids/pool3d_3.tga
	qer_trans .5
	q3map_globaltexture
	surfaceparm trans
	surfaceparm nonsolid
	surfaceparm water
	surfaceparm nomarks	// don't play ripple effects when shooting into water
	tesssize 512
	
	cull disable
	nofog
	deformVertexes wave 160 sin 0 10 0 .3	// the chop

	waterfogvars ( 0.1 0.1 0.1 ) 512
//	waterfogvars ( 0.1 0.1 0.1 ) .005
//	{
//		fog on
//		map $lightmap`
//		blendFunc GL_dst_color GL_zero
//		rgbgen identity		
//	}
	{ 
		fog on
		map textures/liquids/ocean_m1.tga
		rgbgen wave sin .4 0 0 .5	
		tcmod scale .2 .1
		tcmod scroll .03 .02		
	}	
	{ 
		map textures/liquids/ocean_m1.tga
		blendFunc GL_src_alpha GL_ONE // gl_zero_minus_src_alpha invalid
		alphagen const .5		
		tcmod scale .2 .2
		tcmod scroll -.03 -.02	
	}

// single-pass version, ONLY this pass
//	{ 
//		fog on
//		map textures/liquids/ocean_m1.tga
//		tcmod scale .4 .2
//		tcmod scroll -.01 -.03
//	}	

	
//	{ 
//		map textures/liquids/ocean_m2.tga
//		blendFunc GL_dst_color GL_one
//		rgbgen const ( 0.25 0.25 0.25 )
//		rgbgen identity		
//		tcmod scale -.6 .5
//		tcmod scroll -.01 -.01
//	}
}

venomTrailShader
{
	cull none
	entityMergable
	{
		map sprites/multiplayer/nailtrail.tga
		blendFunc GL_ONE GL_ONE
		rgbGen 	vertex
		alphaGen vertex
	}
}
