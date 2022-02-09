#pragma once

#define WEAK __declspec(selectany)

namespace game
{
	WEAK symbol<void(errorParm_t, const char*, ...)> Com_Error{0x627380, 0x651D90};
	WEAK symbol<void(conChannel_t, const char*, ...)> Com_Printf{0x4126C0, 0x43BF30};
	WEAK symbol<void(conChannel_t, const char*, ...)> Com_DPrintf{0x4E3FA0, 0x60F7F0};
	WEAK symbol<void(conChannel_t, const char*, ...)> Com_PrintError{0x568B90, 0x5DFC40};

	WEAK symbol<void(LocalClientNum_t, const char* text)> Cbuf_AddText{0x56EF70, 0x49B930};
	WEAK symbol<void(LocalClientNum_t, const char* text)> Cbuf_InsertText{0x695E10, 0x5B3EB0};
	WEAK symbol<void(LocalClientNum_t, int controllerIndex, const char* text)> Cmd_ExecuteSingleCommand{0x50B470, 0x829AD0};

	WEAK symbol<void(client_s*, svscmd_type, const char*, ...)> SV_SendServerCommand{0x588B10, 0x6106E0};
	WEAK symbol<void(int, svscmd_type, const char*)> SV_GameSendServerCommand{0x6B8730, 0x543CF0};
	WEAK symbol<void(int, char*, int)> SV_Cmd_ArgvBuffer{0x462CB0, 0x4DF5A0};
	WEAK symbol<void(client_s*, const char*)> SV_DelayDropClient{0x4A8DC0, 0x4A2EB0};
	WEAK symbol<client_s*()> SV_GetPlayerByName{0x875180, 0x87C350};
	WEAK symbol<client_s*()> SV_GetPlayerByNum{0x875260, 0x87C430};

	WEAK symbol<bool(netsrc_t, netadr_s, const char*)> NET_OutOfBandPrint{0x560BB0, 0x472850};
	WEAK symbol<const char*(netadr_s)> NET_AdrToString{0x49F970, 0x40D790};

	WEAK symbol<dvar_s*(const char*)> Dvar_FindVar{0x512F70, 0x5AE810};
	WEAK symbol<const char*(const dvar_s*)> Dvar_DisplayableValue{0x681DD0, 0x5B56F0};
	WEAK symbol<const char*(const dvar_s*)> Dvar_DisplayableLatchedValue{0x4AE1A0, 0x675850};
	WEAK symbol<const dvar_s*(const char*, const char*,
		unsigned __int16, const char*)> Dvar_RegisterString{0x4E3410, 0x59B3B0};
	WEAK symbol<const dvar_s*(const char*, float, float,
		float, unsigned __int16, const char*)> Dvar_RegisterFloat{0x670020, 0x679020};
	WEAK symbol<const dvar_s*(const char*, bool,
		unsigned __int16, const char*)> Dvar_RegisterBool{0x5A5350, 0x45BB20};
	WEAK symbol<const dvar_s*(const char*, int, int,
		int, unsigned __int16, const char*)> Dvar_RegisterInt{0x58D900, 0x651910};

	WEAK symbol<void(const char*, void(), cmd_function_t*)> Cmd_AddCommandInternal{0x6AD580, 0x661400};
	WEAK symbol<void(const char* cmdName)> Cmd_RemoveCommand{0x527EA0, 0x5F1A90};
	WEAK symbol<cmd_function_t*(const char*)> Cmd_FindCommand{0x445B60, 0x479DD0};

	WEAK symbol<char*(char*)> I_CleanStr{0x4B0700, 0x0};

	WEAK symbol<char*(int)> ConcatArgs{0x5D5F10, 0x4FB210};
	WEAK symbol<void(int)> ClientCommand{0x63DB70, 0x4AF770};
	WEAK symbol<void(gentity_s*, gentity_s*, int, const char*)> G_Say{0x51BBD0, 0x49A790};

	WEAK symbol<void(gentity_s*, unsigned __int16, unsigned int)> Scr_Notify{0x458D30, 0x0};
	WEAK symbol<void(int, scriptInstance_t)> Scr_AddInt{0x49F830, 0x0};
	WEAK symbol<void(const float*, scriptInstance_t)> Scr_AddVector{0x532EF0, 0x0};

	WEAK symbol<int(const playerState_s*)> PM_GetEffectiveStance{0x659590, 0x0};

	WEAK symbol<CmdArgs> sv_cmd_args{0x355BD88, 0x243D208};
	WEAK symbol<int> dvarCount{0x385BE74, 0x261CBD4};
	WEAK symbol<dvar_t*> sortedDvars{0x385BE88, 0x261CBE8};
	WEAK symbol<client_s> svs_clients{0x372D11C, 0x286D01C};
	WEAK symbol<gentity_s> g_entities{0x32E5640, 0x1A796F8};
	WEAK symbol<int> level_time{0x3443F4C, 0xC2078C};
} 
