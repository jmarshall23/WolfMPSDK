
cin_logo
{
	nopicmip
	nomipmaps
	
	{
		map ui_mp/assets/cin_logo.tga
		blendfunc blend
	}
}




warning
{
	nopicmip
	nomipmaps
	
	{
		map ui_mp/assets/warning.tga
		blendfunc blend
		tcmod scale 8 8 
	}
}


screenwipe
{
	nopicmip
	nomipmaps
	
	{
		map ui_mp/assets/screen_wipe1.tga
		tcmod scroll .1 .1
		blendfunc filter
	}
}





popback
{

	nopicmip
	nomipmaps
        
        {
		clampmap ui_mp/assets/popback.tga          
		blendfunc blend
	}
	{
		clampmap ui_mp/assets/popback.tga
		tcmod rotate 10 
		rgbgen wave sin .5 0 0 0          
		blendfunc add
	}
        
}
centerconsole
{

	nopicmip
	nomipmaps
        
        {
		clampmap textures/sfx/console01.tga          
		tcMod rotate 20
	}
        {
		clampmap textures/sfx/console02.tga
		rgbgen wave sin .5 0 0 0             
		tcMod rotate -60
		blendFunc filter
	}
}


menuback_a
{	
	nopicmip
	nomipmaps	

	{
		map ui_mp/assets/menuback_a.tga
	}
	

	{
		map ui_mp/assets/console02.tga
		blendfunc filter
		tcmod rotate -60
	}
}

menuback_b
{
	
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/menuback_b.tga
		blendfunc add
		rgbgen wave sin .75 0 0 0
		tcmod scroll 0 .2
	}
	{
			map textures/effects2/console01.tga
                	blendFunc Add
                	tcMod scroll -.01  -.02 
                	tcmod scale .02 .01
                	tcmod rotate 3
	}
	
}


menuback_e
{
	nopicmip
	nomipmaps
	{
		clampmap ui_mp/assets/radial.tga
		blendfunc add
		tcmod rotate 100
	}
	{
		clampmap ui_mp/assets/radial.tga
		blendfunc add
		tcmod rotate -100
	}
}
menuback_e3
{
	nopicmip
	nomipmaps
	{
		clampmap ui_mp/assets/radial2.tga
		blendfunc add
		tcmod rotate 200
		tcmod scale .5 .5
		rgbgen wave sin .5 .5 0 10
	}
	{
		clampmap ui_mp/assets/radial2.tga
		blendfunc add
		tcmod rotate -200
		tcmod scale .5 .5
		rgbgen wave sin .5 .5 0 10
	}
	
}
menuback_e4
{
	nopicmip
	nomipmaps
	{
		clampmap ui_mp/assets/radial2.tga
		blendfunc add
		tcmod rotate 200
		rgbgen wave sin .5 .5 0 10
	}
	{
		clampmap ui_mp/assets/radial2.tga
		blendfunc add
		tcmod rotate -200
		rgbgen wave sin .5 .5 0 10
	}
	
}




menuback_c
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/menuback_c.tga
		blendfunc blend
	}


	{
		clampmap ui_mp/assets/menuback_c_light.tga
		blendfunc add
		tcmod rotate -60
	}
	
}


menuback_d
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/teamarena.tga
		blendfunc filter
	}
	{
		map ui_mp/assets/teamarena.tga
		blendfunc add
		rgbgen wave sin .25 .25 0 5
	}
	
}
menuback_f
{	
	
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/menuback_f.tga
		blendfunc blend
	}
	
}
menuback_g
{	
	

	nopicmip
	nomipmaps
	{
		map ui_mp/assets/singleplayer1.tga
		blendfunc blend
	}
	
	
	}


	

lightningkc
	{
	nopicmip
	nomipmaps
	{
		animMap 10 ui_mp/assets/blu01.tga ui_mp/assets/blu02.tga ui_mp/assets/blu03.tga ui_mp/assets/blu04.tga ui_mp/assets/blu05.tga ui_mp/assets/blu06.tga ui_mp/assets/blu07.tga ui_mp/assets/blu08.tga 
		blendFunc add	
		rgbGen wave inverseSawtooth 0 1 0 10	
	}	
	}

