/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __KEYCODE_H__
#define __KEYCODE_H__


/** Unknown key code. */
static const int KEYCODE_UNKNOWN = 0;
/** Soft Left key.
 * Usually situated below the display on phones and used as a multi-function
 * feature key for selecting a software defined function shown on the bottom left
 * of the display. */
static const int KEYCODE_SOFT_LEFT = 1;
/** Soft Right key.
 * Usually situated below the display on phones and used as a multi-function
 * feature key for selecting a software defined function shown on the bottom right
 * of the display. */
static const int KEYCODE_SOFT_RIGHT = 2;
/** Home key.
 * This key is handled by the framework and is never delivered to applications. */
static const int KEYCODE_HOME = 3;
/** Back key. */
static const int KEYCODE_BACK = 4;
/** Call key. */
static const int KEYCODE_CALL = 5;
/** End Call key. */
static const int KEYCODE_ENDCALL = 6;
/** '0' key. */
static const int KEYCODE_0 = 7;
/** '1' key. */
static const int KEYCODE_1 = 8;
/** '2' key. */
static const int KEYCODE_2 = 9;
/** '3' key. */
static const int KEYCODE_3 = 10;
/** '4' key. */
static const int KEYCODE_4 = 11;
/** '5' key. */
static const int KEYCODE_5 = 12;
/** '6' key. */
static const int KEYCODE_6 = 13;
/** '7' key. */
static const int KEYCODE_7 = 14;
/** '8' key. */
static const int KEYCODE_8 = 15;
/** '9' key. */
static const int KEYCODE_9 = 16;
/** '*' key. */
static const int KEYCODE_STAR = 17;
/** '#' key. */
static const int KEYCODE_POUND = 18;
/** Directional Pad Up key.
 * May also be synthesized from trackball motions. */
static const int KEYCODE_DPAD_UP = 19;
/** Directional Pad Down key.
 * May also be synthesized from trackball motions. */
static const int KEYCODE_DPAD_DOWN = 20;
/** Directional Pad Left key.
 * May also be synthesized from trackball motions. */
static const int KEYCODE_DPAD_LEFT = 21;
/** Directional Pad Right key.
 * May also be synthesized from trackball motions. */
static const int KEYCODE_DPAD_RIGHT = 22;
/** Directional Pad Center key.
 * May also be synthesized from trackball motions. */
static const int KEYCODE_DPAD_CENTER = 23;
/** Volume Up key.
 * Adjusts the speaker volume up. */
static const int KEYCODE_VOLUME_UP = 24;
/** Volume Down key.
 * Adjusts the speaker volume down. */
static const int KEYCODE_VOLUME_DOWN = 25;
/** Power key. */
static const int KEYCODE_POWER = 26;
/** Camera key.
 * Used to launch a camera application or take pictures. */
static const int KEYCODE_CAMERA = 27;
/** Clear key. */
static const int KEYCODE_CLEAR = 28;
/** 'A' key. */
static const int KEYCODE_A = 29;
/** 'B' key. */
static const int KEYCODE_B = 30;
/** 'C' key. */
static const int KEYCODE_C = 31;
/** 'D' key. */
static const int KEYCODE_D = 32;
/** 'E' key. */
static const int KEYCODE_E = 33;
/** 'F' key. */
static const int KEYCODE_F = 34;
/** 'G' key. */
static const int KEYCODE_G = 35;
/** 'H' key. */
static const int KEYCODE_H = 36;
/** 'I' key. */
static const int KEYCODE_I = 37;
/** 'J' key. */
static const int KEYCODE_J = 38;
/** 'K' key. */
static const int KEYCODE_K = 39;
/** 'L' key. */
static const int KEYCODE_L = 40;
/** 'M' key. */
static const int KEYCODE_M = 41;
/** 'N' key. */
static const int KEYCODE_N = 42;
/** 'O' key. */
static const int KEYCODE_O = 43;
/** 'P' key. */
static const int KEYCODE_P = 44;
/** 'Q' key. */
static const int KEYCODE_Q = 45;
/** 'R' key. */
static const int KEYCODE_R = 46;
/** 'S' key. */
static const int KEYCODE_S = 47;
/** 'T' key. */
static const int KEYCODE_T = 48;
/** 'U' key. */
static const int KEYCODE_U = 49;
/** 'V' key. */
static const int KEYCODE_V = 50;
/** 'W' key. */
static const int KEYCODE_W = 51;
/** 'X' key. */
static const int KEYCODE_X = 52;
/** 'Y' key. */
static const int KEYCODE_Y = 53;
/** 'Z' key. */
static const int KEYCODE_Z = 54;
/** ',' key. */
static const int KEYCODE_COMMA = 55;
/** '.' key. */
static const int KEYCODE_PERIOD = 56;
/** Left Alt modifier key. */
static const int KEYCODE_ALT_LEFT = 57;
/** Right Alt modifier key. */
static const int KEYCODE_ALT_RIGHT = 58;
/** Left Shift modifier key. */
static const int KEYCODE_SHIFT_LEFT = 59;
/** Right Shift modifier key. */
static const int KEYCODE_SHIFT_RIGHT = 60;
/** Tab key. */
static const int KEYCODE_TAB = 61;
/** Space key. */
static const int KEYCODE_SPACE = 62;
/** Symbol modifier key.
 * Used to enter alternate symbols. */
