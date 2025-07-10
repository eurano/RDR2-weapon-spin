#include "script.h"
#include <windows.h>
#include "IniOptions.h"

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

static IniOptions iniOptions;
static int keybind = iniOptions.getKeybind();
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

    sGuid guidRight = {}, guidLeft = {}, guidHolsterRight = {}, guidHolsterLeft = {};
    int ammoRight = 0, ammoLeft = 0;
    bool gotRight = WEAPON::_0x6929E22158E52265(playerPed, 0, (Any*)&guidRight);
    bool gotLeft = WEAPON::_0x6929E22158E52265(playerPed, 1, (Any*)&guidLeft);
    bool gotAmmoRight = WEAPON::_0x678F00858980F516(playerPed, (Any*)&ammoRight, (Any*)&guidRight);
    bool gotAmmoLeft = WEAPON::_0x678F00858980F516(playerPed, (Any*)&ammoLeft, (Any*)&guidLeft);

    Hash weaponHashRight = 0;
    Hash weaponHashLeft = 0;

    WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponHashRight, true, 0, false);
    WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponHashLeft, true, 1, false);

    int ammoHolsterRight = 0, ammoHolsterLeft = 0;
    WEAPON::_0x6929E22158E52265(playerPed, 2, (Any*)&guidHolsterRight);
    WEAPON::_0x6929E22158E52265(playerPed, 3, (Any*)&guidHolsterLeft);
    WEAPON::_0x678F00858980F516(playerPed, (Any*)&ammoHolsterRight, (Any*)&guidHolsterRight);
    WEAPON::_0x678F00858980F516(playerPed, (Any*)&ammoHolsterLeft, (Any*)&guidHolsterLeft);

    Hash emoteHash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>("KIT_EMOTE_TWIRL_GUN"));
    Hash spinType = GAMEPLAY::GET_HASH_KEY(const_cast<char*>("REVERSE_SPIN"));
    Hash leftEmoteHash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>("KIT_EMOTE_TWIRL_GUN_LEFT_HOLSTER"));
    Hash dualEmoteHash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>("KIT_EMOTE_TWIRL_GUN_DUAL"));

    if (IsKeyDown(keybind) && !trackingSpin && !PED::IS_PED_RELOADING(playerPed)) {
        if (ammoRight != 0 && ammoLeft != 0 && weaponHashLeft != -1569615261 && weaponHashRight != -1569615261) {
            PlayGunSpinEmote(playerPed, dualEmoteHash, spinType);
            trackingSpin = true;
        }
        else if (ammoHolsterRight == 0 && weaponHashRight != -1569615261) {
            PlayGunSpinEmote(playerPed, emoteHash, spinType);
            trackingSpin = true;
        }
        else if (ammoHolsterLeft == 0 && weaponHashRight != -1569615261) {
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