lightningkca
	{
	nopicmip
	nomipmaps
	{
		animMap 10 ui_mp/assets/blu01.tga ui_mp/assets/blu02.tga ui_mp/assets/blu03.tga ui_mp/assets/blu04.tga ui_mp/assets/blu05.tga ui_mp/assets/blu06.tga ui_mp/assets/blu07.tga ui_mp/assets/blu08.tga 
		blendFunc add	
		rgbGen wave inverseSawtooth 0 1 0 10
		tcmod swap	
	}	
	}

menu2back
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/background.tga
		rgbgen identity
	}
}

blur
	{
	nopicmip
	nomipmaps
	{
		clampmap ui_mp/assets/blur.tga
		blendfunc add
		rgbgen wave sin .8 .02 0 1
		tcmod stretch sin .9 .05 0 1
		tcmod turb 1 .005 0 5
	}
	}


vs
	{
	nopicmip
	nomipmaps
	{
		clampmap ui_mp/assets/vs.tga
		blendfunc add
		rgbgen wave sin .25 .15 .25 1.5
		tcmod turb 1 .005 0 4.5	
	}
	{
		clampmap ui_mp/assets/vs_alt.tga
		rgbgen wave square .75 .15 0 1 
		tcmod turb 1 .005 .5 4.5	
		blendfunc add
	}
	}



ui_mp/assets/the_fallen_name
	{
	nopicmip
	nomipmaps
	{
		clampmap ui_mp/assets/the_fallen_name.tga
		blendfunc add
		rgbgen wave sin .75 .25 0 2
		tcmod turb 1 .005 0 5	
	}
	{
		map ui_mp/assets/the_fallen_name_alt.tga
		blendfunc add
	}
	}

ui_mp/assets/stroggs_name
	{
	nopicmip
	nomipmaps
	{
		clampmap ui_mp/assets/stroggs_name.tga
		blendfunc add
		rgbgen wave sin .75 .25 0 2
		tcmod turb 1 .005 0 5
	}

	{
		map ui_mp/assets/stroggs_name_alt.tga
		blendfunc add
	}

}


ui_mp/assets/pagans_name
	{

	nopicmip
	nomipmaps
	{
		map ui_mp/assets/pagans_name.tga
		blendfunc add
		rgbgen wave sin .75 .25 0 2
		tcmod turb 1 .005 0 5
	}

	{
		map ui_mp/assets/pagans_name_alt.tga
		blendfunc add
	}
 
}
	
ui_mp/assets/intruders_name
	{

	nopicmip
	nomipmaps
	{
		map ui_mp/assets/intruders_name.tga
		blendfunc add
		rgbgen wave sin .75 .25 0 2
		tcmod turb 1 .005 0 5
	}

	{
		map ui_mp/assets/intruders_name_alt.tga
		blendfunc add
	}
 
}

ui_mp/assets/crusaders_name
	{

	nopicmip
	nomipmaps
	{
		map ui_mp/assets/crusaders_name.tga
		blendfunc add
		rgbgen wave sin .75 .25 0 2
		tcmod turb 1 .005 0 5
	}

	{
		map ui_mp/assets/crusaders_name_alt.tga
		blendfunc add
	}
 
}


		

uibackground4
{
	nopicmip
	nomipmaps
    
       		{	
			map ui_mp/assets/screen02.tga
                	blendFunc GL_ONE GL_ZERO
                	tcMod scroll 7.1  0.2
               		tcmod scale .8 1
			rgbgen wave square .5 .05 0 5
		}
		{
			map ui_mp/assets/background3.tga
			blendfunc add
			//tcmod turb 1 .002 0 5	
		}
				
}
uibackground8
{
	nopicmip
	nomipmaps
    
       		{	
			map ui_mp/assets/background8.tga
                	blendFunc GL_ONE GL_ZERO 
			rgbgen wave sin .5 0 0 0 
		}

				
}



uibackgroundid
{
	nopicmip
	nomipmaps
    
       	
		{
			map ui_mp/assets/backgroundid.tga
		}
		{	
			map ui_mp/assets/screen02.tga
                	blendFunc add
                	tcMod scroll 7.1  0.2
               		tcmod scale .8 1
			rgbgen wave square .25 .05 0 5
		}
						
		}

