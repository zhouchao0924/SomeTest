#include "SimpleCodeLibrary.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

void USimpleCodeLibrary::SetLocation(UStaticMeshComponent *Target, FVector InLocation)
{
	if (Target)
	{
		Target->SetRelativeLocation(InLocation);
	}
}

void USimpleCodeLibrary::SetRotator(UStaticMeshComponent *Target, FRotator InRot)
{
	if (Target)
	{
		Target->SetRelativeRotation(InRot);
	}
}

void USimpleCodeLibrary::SetMesh(UStaticMeshComponent *Target, UStaticMesh *InMesh)
{
	if (Target)
	{
		Target->SetStaticMesh(InMesh);
	}
}

void USimpleCodeLibrary::SetMaterial(UStaticMeshComponent *Target, int32 Index, UMaterialInterface *InMaterial)
{
	if (Target)
	{
		Target->SetMaterial(Index, InMaterial);
	}
}

void USimpleCodeLibrary::SetMaterialColor(UStaticMeshComponent *Target, FName ColorName, FLinearColor InColor, int32 MatralIndex = 0)
{
	if (Target)
	{
		UMaterialInstance *Instance = Cast<UMaterialInstance>(Target->GetMaterial(MatralIndex));
		if (Instance)
		{

			FMaterialParameterInfo MaterialParameterInfo;
			MaterialParameterInfo.Name = ColorName;
			MaterialParameterInfo.Index = MatralIndex;

			FVectorParameterValue VectorParameterValue;
			VectorParameterValue.ParameterInfo = MaterialParameterInfo;
			VectorParameterValue.ParameterValue = InColor;
			Instance->VectorParameterValues.Add(VectorParameterValue);
		}
	}
}