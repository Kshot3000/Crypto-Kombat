// Copyright CryptoKombat. Phase 1 scaffold.
// Fighter state, move data, and roster definitions (original parody names only).

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FighterTypes.generated.h"

/** Simple combat state machine for Phase 1. */
UENUM(BlueprintType)
enum class EFighterState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Walk		UMETA(DisplayName = "Walk"),
	Jump		UMETA(DisplayName = "Jump"),
	Attack		UMETA(DisplayName = "Attack"),
	Hitstun		UMETA(DisplayName = "Hitstun"),
	Block		UMETA(DisplayName = "Block"),
	Special		UMETA(DisplayName = "Special"),
	KO			UMETA(DisplayName = "KO")
};

/** Attack slot for normals / specials / finishers. */
UENUM(BlueprintType)
enum class EAttackSlot : uint8
{
	LightPunch,
	HeavyPunch,
	LightKick,
	HeavyKick,
	Special1,
	Special2,
	Fatality
};

USTRUCT(BlueprintType)
struct FMoveDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	FName MoveId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	EAttackSlot Slot = EAttackSlot::LightPunch;

	/** Base damage before block / Moon Meter modifiers. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float Damage = 10.f;

	/** Hitstun applied to victim (seconds). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float HitstunDuration = 0.35f;

	/** Attacker recovery / commit time (seconds). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float ActiveDuration = 0.25f;

	/** Moon Meter cost (0 for normals). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float MoonCost = 0.f;

	/** Local-space box half-extent for the attack trace. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	FVector TraceHalfExtent = FVector(40.f, 30.f, 40.f);

	/** Forward offset from fighter origin for the hit box center. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float TraceForwardOffset = 80.f;
};

USTRUCT(BlueprintType)
struct FFatalityConcept
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fatality")
	FName FatalityId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fatality")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fatality")
	FText FlavorText;

	/** Input hint shown when opponent is in "Finish Him/Her" range (Phase 1 text only). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fatality")
	FString InputHint;
};

USTRUCT(BlueprintType)
struct FFighterDefinition : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fighter")
	FName FighterId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fighter")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fighter")
	FText ArchetypeTagline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fighter")
	FText Bio;

	/** Stylized accent color for HUD / capsule tint (no real logos). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fighter")
	FLinearColor AccentColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fighter")
	float MaxHealth = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fighter")
	float MaxMoonMeter = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fighter")
	float WalkSpeed = 450.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fighter")
	TArray<FMoveDefinition> Normals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fighter")
	TArray<FMoveDefinition> Specials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fighter")
	TArray<FFatalityConcept> Fatalities;
};

/** Runtime helpers to seed the default parody roster (no trademarks). */
struct FCryptoRosterFactory
{
	static FFighterDefinition MakeSatoshiShadow();
	static FFighterDefinition MakeVitalSpark();
	static FFighterDefinition MakeCZChain();
	static FFighterDefinition MakeBrianCoin();
	static FFighterDefinition MakeSolFlash();
	static FFighterDefinition MakeDotWeaver();
	static FFighterDefinition MakeHaydenSwap();
	static FFighterDefinition MakeTronBlaze();
	static FFighterDefinition MakeArthurPerp();
	static FFighterDefinition MakeCharlesEpoch();
	static TArray<FFighterDefinition> MakeDefaultRoster();
};