hudalert
{
	nopicmip
	nomipmaps
    
       		{	map ui_mp/assets/red_box.tga
                	blendFunc GL_ONE GL_ZERO
                	tcMod scroll 7.1  0.2
               		tcmod scale .8 1
			rgbgen wave sin .25 .25 0 1
		}   
	}

hudalert_red
{
	nopicmip
	nomipmaps
    
       		{	map ui_mp/assets/red_box.tga
                	blendFunc GL_ONE GL_ZERO
                	tcMod scroll 7.1  0.2
               		tcmod scale .8 1
			rgbgen wave sin .25 .25 0 1
		}   
	}
hudalert_good
{
	nopicmip
	nomipmaps
    
       		{	map ui_mp/assets/green_box.tga
                	blendFunc GL_ONE GL_ZERO
                	tcMod scroll 7.1  0.2
               		tcmod scale .8 1
			rgbgen wave sin .25 .25 0 1
		}   
	}

cinematicscreen
{
	nopicmip
	nomipmaps
    
       		{	map ui_mp/assets/green_box.tga
                	blendFunc filter
                	
		}   
	}

hudalert_blue
{
	nopicmip
	nomipmaps
    
       		{	map ui_mp/assets/blue_box.tga
                	blendFunc GL_ONE GL_ZERO
                	tcMod scroll 7.1  0.2
               		tcmod scale .8 1
			rgbgen wave sin .25 .25 0 1
		}   
	}

menuscreen
{
	nopicmip
	nomipmaps
    
       	{	
			map textures/sfx2/screen01.tga
                	blendFunc add
                	tcMod scroll 7.1  0.2
               	tcmod scale .8 1
			rgbgen wave square .75 .05 0 5
		}
	{
			map textures/effects2/console01.tga
                	blendFunc Add
                	tcMod scroll -.01  -.02 
                	tcmod scale .02 .01
                	tcmod rotate 3
	}

}

menuscreen2
{
	nopicmip
	nomipmaps
    
       	{	
			map textures/sfx2/screen01.tga
                	blendFunc add
                	tcMod scroll 7  0.2
               	tcmod scale .4 .5
			rgbgen wave square .75 .05 0 5
		}


}

clanlogo
{
	nopicmip
	nomipmaps
    
	{
			animMap 5 ui_mp/assets/pagans.tga ui_mp/assets/stroggs.tga ui_mp/assets/crusaders.tga ui_mp/assets/thefallen.tga ui_mp/assets/intruders.tga 
			blendfunc add
			rgbGen wave inverseSawtooth 0 .25 0 5
			
	}
	{
			animMap 5 ui_mp/assets/intruders.tga ui_mp/assets/pagans.tga ui_mp/assets/stroggs.tga ui_mp/assets/crusaders.tga ui_mp/assets/thefallen.tga  
			blendfunc add
			rgbGen wave Sawtooth 0 .25 0 5
			
	}


}		


playerheads
{
	nopicmip
	nomipmaps
    
	{
			animMap 5 ui_mp/assets/head1.tga ui_mp/assets/head2.tga ui_mp/assets/head3.tga ui_mp/assets/head4.tga ui_mp/assets/head5.tga ui_mp/assets/head6.tga ui_mp/assets/head7.tga ui_mp/assets/head8.tga
			blendfunc add
			rgbGen wave inverseSawtooth 0 .5 0 5
			
	}
	{
			animMap 5 ui_mp/assets/head2.tga ui_mp/assets/head3.tga ui_mp/assets/head4.tga ui_mp/assets/head5.tga ui_mp/assets/head6.tga ui_mp/assets/head7.tga ui_mp/assets/head8.tga ui_mp/assets/head1.tga

			blendfunc add
			rgbGen wave Sawtooth 0 .5 0 5
			
	}


}


ui_mp/assets/pagans_shader1
{
	nopicmip
	nomipmaps
	{
		clampmap ui_mp/assets/pagans.tga
		blendfunc add
		rgbgen wave sin .25 0 0 0
	}
}

ui_mp/assets/mainbanner_shader1
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/mainmenubanner.tga
		blendfunc add
		rgbgen wave sin .05 0 0 0 
		tcmod scroll .02 0
	}
}


ui_mp/assets/clan_shader1
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/chooseclan.tga
		blendfunc add
		rgbgen wave sin .05 0 0 0
		tcmod scroll .02 0
	}
}

