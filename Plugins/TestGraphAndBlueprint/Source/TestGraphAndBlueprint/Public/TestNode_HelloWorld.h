// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "TestNode_HelloWorld.generated.h"

/**
 * 
 */
UCLASS()
class TESTGRAPHANDBLUEPRINT_API UTestNode_HelloWorld : public UEdGraphNode
{
	GENERATED_BODY()
public:

	UTestNode_HelloWorld();

	virtual void AllocateDefaultPins() override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};
