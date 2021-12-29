#pragma once

#define WEAK __declspec(selectany)

namespace game
{
//	Functions
	WEAK symbol<void(errorParm_t, const char*, ...)> Com_Error{0x627380, 0x0};
	WEAK symbol<void(conChannel_t, const char*, ...)> Com_Printf{0x4126C0, 0x0};

	WEAK symbol<void(LocalClientNum_t, const char* text)> Cbuf_AddText{0x56EF70, 0x0};
	WEAK symbol<void(LocalClientNum_t, int controllerIndex, const char* text)> Cmd_ExecuteSingleCommand{0x50B470, 0x0};

	WEAK symbol<dvar_s*(const char*)> Dvar_FindVar{0x512F70, 0x0};
	WEAK symbol<const char*(const dvar_s*)> Dvar_DisplayableValue{0x681DD0, 0x0};

	WEAK symbol<void(const char*, void(), cmd_function_t*)> Cmd_AddCommandInternal{0x6AD580, 0x0};
	WEAK symbol<void(const char* cmdName)> Cmd_RemoveCommand{0x527EA0, 0x0};

	WEAK symbol<char*(int)> ConcatArgs{0x5D5F10, 0x0};

	WEAK symbol<CmdArgs> cmd_args{0x355BD88, 0x0};
	WEAK symbol<int> dvarCount{0x385BE74, 0x0};
	WEAK symbol<dvar_t*> sortedDvars{0x385BE88, 0x0};
	WEAK symbol<gentity_s> g_entities{0x32E5640, 0x0};
} 
