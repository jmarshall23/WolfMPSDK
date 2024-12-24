//-----------------------------------------------------------------------------
// MODELS
//-----------------------------------------------------------------------------

models\multiplayer\hedge\hedge
{
	cull twosided
	{
		map models/mapobjects/bush/bush.tga
		alphaFunc GE128
		depthWrite
		rgbgen vertex
	}
}


models\multiplayer\cmarker\shovel
{
        
	{
        	map models/mapobjects/toolshed/shovel_xl.tga
		rgbgen vertex
	}

	{
        	map models/multiplayer/cmarker/shovel_red.tga
		blendFunc	GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		alphaGen	wave sin 0 1 0 .25
	}
}


models\multiplayer\cmarker\wood
{
        
	{
        	map models/multiplayer/cmarker/wood.tga
		rgbgen vertex
	}

	{
        	map models/multiplayer/cmarker/wood_red.tga
		blendFunc	GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		alphaGen	wave sin 0 1 0 .25
	}
}


models\multiplayer\centers\supply\tent2_supply
{

	{
		map models/multiplayer/centers/tent_sky_old.tga
		rgbgen vertex
		tcgen environment
		tcmod scale 1.5 1.5
		tcmod scroll 0 .02
		blendfunc add
	}

	{
		map models/multiplayer/centers/supply/tent2_supply.tga
		rgbgen vertex
		blendFunc	GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		depthwrite
	}
	

}


models\multiplayer\centers\triage\tent2_triage
{

	{
		map models/multiplayer/centers/tent_sky_old.tga
		rgbgen vertex
		tcgen environment
		tcmod scale 1.5 1.5
		tcmod scroll 0 .02
		blendfunc add
	}

	{
		map models/multiplayer/centers/triage/tent2_triage.tga
		rgbgen vertex
		blendFunc	GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		depthwrite
	}
	

}


models\multiplayer\centers\command\tent2_command
{

	{
		map models/multiplayer/centers/tent_sky_old.tga
		rgbgen vertex
		tcgen environment
		tcmod scale 1.5 1.5
		tcmod scroll 0 .02
		blendfunc add
	}

	{
		map models/multiplayer/centers/command/tent2_command.tga
		rgbgen vertex
		blendFunc	GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		depthwrite
	}
	

}


models\multiplayer\centers\tent_flaps
{

	{
		map models/multiplayer/centers/tent_sky_old.tga
		rgbgen vertex
	}
}


models\multiplayer\hammer\hammer
{
        
	{
		map models/multiplayer/hammer/hammer.tga
		rgbgen lightingdiffuse
	}

}


models\multiplayer\spear\spear
{
	{
		map models/multiplayer/spear/spear.tga
	}
}


models\multiplayer\spear\spear_energy
{
	deformVertexes wave 10 sin 0 .1 0 1
	{
		map models/multiplayer/spear/spear_energy.tga
		tcmod scroll 0 .5 
		blendFunc	GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		alphaGen	wave sin 0 .5 0 .15
	}

}

models\multiplayer\syringe\syringe
{
	cull disable
	{
		map models/multiplayer/syringe/fluid.tga
		rgbgen lightingdiffuse
		blendfunc blend
		tcmod scale 4 6
		tcmod scroll 0 -.8
		depthwrite
	}
			
	{
		map models/multiplayer/syringe/syringe.tga
		rgbgen lightingdiffuse
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		depthwrite
	}
	
	{
		map models/multiplayer/syringe/syringe_reflections.tga
		rgbgen lightingdiffuse
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		depthwrite
	}

}


models\multiplayer\syringe\plunger
{
//	cull disable
	{
		map models/multiplayer/syringe/plunger.tga
		rgbgen lightingdiffuse
	}
}


models\multiplayer\treasure\treasure
{
	cull disable
	deformvertexes autosprite
	surfaceparm trans
	sort nearest	
	{
		map models/multiplayer/treasure/treasure.tga
		tcmod stretch sin .8 .08 0 .8
		blendfunc gl_src_alpha gl_one_minus_src_alpha
		//depthwrite
	}
}

models\multiplayer\uboat\sub5
{
	surfaceparm metalsteps
	{
		map models/multiplayer/uboat/sub5.tga
		rgbgen lightingdiffuse
//		rgbgen vertex
	}
}

ui_mp/assets/portraits/allies_win
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/portraits/allies_sky.tga
		tcmod scroll .06 0
	}

	{
		map ui_mp/assets/portraits/allies_win.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui_mp/assets/portraits/axis_win
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/portraits/axis_sky.tga
		tcmod scroll .06 0
	}

	{
		map ui_mp/assets/portraits/axis_win.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui_mp/assets/portraits/allies_win_flag
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/portraits/allies_win_flag.tga
		alphaGen wave sin 0.5 0.2 0 0.2
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

ui_mp/assets/portraits/axis_win_flag
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/portraits/axis_win_flag.tga
		alphaGen wave sin 0.5 0.2 0 0.2
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}


sprites/stopwatch1
{
	nopicmip
	nomipmaps
	{
		map sprites/stopwatch1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}


sprites/stopwatch2
{
	nopicmip
	nomipmaps
	{
		map sprites/stopwatch2.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}