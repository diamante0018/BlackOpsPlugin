#pragma once

namespace game
{
	typedef float vec_t;
	typedef vec_t vec2_t[2];
	typedef vec_t vec3_t[3];
	typedef vec_t vec4_t[4];

	enum FsListBehavior_e
	{
		FS_LIST_PURE_ONLY = 0x0,
		FS_LIST_ALL = 0x1
	};

	enum svscmd_type
	{
		SV_CMD_CAN_IGNORE,
		SV_CMD_RELIABLE
	};

	typedef enum
	{
		ERR_FATAL = 0x0,
		ERR_DROP = 0x1,
		ERR_SERVERDISCONNECT = 0x2,
		ERR_DISCONNECT = 0x3,
		ERR_SCRIPT = 0x4,
		ERR_SCRIPT_DROP = 0x5,
		ERR_LOCALIZATION = 0x6
	} errorParm_t;

	typedef enum
	{
		LOCAL_CLIENT_INVALID = -1,
		LOCAL_CLIENT_0 = 0,
		LOCAL_CLIENT_1 = 1,
		LOCAL_CLIENT_2 = 2,
		LOCAL_CLIENT_3 = 3,
		LOCAL_CLIENT_LAST = 3,
		LOCAL_CLIENT_COUNT = 4
	} LocalClientNum_t;

	typedef enum
	{
		CON_CHANNEL_DONT_FILTER = 0x0,
		CON_CHANNEL_ERROR = 0x1,
		CON_CHANNEL_GAMENOTIFY = 0x2,
		CON_CHANNEL_BOLDGAME = 0x3,
		CON_CHANNEL_SUBTITLE = 0x4,
		CON_CHANNEL_OBITUARY = 0x5,
		CON_CHANNEL_LOGFILEONLY = 0x6,
		CON_CHANNEL_CONSOLEONLY = 0x7,
		CON_CHANNEL_GFX = 0x8,
		CON_CHANNEL_SOUND = 0x9,
		CON_CHANNEL_FILES = 0xA,
		CON_CHANNEL_DEVGUI = 0xB,
		CON_CHANNEL_PROFILE = 0xC,
		CON_CHANNEL_UI = 0xD,
		CON_CHANNEL_CLIENT = 0xE,
		CON_CHANNEL_SERVER = 0xF,
		CON_CHANNEL_SYSTEM = 0x10,
		CON_CHANNEL_PLAYERWEAP = 0x11,
		CON_CHANNEL_AI = 0x12,
		CON_CHANNEL_ANIM = 0x13,
		CON_CHANNEL_PHYS = 0x14,
		CON_CHANNEL_FX = 0x15,
		CON_CHANNEL_LEADERBOARDS = 0x16,
		CON_CHANNEL_LIVE = 0x17,
		CON_CHANNEL_PARSERSCRIPT = 0x18,
		CON_CHANNEL_SCRIPT = 0x19,
		CON_CHANNEL_SPAWNSYSTEM = 0x1A,
		CON_CHANNEL_COOPINFO = 0x1B,
		CON_CHANNEL_SERVERDEMO = 0x1C,
		CON_CHANNEL_DDL = 0x1D,
		CON_CHANNEL_NETWORK = 0x1E,
		CON_CHANNEL_SCHEDULER = 0x1F,
		CON_FIRST_DEBUG_CHANNEL = 0x1F,
		CON_CHANNEL_TASK = 0x20,
		CON_CHANNEL_SPU = 0x21,
		CON_BUILTIN_CHANNEL_COUNT = 0x22
	} conChannel_t;

	typedef enum
	{
		CON_DEST_CONSOLE = 0,
		CON_DEST_MINICON = 1,
		CON_DEST_ERROR = 2,
		CON_DEST_GAME_FIRST = 3,
		CON_DEST_GAME1 = 3,
		CON_DEST_GAME2 = 4,
		CON_DEST_GAME3 = 5,
		CON_DEST_GAME_LAST = 5,
		CON_DEST_COUNT = 6
	} print_msg_dest_t;

