// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Widget/InputKeySelectorMapping.h"
#include "Engine/Font.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/FrameworkObjectVersion.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SInputKeySelector.h"
#include "Internationalization/Internationalization.h"
//#include "SlateWrapperTypes.h"

#define LOCTEXT_NAMESPACE "SimpleUMG"

static FButtonStyle* DefaultInputKeySelectorMappingButtonStyle = nullptr;
static FTextBlockStyle* DefaultInputKeySelectorMapingTextStyle = nullptr;

UInputKeySelectorMapping::UInputKeySelectorMapping( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	if (DefaultInputKeySelectorMappingButtonStyle == nullptr)
	{
		// HACK: THIS SHOULD NOT COME FROM CORESTYLE AND SHOULD INSTEAD BE DEFINED BY ENGINE TEXTURES/PROJECT SETTINGS
		DefaultInputKeySelectorMappingButtonStyle = new FButtonStyle(FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button"));

		// Unlink UMG default colors from the editor settings colors.
		DefaultInputKeySelectorMappingButtonStyle->UnlinkColors();
	}

	if (DefaultInputKeySelectorMapingTextStyle == nullptr)
	{
		// HACK: THIS SHOULD NOT COME FROM CORESTYLE AND SHOULD INSTEAD BE DEFINED BY ENGINE TEXTURES/PROJECT SETTINGS
		DefaultInputKeySelectorMapingTextStyle = new FTextBlockStyle(FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText"));

		// Unlink UMG default colors from the editor settings colors.
		DefaultInputKeySelectorMapingTextStyle->UnlinkColors();
	}

	WidgetStyle = *DefaultInputKeySelectorMappingButtonStyle;
	TextStyle = *DefaultInputKeySelectorMapingTextStyle;

	KeySelectionText = NSLOCTEXT("InputKeySelector", "DefaultKeySelectionText", "...");
	NoKeySpecifiedText = NSLOCTEXT("InputKeySelector", "DefaultEmptyText", "Empty");
	SelectedKey = FInputChord(EKeys::Invalid);
	bAllowModifierKeys = true;
	bAllowGamepadKeys = false;

	EscapeKeys.AddUnique(EKeys::Gamepad_Special_Right); // In most (if not all) cases this is going to be the menu button

	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(*UWidget::GetDefaultFontName());
		TextStyle.Font = FSlateFontInfo(RobotoFontObj.Object, 24, FName("Bold"));
	}
}

void UInputKeySelectorMapping::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar.UsingCustomVersion(FFrameworkObjectVersion::GUID);
}

void UInputKeySelectorMapping::PostLoad()
{
	Super::PostLoad();

	if (GetLinkerCustomVersion(FFrameworkObjectVersion::GUID) < FFrameworkObjectVersion::InputKeySelectorTextStyle)
	{
		TextStyle.Font = Font_DEPRECATED;
		TextStyle.ColorAndOpacity = ColorAndOpacity_DEPRECATED;
	}
}

void UInputKeySelectorMapping::SetSelectedKey( const FInputChord& InSelectedKey )
{
	if ( MyInputKeySelector.IsValid() )
	{
		MyInputKeySelector->SetSelectedKey( InSelectedKey );
	}
	SelectedKey = InSelectedKey;
}

void UInputKeySelectorMapping::SetKeySelectionText( FText InKeySelectionText )
{
	if ( MyInputKeySelector.IsValid() )
	{
		MyInputKeySelector->SetKeySelectionText( InKeySelectionText );
	}
	KeySelectionText = MoveTemp(InKeySelectionText);
}

void UInputKeySelectorMapping::SetNoKeySpecifiedText(FText InNoKeySpecifiedText)
{
	if (MyInputKeySelector.IsValid())
	{
		MyInputKeySelector->SetNoKeySpecifiedText(InNoKeySpecifiedText);
	}
	NoKeySpecifiedText = MoveTemp(InNoKeySpecifiedText);
}

