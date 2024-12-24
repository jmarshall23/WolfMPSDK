ui_mp/assets/buy
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/buy.tga
		blendfunc blend
	}
}
ui_mp/assets/limbo_chat_background
{
	nopicmip
	nomipmaps
	nocompress
	{
		clampmap ui_mp/assets/limbo_chat_background.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui_mp/assets/mp_score_objectives
{
	nopicmip
	nomipmaps
	nocompress
	{
		clampmap ui_mp/assets/limbo_chat_background.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}
ui_mp/assets/mp_score_team
{
	nopicmip
	nomipmaps
	nocompress
	{
		clampmap ui_mp/assets/limbo_player_background.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}
ui_mp/assets/mp_line_strip
{
	nopicmip
	nomipmaps
	nocompress
	{
		clampmap ui_mp/assets/mp_line_strip.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}
ui_mp/assets/limbo_player_background
{
	nopicmip
	nomipmaps
	nocompress
	{
		clampmap ui_mp/assets/limbo_player_background.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui_mp/assets/limbo_team_background
{
	nopicmip
	nomipmaps
	nocompress
	{
		clampmap ui_mp/assets/limbo_team_background.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui_mp/assets/limbo_class_background
{
	nopicmip
	nomipmaps
	nocompress
	{
		clampmap ui_mp/assets/limbo_class_background.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui_mp/assets/limbo_weapon_background
{
	nopicmip
	nomipmaps
	nocompress
	{
		clampmap ui_mp/assets/limbo_weapon_background.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui_mp/assets/limbo_score_background
{
	nopicmip
	nomipmaps
	nocompress
	{
		clampmap ui_mp/assets/limbo_score_background.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}
ui_mp/assets/limbo_view_background
{
	nopicmip
	nomipmaps
	nocompress
	{
		clampmap ui_mp/assets/limbo_view_background.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui_mp/assets/team_hilight2
{
	nopicmip
	nomipmaps
	nocompress
	{
		map ui_mp/assets/team_hilight2.tga
		blendfunc GL_DST_COLOR GL_ONE
	}
}

ui_mp/assets/button_glow
{
	nopicmip
	nomipmaps
	nocompress
	{
		map ui_mp/assets/button_glow.tga
		blendfunc GL_DST_COLOR GL_ONE
	}
}

ui_mp/assets/button_glow128
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/button_glow128.tga
		blendfunc GL_DST_COLOR GL_ONE
	}
}

ui_mp/assets/button_glow128_tall
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/button_glow128_tall.tga
		blendfunc GL_DST_COLOR GL_ONE
	}
}

// LAST MODIFIED BY MBJ

textures/skies/rf_wrecked_castlesky_trench
{
	qer_editorimage textures/skies/topclouds.tga
	surfaceparm noimpact
	surfaceparm nolightmap
	q3map_globaltexture
	q3map_lightsubdivide 512 
	q3map_surfacelight 85
	fogvars ( .3 .3 .3 ) 3200
	skyfogvars ( .23 .23 .24 ) .005

	surfaceparm nodlight	// (SA) added so dlights don't affect surface

	skyparms full 200 -
	
	{
		map textures/skies/dimclouds.tga
		tcMod scroll 0.002 0.002
		tcMod scale 1 1
		tcMod turb 0 0.05 0 .08

		depthWrite
	}
	{
		map textures/skies/dimclouds.tga
		blendfunc GL_ONE GL_ONE
		tcMod scroll -0.001 -0.005
		tcMod scale 10 10
	}
}

textures/lights/light_m25blue
{
    //bright white KP light
    q3map_lightimage textures/lights/light_m25blue.tga
    qer_editorimage textures/lights/light_m25blue.tga
    q3map_surfacelight 8000
    light 1
    surfaceparm nomarks
    {
        map $lightmap
        rgbGen identity
    }
    {
        map textures/lights/light_m25blue.tga
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
    }
    {
        map textures/lights/light_m25blue.tga
        blendFunc GL_ONE GL_ONE
    }
}

//models\multiplayer\syringe\syringe
//{	
//	{
//		map models/multiplayer/syringe/syringe_unwrap.tga
//		blendfunc gl_src_alpha gl_one_minus_src_alpha
//		depthwrite		
//	}
//	{
//		map models/multiplayer/syringe/syringe_ref.tga
//		blendfunc gl_src_alpha gl_one_minus_src_alpha
//		//tcgen environment
//	}
//}

// Added by Brandon

textures/skies/km_assaultsky

{
	qer_editorimage textures/skies/topclouds.tga
	q3map_lightimage textures/skies/n_blue.tga

	nocompress
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm nodlight
	q3map_globaltexture
	q3map_lightsubdivide 2048
	q3map_sun	1.0 0.8 0.8 160 295 40
	q3map_surfacelight 40

	skyparms full 200 -
	
	{
		map textures/skies/sunset1.tga
	//	tcMod scroll -0.001 -0.003
		tcMod scale 1 1
		depthWrite
	}
	{
		map textures/skies/assa_clouds1.tga
		blendfunc blend
		tcMod scroll 0.0005 0.00
		tcMod scale 2 1
	}
}

textures/lights/window_josho
{
    //q3map_lightimage textures/miltary_wall/window_m02.blend.tga
    qer_editorimage textures/miltary_wall/window_m02.tga    
    surfaceparm nomarks
    {
        map $lightmap
        rgbGen identity
    }
    {
        map textures/miltary_wall/window_m02.tga
        blendFunc GL_DST_COLOR GL_ZERO
        rgbGen identity
    }
    {
        map textures/miltary_wall/window_m02.blend.tga
        blendFunc GL_ONE GL_ONE
    }
}

ui_mp/assets/quickmessage_background
{
	nopicmip
	nomipmaps
	nocompress
	{
		map ui_mp/assets/quickmessage_background.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui_mp/assets/limbo_spawn_background
{
	nopicmip
	nomipmaps
	nocompress
	{
		clampmap ui_mp/assets/limbo_spawn_background.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui_mp/assets/limbo_menu1
{
	nopicmip
	nomipmaps
	nocompress
	{
		clampmap ui_mp/assets/limbo_menu1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui_mp/assets/button_click
{
	nopicmip
	nomipmaps
	nocompress
	{
		clampmap ui_mp/assets/button_click.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui_mp/assets/button
{
	nopicmip
	nomipmaps
	nocompress
	{
		clampmap ui_mp/assets/button.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui_mp/assets/button_long
{
	nopicmip
	nomipmaps
	nocompress
	{
		clampmap ui_mp/assets/button_long.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui_mp/assets/background
{
	nopicmip
	nomipmaps
	nocompress
	{
		map ui_mp/assets/background.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA				
	}
}

gfx/2d/compass
{
	nopicmip
	nomipmaps
	nocompress
	{
		map gfx/2d/compass.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA				
	}
}

gfx/2d/compass2
{
	nopicmip
	nomipmaps	
	nocompress
	{
		map gfx/2d/compass2.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA				
	}
}

gfx/2d/crosshairb_alt
{
	nopicmip
	//nomipmaps
	nocompress
	{
		map gfx/2d/crosshairb_alt.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA				
	}
}

gfx/2d/crosshaira_alt
{
	nopicmip
	//nomipmaps
	nocompress
	{
		map gfx/2d/crosshaira_alt.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA				
	}
}

gfx/2d/crosshairc_alt
{
	nopicmip
	//nomipmaps
	nocompress
	{
		map gfx/2d/crosshairf.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA				
	}
}

gfx/2d/crosshaird_alt
{
	nopicmip
	//nomipmaps
	nocompress
	{
		map gfx/2d/crosshairf.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA				
	}
}

gfx/2d/crosshaire_alt
{
	nopicmip
	//nomipmaps
	nocompress
	{
		map gfx/2d/crosshairf.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA				
	}
}

gfx/2d/crosshairf_alt
{
	nopicmip
	//nomipmaps
	nocompress
	{
		map gfx/2d/crosshairf_alt.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA				
	}
}

gfx/2d/crosshairg_alt
{
	nopicmip
	//nomipmaps
	nocompress
	{
		map gfx/2d/crosshairg_alt.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA				
	}
}

gfx/2d/crosshairh_alt
{
	nopicmip
	//nomipmaps
	nocompress
	{
		map gfx/2d/crosshairh_alt.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA				
	}
}

gfx/2d/crosshairi_alt
{
	nopicmip
	//nomipmaps
	nocompress
	{
		map gfx/2d/crosshairi_alt.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA				
	}
}

gfx/2d/crosshairj_alt
{
	nopicmip
	//nomipmaps
	nocompress
	{
		map gfx/2d/crosshairj_alt.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA				
	}
}

ui_mp/assets/nerve_logo
{
	nopicmip
	nocompress
	//nomipmaps
	{
		map ui_mp/assets/nerve_logo_fx.tga
		//blendfunc gl_dst_color gl_one
		blendfunc gl_one gl_one
		rgbgen wave sin .5 .5 .75 .05
	}
	{
		map ui_mp/assets/nerve_logo.tga
		//blendfunc gl_dst_color gl_one
		blendfunc gl_one gl_one
		rgbgen wave sin .5 .5 0 .05
	}
}

ui_mp/assets/id_logo
{
	nopicmip
	nocompress
	nomipmaps
	{
		map ui_mp/assets/id_logo_fx.tga
		blendfunc gl_dst_color gl_one
		rgbgen wave sin .5 .5 1 .05
	}
	{
		map ui_mp/assets/id_logo.tga
		blendfunc gl_dst_color gl_one
		rgbgen wave sin .5 .5 .25 .05
	}
}

ui_mp/assets/activision_logo
{
	nopicmip
	nocompress
	nomipmaps
	{
		map ui_mp/assets/activision_logo_fx.tga
		blendfunc gl_dst_color gl_one
		rgbgen wave sin -5 6 0.500 .05
	}
	{
		map ui_mp/assets/activision_logo.tga
		blendfunc gl_dst_color gl_one
		rgbgen wave sin -5 6 0.625 .05
	}
}

ui_mp/assets/gm_logo
{
	nopicmip
	nocompress
	nomipmaps
	{
		map ui_mp/assets/gm_logo_fx.tga
		blendfunc gl_dst_color gl_one
		rgbgen wave sin -5 6 0.750 .05
	}
	{
		map ui_mp/assets/gm_logo.tga
		blendfunc gl_dst_color gl_one
		rgbgen wave sin -5 6 0.875 .05
	}
}

filmnoise
{
	nopicmip
	nocompress
	nomipmaps
	{
		map ui_mp/assets/noise1.tga
//		blendfunc gl_dst_color gl_one
		blendfunc gl_one gl_one
		rgbgen wave inversesawtooth .05 .05 0 200
		tcmod scale 2 2
		tcmod scroll 3 3
	}
	{
		map ui_mp/assets/noise2.tga
		blendfunc gl_dst_color gl_one
		rgbgen wave sawtooth .05 .05 0.75 200
		tcmod scale 2 2
		tcmod scroll -3 -3
	}
}


ui_mp/assets/click_exit
{
	nopicmip
	nocompress
	nomipmaps
	{
		map ui_mp/assets/click_exit.tga
		blendfunc gl_dst_color gl_one	
	}	
}

ui_mp/assets/tab_grad
{
	nopicmip
	nocompress
	{
		clampmap ui_mp/assets/gradient.tga
		blendfunc gl_zero gl_src_color
	}
}

textures/skies/assault_sky_nerve

{
	qer_editorimage textures/skies/sky_m01dmcmp
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
	q3map_globaltexture
	q3map_lightsubdivide 1024 
	q3map_sun	0.274632 0.274632 0.358662 25 0 35
	q3map_surfacelight 120
	skyparms full 200 -
	fogvars ( .26 .26 .31 ) 8000
	
	{
		map textures/skies/dimclouds1.tga	
		tcMod scroll 0.002 0.002
	}
	{
		map textures/skies/dimclouds1.tga
		blendfunc GL_ONE GL_ONE
		tcMod scale 10 10
		tcMod scroll -0.001 -0.005
	}

	
}

///////////////////////// begin crap //////////////////////

textures/terrain/n_terra8_0
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/stone/mxxsnow1.tga
		rgbGen vertex
		tcmod scale 0.05 0.05
	}
}

textures/terrain/n_terra8_1
{
	surfaceparm gravelsteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/stone/mxxsnow1.tga
		rgbGen vertex
		tcmod scale 0.025 0.025
	}
}

textures/terrain/n_terra8_2
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/stone/mxxsnow1.tga
		tcmod scale 0.05 0.05 
		rgbGen vertex
	}
}

textures/terrain/n_terra8_3
{
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/stone/mxxsnow2.tga
		tcmod scale 0.05 0.05 
		rgbGen vertex
	}
}

textures/terrain/n_terra8_4
{
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/stone/mxrock1aa.tga
		tcmod scale 0.1 0.1 
		rgbGen vertex
	}
}

textures/terrain/n_terra8_0to1
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/stone/mxxsnow1.tga	
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.05 0.05 
	}
	{
		map textures/stone/mxxsnow1.tga
		tcmod scale 0.025 0.025
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/terrain/n_terra8_0to2
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/stone/mxxsnow1.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.05 0.05
	}
	{
		map textures/stone/mxxsnow1.tga
		tcmod scale 0.05 0.05 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/terrain/n_terra8_0to3
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/stone/mxxsnow1.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.05 0.05
	}
	{
		map textures/stone/mxxsnow2.tga
		tcmod scale 0.05 0.05 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/terrain/n_terra8_0to4
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/stone/mxxsnow1.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.05 0.05
	}
	{
		map textures/stone/mxrock1aa.tga
		tcmod scale 0.1 0.1 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}


textures/terrain/n_terra8_1to2
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/stone/mxxsnow1.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.1 0.1 
	}
	{
		map textures/stone/mxxsnow1.tga
		tcmod scale 0.05 0.05 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/terrain/n_terra8_1to3
{
	surfaceparm gravelsteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/stone/mxxsnow1.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.1 0.1 
	}
	{
		map textures/stone/mxxsnow2.tga
		tcmod scale 0.05 0.05 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/terrain/n_terra8_1to4
{
	surfaceparm gravelsteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/stone/mxxsnow1.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.1 0.1 
	}
	{
		map textures/stone/mxrock1aa.tga
		tcmod scale 0.1 0.1 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/terrain/n_terra8_2to3
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/stone/mxxsnow1.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.05 0.05 
	}
	{
		map textures/stone/mxxsnow2.tga
		tcmod scale 0.05 0.05 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/terrain/n_terra8_2to4
{
	surfaceparm grasssteps
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/stone/mxxsnow1.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.05 0.05 
	}
	{
		map textures/stone/mxrock1aa.tga
		tcmod scale 0.1 0.1 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}


textures/terrain/n_terra8_3to4
{
	surfaceparm nolightmap
	q3map_novertexshadows
	q3map_forcesunlight	
	{
		map textures/stone/mxxsnow2.tga
		rgbGen vertex
		alphaGen vertex
		tcmod scale 0.05 0.05 
	}
	{
		map textures/stone/mxrock1aa.tga
		tcmod scale 0.1 0.1 
		rgbGen vertex
		alphaGen vertex
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/liquids/mp_sub_water

//Sippy's Test Water
		
	{
		qer_editorimage textures/liquids/pool3d_3f.tga
		qer_trans .5
		q3map_globaltexture
		surfaceparm trans
		surfaceparm nonsolid
		surfaceparm water
		surfaceparm nomarks
		tesssize 512
		cull disable
		deformVertexes wave 64 sin .25 .25 0 .5	

		{ 
			map textures/liquids/pool3d_5e.tga
			blendFunc GL_dst_color GL_one
			rgbgen identity
			tcmod scale .5 .5
			tcmod scroll .025 .01
		}
	
	//	{ 
	//		map textures/liquids/pool3d_3f.tga
	//		blendFunc GL_dst_color gl_one
	//		rgbgen identity
	//		tcmod scale -1 -1
	//		tcmod scroll .025 .025
	//	}	
		{
			map $lightmap
			blendFunc GL_dst_color GL_zero
			rgbgen identity		
		}
	
	
	}

textures/skies/nerve_castlesky
{
	qer_editorimage textures/skies/topclouds.tga
	surfaceparm noimpact
	surfaceparm nolightmap
	q3map_globaltexture
	q3map_lightsubdivide 512 
        //q3map_sun	0.89 0.93 1 85 135 55
	q3map_surfacelight 85
	// fogvars ( .45 .45 .45 ) .0008
	// MBJ fogvars ( .15 .15 .155 ) .0006
	fogvars ( .15 .15 .155 ) .00035
	// skyfogvars ( .23 .23 .24 ) 1000
	skyfogvars ( .23 .23 .24 ) .005

	surfaceparm nodlight	// (SA) added so dlights don't affect surface

	skyparms full 200 -
	
	{
		map textures/skies/dimclouds2-jpw.tga
		tcMod scroll 0.002 0.002
		tcMod scale 9 9
		rgbGen const ( 0.1 0.1 0.1 )
	}
	{
		map textures/skies/dimclouds2-jpw.tga
		blendfunc gl_one gl_one
		tcMod scroll -0.001 -0.005
		tcMod scale 10 10
		rgbGen const ( 0.2 0.2 0.2 )
	}
}

gfx/2d/numbers/slash
{
	nopicmip
	nocompress
	{
		map gfx/2d/numbers/slash.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

dirt_splash2
{
	nopicmip
	nomipmap
	cull none
	entityMergable
	{
		map models/weaphits/sand_splash2.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
		alphaGen vertex
	}
}

sprites/stopwatch
{
	nopicmip
	nomipmap
	{
		map sprites/stopwatch.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

textures/assault_rock/ground_c09b
{
	surfaceparm grasssteps
	{
		map $lightmap
		rgbGen identity		
	}
	{
		map textures/assault_rock/ground_c09b.tga
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity		
	}
}

sprites/destroy
{
	nopicmip	
	nocompress
	{
		map sprites/destroy.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		alphagen wave sin .6 .5 0 .3
	}
}

textures/alpha/keep_lightning
{
		
	cull twosided
	deformVertexes autoSprite2
	qer_editorimage textures/alpha/lightning1.tga	

	{
		animMap 10 textures/alpha/lightning1.tga textures/alpha/lightning1.tga textures/alpha/lightning2.tga textures/alpha/lightning2.tga textures/alpha/lightning3.tga textures/alpha/lightning3.tga 
		
		blendFunc GL_ONE GL_ONE
		rgbGen wave inverseSawtooth -5 6 0 10
		rgbGen identity
	}

}

levelshots/demo_slide
{
	nopicmip
	nocompress
	{
		map levelshots/mp_village.tga
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		alphagen wave sin -4 5 0 .02
	}
	{
		map levelshots/mp_assault.tga
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		alphagen wave sin -4 5 .1428 .02
	}
	{
		map levelshots/mp_sub.tga
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		alphagen wave sin -4 5 .2856 .02
	}
	{
		map levelshots/mp_destruction.tga
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		alphagen wave sin -4 5 .4284 .02
	}
	{
		map levelshots/mp_base.tga
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		alphagen wave sin -4 5 .5712 .02
	}
	{
		map levelshots/mp_castle.tga
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		alphagen wave sin -4 5 .714 .02
	}
	{
		map levelshots/mp_depot.tga
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		alphagen wave sin -4 5 .8568 .02
	}	
}

