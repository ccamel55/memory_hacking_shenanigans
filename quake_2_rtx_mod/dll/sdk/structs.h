#pragma once

#include <Windows.h>

#define PMF_DUCKED          1
#define PMF_JUMP_HELD       2
#define PMF_ON_GROUND       4
#define PMF_TIME_WATERJUMP  8   // pm_time is waterjump
#define PMF_TIME_LAND       16  // pm_time is time before rejump
#define PMF_TIME_TELEPORT   32  // pm_time is non-moving time
#define PMF_NO_PREDICTION   64  // temporarily disables prediction (used for grappling hook)
#define PMF_TELEPORT_BIT    128 // used by q2pro

#define FL_FLY                  0x00000001
#define FL_SWIM                 0x00000002  // implied immunity to drowining
#define FL_IMMUNE_LASER         0x00000004
#define FL_INWATER              0x00000008
#define FL_GODMODE              0x00000010
#define FL_NOTARGET             0x00000020
#define FL_IMMUNE_SLIME         0x00000040
#define FL_IMMUNE_LAVA          0x00000080
#define FL_PARTIALGROUND        0x00000100  // not all corners are valid
#define FL_WATERJUMP            0x00000200  // player jumping out of water
#define FL_TEAMSLAVE            0x00000400  // not the first on the team
#define FL_NO_KNOCKBACK         0x00000800
#define FL_POWER_ARMOR          0x00001000  // power armor (if any) is active
#define FL_RESPAWN              0x80000000  // used for item respawning

class pmove_state_t {
public:
	char& pm_flags() {
		return *reinterpret_cast<char*>(reinterpret_cast<uint8_t*>(this) + 0x00000010);
	}
private:
	char pad_00[0x1C];
};

class player_state_t {
public:
	pmove_state_t& move() {
		return *reinterpret_cast<pmove_state_t*>(reinterpret_cast<uint8_t*>(this) + 0x00000000);
	}
private:
	char pad_00[0xB8];
};

class gclient_t {
public:
	player_state_t& ps() {
		return *reinterpret_cast<player_state_t*>(reinterpret_cast<uint8_t*>(this) + 0x00000000);
	}
};

class edict_t {
public:
	gclient_t* client() {
		return *reinterpret_cast<gclient_t**>(reinterpret_cast<uint8_t*>(this) + 0x00000058);
	}

	int& health() {
		return *reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(this) + 0x00000254);
	}

	int& max_health() {
		return *reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(this) + 0x00000258);
	}
	
	int& takedamage() {
		return *reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(this) + 0x0000027C);
	}

	int& flags() {
		return *reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(this) + 0x0000011C);
	}

public:
	char& moveFlags() {
		return client()->ps().move().pm_flags();
	}
};

struct usercmd_t {
	
};