static const int KEYCODE_SYM = 63;
/** Explorer special function key.
 * Used to launch a browser application. */
static const int KEYCODE_EXPLORER = 64;
/** Envelope special function key.
 * Used to launch a mail application. */
static const int KEYCODE_ENVELOPE = 65;
/** Enter key. */
static const int KEYCODE_ENTER = 66;
/** Backspace key.
 * Deletes characters before the insertion point, unlike {@link #KEYCODE_FORWARD_DEL}. */
static const int KEYCODE_DEL = 67;
/** '`' (backtick) key. */
static const int KEYCODE_GRAVE = 68;
/** '-'. */
static const int KEYCODE_MINUS = 69;
/** '=' key. */
static const int KEYCODE_EQUALS = 70;
/** '[' key. */
static const int KEYCODE_LEFT_BRACKET = 71;
/** ']' key. */
static const int KEYCODE_RIGHT_BRACKET = 72;
/** '\' key. */
static const int KEYCODE_BACKSLASH = 73;
/** ';' key. */
static const int KEYCODE_SEMICOLON = 74;
/** ''' (apostrophe) key. */
static const int KEYCODE_APOSTROPHE = 75;
/** '/' key. */
static const int KEYCODE_SLASH = 76;
/** '@' key. */
static const int KEYCODE_AT = 77;
/** Number modifier key.
 * Used to enter numeric symbols.
 * This key is not Num Lock; it is more like {@link #KEYCODE_ALT_LEFT} and is
 * interpreted as an ALT key by {@link android.text.method.MetaKeyKeyListener}. */
static const int KEYCODE_NUM = 78;
/** Headset Hook key.
 * Used to hang up calls and stop media. */
static const int KEYCODE_HEADSETHOOK = 79;
/** Camera Focus key.
 * Used to focus the camera. */
static const int KEYCODE_FOCUS = 80; // *Camera* focus
/** '+' key. */
static const int KEYCODE_PLUS = 81;
/** Menu key. */
static const int KEYCODE_MENU = 82;
/** Notification key. */
static const int KEYCODE_NOTIFICATION = 83;
/** Search key. */
static const int KEYCODE_SEARCH = 84;
/** Play/Pause media key. */
static const int KEYCODE_MEDIA_PLAY_PAUSE= 85;
/** Stop media key. */
static const int KEYCODE_MEDIA_STOP = 86;
/** Play Next media key. */
static const int KEYCODE_MEDIA_NEXT = 87;
/** Play Previous media key. */
static const int KEYCODE_MEDIA_PREVIOUS = 88;
/** Rewind media key. */
static const int KEYCODE_MEDIA_REWIND = 89;
/** Fast Forward media key. */
static const int KEYCODE_MEDIA_FAST_FORWARD = 90;
/** Mute key.
 * Mutes the microphone, unlike {@link #KEYCODE_VOLUME_MUTE}. */
static const int KEYCODE_MUTE = 91;
/** Page Up key. */
static const int KEYCODE_PAGE_UP = 92;
/** Page Down key. */
static const int KEYCODE_PAGE_DOWN = 93;
/** Picture Symbols modifier key.
 * Used to switch symbol sets (Emoji, Kao-moji). */
