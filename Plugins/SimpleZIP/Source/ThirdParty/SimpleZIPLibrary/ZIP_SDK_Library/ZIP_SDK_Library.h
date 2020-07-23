// ZIP_SDK_Library.h : main header file for the ZIP_SDK_Library DLL
//
#pragma once

#ifdef GS_EXPORTS
#define SIMPLEZIPLIBRARY_API __declspec(dllexport)
#else
#define SIMPLEZIPLIBRARY_API __declspec(dllimport)
#endif // EEE

#ifdef __cplusplus
extern "C"
{
#endif
	 SIMPLEZIPLIBRARY_API unsigned long __stdcall zip_file_by_loacl(const char *src_path, const char *dest_filename, const char *password);
	 SIMPLEZIPLIBRARY_API unsigned long __stdcall unzip_file_by_loacl(const char *src_path, const char *dest_filename, const char *password,bool b_overwrite);

#ifdef __cplusplus
}
#endif