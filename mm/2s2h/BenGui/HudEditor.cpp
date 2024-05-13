
#include "HudEditor.h"
#include "macros.h"

extern "C" int16_t OTRGetRectDimensionFromLeftEdge(float v);
extern "C" int16_t OTRGetRectDimensionFromRightEdge(float v);

HudEditorElementID hudEditorActiveElement = HUD_EDITOR_ELEMENT_NONE;

HudEditorElement hudEditorElements[HUD_EDITOR_ELEMENT_MAX] = {
    HUD_EDITOR_ELEMENT(HUD_EDITOR_ELEMENT_B,                 "B Button",          "B",             167, 17,  100, 255, 120, 255),
    HUD_EDITOR_ELEMENT(HUD_EDITOR_ELEMENT_C_LEFT,            "C-Left Button",     "CLeft",         227, 18,  255, 240, 0,   255),
    HUD_EDITOR_ELEMENT(HUD_EDITOR_ELEMENT_C_DOWN,            "C-Down Button",     "CDown",         249, 34,  255, 240, 0,   255),
    HUD_EDITOR_ELEMENT(HUD_EDITOR_ELEMENT_C_RIGHT,           "C-Right Button",    "CRight",        271, 18,  255, 240, 0,   255),
    HUD_EDITOR_ELEMENT(HUD_EDITOR_ELEMENT_A,                 "A Button",          "A",             191, 18,  100, 200, 255, 255),
    HUD_EDITOR_ELEMENT(HUD_EDITOR_ELEMENT_C_UP,              "C-Up Button",       "CUp",           254, 16,  255, 240, 0,   255),
    HUD_EDITOR_ELEMENT(HUD_EDITOR_ELEMENT_D_PAD,             "D-Pad",             "DPad",          271, 55,  255, 255, 255, 255),
    HUD_EDITOR_ELEMENT(HUD_EDITOR_ELEMENT_START,             "Start Button",      "Start",         136, 17,  255, 130, 60,  255),
    HUD_EDITOR_ELEMENT(HUD_EDITOR_ELEMENT_HEARTS,            "Hearts",            "Hearts",        30,  26,  255, 70,  50,  255),
    HUD_EDITOR_ELEMENT(HUD_EDITOR_ELEMENT_MAGIC_METER,       "Magic",             "Magic",         18,  34,  0,   200, 0,   255),
    HUD_EDITOR_ELEMENT(HUD_EDITOR_ELEMENT_TIMERS,            "Timers",            "Timers",        26,  46,  255, 255, 255, 255),
    HUD_EDITOR_ELEMENT(HUD_EDITOR_ELEMENT_TIMERS_MOON_CRASH, "Timer - Skull Kid", "SkullKidTimer", 115, 200, 255, 255, 255, 255),
    HUD_EDITOR_ELEMENT(HUD_EDITOR_ELEMENT_MINIGAME_COUNTER,  "Minigames",         "Minigames",     20,  67,  255, 255, 255, 255),
    HUD_EDITOR_ELEMENT(HUD_EDITOR_ELEMENT_RUPEE_COUNTER,     "Rupees",            "Rupees",        26,  206, 200, 255, 100, 255),
    HUD_EDITOR_ELEMENT(HUD_EDITOR_ELEMENT_KEY_COUNTER,       "Keys",              "Keys",          26,  190, 255, 255, 255, 255),
    HUD_EDITOR_ELEMENT(HUD_EDITOR_ELEMENT_SKULLTULA_COUNTER, "Skulltulas",        "Skulltulas",    26,  190, 255, 255, 255, 255),
};

extern "C" bool HudEditor_ShouldOverrideDraw() {
    return hudEditorActiveElement != HUD_EDITOR_ELEMENT_NONE && CVarGetInteger(hudEditorElements[hudEditorActiveElement].modeCvar, HUD_EDITOR_ELEMENT_MODE_VANILLA) != HUD_EDITOR_ELEMENT_MODE_VANILLA;
}

extern "C" void HudEditor_SetActiveElement(HudEditorElementID id) {
    hudEditorActiveElement = id;
}