ui_mp/assets/player_shader1
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/selectplayer.tga
		blendfunc add
		rgbgen wave sin .05 0 0 0
		tcmod scroll .04 0
	}
}

setup
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/setup.tga
		blendfunc add
		rgbgen wave sin .05 0 0 0
		tcmod scroll .04 0
	}
}


join_server
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/join_server.tga
		blendfunc add
		rgbgen wave sin .05 0 0 0
		tcmod scroll .04 0
	}
}

fight
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/fight.tga
		blendfunc add
		rgbgen wave sin .05 0 0 0
		tcmod scroll .04 0
	}
}



start_server
{
	nopicmip
	nomipmaps
{
		map ui_mp/assets/start_server.tga
		blendfunc add
		rgbgen wave sin .15 0 0 0
		tcmod scroll .04 0
	}
}


ui_mp/assets/controls_shader1
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/controls.tga
		blendfunc add
		rgbgen wave sin .15 0 0 0
		tcmod scroll .02 0
	}
}

playertitle
{	nopicmip
	nomipmaps
	{
		clampmap ui_mp/assets/playertitle.tga
		blendfunc add
		tcmod stretch sin .9 .001 0 10
		rgbgen wave triangle .5 .05 0 1
	}

	
}

/////////////////////////////////// wolf ///////////////////////


menuback_cw
{	
	nopicmip
	nomipmaps	

	{
		map ui_mp/assets/menuback_cw.tga
		tcmod scroll 0.025 0 
	}
	

//	{
//		map ui_mp/assets/console02.tga
//		blendfunc filter
//		tcmod rotate -60
//	}
}

sidebar
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/sidebar.tga
		blendfunc blend
	}


//	{
//		clampmap ui_mp/assets/menuback_c_light.tga
//		blendfunc add
//		tcmod rotate -60
//	}
	
}

sidebar2
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/sidebar2.tga
		blendfunc blend
	}


//	{
//		clampmap ui_mp/assets/menuback_c_light.tga
//		blendfunc add
//		tcmod rotate -60
//	}
	
}

bj1
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/bj1.tga
		blendfunc blend
	}


//	{
//		clampmap ui_mp/assets/menuback_c_light.tga
//		blendfunc add
//		tcmod rotate -60
//	}
	
}

red_bar
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/red_bar.tga
		blendfunc blend
	}
}

WOLFLOGO
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/WOLFLOGO.tga
		blendfunc blend
	}
}

WOLFLOGO2
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/WOLFLOGO2.tga
		blendfunc blend
	}
}

WOLFLOGO3
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/WOLFLOGO3.tga
		blendfunc blend
	}
}

WOLFLOGO4
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/WOLFLOGO4.tga
		blendfunc blend
	}
}

WOLFLOGO4a
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/WOLFLOGO4a.tga
		blendfunc blend
	}
}

backimage2
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/backimage2.tga
		blendfunc blend
	}
}

backimage3
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/backimage3.tga
		blendfunc blend
	}
}

backimage4
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/backimage4.tga
		blendfunc blend
	}
}



bands
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/bands.tga
		blendfunc blend
	//	tcmod stretch sin 1 .2 0 1
		tcmod scroll -0.03 0 
		rgbgen wave sawtooth 2 .7 0 .5
	}

	{
		map ui_mp/assets/bandsa.tga
		blendfunc blend
	//	tcmod stretch sin 1 .1 0 1 
	//	tcmod turb  1 .01 0 .1
		tcmod scroll 0.05 0

	}
}

band_logo
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/band_logo.tga
		blendfunc blend
	//	tcmod stretch sin 1 .2 0 1
		tcmod scroll -0.03 0 
		rgbgen wave sawtooth 2 .7 0 .5
	}

	{
		map ui_mp/assets/band_logo.tga
		blendfunc blend
	//	tcmod stretch sin 1 .1 0 1 
	//	tcmod turb  1 .01 0 .1
		tcmod scroll 0.03 0

	}
}

//bandsa
//{
//	nopicmip
//	nomipmaps
//	{
///		map ui_mp/assets/bandsa.tga
//		blendfunc blend
//	//	tcmod stretch sin 1 .2 0 1
//		tcmod scroll 0.02 0 
//	}
//
//	{
//		map ui_mp/assets/bandsa.tga
//		blendfunc blend
//	//	tcmod stretch sin 1 .1 0 1 
//		tcmod scroll 0.04 0
//	}
//}


