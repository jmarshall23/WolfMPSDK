
///////////////////////////////////////////////////////////////////////////////
// MODELS
///////////////////////////////////////////////////////////////////////////////

models/players/multi/acc/backpack/backpack_lieu
{	
	cull disable
	{
		map models/players/multi/acc/backpack/backpack_lieu.tga
		alphafunc ge128
		rgbGen lightingDiffuse
	}
}
models/players/multi_axis/body_upper_german
{	
	cull disable
	{
		map models/players/multi_axis/body_upper_german.tga
		rgbGen lightingDiffuse
	}
}

models\multiplayer\flagpole\flagpole
{
	{
		map models/multiplayer/flagpole/flag_clouds.tga
		tcgen environment
		rgbGen lightingDiffuse
		//rgbgen identity
	}
}
models\multiplayer\flagpole\american
{	
	deformVertexes wave 194 sin 0 3 0 .4
	cull disable
	{
		map models/multiplayer/flagpole/american.tga
		//rgbGen const ( 0.7 0.7 0.7 )
		rgbGen lightingDiffuse
	}
}
models\multiplayer\flagpole\german
{	
	deformVertexes wave 194 sin 0 3 0 .4
	cull disable
	{
		map models/multiplayer/flagpole/german.tga
		rgbGen lightingDiffuse
	}
}

models\multiplayer\flagpole\american_reinforce
{	
	deformVertexes wave 194 sin 0 3 0 .4
	cull disable
	{
		map models/multiplayer/flagpole/american_reinforce.tga
		rgbGen lightingDiffuse
	}
}
models\multiplayer\flagpole\german_reinforce
{	
	deformVertexes wave 194 sin 0 3 0 .4
	cull disable
	{
		map models/multiplayer/flagpole/german_reinforce.tga
		rgbGen lightingDiffuse
	}
}

models\multiplayer\gold\gold
{	
	{
		map models\multiplayer\gold\gold.tga		
		rgbGen identity
	}
	{
		map textures\effects\chrome4.tga
		blendFunc GL_ONE GL_ONE
		rgbgen wave sin .5 0 0 1		
		tcMod Scale 2 2 
		tcGen environment
	}
//	{
//		map $lightmap
//		blendFunc GL_DST_COLOR GL_ONE_MINUS_DST_ALPHA
//		rgbGen identity
//	}
}

allied_medic
{
	nopicmip
	nomipmaps
	{
		map ui_mp\assets\portraits\medic.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

allied_eng
{
	nopicmip
	nomipmaps
	{
		map ui_mp\assets\portraits\eng.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

allied_soldier
{
	nopicmip
	nomipmaps
	{
		map ui_mp\assets\portraits\soldier.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

allied_lt
{
	nopicmip
	nomipmaps
	{
		map ui_mp\assets\portraits\lt.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

axis_medic
{
	nopicmip	
	nomipmaps
	{
		map ui_mp\assets\portraits\medic_ger.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

axis_lt
{
	nopicmip	
	nomipmaps
	{
		map ui_mp\assets\portraits\LT_ger.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

axis_eng
{
	nopicmip	
	nomipmaps
	{
		map ui_mp\assets\portraits\eng_ger.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

axis_soldier
{
	nopicmip	
	nomipmaps
	{
		map ui_mp\assets\portraits\soldier_ger.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

multi_axisflag
{
	nopicmip	
	nomipmaps
	{
		map ui_mp\assets\portraits\axisflag.tga
	}
	{
		map ui_mp\assets\portraits\wolflogo.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		tcMod scroll 0.125 0
	}
}

multi_alliedflag
{
	nopicmip
	nomipmaps
	{
		map ui_mp\assets\portraits\alliedflag.tga
	}
	{
		map ui_mp\assets\portraits\stars.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		tcMod scroll -.125 -.125
	}
}

multi_spectator
{
	nopicmip	
	nomipmaps
	{
		map ui_mp\assets\portraits\spectator.tga
	}
}

limbo_sten
{
	nopicmip	
	nomipmaps
	{
		map ui_mp\assets\portraits\wep_sten.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

limbo_mauser
{
	nopicmip	
	nomipmaps
	{
		map ui_mp\assets\portraits\wep_mauser.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

limbo_mp40
{
	nopicmip	
	nomipmaps
	{
		map ui_mp\assets\portraits\wep_mp40.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

limbo_flame
{
	nopicmip	
	nomipmaps
	{
		map ui_mp\assets\portraits\wep_flame.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

limbo_panzer
{
	nopicmip	
	nomipmaps
	{
		map ui_mp\assets\portraits\wep_panzer.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

limbo_thompson
{
	nopicmip	
	nomipmaps
	{
		map ui_mp\assets\portraits\wep_thompson.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

limbo_venom
{
	nopicmip	
	nomipmaps
	{
		map ui_mp\assets\portraits\wep_venom.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

limbo_weap_overlay
{
	nopicmip	
	nomipmaps
	{
		map ui_mp\assets\portraits\wep_overlay.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

//TEST STUFF
models\multiplayer\test\alliedflag
{
	nopicmip
	nocompress
	nomipmaps
	{
		map ui_mp\assets\portraits\alliedflag.tga
	}
	{
		map ui_mp\assets\portraits\stars.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		tcMod scroll -.125 -.125
	}
	{
		map ui_mp\assets\portraits\medic.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

models\multiplayer\test\axisflag
{
	nopicmip
	nocompress
	nomipmaps
	{
		map ui_mp\assets\portraits\axisflag.tga
	}
	{
		map ui_mp\assets\portraits\wolflogo.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		tcMod scroll 0.125 0
	}
	{
		map ui_mp\assets\portraits\medic_ger.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}