static const int KEYCODE_PICTSYMBOLS = 94; // switch symbol-sets (Emoji,Kao-moji)
/** Switch Charset modifier key.
 * Used to switch character sets (Kanji, Katakana). */
static const int KEYCODE_SWITCH_CHARSET = 95; // switch char-sets (Kanji,Katakana)
/** A Button key.
 * On a game controller, the A button should be either the button labeled A
 * or the first button on the bottom row of controller buttons. */
static const int KEYCODE_BUTTON_A = 96;
/** B Button key.
 * On a game controller, the B button should be either the button labeled B
 * or the second button on the bottom row of controller buttons. */
static const int KEYCODE_BUTTON_B = 97;
/** C Button key.
 * On a game controller, the C button should be either the button labeled C
 * or the third button on the bottom row of controller buttons. */
static const int KEYCODE_BUTTON_C = 98;
/** X Button key.
 * On a game controller, the X button should be either the button labeled X
 * or the first button on the upper row of controller buttons. */
static const int KEYCODE_BUTTON_X = 99;
/** Y Button key.
 * On a game controller, the Y button should be either the button labeled Y
 * or the second button on the upper row of controller buttons. */
static const int KEYCODE_BUTTON_Y = 100;
/** Z Button key.
 * On a game controller, the Z button should be either the button labeled Z
 * or the third button on the upper row of controller buttons. */
static const int KEYCODE_BUTTON_Z = 101;
/** L1 Button key.
 * On a game controller, the L1 button should be either the button labeled L1 (or L)
 * or the top left trigger button. */
static const int KEYCODE_BUTTON_L1 = 102;
/** R1 Button key.
 * On a game controller, the R1 button should be either the button labeled R1 (or R)
 * or the top right trigger button. */
static const int KEYCODE_BUTTON_R1 = 103;
/** L2 Button key.
 * On a game controller, the L2 button should be either the button labeled L2
 * or the bottom left trigger button. */
static const int KEYCODE_BUTTON_L2 = 104;
/** R2 Button key.
 * On a game controller, the R2 button should be either the button labeled R2
 * or the bottom right trigger button. */
static const int KEYCODE_BUTTON_R2 = 105;
/** Left Thumb Button key.
 * On a game controller, the left thumb button indicates that the left (or only)
 * joystick is pressed. */
static const int KEYCODE_BUTTON_THUMBL = 106;
/** Right Thumb Button key.
 * On a game controller, the right thumb button indicates that the right
 * joystick is pressed. */
static const int KEYCODE_BUTTON_THUMBR = 107;
/** Start Button key.
 * On a game controller, the button labeled Start. */
static const int KEYCODE_BUTTON_START = 108;
/** Select Button key.
 * On a game controller, the button labeled Select. */
static const int KEYCODE_BUTTON_SELECT = 109;
/** Mode Button key.
 * On a game controller, the button labeled Mode. */
static const int KEYCODE_BUTTON_MODE = 110;
/** Escape key. */
static const int KEYCODE_ESCAPE = 111;
/** Forward Delete key.
 * Deletes characters ahead of the insertion point, unlike {@link #KEYCODE_DEL}. */
static const int KEYCODE_FORWARD_DEL = 112;
/** Left Control modifier key. */
static const int KEYCODE_CTRL_LEFT = 113;
/** Right Control modifier key. */
static const int KEYCODE_CTRL_RIGHT = 114;
/** Caps Lock key. */
static const int KEYCODE_CAPS_LOCK = 115;
/** Scroll Lock key. */
static const int KEYCODE_SCROLL_LOCK = 116;
/** Left Meta modifier key. */
static const int KEYCODE_META_LEFT = 117;
/** Right Meta modifier key. */
static const int KEYCODE_META_RIGHT = 118;
/** Function modifier key. */
static const int KEYCODE_FUNCTION = 119;
/** System Request / Print Screen key. */
static const int KEYCODE_SYSRQ = 120;
/** Break / Pause key. */
static const int KEYCODE_BREAK = 121;
/** Home Movement key.
 * Used for scrolling or moving the cursor around to the start of a line
 * or to the top of a list. */
