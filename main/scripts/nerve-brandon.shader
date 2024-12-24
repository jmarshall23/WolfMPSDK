// Added by Brandon

textures/skies/km_beachsky
{
	qer_editorimage textures/skies/beachsky_m01
	q3map_lightimage textures/skies/n_blue2.tga
	nocompress
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nodlight
	q3map_globaltexture
	q3map_lightsubdivide 2048
	q3map_sun	0.274632 0.274632 0.39 35 120 45
	q3map_surfacelight 30

	skyparms full 200 -
	
	{
		map textures/skies/newclouds2.tga
	//	tcMod scroll 0.005 0.0005
		tcMod scale 16.0 16.0
		depthWrite
	}	

}

// Added by Brandon

textures/skies/sky_baseout2
{
	nocompress
	qer_editorimage textures/skies/beachsky_m01
	q3map_lightimage textures/skies/n_blue2.tga
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
	q3map_globaltexture
	q3map_lightsubdivide 512 
	q3map_sun	0.274632 0.274632 0.39 30 35 35
	q3map_surfacelight 50
	skyparms full 200 -
	fogvars ( .1 .1 .1 ) .0002
	skyfogvars ( .23 .23 .24 ) .05

	
	{
		map textures/skies/newclouds2.tga
//		tcMod scroll 0.0005 0.0001
		tcMod scale 16.0 16.0
		depthWrite
	}

	{
		map textures/skies/vil_clouds1.tga
		blendfunc blend
		tcMod scroll 0.001 0.00
		tcMod scale 2 1
	}
}

textures/assault_rock/ground_c07nerve
{
	surfaceparm gravelsteps
	{
		map $lightmap
		rgbGen identity
	}
	{
		map textures/assault_rock/ground_c07a.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
	}
}

textures/assault_rock/ground_c09nerve
{
	surfaceparm grasssteps
	{
		map $lightmap
		rgbGen identity		
	}
	{
		map textures/assault_rock/ground_c09a.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity		
	}
}