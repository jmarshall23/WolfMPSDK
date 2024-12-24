// DHM
gfx/2d/buildHint
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/buildhint.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

gfx/2d/disarmHint
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/disarmhint.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

gfx/2d/reviveHint
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/revivehint.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

gfx/2d/dynamiteHint
{
	nopicmip
	nomipmaps
	{
		map gfx/2d/dynamitehint.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

sprites/medic_revive
{
	nopicmip
	nocompress
//	nomipmaps
	{
		map sprites/medicrevive.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

sprites/voiceChat
{
	nopicmip
	nocompress
//	nomipmaps
	{
		map sprites/voicechat.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

sprites/voiceMedic
{
	nopicmip
	nocompress
//	nomipmaps
	{
		map sprites/voicemedic.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

sprites/voiceAmmo
{
	nopicmip
	nocompress
//	nomipmaps
	{
		map sprites/voiceammo.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

sprites/voice_yes
{
	nopicmip
	nocompress
//	nomipmaps
	{
		map sprites/voice_yes.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

sprites/voice_no
{
	nopicmip
	nocompress
//	nomipmaps
	{
		map sprites/voice_no.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

sprites/shield
{
	nopicmip
	nocompress
//	nomipmaps
	{
		map sprites/icon_shield.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
	}
}

sprites/skull
{
	nopicmip
	nocompress
	nomipmaps
	{
		map gfx/2d/multi_dead.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

icons/iconw_hammer_1
{
	nopicmip
	nomipmaps
	{
		map icons/iconw_hammer.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

// (selected version)
icons/iconw_hammer_1_select
{
	nopicmip
	nomipmaps
	{
		map icons/iconw_hammer_select.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

icons/iconw_pliers_1
{
	nopicmip
	nomipmaps
	{
		map icons/iconw_pliers_1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

// (selected version)
icons/iconw_pliers_1_select
{
	nopicmip
	nomipmaps
	{
		map icons/iconw_pliers_1_select.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

icons/iconw_ammopack_1
{
	nopicmip
	nomipmaps
	{
		map icons/iconw_ammopack_1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

// (selected version)
icons/iconw_ammopack_1_select
{
	nopicmip
	nomipmaps
	{
		map icons/iconw_ammopack_1_select.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

icons/iconw_medheal_1
{
	nopicmip
	nomipmaps
	{
		map icons/iconw_medheal.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

// (selected version)
icons/iconw_medheal_1_select
{
	nopicmip
	nomipmaps
	{
		map icons/iconw_medheal_select.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

icons/iconw_smokegrenade_1
{
	nopicmip
	nomipmaps
	{
		map icons/iconw_smokegrenade_1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

// (selected version)
icons/iconw_smokegrenade_1_select
{
	nopicmip
	nomipmaps
	{
		map icons/iconw_smokegrenade_1_select.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
	}
}

AlliedHelmet
{
	nopicmip
	nomipmap
	{
		map gfx/2d/lives_allies.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

AxisHelmet
{
	nopicmip
	nomipmap
	{
		map gfx/2d/lives_axis.tga
		blendfunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbgen vertex
	}
}

// ===========================
// Gun fire hitting grass/sand
// ===========================
dirtParticle1
{
	nopicmip
	entityMergable
	{
		map models/weaphits/bullet_sand1.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
		alphaGen vertex
	}
}

dirtParticle2
{
	nopicmip
	entityMergable
	{
		map models/weaphits/bullet_sand2.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
		alphaGen vertex
	}
}

dirtParticle3
{
	nopicmip
	entityMergable
	{
		map models/weaphits/bullet_sand3.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
		alphaGen vertex
	}
}

dirt_splash
{
	nopicmip
	entityMergable
	{
		map models/weaphits/sand_splash.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
		alphaGen vertex
	}
}

water_splash
{
	nopicmip
	entityMergable
	{
		map models/weaphits/water_splash.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
		alphaGen vertex
	}
}