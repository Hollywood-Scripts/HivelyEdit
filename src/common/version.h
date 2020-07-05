#define PLUGIN_VER 2
#define PLUGIN_REV 0
#define PLUGIN_VER_STR "2.0"

#if defined(HW_AMIGAOS3)
#define PLUGIN_PLAT "AmigaOS3"
#define PLUGIN_ARCH HWARCH_OS3
#elif defined(HW_WARPOS)
#define PLUGIN_PLAT "WarpOS"
#define PLUGIN_ARCH HWARCH_WOS
#elif defined(HW_AMIGAOS4)
#define PLUGIN_PLAT "AmigaOS4"
#define PLUGIN_ARCH HWARCH_OS4
#elif defined(HW_MORPHOS)
#define PLUGIN_PLAT "MorphOS"
#define PLUGIN_ARCH HWARCH_MOS
#elif defined(HW_AROS)
#define PLUGIN_PLAT "AROS"
#define PLUGIN_ARCH HWARCH_AROS
#elif defined(HW_WIN32)
#define PLUGIN_PLAT "Windows"
#define PLUGIN_ARCH HWARCH_WIN32
#elif defined(HW_MACOS)
#define PLUGIN_PLAT "MacOS"
#define PLUGIN_ARCH HWARCH_MACOS
#elif defined(HW_LINUX)
#define PLUGIN_PLAT "Linux"
#define PLUGIN_ARCH HWARCH_LINUX
#elif defined(HW_ANDROID)
#define PLUGIN_PLAT "Android"
#define PLUGIN_ARCH HWARCH_ANDROID
#elif defined(HW_IOS)
#define PLUGIN_PLAT "iOS"
#define PLUGIN_ARCH HWARCH_IOS
#endif

#define PLUGIN_NAME "HivelyTracker"
#define PLUGIN_MODULENAME "ahx"
#define PLUGIN_AUTHOR "Andreas Falkenhahn and Samuel D. Crow, based on code by Xeron/IRIS"
#define PLUGIN_DESCRIPTION "Enables Hollywood to play and edit AHX and HivelyTracker modules"
#define PLUGIN_COPYRIGHT "BSD"
#define PLUGIN_URL "http://www.github.com/samuraicrow/hivelyedit/"
#define PLUGIN_DATE "05.07.20"