	typedef enum
	{
		NS_CLIENT1 = 0x0,
		NS_SERVER = 0x1,
		NS_MAXCLIENTS = 0x1,
		NS_PACKET = 0x2
	} netsrc_t;

	typedef enum
	{
		CS_MOTD = 11,
		CS_TIMESCALE = 14
	} ConfigString;

	typedef enum
	{
		FS_READ = 0,
		FS_WRITE = 1,
		FS_APPEND = 2,
		FS_APPEND_SYNC = 3
	} fsMode_t;

	struct PrintChannel
	{
		char name[32];
		bool allowScript;
	};

	static_assert(sizeof(PrintChannel) == 33);

	struct PrintChannelGlob
	{
		PrintChannel openChannels[256];
		unsigned int filters[6][8];
	};

	struct usercmd_s
	{
		int serverTime;
		int button_bits[2];
		int angles[3];
		unsigned __int16 weapon;
		unsigned __int16 offHandIndex;
		unsigned __int16 lastWeaponAltModeSwitch;
		char forwardmove;
		char rightmove;
		char upmove;
		char pitchmove;
		char yawmove;
		float meleeChargeYaw;
		unsigned char meleeChargeDist;
		float rollmove;
		char selectedLocation[2];
		unsigned char selectedYaw;
	};

	static_assert(sizeof(usercmd_s) == 52);

	struct cmd_function_t
	{
		cmd_function_t* next;
		const char* name;
		const char* autoCompleteDir;
		const char* autoCompleteExt;
		void(__cdecl* function)();
		bool consoleAccess;
	};

	static_assert(sizeof(cmd_function_t) == 24);

	struct CmdArgs
	{
		int nesting;
		int localClientNum[8];
		int controllerIndex[8];
		void* itemDef[8];
		int argshift[8];
		int argc[8];
		const char** argv[8];
		char textPool[8192];
		const char* argvPool[512];
		int usedTextPool[8];
		int totalUsedArgvPool;
		int totalUsedTextPool;
	};

	static_assert(sizeof(CmdArgs) == 10476);

	typedef enum
	{
		NA_BOT = 0x0,
		NA_BAD = 0x1,
		NA_LOOPBACK = 0x2,
		NA_BROADCAST = 0x3,
		NA_IP = 0x4
	} netadrtype_t;

	struct netadr_s // Confirm nigga
	{
		netadrtype_t type;
		unsigned char ip[4];
		unsigned __int16 port;
		int addrHandleIndex;
	};

	static_assert(sizeof(netadr_s) == 16);

	struct msg_t
	{
		int overflowed;
		int readOnly;
		unsigned char* data;
		unsigned char* splitData;
		int maxsize;
		int cursize;
		int splitSize;
		int readcount;
		int bit;
		int lastEntityRef;
		int flush;
		netsrc_t targetLocalNetID;
	};

	static_assert(sizeof(msg_t) == 48);

	typedef enum
	{
		SCRIPTINSTANCE_SERVER,
		SCRIPTINSTANCE_CLIENT,
		SCRIPT_INSTANCE_MAX
	} scriptInstance_t;

	enum dvar_flags : unsigned __int16
	{
		DVAR_FLAG_SAVED = 0x1,
		DVAR_FLAG_WRITEPROTECTED = 0x10,
		DVAR_FLAG_READONLY = 0x40,
		DVAR_FLAG_CHEAT = 0x80,
		DVAR_FLAG_REPLICATED = 0x100,
	};

	typedef enum : char
	{
		DVAR_TYPE_BOOL = 0x0,
		DVAR_TYPE_FLOAT = 0x1,
		DVAR_TYPE_FLOAT_2 = 0x2,
		DVAR_TYPE_FLOAT_3 = 0x3,
		DVAR_TYPE_FLOAT_4 = 0x4,
		DVAR_TYPE_INT = 0x5,
		DVAR_TYPE_ENUM = 0x6,
		DVAR_TYPE_STRING = 0x7,
		DVAR_TYPE_COLOR = 0x8,
		DVAR_TYPE_INT64 = 0x9,
		DVAR_TYPE_LINEAR_COLOR_RGB = 0xA,
		DVAR_TYPE_COLOR_XYZ = 0xB,
		DVAR_TYPE_COUNT = 0xC
	} dvarType_t;