static const int KEYCODE_MOVE_HOME = 122;
/** End Movement key.
 * Used for scrolling or moving the cursor around to the end of a line
 * or to the bottom of a list. */
static const int KEYCODE_MOVE_END = 123;
/** Insert key.
 * Toggles insert / overwrite edit mode. */
static const int KEYCODE_INSERT = 124;
/** Forward key.
 * Navigates forward in the history stack. Complement of {@link #KEYCODE_BACK}. */
static const int KEYCODE_FORWARD = 125;
/** Play media key. */
static const int KEYCODE_MEDIA_PLAY = 126;
/** Pause media key. */
static const int KEYCODE_MEDIA_PAUSE = 127;
/** Close media key.
 * May be used to close a CD tray, for example. */
static const int KEYCODE_MEDIA_CLOSE = 128;
/** Eject media key.
 * May be used to eject a CD tray, for example. */
static const int KEYCODE_MEDIA_EJECT = 129;
/** Record media key. */
static const int KEYCODE_MEDIA_RECORD = 130;
/** F1 key. */
static const int KEYCODE_F1 = 131;
/** F2 key. */
static const int KEYCODE_F2 = 132;
/** F3 key. */
static const int KEYCODE_F3 = 133;
/** F4 key. */
static const int KEYCODE_F4 = 134;
/** F5 key. */
static const int KEYCODE_F5 = 135;
/** F6 key. */
static const int KEYCODE_F6 = 136;
/** F7 key. */
static const int KEYCODE_F7 = 137;
/** F8 key. */
static const int KEYCODE_F8 = 138;
/** F9 key. */
static const int KEYCODE_F9 = 139;
/** F10 key. */
static const int KEYCODE_F10 = 140;
/** F11 key. */
static const int KEYCODE_F11 = 141;
/** F12 key. */
static const int KEYCODE_F12 = 142;
/** Num Lock key.
 * This is the Num Lock key; it is different from {@link #KEYCODE_NUM}.
 * This key alters the behavior of other keys on the numeric keypad. */
static const int KEYCODE_NUM_LOCK = 143;
/** Numeric keypad '0' key. */
static const int KEYCODE_NUMPAD_0 = 144;
/** Numeric keypad '1' key. */
static const int KEYCODE_NUMPAD_1 = 145;
/** Numeric keypad '2' key. */
static const int KEYCODE_NUMPAD_2 = 146;
/** Numeric keypad '3' key. */
static const int KEYCODE_NUMPAD_3 = 147;
/** Numeric keypad '4' key. */
static const int KEYCODE_NUMPAD_4 = 148;
/** Numeric keypad '5' key. */
static const int KEYCODE_NUMPAD_5 = 149;
/** Numeric keypad '6' key. */
static const int KEYCODE_NUMPAD_6 = 150;
/** Numeric keypad '7' key. */
static const int KEYCODE_NUMPAD_7 = 151;
/** Numeric keypad '8' key. */
static const int KEYCODE_NUMPAD_8 = 152;
/** Numeric keypad '9' key. */
static const int KEYCODE_NUMPAD_9 = 153;
/** Numeric keypad '/' key (for division). */
static const int KEYCODE_NUMPAD_DIVIDE = 154;
/** Numeric keypad '*' key (for multiplication). */
static const int KEYCODE_NUMPAD_MULTIPLY = 155;
/** Numeric keypad '-' key (for subtraction). */
static const int KEYCODE_NUMPAD_SUBTRACT = 156;
/** Numeric keypad '+' key (for addition). */
static const int KEYCODE_NUMPAD_ADD = 157;
/** Numeric keypad '.' key (for decimals or digit grouping). */
static const int KEYCODE_NUMPAD_DOT = 158;
/** Numeric keypad ',' key (for decimals or digit grouping). */
static const int KEYCODE_NUMPAD_COMMA = 159;
/** Numeric keypad Enter key. */
static const int KEYCODE_NUMPAD_ENTER = 160;
/** Numeric keypad '=' key. */
static const int KEYCODE_NUMPAD_EQUALS = 161;
/** Numeric keypad '(' key. */
static const int KEYCODE_NUMPAD_LEFT_PAREN = 162;
/** Numeric keypad ')' key. */
static const int KEYCODE_NUMPAD_RIGHT_PAREN = 163;
/** Volume Mute key.
 * Mutes the speaker, unlike {@link #KEYCODE_MUTE}.
 * This key should normally be implemented as a toggle such that the first press
 * mutes the speaker and the second press restores the original volume. */
