#ifndef _SE_KEYBOARD_H_
#define _SE_KEYBOARD_H_

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

		static KeyState GetKeyState(unsigned char key);
		static bool GetKeyPress(unsigned char key, bool repeat = true);

	private:
		static std::map<unsigned char, KeyState> KeyStateList;
	};

#define SE_KEY_SPACE              (unsigned char)32
#define SE_KEY_APOSTROPHE         (unsigned char)39
#define SE_KEY_COMMA              (unsigned char)44
#define SE_KEY_MINUS              (unsigned char)45
#define SE_KEY_PERIOD             (unsigned char)46
#define SE_KEY_SLASH              (unsigned char)47
#define SE_KEY_0                  (unsigned char)48
#define SE_KEY_1                  (unsigned char)49
#define SE_KEY_2                  (unsigned char)50
#define SE_KEY_3                  (unsigned char)51
#define SE_KEY_4                  (unsigned char)52
#define SE_KEY_5                  (unsigned char)53
#define SE_KEY_6                  (unsigned char)54
#define SE_KEY_7                  (unsigned char)55
#define SE_KEY_8                  (unsigned char)56
#define SE_KEY_9                  (unsigned char)57
#define SE_KEY_SEMICOLON          (unsigned char)59
#define SE_KEY_EQUAL              (unsigned char)61
#define SE_KEY_A                  (unsigned char)65
#define SE_KEY_B                  (unsigned char)66
#define SE_KEY_C                  (unsigned char)67
#define SE_KEY_D                  (unsigned char)68
#define SE_KEY_E                  (unsigned char)69
#define SE_KEY_F                  (unsigned char)70
#define SE_KEY_G                  (unsigned char)71
#define SE_KEY_H                  (unsigned char)72
#define SE_KEY_I                  (unsigned char)73
#define SE_KEY_J                  (unsigned char)74
#define SE_KEY_K                  (unsigned char)75
#define SE_KEY_L                  (unsigned char)76
#define SE_KEY_M                  (unsigned char)77
#define SE_KEY_N                  (unsigned char)78
#define SE_KEY_O                  (unsigned char)79
#define SE_KEY_P                  (unsigned char)80
#define SE_KEY_Q                  (unsigned char)81
#define SE_KEY_R                  (unsigned char)82
#define SE_KEY_S                  (unsigned char)83
#define SE_KEY_T                  (unsigned char)84
#define SE_KEY_U                  (unsigned char)85
#define SE_KEY_V                  (unsigned char)86
#define SE_KEY_W                  (unsigned char)87
#define SE_KEY_X                  (unsigned char)88
#define SE_KEY_Y                  (unsigned char)89
#define SE_KEY_Z                  (unsigned char)90
#define SE_KEY_LEFT_BRACKET       (unsigned char)91 
#define SE_KEY_BACKSLASH          (unsigned char)92 
#define SE_KEY_RIGHT_BRACKET      (unsigned char)93 
#define SE_KEY_GRAVE_ACCENT       (unsigned char)96 
#define SE_KEY_WORLD_1            (unsigned char)161
#define SE_KEY_WORLD_2            (unsigned char)162
								   
#define SE_KEY_ESCAPE             (unsigned char)256
#define SE_KEY_ENTER              (unsigned char)257
#define SE_KEY_TAB                (unsigned char)258
#define SE_KEY_BACKSPACE          (unsigned char)259
#define SE_KEY_INSERT             (unsigned char)260
#define SE_KEY_DELETE             (unsigned char)261
#define SE_KEY_RIGHT              (unsigned char)262
#define SE_KEY_LEFT               (unsigned char)263
#define SE_KEY_DOWN               (unsigned char)264
#define SE_KEY_UP                 (unsigned char)265
#define SE_KEY_PAGE_UP            (unsigned char)266
#define SE_KEY_PAGE_DOWN          (unsigned char)267
#define SE_KEY_HOME               (unsigned char)268
#define SE_KEY_END                (unsigned char)269
#define SE_KEY_CAPS_LOCK          (unsigned char)280
#define SE_KEY_SCROLL_LOCK        (unsigned char)281
#define SE_KEY_NUM_LOCK           (unsigned char)282
#define SE_KEY_PRINT_SCREEN       (unsigned char)283
#define SE_KEY_PAUSE              (unsigned char)284
#define SE_KEY_F1                 (unsigned char)290
#define SE_KEY_F2                 (unsigned char)291
#define SE_KEY_F3                 (unsigned char)292
#define SE_KEY_F4                 (unsigned char)293
#define SE_KEY_F5                 (unsigned char)294
#define SE_KEY_F6                 (unsigned char)295
#define SE_KEY_F7                 (unsigned char)296
#define SE_KEY_F8                 (unsigned char)297
#define SE_KEY_F9                 (unsigned char)298
#define SE_KEY_F10                (unsigned char)299
#define SE_KEY_F11                (unsigned char)300
#define SE_KEY_F12                (unsigned char)301
#define SE_KEY_F13                (unsigned char)302
#define SE_KEY_F14                (unsigned char)303
#define SE_KEY_F15                (unsigned char)304
#define SE_KEY_F16                (unsigned char)305
#define SE_KEY_F17                (unsigned char)306
#define SE_KEY_F18                (unsigned char)307
#define SE_KEY_F19                (unsigned char)308
#define SE_KEY_F20                (unsigned char)309
#define SE_KEY_F21                (unsigned char)310
#define SE_KEY_F22                (unsigned char)311
#define SE_KEY_F23                (unsigned char)312
#define SE_KEY_F24                (unsigned char)313
#define SE_KEY_F25                (unsigned char)314
#define SE_KEY_KP_0               (unsigned char)320
#define SE_KEY_KP_1               (unsigned char)321
#define SE_KEY_KP_2               (unsigned char)322
#define SE_KEY_KP_3               (unsigned char)323
#define SE_KEY_KP_4               (unsigned char)324
#define SE_KEY_KP_5               (unsigned char)325
#define SE_KEY_KP_6               (unsigned char)326
#define SE_KEY_KP_7               (unsigned char)327
#define SE_KEY_KP_8               (unsigned char)328
#define SE_KEY_KP_9               (unsigned char)329
#define SE_KEY_KP_DECIMAL         (unsigned char)330
#define SE_KEY_KP_DIVIDE          (unsigned char)331
#define SE_KEY_KP_MULTIPLY        (unsigned char)332
#define SE_KEY_KP_SUBTRACT        (unsigned char)333
#define SE_KEY_KP_ADD             (unsigned char)334
#define SE_KEY_KP_ENTER           (unsigned char)335
#define SE_KEY_KP_EQUAL           (unsigned char)336
#define SE_KEY_LEFT_SHIFT         (unsigned char)340
#define SE_KEY_LEFT_CONTROL       (unsigned char)341
#define SE_KEY_LEFT_ALT           (unsigned char)342
#define SE_KEY_LEFT_SUPER         (unsigned char)343
#define SE_KEY_RIGHT_SHIFT		  (unsigned char)344
#define SE_KEY_RIGHT_CONTROL      (unsigned char)345
#define SE_KEY_RIGHT_ALT          (unsigned char)346
#define SE_KEY_RIGHT_SUPER        (unsigned char)347
#define SE_KEY_MENU               (unsigned char)348

#define SE_KEY_LAST               GLFW_KEY_MENU
}

#endif