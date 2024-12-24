textures/skies/bj_assaultsky

{
//	qer_editorimage textures/skies/topclouds.tga
//	q3map_lightimage textures/skies/n_blue.tga


	qer_editorimage textures/skies/topclouds.tga
	q3map_lightimage textures/skies/n_blue.tga

	nocompress
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nodlight
	q3map_globaltexture
	q3map_lightsubdivide 2048
	q3map_sun	1.0 0.8 0.8 160 295 50
	q3map_surfacelight 40

//	fogvars ( .35 .35 .35 ) .0001
//	skyfogvars ( .25 .25 .255 ) .1
	fogvars ( .25 .25 .255 ) .00002
	skyfogvars ( .23 .23 .24 ) .25

	skyparms full 200 -
	
	{
		map textures/skies/sunset1.tga
	//	tcMod scroll -0.001 -0.003
		tcMod scale 1 1
		depthWrite
	}
	{
		map textures/skies/newclouds4a.tga
		blendfunc blend
		tcMod scroll 0.01 0.00
		tcMod scale 1 1
	}
}