static const int KEYCODE_VOLUME_MUTE = 164;
/** Info key.
 * Common on TV remotes to show additional information related to what is
 * currently being viewed. */
static const int KEYCODE_INFO = 165;
/** Channel up key.
 * On TV remotes, increments the television channel. */
static const int KEYCODE_CHANNEL_UP = 166;
/** Channel down key.
 * On TV remotes, decrements the television channel. */
static const int KEYCODE_CHANNEL_DOWN = 167;
/** Zoom in key. */
static const int KEYCODE_ZOOM_IN = 168;
/** Zoom out key. */
static const int KEYCODE_ZOOM_OUT = 169;
/** TV key.
 * On TV remotes, switches to viewing live TV. */
static const int KEYCODE_TV = 170;
/** Window key.
 * On TV remotes, toggles picture-in-picture mode or other windowing functions. */
static const int KEYCODE_WINDOW = 171;
/** Guide key.
 * On TV remotes, shows a programming guide. */
static const int KEYCODE_GUIDE = 172;
/** DVR key.
 * On some TV remotes, switches to a DVR mode for recorded shows. */
static const int KEYCODE_DVR = 173;
/** Bookmark key.
 * On some TV remotes, bookmarks content or web pages. */
static const int KEYCODE_BOOKMARK = 174;
/** Toggle captions key.
 * Switches the mode for closed-captioning text, for example during television shows. */
static const int KEYCODE_CAPTIONS = 175;
/** Settings key.
 * Starts the system settings activity. */
static const int KEYCODE_SETTINGS = 176;
/** TV power key.
 * On TV remotes, toggles the power on a television screen. */
static const int KEYCODE_TV_POWER = 177;
/** TV input key.
 * On TV remotes, switches the input on a television screen. */
static const int KEYCODE_TV_INPUT = 178;
/** Set-top-box power key.
 * On TV remotes, toggles the power on an external Set-top-box. */
static const int KEYCODE_STB_POWER = 179;
/** Set-top-box input key.
 * On TV remotes, switches the input mode on an external Set-top-box. */
static const int KEYCODE_STB_INPUT = 180;
/** A/V Receiver power key.
 * On TV remotes, toggles the power on an external A/V Receiver. */
static const int KEYCODE_AVR_POWER = 181;
/** A/V Receiver input key.
 * On TV remotes, switches the input mode on an external A/V Receiver. */
static const int KEYCODE_AVR_INPUT = 182;
/** Red "programmable" key.
 * On TV remotes, acts as a contextual/programmable key. */
static const int KEYCODE_PROG_RED = 183;
/** Green "programmable" key.
 * On TV remotes, actsas a contextual/programmable key. */
static const int KEYCODE_PROG_GREEN = 184;
/** Yellow "programmable" key.
 * On TV remotes, acts as a contextual/programmable key. */
static const int KEYCODE_PROG_YELLOW = 185;
/** Blue "programmable" key.
 * On TV remotes, acts as a contextual/programmable key. */
static const int KEYCODE_PROG_BLUE = 186;
/** App switch key.
 * Should bring up the application switcher dialog. */