void UInputKeySelectorMapping::SetAllowModifierKeys( const bool bInAllowModifierKeys )
{
	if ( MyInputKeySelector.IsValid() )
	{
		MyInputKeySelector->SetAllowModifierKeys( bInAllowModifierKeys );
	}
	bAllowModifierKeys = bInAllowModifierKeys;
}

void UInputKeySelectorMapping::SetAllowGamepadKeys(const bool bInAllowGamepadKeys)
{
	if (MyInputKeySelector.IsValid())
	{
		MyInputKeySelector->SetAllowGamepadKeys(bInAllowGamepadKeys);
	}
	bAllowGamepadKeys = bInAllowGamepadKeys;
}

bool UInputKeySelectorMapping::GetIsSelectingKey() const
{
	return MyInputKeySelector.IsValid() ? MyInputKeySelector->GetIsSelectingKey() : false;
}

void UInputKeySelectorMapping::SetButtonStyle( const FButtonStyle* InButtonStyle )
{
	if ( MyInputKeySelector.IsValid() )
	{
		MyInputKeySelector->SetButtonStyle(InButtonStyle);
	}
	WidgetStyle = *InButtonStyle;
}

void UInputKeySelectorMapping::SetEscapeKeys(const TArray<FKey>& InKeys)
{
	if (MyInputKeySelector.IsValid())
	{
		MyInputKeySelector->SetEscapeKeys(InKeys);
	}
	EscapeKeys = InKeys;
}
#if WITH_EDITOR
const FText UInputKeySelectorMapping::GetPaletteCategory()
{
	return LOCTEXT("Advanced", "Advanced");
}
#endif

void UInputKeySelectorMapping::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyInputKeySelector->SetSelectedKey( SelectedKey );
	MyInputKeySelector->SetMargin( Margin );
	MyInputKeySelector->SetButtonStyle( &WidgetStyle );
	MyInputKeySelector->SetTextStyle( &TextStyle );
	MyInputKeySelector->SetKeySelectionText( KeySelectionText );
	MyInputKeySelector->SetNoKeySpecifiedText(NoKeySpecifiedText);
	MyInputKeySelector->SetAllowModifierKeys( bAllowModifierKeys );
	MyInputKeySelector->SetAllowGamepadKeys(bAllowGamepadKeys);
	MyInputKeySelector->SetEscapeKeys(EscapeKeys);
}

void UInputKeySelectorMapping::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyInputKeySelector.Reset();
}

TSharedRef<SWidget> UInputKeySelectorMapping::RebuildWidget()
{
	MyInputKeySelector = SNew(SInputKeySelector)
		.SelectedKey(SelectedKey)
		.Margin(Margin)
		.ButtonStyle(&WidgetStyle)
		.TextStyle(&TextStyle)
		.KeySelectionText(KeySelectionText)
		.NoKeySpecifiedText(NoKeySpecifiedText)
		.AllowModifierKeys(bAllowModifierKeys)
		.AllowGamepadKeys(bAllowGamepadKeys)
		.EscapeKeys(EscapeKeys)
		.OnKeySelected( BIND_UOBJECT_DELEGATE( SInputKeySelector::FOnKeySelected, HandleKeySelected ) )
		.OnIsSelectingKeyChanged( BIND_UOBJECT_DELEGATE( SInputKeySelector::FOnIsSelectingKeyChanged, HandleIsSelectingKeyChanged ) );
	return MyInputKeySelector.ToSharedRef();
}

void UInputKeySelectorMapping::HandleKeySelected(const FInputChord& InSelectedKey)
{
	SelectedKey = InSelectedKey;
	OnKeySelected.Broadcast(SelectedKey);
}

void UInputKeySelectorMapping::HandleIsSelectingKeyChanged()
{
	OnIsSelectingKeyChanged.Broadcast(this);
}

void UInputKeySelectorMapping::SetTextBlockVisibility(const ESlateVisibility InVisibility)
{
	if (MyInputKeySelector.IsValid())
	{
		EVisibility SlateVisibility = UWidget::ConvertSerializedVisibilityToRuntime(InVisibility);
		MyInputKeySelector->SetTextBlockVisibility(SlateVisibility);
	}
}

#undef LOCTEXT_NAMESPACE