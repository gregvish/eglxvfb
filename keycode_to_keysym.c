#include <stdio.h>
#include <sys/param.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#include "android_keycode.h"


typedef struct {
    int keycode;
    char keysym[64];
} keycode_xlate_t;


keycode_xlate_t keycode_xlate[] = {
    {KEYCODE_0, "0"},
    {KEYCODE_1, "1"},
    {KEYCODE_2, "2"},
    {KEYCODE_3, "3"},
    {KEYCODE_4, "4"},
    {KEYCODE_5, "5"},
    {KEYCODE_6, "6"},
    {KEYCODE_7, "7"},
    {KEYCODE_8, "8"},
    {KEYCODE_9, "9"},
    {KEYCODE_A, "A"},
    {KEYCODE_ALT_LEFT, "Alt_L"},
    {KEYCODE_ALT_RIGHT, "Alt_R"},
    {KEYCODE_APOSTROPHE, "apostrophe"},
    {KEYCODE_B, "B"},
    {KEYCODE_BACKSLASH, "backslash"},
    {KEYCODE_BREAK, "Break"},
    {KEYCODE_C, "C"},
    {KEYCODE_CAPS_LOCK, "Caps_Lock"},
    {KEYCODE_COMMA, "comma"},
    {KEYCODE_CTRL_LEFT, "Control_L"},
    {KEYCODE_CTRL_RIGHT, "Control_R"},
    {KEYCODE_D, "D"},
    {KEYCODE_DEL, "BackSpace"},
    {KEYCODE_DPAD_DOWN, "Down"},
    {KEYCODE_DPAD_LEFT, "Left"},
    {KEYCODE_DPAD_RIGHT, "Right"},
    {KEYCODE_DPAD_UP, "Up"},
    {KEYCODE_E, "E"},
    {KEYCODE_ENTER, "Return"},
    {KEYCODE_EQUALS, "equal"},
    {KEYCODE_ESCAPE, "Escape"},
    {KEYCODE_F, "F"},
    {KEYCODE_F1, "F1"},
    {KEYCODE_F10, "F10"},
    {KEYCODE_F11, "F11"},
    {KEYCODE_F12, "F12"},
    {KEYCODE_F2, "F2"},
    {KEYCODE_F3, "F3"},
    {KEYCODE_F4, "F4"},
    {KEYCODE_F5, "F5"},
    {KEYCODE_F6, "F6"},
    {KEYCODE_F7, "F7"},
    {KEYCODE_F8, "F8"},
    {KEYCODE_F9, "F9"},
    {KEYCODE_FORWARD_DEL, "Delete"},
    {KEYCODE_FUNCTION, "function"},
    {KEYCODE_G, "G"},
    {KEYCODE_GRAVE, "grave"},
    {KEYCODE_H, "H"},
    {KEYCODE_HELP, "Help"},
    {KEYCODE_HOME, "Home"},
    {KEYCODE_I, "I"},
    {KEYCODE_INSERT, "Insert"},
    {KEYCODE_J, "J"},
    {KEYCODE_K, "K"},
    {KEYCODE_L, "L"},
    {KEYCODE_LEFT_BRACKET, "bracketleft"},
    {KEYCODE_M, "M"},
    {KEYCODE_MENU, "Menu"},
    {KEYCODE_META_LEFT, "Meta_L"},
    {KEYCODE_META_RIGHT, "Meta_R"},
    {KEYCODE_MINUS, "minus"},
    {KEYCODE_MOVE_END, "End"},
    {KEYCODE_MOVE_HOME, "Home"},
    {KEYCODE_N, "N"},
    {KEYCODE_NUM_LOCK, "Num_Lock"},
    {KEYCODE_NUMPAD_0, "KP_0"},
    {KEYCODE_NUMPAD_1, "KP_1"},
    {KEYCODE_NUMPAD_2, "KP_2"},
    {KEYCODE_NUMPAD_3, "KP_3"},
    {KEYCODE_NUMPAD_4, "KP_4"},
    {KEYCODE_NUMPAD_5, "KP_5"},
    {KEYCODE_NUMPAD_6, "KP_6"},
    {KEYCODE_NUMPAD_7, "KP_7"},
    {KEYCODE_NUMPAD_8, "KP_8"},
    {KEYCODE_NUMPAD_9, "KP_9"},
    {KEYCODE_NUMPAD_ADD, "KP_Add"},
    {KEYCODE_NUMPAD_COMMA, "KP_Separator"},
    {KEYCODE_NUMPAD_DIVIDE, "KP_Divide"},
    {KEYCODE_NUMPAD_DOT, "KP_Separator"},
    {KEYCODE_NUMPAD_ENTER, "KP_Enter"},
    {KEYCODE_NUMPAD_EQUALS, "KP_Equal"},
    {KEYCODE_NUMPAD_MULTIPLY, "KP_Multiply"},
    {KEYCODE_NUMPAD_SUBTRACT, "KP_Subtract"},
    {KEYCODE_O, "O"},
    {KEYCODE_P, "P"},
    {KEYCODE_PAGE_DOWN, "Next"},
    {KEYCODE_PAGE_UP, "Prior"},
    {KEYCODE_PERIOD, "period"},
    {KEYCODE_PLUS, "plus"},
    {KEYCODE_POUND, "numbersign"},
    {KEYCODE_Q, "Q"},
    {KEYCODE_R, "R"},
    {KEYCODE_RIGHT_BRACKET, "bracketright"},
    {KEYCODE_S, "S"},
    {KEYCODE_SCROLL_LOCK, "Scroll_Lock"},
    {KEYCODE_SEMICOLON, "semicolon"},
    {KEYCODE_SHIFT_LEFT, "Shift_L"},
    {KEYCODE_SHIFT_RIGHT, "Shift_R"},
    {KEYCODE_SLASH, "slash"},
    {KEYCODE_SPACE, "space"},
    {KEYCODE_STAR, "asterisk"},
    {KEYCODE_SYSRQ, "Sys_Req"},
    {KEYCODE_T, "T"},
    {KEYCODE_TAB, "Tab"},
    {KEYCODE_U, "U"},
    {KEYCODE_V, "V"},
    {KEYCODE_W, "W"},
    {KEYCODE_X, "X"},
    {KEYCODE_Y, "Y"},
    {KEYCODE_Z, "Z"}
};


int main(void)
{
    int i = 0;
    KeySym x_keysym = 0;
    int key_count = 0;
    int lookup_table[300] = {0};

    for (i = 0; i < sizeof(keycode_xlate) / sizeof(keycode_xlate[0]); i += 1) {
        x_keysym = XStringToKeysym(keycode_xlate[i].keysym);
        lookup_table[keycode_xlate[i].keycode] = x_keysym;
    }

    for (i = 0; i < sizeof(lookup_table) / sizeof(lookup_table[0]); i += 1) {
        if (lookup_table[i] != 0)  {
            key_count += 1;
        }
        if (i % 10 == 0) {
            printf("\n");
        }
        printf("0x%04x, ", lookup_table[i]);
    }
    printf("\n// key count: %d\n", key_count);

    return 0;
}