band2
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/band2.tga
		blendfunc blend
		rgbgen wave sawtooth 2 .5 0 .5
	}
}

video_over1
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/video_over1.tga
		blendfunc blend
	}
}



//////////////// help menu ////////////////

back_help
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/back_help.tga
		blendfunc blend
	}
}


back_help2
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/back_help2.tga
		blendfunc blend
	}
}

vid_frame
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/vid_frame.tga
		blendfunc blend
	}
}

////////////////////// maxx /////////////////////////

centerconsole2
{

	nopicmip
	nomipmaps
        
//        {
//		clampmap textures/sfx/console01.tga
//		blendfunc add   
//		rgbgen wave sin .15 0 0 0         
//		tcMod rotate 20
//	}
//        {
//		clampmap textures/sfx/console02.tga
//		rgbgen wave sin .5 0 0 0             
//		tcMod rotate -60
//		blendFunc add
//	}


        {
		clampmap ui_mp/assets/menuback_c_light.tga
		rgbgen wave sin .5 0 0 0             
		tcMod rotate -30
		blendFunc add
	}

}


wolf_flag1
{
	
	nopicmip
	nomipmaps
	tesssize 16
//	deformVertexes wave 160 sin 0 10 0 .3	// the chop
	deformVertexes wave 10 sin 0 2 0 .2
	cull twosided

	{
		map ui_mp/assets/wolf_flag1.tga
		blendfunc blend
	}
}

menu_controls
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/menu_controls.tga
		blendfunc blend
	}
}

notebook1
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/notebook1.tga
		blendfunc blend
	}
}

machine1
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/machine1.tga
		blendfunc blend
	}
}

machine2
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/machine2.tga
		blendfunc blend
	}
}

menu_start
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/menu_start.tga
		blendfunc blend
	}
}

menu_load
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/menu_load.tga
		blendfunc blend
	}
}

menu_select
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/menu_select.tga
		blendfunc blend
	}
}
machine_video
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/machine_video.tga
		blendfunc blend
	}
}


machine_video2
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/machine_video2.tga
		blendfunc blend
	}
}

machine_videob
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/machine_videob.tga
		blendfunc blend
	}
}


cross
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/cross.tga
		blendfunc blend
	}
}

cross2
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/cross2.tga
		blendfunc blend
	}
}

pageturn
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/pageturn.tga
		blendfunc blend
	}
}

pageturn2
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/pageturn2.tga
		blendfunc blend
	}
}

bookmark_cover
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/bookmark_cover.tga
		blendfunc blend
	}
}

bookmark_back
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/bookmark_back.tga
		blendfunc blend
	}
}

gfx/clipboard/icon1
{
	nopicmip
	nomipmaps
	{
		map gfx/clipboard/icon1.tga
		blendfunc blend
	}
}

gfx/clipboard/icon2
{
	nopicmip
	nomipmaps
	{
		map gfx/clipboard/icon2.tga
		blendfunc blend
	}
}

gfx/clipboard/icon3
{
	nopicmip
	nomipmaps
	{
		map gfx/clipboard/icon3.tga
		blendfunc blend
	}
}

ui_mp/assets/ingame1
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/ingame1.tga
		blendfunc blend
	}
}

ui_mp/assets/about
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/about.tga
		blendfunc blend
	}
}

ui_mp/assets/load_save
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/load_save.tga
		blendfunc blend
	}
}

ui_mp/assets/controls
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/controls.tga
		blendfunc blend
	}
}
ui_mp/assets/options
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/options.tga
		blendfunc blend
	}
}

ui_mp/assets/top_left
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/top_left.tga
		blendfunc blend
	}
}
ui_mp/assets/top_right
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/top_right.tga
		blendfunc blend
	}
}
ui_mp/assets/bottom_left
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/bottom_left.tga
		blendfunc blend
	}
}
ui_mp/assets/bottom_right
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/bottom_right.tga
		blendfunc blend
	}
}
ui_mp/assets/middle
{
	nopicmip
	nomipmaps
	{
		map ui_mp/assets/middle.tga
		blendfunc blend
	}
}





