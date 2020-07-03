// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MeshlImport.h"
#include "ProceduralMeshComponent.h"
#include "CustomAsset.generated.h"


/**
 * 
 */

struct FCustomAssetCache
{
	int32 VerticesNum;
	int32 TrianglesNum;
	int32 NormalsNum;
	int32 UV0Num;
	int32 UV1Num;
	int32 UV2Num;
	int32 TangentsNum;

	FCustomAssetCache()
		:VerticesNum(0)
		, TrianglesNum(0)
		, NormalsNum(0)
		, UV0Num(0)
		, UV1Num(0)
		, UV2Num(0)
		, TangentsNum(0)
	{}
};

USTRUCT(BlueprintType)
struct FCustomMeshSection
{
	GENERATED_BODY()

	//点
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Section")
	TArray<FVector> Vertices;

	//三角形
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Section")
	TArray<int32> Triangles;

	//法线
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Section")
	TArray<FVector> Normals;

	//UV
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Section")
	TArray<FVector2D> UV0;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Section")
	TArray<FVector2D> UV1;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Section")
	TArray<FVector2D> UV2;

	//切线
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Section")
	TArray<FProcMeshTangent> Tangents;
};

USTRUCT(BlueprintType)
struct FCustomMeshLOD
{
	GENERATED_BODY()

	//LOD
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Section")
	TArray<FCustomMeshSection> MeshLOD;
};

UCLASS()
class TESTASSETDATA_API UCustomAsset : public UObject
{
	GENERATED_BODY()
	
public:

	//Mesh
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mesh")
	TArray<FCustomMeshLOD> MeshData;

	//模型读取路径
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "CustomAsset")
	FMeshlImport  MeshlImport;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Material")
	TArray<UMaterialInterface*> MaterialInterface;

	bool IsModify();

	UCustomAsset();

	bool IsModifyMaterial() { return bModifyMaterial; }

	void EndModifyMaterial() { bModifyMaterial = false; }

	void CreateBox();

	void ClearMesh();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:

	FCustomAssetCache CustomAssetCache;
	bool bModifyMaterial;
};