static const int KEYCODE_APP_SWITCH = 187;
/** Generic Game Pad Button #1.*/
static const int KEYCODE_BUTTON_1 = 188;
/** Generic Game Pad Button #2.*/
static const int KEYCODE_BUTTON_2 = 189;
/** Generic Game Pad Button #3.*/
static const int KEYCODE_BUTTON_3 = 190;
/** Generic Game Pad Button #4.*/
static const int KEYCODE_BUTTON_4 = 191;
/** Generic Game Pad Button #5.*/
static const int KEYCODE_BUTTON_5 = 192;
/** Generic Game Pad Button #6.*/
static const int KEYCODE_BUTTON_6 = 193;
/** Generic Game Pad Button #7.*/
static const int KEYCODE_BUTTON_7 = 194;
/** Generic Game Pad Button #8.*/
static const int KEYCODE_BUTTON_8 = 195;
/** Generic Game Pad Button #9.*/
static const int KEYCODE_BUTTON_9 = 196;
/** Generic Game Pad Button #10.*/
static const int KEYCODE_BUTTON_10 = 197;
/** Generic Game Pad Button #11.*/
static const int KEYCODE_BUTTON_11 = 198;
/** Generic Game Pad Button #12.*/
static const int KEYCODE_BUTTON_12 = 199;
/** Generic Game Pad Button #13.*/
static const int KEYCODE_BUTTON_13 = 200;
/** Generic Game Pad Button #14.*/
static const int KEYCODE_BUTTON_14 = 201;
/** Generic Game Pad Button #15.*/
static const int KEYCODE_BUTTON_15 = 202;
/** Generic Game Pad Button #16.*/
static const int KEYCODE_BUTTON_16 = 203;
/** Language Switch key.
 * Toggles the current input language such as switching between English and Japanese on
 * a QWERTY keyboard. On some devices, the same function may be performed by
 * pressing Shift+Spacebar. */
static const int KEYCODE_LANGUAGE_SWITCH = 204;
/** Manner Mode key.
 * Toggles silent or vibrate mode on and off to make the device behave more politely
 * in certain settings such as on a crowded train. On some devices, the key may only
 * operate when long-pressed. */
static const int KEYCODE_MANNER_MODE = 205;
/** 3D Mode key.
 * Toggles the display between 2D and 3D mode. */
static const int KEYCODE_3D_MODE = 206;
/** Contacts special function key.
 * Used to launch an address book application. */
static const int KEYCODE_CONTACTS = 207;
/** Calendar special function key.
 * Used to launch a calendar application. */
static const int KEYCODE_CALENDAR = 208;
/** Music special function key.
 * Used to launch a music player application. */
static const int KEYCODE_MUSIC = 209;
/** Calculator special function key.
 * Used to launch a calculator application. */
static const int KEYCODE_CALCULATOR = 210;
/** Japanese full-width / half-width key. */
static const int KEYCODE_ZENKAKU_HANKAKU = 211;
/** Japanese alphanumeric key. */
static const int KEYCODE_EISU = 212;
/** Japanese non-conversion key. */
static const int KEYCODE_MUHENKAN = 213;
/** Japanese conversion key. */
static const int KEYCODE_HENKAN = 214;
/** Japanese katakana / hiragana key. */
static const int KEYCODE_KATAKANA_HIRAGANA = 215;
/** Japanese Yen key. */
static const int KEYCODE_YEN = 216;
/** Japanese Ro key. */
static const int KEYCODE_RO = 217;
/** Japanese kana key. */
static const int KEYCODE_KANA = 218;
/** Assist key.
 * Launches the global assist activity. Not delivered to applications. */
static const int KEYCODE_ASSIST = 219;
/** Brightness Down key.
 * Adjusts the screen brightness down. */
static const int KEYCODE_BRIGHTNESS_DOWN = 220;
/** Brightness Up key.
 * Adjusts the screen brightness up. */
static const int KEYCODE_BRIGHTNESS_UP = 221;
/** Audio Track key.
 * Switches the audio tracks. */
static const int KEYCODE_MEDIA_AUDIO_TRACK = 222;
/** Sleep key.
 * Puts the device to sleep. Behaves somewhat like {@link #KEYCODE_POWER} but it
 * has no effect if the device is already asleep. */
static const int KEYCODE_SLEEP = 223;
/** Wakeup key.
 * Wakes up the device. Behaves somewhat like {@link #KEYCODE_POWER} but it
 * has no effect if the device is already awake. */
static const int KEYCODE_WAKEUP = 224;
/** Pairing key.
 * Initiates peripheral pairing mode. Useful for pairing remote control
 * devices or game controllers, especially if no other input mode is
 * available. */
static const int KEYCODE_PAIRING = 225;
/** Media Top Menu key.
 * Goes to the top of media menu. */
