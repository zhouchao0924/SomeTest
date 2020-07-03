#pragma once

#define CHECK_MESH(VariableName)\
if (Section.VariableName.Num() != CustomAssetCache.VariableName##Num)\
{\
	bIsModify = true;\
	CustomAssetCache.VariableName##Num = Section.VariableName.Num();\
}