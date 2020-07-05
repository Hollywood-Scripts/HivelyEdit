#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <hollywood/plugin.h>

#include "replayer.h"

#include "common/purefuncs.h"
#include "common/version.h"

struct ahxpluginfo
{
	struct hvl_tune *tune;
	UBYTE *buf;
	int buflen;
	UBYTE *mixbuf;
	ULONG samples;
	ULONG streampos;
	int bpf;
	int framelen;
	ULONG buffersmp;
	int subsong;
};

#if defined(HW_AMIGAOS3) || defined(HW_WARPOS)
#define SAVEDS __saveds
#else
#define SAVEDS
#endif

#define MIX_FREQUENCY 44100

hwPluginAPI *hwcl = NULL;
int hwver, hwrev;

// this is a trick to force the optimizer to preserve our version string
static const char plugin_name[] = PLUGIN_NAME "\0$VER: " PLUGIN_MODULENAME ".hwp " PLUGIN_VER_STR " (" PLUGIN_DATE ") [" PLUGIN_PLAT "]";
static const char plugin_modulename[] = PLUGIN_MODULENAME;
static const char plugin_author[] = PLUGIN_AUTHOR;
static const char plugin_description[] = PLUGIN_DESCRIPTION;
static const char plugin_copyright[] = PLUGIN_COPYRIGHT;
static const char plugin_url[] = PLUGIN_URL;
static const char plugin_date[] = PLUGIN_DATE;

static const char *basetable = "ahx";

static int g_playingtime = 60 * 1000 * 10;

#ifdef HW_AMIGA
int initamigastuff(void);
void freeamigastuff(void);
#endif

#ifndef HW_WIN32
#ifndef min
#define   min(a,b)    ((a) <= (b) ? (a) : (b))
#endif
#endif

HW_EXPORT int InitPlugin(hwPluginBase *self, hwPluginAPI *cl, STRPTR path)
{
#ifdef HW_AMIGA
	if(!initamigastuff()) return FALSE;
#endif

	hwver = self->hwVersion;
	hwrev = self->hwRevision;
	
	self->CapsMask = HWPLUG_CAPS_SOUND|HWPLUG_CAPS_LIBRARY|HWPLUG_CAPS_EXTENSION;
	self->Version = PLUGIN_VER;
	self->Revision = PLUGIN_REV;
	self->hwVersion = 5;
#ifdef HW_WARPOS
	self->hwRevision = 3;
#else		
	self->hwRevision = 0;
#endif	
	self->Name = (STRPTR) plugin_name;	
	self->ModuleName = (STRPTR) plugin_modulename;
	self->Author = (STRPTR) plugin_author;
	self->Description = (STRPTR) plugin_description;
	self->Copyright = (STRPTR) plugin_copyright;
	self->URL = (STRPTR) plugin_url;
	self->Date = (STRPTR) plugin_date;
	self->Settings = NULL;
	self->HelpFile = NULL;

	if(cl) {
		
		hwcl = cl;
		
		if(hwver > 5 || (hwver == 5 && hwrev >= 3)) {			
			hwcl->SysBase->hw_RegisterFileType(self, HWFILETYPE_SOUND, "AHX", NULL, "ahx", 0, 0);
			hwcl->SysBase->hw_RegisterFileType(self, HWFILETYPE_SOUND, "Hively Tracker", NULL, "hvl", 0, 0);			
		}
		
		hvl_InitReplayer();
	}
	
	return TRUE;
}

HW_EXPORT void ClosePlugin(void)
{
#ifdef HW_AMIGA
	freeamigastuff();
#endif
}

static const char codecname[] = "AHX/Hively Tracker";

HW_EXPORT STRPTR GetFormatName(APTR handle)
{
	return (STRPTR) codecname;
}