static const int KEYCODE_MEDIA_TOP_MENU = 226;
/** '11' key. */
static const int KEYCODE_11 = 227;
/** '12' key. */
static const int KEYCODE_12 = 228;
/** Last Channel key.
 * Goes to the last viewed channel. */
static const int KEYCODE_LAST_CHANNEL = 229;
/** TV data service key.
 * Displays data services like weather, sports. */
static const int KEYCODE_TV_DATA_SERVICE = 230;
/** Voice Assist key.
 * Launches the global voice assist activity. Not delivered to applications. */
static const int KEYCODE_VOICE_ASSIST = 231;
/** Radio key.
 * Toggles TV service / Radio service. */
static const int KEYCODE_TV_RADIO_SERVICE = 232;
/** Teletext key.
 * Displays Teletext service. */
static const int KEYCODE_TV_TELETEXT = 233;
/** Number entry key.
 * Initiates to enter multi-digit channel nubmber when each digit key is assigned
 * for selecting separate channel. Corresponds to Number Entry Mode (0x1D) of CEC
 * User Control Code. */
static const int KEYCODE_TV_NUMBER_ENTRY = 234;
/** Analog Terrestrial key.
 * Switches to analog terrestrial broadcast service. */
static const int KEYCODE_TV_TERRESTRIAL_ANALOG = 235;
/** Digital Terrestrial key.
 * Switches to digital terrestrial broadcast service. */
static const int KEYCODE_TV_TERRESTRIAL_DIGITAL = 236;
/** Satellite key.
 * Switches to digital satellite broadcast service. */
static const int KEYCODE_TV_SATELLITE = 237;
/** BS key.
 * Switches to BS digital satellite broadcasting service available in Japan. */
static const int KEYCODE_TV_SATELLITE_BS = 238;
/** CS key.
 * Switches to CS digital satellite broadcasting service available in Japan. */
static const int KEYCODE_TV_SATELLITE_CS = 239;
/** BS/CS key.
 * Toggles between BS and CS digital satellite services. */
static const int KEYCODE_TV_SATELLITE_SERVICE = 240;
/** Toggle Network key.
 * Toggles selecting broacast services. */
static const int KEYCODE_TV_NETWORK = 241;
/** Antenna/Cable key.
 * Toggles broadcast input source between antenna and cable. */
static const int KEYCODE_TV_ANTENNA_CABLE = 242;
/** HDMI #1 key.
 * Switches to HDMI input #1. */
static const int KEYCODE_TV_INPUT_HDMI_1 = 243;
/** HDMI #2 key.
 * Switches to HDMI input #2. */
static const int KEYCODE_TV_INPUT_HDMI_2 = 244;
/** HDMI #3 key.
 * Switches to HDMI input #3. */
static const int KEYCODE_TV_INPUT_HDMI_3 = 245;
/** HDMI #4 key.
 * Switches to HDMI input #4. */
static const int KEYCODE_TV_INPUT_HDMI_4 = 246;
/** Composite #1 key.
 * Switches to composite video input #1. */
static const int KEYCODE_TV_INPUT_COMPOSITE_1 = 247;
/** Composite #2 key.
 * Switches to composite video input #2. */
static const int KEYCODE_TV_INPUT_COMPOSITE_2 = 248;
/** Component #1 key.
 * Switches to component video input #1. */
static const int KEYCODE_TV_INPUT_COMPONENT_1 = 249;
/** Component #2 key.
 * Switches to component video input #2. */
static const int KEYCODE_TV_INPUT_COMPONENT_2 = 250;
/** VGA #1 key.
 * Switches to VGA (analog RGB) input #1. */
static const int KEYCODE_TV_INPUT_VGA_1 = 251;
/** Audio description key.
 * Toggles audio description off / on. */
static const int KEYCODE_TV_AUDIO_DESCRIPTION = 252;
/** Audio description mixing volume up key.
 * Louden audio description volume as compared with normal audio volume. */
static const int KEYCODE_TV_AUDIO_DESCRIPTION_MIX_UP = 253;
/** Audio description mixing volume down key.
 * Lessen audio description volume as compared with normal audio volume. */
static const int KEYCODE_TV_AUDIO_DESCRIPTION_MIX_DOWN = 254;
/** Zoom mode key.
 * Changes Zoom mode (Normal, Full, Zoom, Wide-zoom, etc.) */