	union DvarValue
	{
		bool enabled;
		int integer;
		unsigned int unsignedInt;
		__int64 integer64;
		unsigned __int64 unsignedInt64;
		float value;
		float vector[4];
		const char* string;
		unsigned char color[4];
	};

	struct enum_limit
	{
		int stringCount;
		const char** strings;
	};

	struct int_limit
	{
		int min;
		int max;
	};

	struct int64_limit
	{
		__int64 min;
		__int64 max;
	};

	struct float_limit
	{
		float min;
		float max;
	};

	union DvarLimits
	{
		enum_limit enumeration;
		int_limit integer;
		float_limit value;
		float_limit vector;
	};

	typedef struct dvar_s
	{
		const char* name;
		const char* description;
		long hash;
		unsigned int flags;
		dvarType_t type;
		bool modified;
		bool loadedFromSaveGame;
		DvarValue current;
		DvarValue latched;
		DvarValue reset;
		DvarValue saved;
		DvarLimits domain;
		dvar_s* hashNext;
		unsigned char pad0[8];
	} dvar_t;

	static_assert(sizeof(dvar_s) == 112);

	enum playerFlag
	{
		PLAYER_FLAG_NOCLIP = 1 << 0,
		PLAYER_FLAG_UFO = 1 << 1,
		PLAYER_FLAG_FROZEN = 1 << 2,
	};

	struct playerState_s
	{
		int commandTime;
		int pm_type;
		int bobCycle;
		int pm_flags;
		int weapFlags;
		int otherFlags;
		int pm_time;
	};

	struct pmove_t
	{
		playerState_s* ps;
	};

	struct gclient_s
	{
		char __pad0[10396];
		int flags;
		char __pad1[320];
	};

	static_assert(sizeof(gclient_s) == 10720);

	enum entityFlag
	{
		FL_GODMODE = 1,
		FL_DEMI_GODMODE = 2,
		FL_NOTARGET = 4,
		FL_NO_KNOCKBACK = 8,
		FL_DROPPED_ITEM = 16,
		FL_NO_BOTS = 32,
		FL_NO_HUMANS = 64,
		FL_TOGGLE = 128,
		FL_SOFTACTIVATE = 256,
		FL_LOW_PRIORITY_USEABLE = 512,
		FL_NO_HEADCHECK = 1024,
		FL_DYNAMICPATH = 2048,
		FL_SUPPORTS_LINKTO = 4096,
		FL_NO_AUTO_ANIM_UPDATE = 8192,
		FL_GRENADE_TOUCH_DAMAGE = 16384,
		FL_GRENADE_MARTYRDOM = 32768,
		FL_MISSILE_DESTABILIZED = 65536,
		FL_STABLE_MISSILES = 131072,
		FL_REPEAT_ANIM_UPDATE = 262144,
		FL_VEHICLE_TARGET = 524288,
		FL_GROUND_ENT = 1048576,
		FL_CURSOR_HINT = 2097152,
		FL_USE_TURRET = 4194304,
		FL_MISSILE_ATTRACTOR = 8388608,
		FL_TARGET = 16777216,
		FL_WEAPON_BEING_GRABBED = 33554432,
		FL_OBSTACLE = 67108864,
		FL_DODGE_LEFT = 134217728,
		FL_DODGE_RIGHT = 268435456,
		FL_BADPLACE_VOLUME = 536870912,
		FL_AUTO_BLOCKPATHS = 1073741824
	};

	enum playerStateFlag
	{
		PMF_PRONE = 0x1,
		PMF_DUCKED = 0x2,
		PMF_MANTLE = 0x4
	};

	struct gentity_s
	{
		int entnum;
		char __pad0[320];
		gclient_s* client;
		char __pad1[44];
		int flags;
		char __pad2[384];
	};

	static_assert(sizeof(gentity_s) == 760);

