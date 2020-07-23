// Copyright (C) RenZhai.2020.All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FileHelperType.h"
#include "SimpleFileHelperBPLibrary.generated.h"

/*-----------------------------------------------------------------------------
	USimpleFileHelperBPLibrary
-----------------------------------------------------------------------------*/
UCLASS()
class USimpleFileHelperBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/**
	* Load a binary file to a dynamic array with two uninitialized bytes at end as padding.
	*
	* @param Result    Receives the contents of the file
	* @param Filename  The file to read
	* @param Flags     Flags to pass to IFileManager::CreateFileReader
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleFileHelper")
	static bool LoadFileToArray(TArray<uint8>& Result, const FString &Filename, int32 Flags = 0);

	/**
	* Load a text file to an FString. Supports all combination of ANSI/Unicode files and platforms.
	*
	* @param Result       String representation of the loaded file
	* @param Filename     Name of the file to load
	* @param VerifyFlags  Flags controlling the hash verification behavior ( see EHashOptions )
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleFileHelper")
	static bool LoadFileToString(FString& Result, const FString &Filename, ESimpleHashOptions VerifyFlags);

	/**
	* Load a text file to an array of strings. Supports all combination of ANSI/Unicode files and platforms.
	*
	* @param Result       String representation of the loaded file
	* @param Filename     Name of the file to load
	* @param VerifyFlags  Flags controlling the hash verification behavior ( see EHashOptions )
	*/
	UFUNCTION(BlueprintCallable,Category = "SimpleFileHelper")
	static bool LoadFileToStringArray(TArray<FString>& Result, const FString &Filename, ESimpleHashOptions VerifyFlags);

	/**
	* Save a binary array to a file.
	*/
	UFUNCTION(BlueprintCallable,Category = "SimpleFileHelper")
	static bool SaveArrayToFile(const TArray<uint8> &Array, const FString &Filename,int32 WriteFlags = 0);

	/**
	* Write the FString to a file.
	* Supports all combination of ANSI/Unicode files and platforms.
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleFileHelper")
	static bool SaveStringToFile(const FString& String, const FString &Filename, ESimpleEncodingOptions EncodingOptions = ESimpleEncodingOptions::AutoDetect, int32 WriteFlags = 0);

	/**
	* Write the FString to a file.
	* Supports all combination of ANSI/Unicode files and platforms.
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleFileHelper")
	static bool SaveStringArrayToFile(const TArray<FString>& Lines, const FString &Filename, ESimpleEncodingOptions EncodingOptions = ESimpleEncodingOptions::AutoDetect, int32 WriteFlags = 0);

	/**
	* Generates the next unique bitmap filename with a specified extension
	*
	* @param Pattern		Filename with path, but without extension.
	* @param Extension		File extension to be appended
	* @param OutFilename	Reference to an FString where the newly generated filename will be placed
	* @param FileManager	Reference to a IFileManager (or the global instance by default)
	*
	* @return true if success
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleFileHelper")
	static bool GenerateNextBitmapFilename(const FString& Pattern, const FString& Extension, FString& OutFilename);
	
	/**
	 * Saves a 24/32Bit BMP file to disk
	 *
	 * @param Pattern filename with path, must not be 0, if with "bmp" extension (e.g. "out.bmp") the filename stays like this, if without (e.g. "out") automatic index numbers are addended (e.g. "out00002.bmp")
	 * @param DataWidth - Width of the bitmap supplied in Data >0
	 * @param DataHeight - Height of the bitmap supplied in Data >0
	 * @param Data must not be 0
	 * @param SubRectangle optional, specifies a sub-rectangle of the source image to save out. If NULL, the whole bitmap is saved
	 * @param FileManager must not be 0
	 * @param OutFilename optional, if specified filename will be output
	 * @param bInWriteAlpha optional, specifies whether to write out the alpha channel. Will force BMP V4 format.
	 *
	 * @return true if success
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleFileHelper")
	static bool CreateBitmap(const FString &Pattern, int32 DataWidth, int32 DataHeight, const FColor &Data, FVector4 SubRectangle, FString &OutFilename, bool bInWriteAlpha = false);

	/**
	*	Load the given ANSI text file to an array of strings - one FString per line of the file.
	*	Intended for use in simple text parsing actions
	*
	*	@param	InFilename			The text file to read, full path
	*	@param	InFileManager		The filemanager to use - NULL will use &IFileManager::Get()
	*	@param	OutStrings			The array of FStrings to fill in
	*
	*	@return	bool				true if successful, false if not
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleFileHelper")
	static bool LoadANSITextFileToStrings(const FString &InFilename, TArray<FString>& OutStrings);

	/**
	* Checks to see if a filename is valid for saving.
	* A filename must be under FPlatformMisc::GetMaxPathLength() to be saved
	*
	* @param Filename	Filename, with or without path information, to check.
	* @param OutError	If an error occurs, this is the reason why
	*/
	UFUNCTION(BlueprintCallable,Category = "SimpleFileHelper")
	static bool IsFilenameValidForSaving(const FString& Filename, FText& OutError);
};
