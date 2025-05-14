#include "script.h"
#include "natives.h"
#include "types.h"
#include "nativeCaller.h"
#include "enums.h"
#include "main.h"
#include <windows.h>
#pragma comment (lib, "User32.lib")

struct sGuid {
    alignas(8) int data1;
    alignas(8) int data2;
    alignas(8) int data3;
    alignas(8) int data4;
};

bool IsKeyDown(int virtualKey) {
    return (GetAsyncKeyState(virtualKey));
}

void update() {
    Ped playerPed = PLAYER::PLAYER_PED_ID();

    const char* kit_emote_twirl_type = "KIT_EMOTE_TWIRL_GUN";
    Hash emoteHash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(kit_emote_twirl_type));
    const char* spin_type = "SHOULDER_TOSS";
	Hash spin = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(spin_type));

    const char* left_kit_emote_twirl_type = "KIT_EMOTE_TWIRL_GUN_LEFT_HOLSTER";
    Hash leftEmoteHash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(left_kit_emote_twirl_type));

    const char* dual_kit_emote_twirl_type = "KIT_EMOTE_TWIRL_GUN_DUAL";
    Hash dualEmoteHash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(dual_kit_emote_twirl_type));

    sGuid guidRight = {};
    sGuid guidLeft = {};
    bool gotRight = WEAPON::_0x6929E22158E52265(playerPed, 2, (Any*)&guidRight);
    bool gotLeft = WEAPON::_0x6929E22158E52265(playerPed, 3, (Any*)&guidLeft);

    int currentAmmoRight = 0;
    int currentAmmoLeft = 0;
    bool gotAmmoRight = WEAPON::_0x678F00858980F516(playerPed, (Any*)&currentAmmoRight, (Any*)&guidRight);
    bool gotAmmoLeft = WEAPON::_0x678F00858980F516(playerPed, (Any*)&currentAmmoLeft, (Any*)&guidLeft);

    Hash currentWeapon = 0;
    bool getCurrent = WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &currentWeapon, true, 0, false);

	bool dualWield = false;
    if (currentAmmoRight == 0 && currentAmmoLeft == 0) {
        dualWield = true;
    }

    bool isCurrentTwoHanded = WEAPON::_0x0556E9D2ECF39D01(currentWeapon);
    bool isMounted = PED::IS_PED_ON_MOUNT(playerPed);
    bool isStill = AI::IS_PED_STILL(playerPed);
	bool isSprinting = AI::IS_PED_SPRINTING(playerPed);


    if (IsKeyDown(0x54) && isStill && !isSprinting && currentWeapon != 0 && !isCurrentTwoHanded && !isMounted) {

        if (currentAmmoRight == 0 && !dualWield) {
            WEAPON::_0xCBCFFF805F1B4596(playerPed, emoteHash); //function needs to be added to natives.h manually
            AI::_0xB31A277C1AC7B7FF(playerPed, 4, 1, emoteHash, true, false, false, false);
            WEAPON::_0x01F661BB9C71B465(playerPed, 1, spin);
            WEAPON::_SET_GUN_SPINNING_INVENTORY_SLOT_ID_ACTIVATE(playerPed, 0); //function needs to be added to natives.h manually
        }
        else if (currentAmmoLeft == 0 && !dualWield) {
            WEAPON::_0xCBCFFF805F1B4596(playerPed, leftEmoteHash); //function needs to be added to natives.h manually
            AI::_0xB31A277C1AC7B7FF(playerPed, 4, 1, leftEmoteHash, true, false, false, false);
            WEAPON::_0x01F661BB9C71B465(playerPed, 1, spin);
            WEAPON::_SET_GUN_SPINNING_INVENTORY_SLOT_ID_ACTIVATE(playerPed, 0); //function needs to be added to natives.h manually
        }
        else if (dualWield == true) {
            WEAPON::_0xCBCFFF805F1B4596(playerPed, dualEmoteHash); //function needs to be added to natives.h manually
            AI::_0xB31A277C1AC7B7FF(playerPed, 4, 1, dualEmoteHash, true, false, false, false);
            WEAPON::_0x01F661BB9C71B465(playerPed, 1, spin);
            WEAPON::_SET_GUN_SPINNING_INVENTORY_SLOT_ID_ACTIVATE(playerPed, 0); //function needs to be added to natives.h manually
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
