// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SynthComponent.h"
#include "GameFramework/Pawn.h"
#include "OdinLipSyncActor.generated.h"

UCLASS()
class OVRLIPSYNCDEMO_API AOdinLipSyncActor : public APawn, public IAudioBufferListener
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AOdinLipSyncActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UCapsuleComponent *CollisionComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UOdinSynthComponent *SynthComp;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UOVRLipSyncActorComponent *LipSyncComp;

	virtual void OnGeneratedBuffer(const float *AudioBuffer, const int32 NumSamples, const int32 NumChannels) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

private:
	TArray<uint8> ConvertToMonoPCM(const float* interleavedBuffer, size_t bufferLen);
};
