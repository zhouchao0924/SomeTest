#include "Commands/BTCommands.h"
#include "Style/BlueprintToolGraphStyle.h"

#define LOCTEXT_NAMESPACE "BTditorCommands"

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
	UI_COMMAND(Help, "Help", "About BT help Doc", EUserInterfaceActionType::ToggleButton, FInputGesture());
	UI_COMMAND(Compile, "Compile", "Compilation blueprint", EUserInterfaceActionType::ToggleButton, FInputGesture());
	UI_COMMAND(Run, "Run", "Run blueprint.", EUserInterfaceActionType::ToggleButton, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
