#include "script.h"
#include "natives.h"
#include "types.h"
#include "nativeCaller.h"
#include "enums.h"

void update() {
    Ped playerPed = PLAYER::PLAYER_PED_ID();
    const char* kit_emote_twirl_type = "KIT_EMOTE_TWIRL_GUN";
    Hash emoteHash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(kit_emote_twirl_type));
    const char* spin_type = "SHOULDER_TOSS";
	Hash spin = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(spin_type));

    if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, 3350541322)) {
            WEAPON::_0xCBCFFF805F1B4596(playerPed, emoteHash);
            AI::_0xB31A277C1AC7B7FF(playerPed, 4, 1, emoteHash, true, false, false, false);
            WEAPON::_0x01F661BB9C71B465(playerPed, 0, spin);
            WEAPON::_SET_GUN_SPINNING_INVENTORY_SLOT_ID_ACTIVATE(playerPed, 0);
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
