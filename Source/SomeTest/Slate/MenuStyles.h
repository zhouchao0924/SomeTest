// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FMenuStyles
{
public:
	// Initializes the value of MenuStyleInstance and registers it with the Slate Style Registry.
	static void Initialize();

	// Unregisters the Slate Style Set and then resets the MenuStyleInstance pointer.
	static void Shutdown();

	// Retrieves a reference to the Slate Style pointed to by MenuStyleInstance.
	static const class ISlateStyle& Get();

	// Retrieves the name of the Style Set.
	static FName GetStyleSetName();

private:
	// Creates the Style Set.
	static TSharedRef<class FSlateStyleSet> Create();

	// Singleton instance used for our Style Set.
	static TSharedPtr<class FSlateStyleSet> MenuStyleInstance;

};
