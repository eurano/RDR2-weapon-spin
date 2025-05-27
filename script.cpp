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
    WEAPON::_0x01F661BB9C71B465(ped, 1, spinType);
    WEAPON::_SET_GUN_SPINNING_INVENTORY_SLOT_ID_ACTIVATE(ped, 0);
}

void update() {
    Ped playerPed = PLAYER::PLAYER_PED_ID();

    static DWORD lastSpinTime = 0;
    static bool gunSpinInProgress = false;
    const DWORD spinCooldown = 2000; 

    if (gunSpinInProgress && (GetTickCount() - lastSpinTime >= spinCooldown)) {
        gunSpinInProgress = false;
    }

    Hash emoteHash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>("KIT_EMOTE_TWIRL_GUN"));
    Hash spinType = GAMEPLAY::GET_HASH_KEY(const_cast<char*>("SHOULDER_TOSS"));
    Hash leftEmoteHash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>("KIT_EMOTE_TWIRL_GUN_LEFT_HOLSTER"));
    Hash dualEmoteHash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>("KIT_EMOTE_TWIRL_GUN_DUAL"));

    if (PED::IS_PED_ON_MOUNT(playerPed)) return;
    if (!AI::IS_PED_STILL(playerPed) || AI::IS_PED_SPRINTING(playerPed)) return;

    Hash currentWeapon = 0;
    if (!WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &currentWeapon, true, 0, false)) return;
    if (WEAPON::_0x0556E9D2ECF39D01(currentWeapon)) return;
    if (!WEAPON::_0x705BE297EEBDB95D(currentWeapon)) return;

    sGuid guidRight = {}, guidLeft = {};
    int currentAmmoRight = 0, currentAmmoLeft = 0;

    bool gotRight = WEAPON::_0x6929E22158E52265(playerPed, 2, (Any*)&guidRight);
    bool gotLeft = WEAPON::_0x6929E22158E52265(playerPed, 3, (Any*)&guidLeft);
    bool gotAmmoRight = WEAPON::_0x678F00858980F516(playerPed, (Any*)&currentAmmoRight, (Any*)&guidRight);
    bool gotAmmoLeft = WEAPON::_0x678F00858980F516(playerPed, (Any*)&currentAmmoLeft, (Any*)&guidLeft);

    if (IsKeyDown(0x54) && !gunSpinInProgress) {
        if (currentAmmoRight == 0 && currentAmmoLeft == 0) {
            PlayGunSpinEmote(playerPed, dualEmoteHash, spinType);
            gunSpinInProgress = true;
        }
        else if (currentAmmoRight == 0) {
            PlayGunSpinEmote(playerPed, emoteHash, spinType);
            gunSpinInProgress = true;
        }
        else if (currentAmmoLeft == 0) {
            PlayGunSpinEmote(playerPed, leftEmoteHash, spinType);
            gunSpinInProgress = true;
        }

        lastSpinTime = GetTickCount();
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
