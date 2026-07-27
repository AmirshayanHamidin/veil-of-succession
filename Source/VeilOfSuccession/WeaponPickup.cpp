// Veil of Succession - fictional political thriller. All characters fictitious.
#include "WeaponPickup.h"
#include "SpyCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AWeaponPickup::AWeaponPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(Root);
	GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded()) { GunMesh->SetStaticMesh(Cube.Object); }

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (Mat.Succeeded()) { GunMesh->SetMaterial(0, Mat.Object); }

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Root);
	Trigger->SetSphereRadius(140.f);
	Trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();

	// Rifle reads longer and darker than the pistol.
	const bool bRifle = (WeaponType == EVeilWeapon::AssaultRifle);
	GunMesh->SetRelativeScale3D(bRifle ? FVector(0.7f, 0.08f, 0.10f) : FVector(0.28f, 0.07f, 0.16f));
	GunMesh->SetRelativeLocation(FVector(0.f, 0.f, 26.f));

	if (UMaterialInterface* Base = GunMesh->GetMaterial(0))
	{
		MID = UMaterialInstanceDynamic::Create(Base, this);
		MID->SetVectorParameterValue(TEXT("Color"),
			bRifle ? FLinearColor(0.9f, 0.55f, 0.1f) : FLinearColor(0.9f, 0.9f, 0.95f));
		GunMesh->SetMaterial(0, MID);
	}

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AWeaponPickup::OnOverlap);
}

void AWeaponPickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	BobTime += DeltaSeconds;
	GunMesh->SetRelativeLocation(FVector(0.f, 0.f, 26.f + FMath::Sin(BobTime * 2.5f) * 5.f));
	GunMesh->AddLocalRotation(FRotator(0.f, DeltaSeconds * 45.f, 0.f));
}

void AWeaponPickup::OnOverlap(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (ASpyCharacter* Player = Cast<ASpyCharacter>(OtherActor))
	{
		Player->GiveWeapon(WeaponType, AmmoAmount);
		Destroy();
	}
}
