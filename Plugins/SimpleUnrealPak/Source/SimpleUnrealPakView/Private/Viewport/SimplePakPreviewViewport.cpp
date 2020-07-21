#include "Viewport/SimplePakPreviewViewport.h"
#include "Components/SkyLightComponent.h"
#include "AdvancedPreviewScene.h"
#include "Components/DirectionalLightComponent.h"
#include "Atmosphere/AtmosphericFogComponent.h"
#include "Viewport/PakViewEditorViewportClient.h"
#include "SimpleUnrealPakViewType.h"

void SimplePakPreviewViewport::Construct(const FArguments& InArgs)
{
	{
		FAdvancedPreviewScene::ConstructionValues ViewConstructionValues;
		ViewConstructionValues.bCreatePhysicsScene = false;
		ViewConstructionValues.LightBrightness = 3;
		ViewConstructionValues.SkyBrightness = 1;

		PreviewScene = MakeShareable(new FAdvancedPreviewScene(ViewConstructionValues));
		PreviewScene->SetFloorVisibility(false);

		PreviewScene->SetFloorOffset(-100000);
	}

	Skylight = NewObject<USkyLightComponent>();
	PreviewScene->AddComponent(Skylight, FTransform::Identity);

	AtmosphericFog = NewObject<UAtmosphericFogComponent>();
	PreviewScene->AddComponent(AtmosphericFog, FTransform::Identity);

	PreviewScene->DirectionalLight->SetMobility(EComponentMobility::Movable);
	PreviewScene->DirectionalLight->CastShadows = true;
	PreviewScene->DirectionalLight->CastStaticShadows = true;
	PreviewScene->DirectionalLight->CastDynamicShadows = true;
	PreviewScene->DirectionalLight->SetIntensity(3);

	//Ìí¼ÓÄ£ÐÍ
	if (GMeshComponent)
	{
		GMeshComponent->SetRelativeScale3D(FVector(3.f, 3.f, 1.f));
		GMeshComponent->SetVisibility(true);
		PreviewScene->AddComponent(GMeshComponent, FTransform::Identity);
	}

	if (GProceduralMeshComponent)
	{
		GProceduralMeshComponent->SetRelativeScale3D(FVector(3.f, 3.f, 1.f));
		GProceduralMeshComponent->SetVisibility(true);
		PreviewScene->AddComponent(GProceduralMeshComponent, FTransform::Identity);
	}

	SEditorViewport::Construct(SEditorViewport::FArguments());

	ViewProtWorld = PreviewScene->GetWorld();
}

TSharedRef<FEditorViewportClient> SimplePakPreviewViewport::MakeEditorViewportClient()
{
	EditorViewportClient = MakeShareable(new FPakViewEditorViewportClient(*PreviewScene));
	EditorViewportClient->SetRealtime(true);

	return EditorViewportClient.ToSharedRef();
}