extern "C" void HudEditor_ModifyRectLeftRectTopValues(s16* rectLeft, s16* rectTop) {
    s16 offsetFromBaseX = *rectLeft - hudEditorElements[hudEditorActiveElement].defaultX;
    s16 offsetFromBaseY = *rectTop - hudEditorElements[hudEditorActiveElement].defaultY;
    *rectLeft = CVarGetInteger(hudEditorElements[hudEditorActiveElement].xCvar,
                               hudEditorElements[hudEditorActiveElement].defaultX) +
                (offsetFromBaseX * CVarGetFloat(hudEditorElements[hudEditorActiveElement].scaleCvar, 1.0f));
    *rectTop = CVarGetInteger(hudEditorElements[hudEditorActiveElement].yCvar,
                              hudEditorElements[hudEditorActiveElement].defaultY) +
               (offsetFromBaseY * CVarGetFloat(hudEditorElements[hudEditorActiveElement].scaleCvar, 1.0f));

    if (CVarGetInteger(hudEditorElements[hudEditorActiveElement].modeCvar, HUD_EDITOR_ELEMENT_MODE_VANILLA) ==
        HUD_EDITOR_ELEMENT_MODE_MOVABLE_LEFT) {
        *rectLeft = OTRGetRectDimensionFromLeftEdge(*rectLeft);
    } else if (CVarGetInteger(hudEditorElements[hudEditorActiveElement].modeCvar, HUD_EDITOR_ELEMENT_MODE_VANILLA) ==
               HUD_EDITOR_ELEMENT_MODE_MOVABLE_RIGHT) {
        *rectLeft = OTRGetRectDimensionFromRightEdge(*rectLeft);
    }
}

extern "C" void HudEditor_ModifyKaleidoEquipAnimValues(s16* ulx, s16* uly, s16* shrinkRate) {
    // Normalize the kaleido matrix values to screen rectangle dimensions
    *ulx = (*ulx / 10) + (SCREEN_WIDTH / 2);
    *uly = (SCREEN_HEIGHT / 2) - (*uly / 10);

    HudEditor_ModifyRectLeftRectTopValues(ulx, uly);

    // Adjust the values to match the kaleido matrix (origin 0,0 at center of screen, +y going up)
    *ulx -= SCREEN_WIDTH / 2;
    // *uly = SCREEN_HEIGHT - *uly;
    *uly = (SCREEN_HEIGHT / 2) - *uly;

    // Scale by 10 for kaleido animation logic
    *ulx *= 10;
    *uly *= 10;

    float scale = CVarGetFloat(hudEditorElements[hudEditorActiveElement].scaleCvar, 1.0f);
    // 320 is the vanilla start size, and 280 is the vanilla end size (or 160 for dpad)
    // So we apply the scale to 280 and subtract to get the shrink rate
    int16_t endAnimSize = hudEditorActiveElement == HUD_EDITOR_ELEMENT_D_PAD ? 160 : 280;
    *shrinkRate = 320 - (s16)(endAnimSize * scale);
}

extern "C" void HudEditor_ModifyTimerDrawValues(s16 timerStartX, s16 timerStartY, s16* rectLeft, s16* rectTop, s16* rectWidth, s16* rectHeight, s16* dsdx, s16* dtdy) {
    s16 offsetFromBaseX = *rectLeft - timerStartX;
    s16 offsetFromBaseY = *rectTop - timerStartY;
    *rectLeft = timerStartX + (offsetFromBaseX * CVarGetFloat(hudEditorElements[hudEditorActiveElement].scaleCvar, 1.0f));
    *rectTop = timerStartY + (offsetFromBaseY * CVarGetFloat(hudEditorElements[hudEditorActiveElement].scaleCvar, 1.0f));

    *rectWidth *= CVarGetFloat(hudEditorElements[hudEditorActiveElement].scaleCvar, 1.0f);
    *rectHeight *= CVarGetFloat(hudEditorElements[hudEditorActiveElement].scaleCvar, 1.0f);
    *dsdx /= CVarGetFloat(hudEditorElements[hudEditorActiveElement].scaleCvar, 1.0f);
    *dtdy /= CVarGetFloat(hudEditorElements[hudEditorActiveElement].scaleCvar, 1.0f);
}

