#ifndef _SE_KEYBOARD_H_
#define _SE_KEYBOARD_H_
#include "../../../Core/Core.h"

namespace SE
{
	class FKeyboard
	{
	public:
		enum KeyState
		{
			SE_KEY_RELEASE,
			SE_KEY_PRESS,
			SE_KEY_REPEAT,
		};

	public:
		static void InitializeKeyboard(GLFWwindow* bindingWindow);

		static void UpdateKeyboard();

		static KeyState GetKeyState(SUChar key);
		static SBool GetKeyPress(SUChar key, SBool repeat = true);

	private:
		static std::map<SUChar, KeyState> KeyStateList;
	};

#define SE_KEY_SPACE              (SUChar)32
#define SE_KEY_APOSTROPHE         (SUChar)39
#define SE_KEY_COMMA              (SUChar)44
#define SE_KEY_MINUS              (SUChar)45
#define SE_KEY_PERIOD             (SUChar)46
#define SE_KEY_SLASH              (SUChar)47
#define SE_KEY_0                  (SUChar)48
#define SE_KEY_1                  (SUChar)49
#define SE_KEY_2                  (SUChar)50
#define SE_KEY_3                  (SUChar)51
#define SE_KEY_4                  (SUChar)52
#define SE_KEY_5                  (SUChar)53
#define SE_KEY_6                  (SUChar)54
#define SE_KEY_7                  (SUChar)55
#define SE_KEY_8                  (SUChar)56
#define SE_KEY_9                  (SUChar)57
#define SE_KEY_SEMICOLON          (SUChar)59
#define SE_KEY_EQUAL              (SUChar)61
#define SE_KEY_A                  (SUChar)65
#define SE_KEY_B                  (SUChar)66
#define SE_KEY_C                  (SUChar)67
#define SE_KEY_D                  (SUChar)68
#define SE_KEY_E                  (SUChar)69
#define SE_KEY_F                  (SUChar)70
#define SE_KEY_G                  (SUChar)71
#define SE_KEY_H                  (SUChar)72
#define SE_KEY_I                  (SUChar)73
#define SE_KEY_J                  (SUChar)74
#define SE_KEY_K                  (SUChar)75
#define SE_KEY_L                  (SUChar)76
#define SE_KEY_M                  (SUChar)77
#define SE_KEY_N                  (SUChar)78
#define SE_KEY_O                  (SUChar)79
#define SE_KEY_P                  (SUChar)80
#define SE_KEY_Q                  (SUChar)81
#define SE_KEY_R                  (SUChar)82
#define SE_KEY_S                  (SUChar)83
#define SE_KEY_T                  (SUChar)84
#define SE_KEY_U                  (SUChar)85
#define SE_KEY_V                  (SUChar)86
#define SE_KEY_W                  (SUChar)87
#define SE_KEY_X                  (SUChar)88
#define SE_KEY_Y                  (SUChar)89
#define SE_KEY_Z                  (SUChar)90
#define SE_KEY_LEFT_BRACKET       (SUChar)91 
#define SE_KEY_BACKSLASH          (SUChar)92 
#define SE_KEY_RIGHT_BRACKET      (SUChar)93 
#define SE_KEY_GRAVE_ACCENT       (SUChar)96 
#define SE_KEY_WORLD_1            (SUChar)161
#define SE_KEY_WORLD_2            (SUChar)162

#define SE_KEY_ESCAPE             (SUChar)256
#define SE_KEY_ENTER              (SUChar)257
#define SE_KEY_TAB                (SUChar)258
#define SE_KEY_BACKSPACE          (SUChar)259
#define SE_KEY_INSERT             (SUChar)260
#define SE_KEY_DELETE             (SUChar)261
#define SE_KEY_RIGHT              (SUChar)262
#define SE_KEY_LEFT               (SUChar)263
#define SE_KEY_DOWN               (SUChar)264
#define SE_KEY_UP                 (SUChar)265
#define SE_KEY_PAGE_UP            (SUChar)266
#define SE_KEY_PAGE_DOWN          (SUChar)267
#define SE_KEY_HOME               (SUChar)268
#define SE_KEY_END                (SUChar)269
#define SE_KEY_CAPS_LOCK          (SUChar)280
#define SE_KEY_SCROLL_LOCK        (SUChar)281
#define SE_KEY_NUM_LOCK           (SUChar)282
#define SE_KEY_PRINT_SCREEN       (SUChar)283
#define SE_KEY_PAUSE              (SUChar)284
#define SE_KEY_F1                 (SUChar)290
#define SE_KEY_F2                 (SUChar)291
#define SE_KEY_F3                 (SUChar)292
#define SE_KEY_F4                 (SUChar)293
#define SE_KEY_F5                 (SUChar)294
#define SE_KEY_F6                 (SUChar)295
#define SE_KEY_F7                 (SUChar)296
#define SE_KEY_F8                 (SUChar)297
#define SE_KEY_F9                 (SUChar)298
#define SE_KEY_F10                (SUChar)299
#define SE_KEY_F11                (SUChar)300
#define SE_KEY_F12                (SUChar)301
#define SE_KEY_F13                (SUChar)302
#define SE_KEY_F14                (SUChar)303
#define SE_KEY_F15                (SUChar)304
#define SE_KEY_F16                (SUChar)305
#define SE_KEY_F17                (SUChar)306
#define SE_KEY_F18                (SUChar)307
#define SE_KEY_F19                (SUChar)308
#define SE_KEY_F20                (SUChar)309
#define SE_KEY_F21                (SUChar)310
#define SE_KEY_F22                (SUChar)311
#define SE_KEY_F23                (SUChar)312
#define SE_KEY_F24                (SUChar)313
#define SE_KEY_F25                (SUChar)314
#define SE_KEY_KP_0               (SUChar)320
#define SE_KEY_KP_1               (SUChar)321
#define SE_KEY_KP_2               (SUChar)322
#define SE_KEY_KP_3               (SUChar)323
#define SE_KEY_KP_4               (SUChar)324
#define SE_KEY_KP_5               (SUChar)325
#define SE_KEY_KP_6               (SUChar)326
#define SE_KEY_KP_7               (SUChar)327
#define SE_KEY_KP_8               (SUChar)328
#define SE_KEY_KP_9               (SUChar)329
#define SE_KEY_KP_DECIMAL         (SUChar)330
#define SE_KEY_KP_DIVIDE          (SUChar)331
#define SE_KEY_KP_MULTIPLY        (SUChar)332
#define SE_KEY_KP_SUBTRACT        (SUChar)333
#define SE_KEY_KP_ADD             (SUChar)334
#define SE_KEY_KP_ENTER           (SUChar)335
#define SE_KEY_KP_EQUAL           (SUChar)336
#define SE_KEY_LEFT_SHIFT         (SUChar)340
#define SE_KEY_LEFT_CONTROL       (SUChar)341
#define SE_KEY_LEFT_ALT           (SUChar)342
#define SE_KEY_LEFT_SUPER         (SUChar)343
#define SE_KEY_RIGHT_SHIFT		  (SUChar)344
#define SE_KEY_RIGHT_CONTROL      (SUChar)345
#define SE_KEY_RIGHT_ALT          (SUChar)346
#define SE_KEY_RIGHT_SUPER        (SUChar)347
#define SE_KEY_MENU               (SUChar)348

#define SE_KEY_LAST               GLFW_KEY_MENU
}

#endif