	struct netProfilePacket_t
	{
		int iTime;
		int iSize;
		int bFragment;
	};

	struct netProfileStream_t
	{
		netProfilePacket_t packets[60];
		int iCurrPacket;
		int iBytesPerSecond;
		int iLastBPSCalcTime;
		int iCountedPackets;
		int iCountedFragments;
		int iFragmentPercentage;
		int iLargestPacket;
		int iSmallestPacket;
	};

	struct netProfileInfo_t
	{
		netProfileStream_t send;
		netProfileStream_t recieve;
	};

	static_assert(sizeof(netProfileInfo_t) == 1504);

	struct netchan_t
	{
		int outgoingSequence;
		netsrc_t sock;
		int dropped;
		int incomingSequence;
		netadr_s remoteAddress;
		int qport;
		int fragmentSequence;
		int fragmentLength;
		unsigned char* fragmentBuffer;
		int fragmentBufferSize;
		int unsentFragments;
		int unsentFragmentStart;
		int unsentLength;
		unsigned char* unsentBuffer;
		int unsentBufferSize;
		int reliable_fragments;
		unsigned char fragment_send_count[128];
		unsigned int fragment_ack[4];
		int lowest_send_count;
		netProfileInfo_t prof;
	};

	static_assert(sizeof(netchan_t) == 1728);

	struct MantleState
	{
		float yaw;
		int timer;
		int transIndex;
		int flags;
	};

	enum ViewLockTypes
	{
		PLAYERVIEWLOCK_NONE = 0,
		PLAYERVIEWLOCK_FULL = 1,
		PLAYERVIEWLOCK_WEAPONJITTER = 2,
		PLAYERVIEWLOCKCOUNT = 3
	};

	typedef enum
	{
		PM_NORMAL = 0,
		PM_NORMAL_LINKED = 1,
		PM_NOCLIP = 2,
		PM_UFO = 3,
		PM_SPECTATOR = 4,
		PM_INTERMISSION = 5,
		PM_LASTSTAND = 6,
		PM_REVIVEE = 7,
		PM_LASTSTAND_TRANSITION = 8,
		PM_DEAD = 9,
		PM_DEAD_LINKED = 10
	} pmtype_t;

	enum clientState_t
	{
		CS_FREE = 0,
		CS_ZOMBIE = 1,
		CS_RECONNECTING = 2,
		CS_CONNECTED = 3,
		CS_CLIENTLOADING = 4,
		CS_ACTIVE = 5
	};

	struct PredictedVehicleInfo
	{
		bool inVehicle;
		vec3_t origin;
		vec3_t angles;
		vec3_t tVel;
		vec3_t aVel;
	};

	static_assert(sizeof(PredictedVehicleInfo) == 52);

	struct clientHeader_t
	{
		clientState_t state;
		int sendAsActive;
		int deltaMessage;
		int rateDealyed;
		netchan_t netchan;
		vec3_t predictedOrigin;
		int predictedOriginServerTime;
		PredictedVehicleInfo vehicle;
	};

	static_assert(sizeof(clientHeader_t) == 1812);

	struct svscmd_info_t
	{
		const char* cmd;
		int time;
		int type;
	};

	struct client_s
	{
		clientHeader_t header;
		const char* dropReason; // 1812
		char userinfo[1024]; // 1816
		char reliableCommandBuffer[16384]; // 2840
		int reliableCommandBufferNext; // 19224
		svscmd_info_t reliableCommandInfo[128]; // 19228
		int reliableSequence; // 20764
		int reliableAcknowledge; // 20768
		int reliableSent; // 20772
		int messageAcknowledge; // 20776
		int gamestateMessageNum; // 20780
		int challenge; // 20784
		usercmd_s lastUsercmd; // 20788
		int lastClientCommand; // 20840
		char lastClientCommandString[1024]; // 20844
		gentity_s* gentity; // 21868
		char name[32]; // 21872
		char clanAbbrev[5]; // 21904
		unsigned __int64 xuid; // 21912
		unsigned char __pad0[0x76D48];
	};

	static_assert(sizeof(client_s) == 0x7C2E8);
}