extern "C" void HudEditor_ModifyDrawValues(s16* rectLeft, s16* rectTop, s16* rectWidth, s16* rectHeight, s16* dsdx, s16* dtdy) {
    HudEditor_ModifyRectLeftRectTopValues(rectLeft, rectTop);

    *rectWidth *= CVarGetFloat(hudEditorElements[hudEditorActiveElement].scaleCvar, 1.0f);
    *rectHeight *= CVarGetFloat(hudEditorElements[hudEditorActiveElement].scaleCvar, 1.0f);
    *dsdx /= CVarGetFloat(hudEditorElements[hudEditorActiveElement].scaleCvar, 1.0f);
    *dtdy /= CVarGetFloat(hudEditorElements[hudEditorActiveElement].scaleCvar, 1.0f);
}

const char* modeNames[] = {
    "Vanilla (4:3)",
    "Hidden",
    "Movable (Align Center)",
    "Movable (Align Left)",
    "Movable (Align Right)",
};

const char* presetNames[] = {
    "Vanilla (4:3)",
    "Hidden",
    "Widescreen",
};

namespace HudEditor {
enum Presets {
    VANILLA,
    HIDDEN,
    WIDESCREEN,
};
};

void HudEditorWindow::DrawElement() {
    ImGui::SetNextWindowSize(ImVec2(480,600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Hud Editor", &mIsVisible, ImGuiWindowFlags_NoFocusOnAppearing)) {
        ImGui::End();
        return;
    }

    static HudEditor::Presets preset = HudEditor::Presets::VANILLA;
    if (UIWidgets::Combobox("Preset", &preset, presetNames)) {
        for (int i = HUD_EDITOR_ELEMENT_B; i < HUD_EDITOR_ELEMENT_MAX; i++) {
            CVarClear(hudEditorElements[i].xCvar);
            CVarClear(hudEditorElements[i].yCvar);
            CVarClear(hudEditorElements[i].scaleCvar);
            CVarClear(hudEditorElements[i].colorCvar);
            CVarClear(hudEditorElements[i].modeCvar);
        }

        switch (preset) {
            case HudEditor::Presets::HIDDEN: {
                for (int i = HUD_EDITOR_ELEMENT_B; i < HUD_EDITOR_ELEMENT_MAX; i++) {
                    CVarSetInteger(hudEditorElements[i].modeCvar, HUD_EDITOR_ELEMENT_MODE_HIDDEN);
                }
                break;
            }
            case HudEditor::Presets::WIDESCREEN: {
                CVarSetInteger(hudEditorElements[HUD_EDITOR_ELEMENT_B].modeCvar, HUD_EDITOR_ELEMENT_MODE_MOVABLE_RIGHT);
                CVarSetInteger(hudEditorElements[HUD_EDITOR_ELEMENT_C_LEFT].modeCvar, HUD_EDITOR_ELEMENT_MODE_MOVABLE_RIGHT);
                CVarSetInteger(hudEditorElements[HUD_EDITOR_ELEMENT_C_DOWN].modeCvar, HUD_EDITOR_ELEMENT_MODE_MOVABLE_RIGHT);
                CVarSetInteger(hudEditorElements[HUD_EDITOR_ELEMENT_C_RIGHT].modeCvar, HUD_EDITOR_ELEMENT_MODE_MOVABLE_RIGHT);
                CVarSetInteger(hudEditorElements[HUD_EDITOR_ELEMENT_A].modeCvar, HUD_EDITOR_ELEMENT_MODE_MOVABLE_RIGHT);
                CVarSetInteger(hudEditorElements[HUD_EDITOR_ELEMENT_C_UP].modeCvar, HUD_EDITOR_ELEMENT_MODE_MOVABLE_RIGHT);
                CVarSetInteger(hudEditorElements[HUD_EDITOR_ELEMENT_D_PAD].modeCvar, HUD_EDITOR_ELEMENT_MODE_MOVABLE_RIGHT);
                CVarSetInteger(hudEditorElements[HUD_EDITOR_ELEMENT_START].modeCvar, HUD_EDITOR_ELEMENT_MODE_MOVABLE_RIGHT);
                CVarSetInteger(hudEditorElements[HUD_EDITOR_ELEMENT_HEARTS].modeCvar, HUD_EDITOR_ELEMENT_MODE_MOVABLE_LEFT);
                CVarSetInteger(hudEditorElements[HUD_EDITOR_ELEMENT_MAGIC_METER].modeCvar, HUD_EDITOR_ELEMENT_MODE_MOVABLE_LEFT);
                CVarSetInteger(hudEditorElements[HUD_EDITOR_ELEMENT_TIMERS].modeCvar, HUD_EDITOR_ELEMENT_MODE_MOVABLE_LEFT);
                CVarSetInteger(hudEditorElements[HUD_EDITOR_ELEMENT_TIMERS_MOON_CRASH].modeCvar, HUD_EDITOR_ELEMENT_MODE_MOVABLE_43);
                CVarSetInteger(hudEditorElements[HUD_EDITOR_ELEMENT_MINIGAME_COUNTER].modeCvar, HUD_EDITOR_ELEMENT_MODE_MOVABLE_LEFT);
                CVarSetInteger(hudEditorElements[HUD_EDITOR_ELEMENT_RUPEE_COUNTER].modeCvar, HUD_EDITOR_ELEMENT_MODE_MOVABLE_LEFT);
                CVarSetInteger(hudEditorElements[HUD_EDITOR_ELEMENT_KEY_COUNTER].modeCvar, HUD_EDITOR_ELEMENT_MODE_MOVABLE_LEFT);
                CVarSetInteger(hudEditorElements[HUD_EDITOR_ELEMENT_SKULLTULA_COUNTER].modeCvar, HUD_EDITOR_ELEMENT_MODE_MOVABLE_LEFT);
                break;
            }
        }
        Ship::Context::GetInstance()->GetWindow()->GetGui()->SaveConsoleVariablesOnNextTick();
    }

    for (int i = HUD_EDITOR_ELEMENT_B; i < HUD_EDITOR_ELEMENT_MAX; i++) {
        ImGui::PushID(hudEditorElements[i].name);
        ImGui::SeparatorText(hudEditorElements[i].name);
        float color[3] = {  (float)hudEditorElements[i].defaultR / 255, (float)hudEditorElements[i].defaultG / 255, (float)hudEditorElements[i].defaultB / 255 };
        // This color picker currently doesn't do anything other than serve as a visual indicator. Eventually it will be used to set the color of the element.
        ImGui::ColorEdit3("Color", color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
        ImGui::SameLine();
        if (UIWidgets::CVarCombobox("Mode", hudEditorElements[i].modeCvar, modeNames, {
            .labelPosition = UIWidgets::LabelPosition::None
        })) {
            CVarClear(hudEditorElements[i].xCvar);
            CVarClear(hudEditorElements[i].yCvar);
            CVarClear(hudEditorElements[i].scaleCvar);
        }
        if (CVarGetInteger(hudEditorElements[i].modeCvar, HUD_EDITOR_ELEMENT_MODE_VANILLA) >= HUD_EDITOR_ELEMENT_MODE_MOVABLE_43) {
            if (ImGui::BeginTable("##table", 3, ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_NoBordersInBody)) {
                ImGui::TableNextColumn();
                UIWidgets::CVarSliderInt("X", hudEditorElements[i].xCvar, -10, 330, hudEditorElements[i].defaultX, {
                    .showButtons = false,
                    .format = "X: %d",
                    .labelPosition = UIWidgets::LabelPosition::None,
                });
                ImGui::TableNextColumn();
                UIWidgets::CVarSliderInt("Y", hudEditorElements[i].yCvar, -10, 250, hudEditorElements[i].defaultY, {
                    .showButtons = false,
                    .format = "Y: %d",
                    .labelPosition = UIWidgets::LabelPosition::None,
                });
                ImGui::TableNextColumn();
                UIWidgets::CVarSliderFloat("Scale", hudEditorElements[i].scaleCvar, 0.25f, 4.0f, 1.0f, {
                    .showButtons = false,
                    .format = "Scale: %.2f",
                    .labelPosition = UIWidgets::LabelPosition::None,
                });
                ImGui::EndTable();
            }
        }
        ImGui::PopID();
    }

    ImGui::End();
}