static const int KEYCODE_TV_ZOOM_MODE = 255;
/** Contents menu key.
 * Goes to the title list. Corresponds to Contents Menu (0x0B) of CEC User Control
 * Code */
static const int KEYCODE_TV_CONTENTS_MENU = 256;
/** Media context menu key.
 * Goes to the context menu of media contents. Corresponds to Media Context-sensitive
 * Menu (0x11) of CEC User Control Code. */
static const int KEYCODE_TV_MEDIA_CONTEXT_MENU = 257;
/** Timer programming key.
 * Goes to the timer recording menu. Corresponds to Timer Programming (0x54) of
 * CEC User Control Code. */
static const int KEYCODE_TV_TIMER_PROGRAMMING = 258;
/** Help key. */
static const int KEYCODE_HELP = 259;
/** Navigate to previous key.
 * Goes backward by one item in an ordered collection of items. */
static const int KEYCODE_NAVIGATE_PREVIOUS = 260;
/** Navigate to next key.
 * Advances to the next item in an ordered collection of items. */
static const int KEYCODE_NAVIGATE_NEXT = 261;
/** Navigate in key.
 * Activates the item that currently has focus or expands to the next level of a navigation
 * hierarchy. */
static const int KEYCODE_NAVIGATE_IN = 262;
/** Navigate out key.
 * Backs out one level of a navigation hierarchy or collapses the item that currently has
 * focus. */
static const int KEYCODE_NAVIGATE_OUT = 263;
/** Primary stem key for Wear
 * Main power/reset button on watch. */
static const int KEYCODE_STEM_PRIMARY = 264;
/** Generic stem key 1 for Wear */
static const int KEYCODE_STEM_1 = 265;
/** Generic stem key 2 for Wear */
static const int KEYCODE_STEM_2 = 266;
/** Generic stem key 3 for Wear */
static const int KEYCODE_STEM_3 = 267;
/** Directional Pad Up-Left */
static const int KEYCODE_DPAD_UP_LEFT = 268;
/** Directional Pad Down-Left */
static const int KEYCODE_DPAD_DOWN_LEFT = 269;
/** Directional Pad Up-Right */
static const int KEYCODE_DPAD_UP_RIGHT = 270;
/** Directional Pad Down-Right */
static const int KEYCODE_DPAD_DOWN_RIGHT = 271;
/** Skip forward media key. */
static const int KEYCODE_MEDIA_SKIP_FORWARD = 272;
/** Skip backward media key. */
static const int KEYCODE_MEDIA_SKIP_BACKWARD = 273;
/** Step forward media key.
 * Steps media forward, one frame at a time. */
static const int KEYCODE_MEDIA_STEP_FORWARD = 274;
/** Step backward media key.
 * Steps media backward, one frame at a time. */
static const int KEYCODE_MEDIA_STEP_BACKWARD = 275;
/** put device to sleep unless a wakelock is held. */
static const int KEYCODE_SOFT_SLEEP = 276;
/** Cut key. */
static const int KEYCODE_CUT = 277;
/** Copy key. */
static const int KEYCODE_COPY = 278;
/** Paste key. */
static const int KEYCODE_PASTE = 279;
/** Consumed by the system for navigation up */
static const int KEYCODE_SYSTEM_NAVIGATION_UP = 280;
/** Consumed by the system for navigation down */
static const int KEYCODE_SYSTEM_NAVIGATION_DOWN = 281;
/** Consumed by the system for navigation left*/
static const int KEYCODE_SYSTEM_NAVIGATION_LEFT = 282;
/** Consumed by the system for navigation right */
static const int KEYCODE_SYSTEM_NAVIGATION_RIGHT = 283;

  /** Key code constant: Show all apps */
static const int KEYCODE_ALL_APPS = 284;
/** Key code constant: Refresh key. */
static const int KEYCODE_REFRESH = 285;
/** Key code constant: Thumbs up key. Apps can use this to let user upvote content. */
static const int KEYCODE_THUMBS_UP = 286;
/** Key code constant: Thumbs down key. Apps can use this to let user downvote content. */
static const int KEYCODE_THUMBS_DOWN = 287;


#endif /* __KEYCODE_H__ */
