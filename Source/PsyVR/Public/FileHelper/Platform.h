#pragma once

#if PLATFORM_WINDOWS
	#define SPLIT "\\"
#else
	#define SPLIT "/"
#endif


// NOTE: When using Android, you need to add following settings in Project Settings -> Android -> Advanced -> Extra Permissions
// android.permission.WRITE_EXTERNAL_STORAGE
// android.permission.READ_EXTERNAL_STORAGE

#if PLATFORM_WINDOWS
	#define BASE_DIR "./";
#else
	#define BASE_DIR "/sdcard/";
#endif
