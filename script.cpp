#include "script.h"
#include "natives.h"
#include "types.h"
#include "nativeCaller.h"
#include "enums.h"

// Constants

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

    if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, 3350541322)) {

        draw_text("Button A pressed", 900.0f, 500.0f);

        WEAPON::_0xCBCFFF805F1B4596(playerPed, emoteHash);
        AI::_0xB31A277C1AC7B7FF(playerPed, 4, emoteHash, 1, true, false, false, false);
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
