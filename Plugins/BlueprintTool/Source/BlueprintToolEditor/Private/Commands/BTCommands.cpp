#include "Commands/BTCommands.h"
#include "Style/BlueprintToolGraphStyle.h"

#define LOCTEXT_NAMESPACE "BTditorCommands"

/** Construct a set of commands; call this from your custom commands class. */
//TCommands(const FName InContextName, 
//	const FText& InContextDesc,
//	const FName InContextParent, 
//	const FName InStyleSetName)
//	: FBindingContext(InContextName, InContextDesc, InContextParent, InStyleSetName)
//{
//}

FBTCommands::FBTCommands()
	:TCommands<FBTCommands>(
		TEXT("BTditor"),
		LOCTEXT("BTditor", "BTditor Commands"),
		NAME_None,
		FBlueprintToolGraphStyle::GetStyleSetName())
{

}

void FBTCommands::RegisterCommands()
{
	//#define UI_COMMAND( CommandId, FriendlyName, InDescription, CommandType, InDefaultChord, ... )
	UI_COMMAND(Help, "Help", "About BT help Doc", EUserInterfaceActionType::ToggleButton, FInputGesture());
	UI_COMMAND(Compile, "Compile", "Compilation blueprint", EUserInterfaceActionType::ToggleButton, FInputGesture());
	UI_COMMAND(Run, "Run", "Run blueprint.", EUserInterfaceActionType::ToggleButton, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
