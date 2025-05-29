#include "script.h"
#include <windows.h>

struct sGuid {
    alignas(8) int data1;
    alignas(8) int data2;
    alignas(8) int data3;
    alignas(8) int data4;
};

bool IsKeyDown(int virtualKey) {
    return (GetAsyncKeyState(virtualKey));
}

void PlayGunSpinEmote(Ped ped, Hash emote, Hash spinType) {
    WEAPON::_0xCBCFFF805F1B4596(ped, emote);
    AI::_0xB31A277C1AC7B7FF(ped, 4, 1, emote, true, false, false, false);
    WEAPON::_0x01F661BB9C71B465(ped, 0, spinType);
    WEAPON::_SET_GUN_SPINNING_INVENTORY_SLOT_ID_ACTIVATE(ped, 0);
}

void ResetAnimation(Ped ped) {
    WEAPON::_0xCBCFFF805F1B4596(ped, 0);
    WEAPON::_0x01F661BB9C71B465(ped, 0, 0);
}

bool isAnimRunning(Ped ped) {
    return AI::_0xCF9B71C0AF824036(ped, 4);
}

static bool trackingSpin = false;

void update() {
    Ped playerPed = PLAYER::PLAYER_PED_ID();

    if (PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID())) {
        if (trackingSpin) {
            ResetAnimation(playerPed);
            trackingSpin = false;
        }
        return;
    }

    bool currentlyRunning = isAnimRunning(playerPed);

    if (trackingSpin && !currentlyRunning) {
        ResetAnimation(playerPed);
        trackingSpin = false;
    }

    if (PED::IS_PED_ON_MOUNT(playerPed)) return;
    if (!currentlyRunning && (AI::IS_PED_SPRINTING(playerPed) || !AI::IS_PED_STILL(playerPed))) return;

    Hash currentWeapon = 0;
    if (!WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &currentWeapon, true, 0, false)) return;
    if (currentWeapon == 0) return;
    if (!currentlyRunning && !WEAPON::_0x705BE297EEBDB95D(currentWeapon)) return;
    if (!currentlyRunning && WEAPON::_0x0556E9D2ECF39D01(currentWeapon)) return;

    sGuid guidRight = {}, guidLeft = {};
    int ammoRight = 0, ammoLeft = 0;
    WEAPON::_0x6929E22158E52265(playerPed, 2, (Any*)&guidRight);
    WEAPON::_0x6929E22158E52265(playerPed, 3, (Any*)&guidLeft);
    WEAPON::_0x678F00858980F516(playerPed, (Any*)&ammoRight, (Any*)&guidRight);
    WEAPON::_0x678F00858980F516(playerPed, (Any*)&ammoLeft, (Any*)&guidLeft);

    Hash emoteHash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>("KIT_EMOTE_TWIRL_GUN"));
    Hash spinType = GAMEPLAY::GET_HASH_KEY(const_cast<char*>("REVERSE_SPIN"));
    Hash leftEmoteHash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>("KIT_EMOTE_TWIRL_GUN_LEFT_HOLSTER"));
    Hash dualEmoteHash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>("KIT_EMOTE_TWIRL_GUN_DUAL"));

    if (IsKeyDown(0x54) && !trackingSpin && !PED::IS_PED_RELOADING(playerPed)) {
        if (ammoRight == 0 && ammoLeft == 0) {
            PlayGunSpinEmote(playerPed, dualEmoteHash, spinType);
            trackingSpin = true;
        }
        else if (ammoRight == 0) {
            PlayGunSpinEmote(playerPed, emoteHash, spinType);
            trackingSpin = true;
        }
        else if (ammoLeft == 0) {
            PlayGunSpinEmote(playerPed, leftEmoteHash, spinType);
            trackingSpin = true;
        }
    }
}

void main() {
    while (true) {
        update();
        WAIT(0);
    }
}

void ScriptMain() {
    srand(GetTickCount());
    main();
}