HW_EXPORT void SeekStream(APTR handle, ULONG seekpos)
{	
	struct ahxpluginfo *ap = (struct ahxpluginfo *) handle;

	// seeking is disabled because the replayer doesn't offer any seeking functionality
	// and brute force seeking is too slow --> so we just handle rewind here
	if(!seekpos) {

		struct hvl_tune *newtune;

		if(!(newtune = hvl_reset(ap->buf, ap->buflen, 0, MIX_FREQUENCY))) return;

		hvl_FreeTune(ap->tune);
		ap->tune = newtune;

		hvl_InitSubsong(ap->tune, ap->subsong);

		ap->streampos = 0;
	}

/*
	if(seekpos < ap->streampos) {
		
		struct hvl_tune *newtune;
		
		if(!(newtune = hvl_reset(ap->buf, ap->buflen, 0, MIX_FREQUENCY))) return;
			
		hvl_FreeTune(ap->tune);
		ap->tune = newtune;
			
	  	hvl_InitSubsong(ap->tune, 0);	
	  	
	  	ap->streampos = 0;
	}

	while(ap->streampos < seekpos) {		
		hvl_DecodeFrame(ap->tune, ap->mixbuf, ap->mixbuf + 2, 4);			
		ap->streampos += ap->framelen / 4;
	}
	
	ap->buffersmp = ap->framelen / 4;
*/	
}

HW_EXPORT int StreamSamples(APTR handle, struct StreamSamplesCtrl *ctrl)
{
	struct ahxpluginfo *ap = (struct ahxpluginfo *) handle;
	UBYTE *ptr = ctrl->Buffer;
	ULONG numsmp = ctrl->Request;			
			
	while(numsmp) {
		
		if(ap->buffersmp) {
		
			ULONG m = min(numsmp, ap->buffersmp);
		
			memcpy(ptr, ap->mixbuf, m * 4);
		
			ap->buffersmp -= m;
			numsmp -= m;

			ptr += m * 4;
		
			if(ap->buffersmp) {
			
				UBYTE *src, *dst;
				ULONG k;
			
				src = ap->mixbuf + m * 4;
				dst = ap->mixbuf;
		
				for(k = 0; k < ap->buffersmp; k++) *dst++ = *src++;
			}
		}		
		
		if(numsmp) {	
			hvl_DecodeFrame(ap->tune, ap->mixbuf, ap->mixbuf + 2, 4);			
			ap->buffersmp = ap->framelen / 4;
		}
	}	

	ctrl->Done = FALSE;
	ctrl->Written = ctrl->Request;

	ap->streampos += ctrl->Written;
	
	if(ap->streampos >= ap->samples) {
		ctrl->Done = TRUE;
		ctrl->Written -= (ap->streampos - ap->samples);
		ap->streampos = ap->samples;
	}
		
	return 0;
}

static APTR hwos_Open(STRPTR name, int mode, STRPTR adapter)
{
	if(hwcl->hwVersion >= 6 && adapter) {
		
		struct hwTagList tags[16];
		
		tags[0].Tag = HWFOPENTAG_ADAPTER;
		tags[0].Data.pData = adapter;
		tags[1].Tag = 0;
		
		return hwcl->DOSBase->hw_FOpenExt(name, mode, tags);

	} else {		

		return hwcl->DOSBase->hw_FOpen(name, mode);
	}	
}  

