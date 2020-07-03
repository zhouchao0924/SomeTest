// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomAsset.h"
#include "AssetCore.h"

bool UCustomAsset::IsModify()
{
	bool bIsModify = false;

	for (FCustomMeshLOD &Lod : MeshData)
	{
		for (FCustomMeshSection &Section :Lod.MeshLOD)
		{
			CHECK_MESH(Vertices)
			CHECK_MESH(Triangles)
			CHECK_MESH(Normals)
			CHECK_MESH(UV0)
			CHECK_MESH(UV1)
			CHECK_MESH(UV2)
			CHECK_MESH(Tangents)
		}
	}

	return bIsModify;
}

UCustomAsset::UCustomAsset()
	:bModifyMaterial(true)
{
	CreateBox();
}

void UCustomAsset::CreateBox()
{
	//点位置
	//Vertices.Add(FVector(0.f, 0.f, 0.f));
	//Vertices.Add(FVector(0.f,100.f,0.f));
	//Vertices.Add(FVector(100.f, 100.f, 0.f));
	//Vertices.Add(FVector(100.f, 0.f, 0.f));
	//Vertices.Add(FVector(0.f, 0.f, 100.f));
	//Vertices.Add(FVector(0.f, 100.f, 100.f));
	//Vertices.Add(FVector(100.f, 100.f, 100.f));
	//Vertices.Add(FVector(100.f, 0.f, 100.f));

	////三角形
	//Triangles.Add(0); Triangles.Add(1); Triangles.Add(5);
	//Triangles.Add(0); Triangles.Add(5); Triangles.Add(4);

	//Triangles.Add(1); Triangles.Add(2); Triangles.Add(6);
	//Triangles.Add(1); Triangles.Add(6); Triangles.Add(5);

	//Triangles.Add(2); Triangles.Add(3); Triangles.Add(7);
	//Triangles.Add(2); Triangles.Add(7); Triangles.Add(6);

	//Triangles.Add(3); Triangles.Add(0); Triangles.Add(4);
	//Triangles.Add(3); Triangles.Add(4); Triangles.Add(7);

	//Triangles.Add(5); Triangles.Add(6); Triangles.Add(7);
	//Triangles.Add(5); Triangles.Add(7); Triangles.Add(4);

	//Triangles.Add(1); Triangles.Add(3); Triangles.Add(2);
	//Triangles.Add(1); Triangles.Add(0); Triangles.Add(3);
}

void UCustomAsset::ClearMesh()
{
	/*Vertices.Empty();
	Triangles.Empty();*/
}
#if WITH_EDITOR
void UCustomAsset::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property &&																		//判断修改的变量存不存在
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UCustomAsset, MaterialInterface)   //判断是不是修改的这个变量
		)
	{
		bModifyMaterial = true;
	}
}
#endif