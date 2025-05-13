#include "script.h"
#include "natives.h"
#include "types.h"
#include "nativeCaller.h"
#include "enums.h"
#include "main.h"
#include <string>
#include <cstring>


struct sGuid {
    alignas(8) int data1;
    alignas(8) int data2;
    alignas(8) int data3;
    alignas(8) int data4;
};


bool isEmpty(const sGuid& guid) {
    static const sGuid emptyGuid = { 0, 0, 0, 0 };
    return std::memcmp(&guid, &emptyGuid, sizeof(sGuid)) == 0;
}


void draw_text(const char* str, float x, float y) {
    // Normalize x and y coordinates based on screen resolution (assuming 1920x1080)
    float fX = x / 1920.0f;
    float fY = y / 1080.0f;

    // Set text scale (adjust to your preference)
    UI::SET_TEXT_SCALE(0.2f, 0.2f);  // Smaller scale for the text

    // Set text color to white with full opacity
    UI::SET_TEXT_COLOR_RGBA(255, 255, 255, 255);

    // Set text to be centered
    UI::SET_TEXT_CENTRE(1);

    // Disable drop shadow (optional)
    UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);

    // Create a mutable buffer (not a string literal)
    char buffer[256];  // Ensure enough space for the string
    strcpy_s(buffer, sizeof(buffer), "LITERAL_STRING");

    // Pass the mutable buffer instead of the literal
    UI::DRAW_TEXT(
        GAMEPLAY::CREATE_STRING(10, buffer, const_cast<char*>(str)),
        fX,
        fY
    );
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

    // GUIDs for right and left weapons
    sGuid guidRight = {};
    sGuid guidLeft = {};
    bool gotRight = WEAPON::_0x6929E22158E52265(playerPed, 2, (Any*)&guidRight);
    bool gotLeft = WEAPON::_0x6929E22158E52265(playerPed, 3, (Any*)&guidLeft);

    // Ammo values
    int currentAmmoRight = 0;
    int currentAmmoLeft = 0;
    bool gotAmmoRight = WEAPON::_0x678F00858980F516(playerPed, (Any*)&currentAmmoRight, (Any*)&guidRight);
    bool gotAmmoLeft = WEAPON::_0x678F00858980F516(playerPed, (Any*)&currentAmmoLeft, (Any*)&guidLeft);

    Hash currentWeapon = 0;
    bool getCurrent = WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &currentWeapon, true, 0, false);


    //TODO logic for same revolver models
	bool dualWield = false;
    if (currentAmmoRight == 0 && currentAmmoLeft == 0) {
        dualWield = true;
    }

    //TODO is twoHanded>?
    bool isCurrentTwoHanded = false;
    isCurrentTwoHanded = WEAPON::_0x0556E9D2ECF39D01(currentWeapon);

    bool isMounted = false;
    if (PED::IS_PED_ON_MOUNT(playerPed)) {
		isMounted = true;
    }

    bool isStill = AI::IS_PED_STILL(playerPed);
	bool isSprinting = AI::IS_PED_SPRINTING(playerPed);

    // https://learn.microsoft.com/en-us/gaming/gdk/docs/reference/input/xinputongameinput/structs/xinput_keystroke


    char debugOldx[128];
    sprintf_s(debugOldx, sizeof(debugOldx), "Current wepHash: %d", currentWeapon);
    draw_text(debugOldx, 900.0f, 500.0f);

    char debugOldy[128];
    sprintf_s(debugOldy, sizeof(debugOldy), "Left wepHASH : %d", currentAmmoRight);
    draw_text(debugOldy, 900.0f, 520.0f);

    char debugOldc[128];
    sprintf_s(debugOldc, sizeof(debugOldc), "Right wepHASH: %d", currentAmmoLeft);
    draw_text(debugOldc, 900.0f, 540.0f);

    char debugOldv[128];
    sprintf_s(debugOldv, sizeof(debugOldv), "is TwoHanded: %d", isCurrentTwoHanded);
    draw_text(debugOldv, 900.0f, 560.0f);

    char debugOldb[128];
    sprintf_s(debugOldb, sizeof(debugOldb), "is dualWield: %d", dualWield);
    draw_text(debugOldb, 900.0f, 580.0f);

    char debugOldbv[128];
    sprintf_s(debugOldbv, sizeof(debugOldbv), "is mounted: %d", isMounted);
    draw_text(debugOldbv, 900.0f, 600.0f);

    char debugOldn[128];
    sprintf_s(debugOldn, sizeof(debugOldn), "is still: %d", isStill);
    draw_text(debugOldn, 900.0f, 620.0f);

    char debugOldbm[128];
    sprintf_s(debugOldbm, sizeof(debugOldbm), "is sprinting: %d", isSprinting);
    draw_text(debugOldbm, 900.0f, 640.0f);




    if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, 3350541322) && AI::IS_PED_STILL(playerPed) && !AI::IS_PED_SPRINTING(playerPed) && currentWeapon != 0 && !isCurrentTwoHanded && !isMounted) {
        if (currentAmmoRight == 0 && !dualWield) {

            //clear old
            WEAPON::_0x01F661BB9C71B465(playerPed, 0, 0);
            WEAPON::_0x01F661BB9C71B465(playerPed, 1, 0);
            //------------------

            WEAPON::_0xCBCFFF805F1B4596(playerPed, emoteHash);
            AI::_0xB31A277C1AC7B7FF(playerPed, 4, 1, emoteHash, true, false, false, false);
            WEAPON::_0x01F661BB9C71B465(playerPed, 0, spin);
            WEAPON::_SET_GUN_SPINNING_INVENTORY_SLOT_ID_ACTIVATE(playerPed, 0);
        }
        else if (currentAmmoLeft == 0 && !dualWield) {

            //clear old
            //------------------
            WEAPON::_0x01F661BB9C71B465(playerPed, 0, 0);
            WEAPON::_0x01F661BB9C71B465(playerPed, 1, 0);

            WEAPON::_0xCBCFFF805F1B4596(playerPed, leftEmoteHash);
            AI::_0xB31A277C1AC7B7FF(playerPed, 4, 1, leftEmoteHash, true, false, false, false);
            WEAPON::_0x01F661BB9C71B465(playerPed, 1, spin);
            WEAPON::_SET_GUN_SPINNING_INVENTORY_SLOT_ID_ACTIVATE(playerPed, 0);
        }
        else if (dualWield == true) {

            //clear old
            WEAPON::_0x01F661BB9C71B465(playerPed, 0, 0);
            WEAPON::_0x01F661BB9C71B465(playerPed, 1, 0);
            //------------------

            WEAPON::_0xCBCFFF805F1B4596(playerPed, dualEmoteHash);
            AI::_0xB31A277C1AC7B7FF(playerPed, 4, 1, dualEmoteHash, true, false, false, false);
            WEAPON::_0x01F661BB9C71B465(playerPed, 0, spin);
            WEAPON::_SET_GUN_SPINNING_INVENTORY_SLOT_ID_ACTIVATE(playerPed, 0);
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
