#pragma once

#define WEAK __declspec(selectany)

namespace game
{
//	Functions
	WEAK symbol<void(errorParm_t, const char*, ...)> Com_Error{0x627380, 0x0};
	WEAK symbol<void(conChannel_t, const char*, ...)> Com_Printf{0x4126C0, 0x0};
	WEAK symbol<void(conChannel_t, const char*, ...)> Com_DPrintf{0x4E3FA0, 0x0};

	WEAK symbol<void(LocalClientNum_t, const char* text)> Cbuf_AddText{0x56EF70, 0x0};
	WEAK symbol<void(LocalClientNum_t, int controllerIndex, const char* text)> Cmd_ExecuteSingleCommand{0x50B470, 0x0};

	WEAK symbol<void(client_s*, svscmd_type, const char*, ...)> SV_SendServerCommand{0x588B10, 0x0};
	WEAK symbol<void(client_s*, const char*)> SV_DelayDropClient{0x4A8DC0, 0x0};
	WEAK symbol<client_s*()> SV_GetPlayerByName{0x875180, 0x0};
	WEAK symbol<client_s*()> SV_GetPlayerByNum{0x875260, 0x0};

	WEAK symbol<bool(netsrc_t, netadr_s, const char*)> NET_OutOfBandPrint{0x560BB0, 0x0};
	WEAK symbol<const char*(netadr_s)> NET_AdrToString {0x49F970, 0x0};

	WEAK symbol<dvar_s*(const char*)> Dvar_FindVar{0x512F70, 0x0};
	WEAK symbol<const char*(const dvar_s*)> Dvar_DisplayableValue{0x681DD0, 0x0};
	WEAK symbol<const dvar_s*(const char*, const char*,
		unsigned __int16, const char*)> Dvar_RegisterString{0x4E3410, 0x0};

	WEAK symbol<void(const char*, void(), cmd_function_t*)> Cmd_AddCommandInternal{0x6AD580, 0x0};
	WEAK symbol<void(const char* cmdName)> Cmd_RemoveCommand{0x527EA0, 0x0};

	WEAK symbol<char*(char*)> I_CleanStr{0x4B0700, 0x0};

	WEAK symbol<char*(int)> ConcatArgs{0x5D5F10, 0x0};

	WEAK symbol<CmdArgs> cmd_args{0x355BD88, 0x0};
	WEAK symbol<int> dvarCount{0x385BE74, 0x0};
	WEAK symbol<dvar_t*> sortedDvars{0x385BE88, 0x0};
	WEAK symbol<gentity_s> g_entities{0x32E5640, 0x0};
} 