HW_EXPORT APTR OpenStream(STRPTR filename, struct LoadSoundCtrl *ctrl)
{
	struct ahxpluginfo *ap = pure_calloc(sizeof(struct ahxpluginfo), 1);
	APTR fh = NULL;
	UBYTE id[3];
	int subsong = 0;
	STRPTR adapter = NULL;
	
	if(!ap) return NULL;

	if(hwcl->hwVersion >= 6) adapter = ctrl->Adapter;
	
	if(!(fh = hwos_Open(filename, HWFOPENMODE_READ_LEGACY, adapter))) goto error_openstream;
	
	hwos_FRead(fh, id, 3);
	
	if(strncmp(id, "THX", 3) && strncmp(id, "HVL", 3)) goto error_openstream;	
	
	hwos_Seek(fh, 0, HWFSEEKMODE_END);
	ap->buflen = hwos_Seek(fh, 0, HWFSEEKMODE_BEGINNING);
	
	if(!(ap->buf = pure_malloc(ap->buflen))) goto error_openstream;
			
	hwos_FRead(fh, ap->buf, ap->buflen);
	hwos_Close(fh);
	fh = NULL;

	if(!(ap->tune = hvl_reset(ap->buf, ap->buflen, 0, MIX_FREQUENCY))) goto error_openstream;
	
	if(hwver > 5 || (hwver == 5 && hwrev >= 3)) subsong = ctrl->SubSong;	
		
  	if(!hvl_InitSubsong(ap->tune, subsong)) goto error_openstream;
  	
	ap->framelen = (MIX_FREQUENCY * 2 * 2) / 50;		
	ap->samples = (g_playingtime / 1000) * MIX_FREQUENCY;
	ap->subsong = subsong;
	
	if(!(ap->mixbuf = pure_malloc(ap->framelen))) goto error_openstream;
		
	ctrl->Frequency = MIX_FREQUENCY;
	ctrl->Channels = 2;
	ctrl->Bits = 16;
 	ctrl->Samples = ap->samples;
	ctrl->Flags |= HWSNDFLAGS_SIGNEDINT;
#ifndef HW_LITTLE_ENDIAN
	ctrl->Flags |= HWSNDFLAGS_BIGENDIAN;
#endif

	if(hwver > 5 || (hwver == 5 && hwrev >= 3)) ctrl->NumSubSongs = ap->tune->ht_SubsongNr + 1;	
	
	return ap;
	
error_openstream:
	if(fh) hwos_Close(fh);	
		
	if(ap->tune) hvl_FreeTune(ap->tune);		
	if(ap->buf) pure_free(ap->buf);
	pure_free(ap);

	return NULL;	
}

HW_EXPORT void CloseStream(APTR handle)
{
	struct ahxpluginfo *ap = (struct ahxpluginfo *) handle;

	hvl_FreeTune(ap->tune);
	pure_free(ap->mixbuf);
	pure_free(ap->buf);
	pure_free(ap);
}

static SAVEDS int ahx_SetPlayingTime(lua_State *L)
{
	g_playingtime = (int) hwcl->LuaBase->luaL_checknumber(L, 1);

	return 0;
}

static const struct hwCmdStruct plug_commands[] = {
	{"SetPlayingTime", ahx_SetPlayingTime},	
	{NULL, NULL}
};

static const struct hwHelpStruct plug_helpstrings[] = {
	{"ahx.SetPlayingTime(ms) -- set playing duration of music in milliseconds", NULL}, 
};

HW_EXPORT STRPTR GetBaseTable(void)
{
	return (STRPTR) basetable;
}

HW_EXPORT struct hwCmdStruct *GetCommands(void)
{
	return (struct hwCmdStruct *) plug_commands;
}

HW_EXPORT struct hwCstStruct *GetConstants(void)
{
	return NULL;
}

HW_EXPORT struct hwHelpStruct *GetHelpStrings(void)
{
	return (struct hwHelpStruct *) plug_helpstrings;
}

HW_EXPORT int InitLibrary(lua_State *L)
{
	return 0;
}

#if defined(HW_WIN32) && defined(HW_64BIT)
HW_EXPORT void _FreeLibrary(lua_State *L)
#else
HW_EXPORT void FreeLibrary(lua_State *L)
#endif
{
}

HW_EXPORT ULONG GetExtensions(ULONG capbit, struct hwTagList *tags)
{
	if(capbit == HWPLUG_CAPS_LIBRARY) return HWEXT_LIBRARY_HELPSTRINGS;
		
	return 0;
} 
