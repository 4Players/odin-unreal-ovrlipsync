// Fill out your copyright notice in the Description page of Project Settings.


#include "OdinLipSyncActor.h"

#include "OVRLipSyncLiveActorComponent.h"
#include "OdinSynthComponent.h"
#include "Components/CapsuleComponent.h"

const float kMaxInt16AsFloat = 32767.0f;


// Sets default values
AOdinLipSyncActor::AOdinLipSyncActor()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>("RootComponent");
	RootComponent = CollisionComp;
	SynthComp =  CreateDefaultSubobject<UOdinSynthComponent>(TEXT("SynthComponent"));
	SynthComp->SetupAttachment(RootComponent);
	LipSyncComp = CreateDefaultSubobject<UOVRLipSyncActorComponent>(TEXT("LipSyncComponent"));
}

// Called when the game starts or when spawned
void AOdinLipSyncActor::BeginPlay()
{
	Super::BeginPlay();
	SynthComp->AddAudioBufferListener(this);
}

void AOdinLipSyncActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	SynthComp->RemoveAudioBufferListener(this);
}

void AOdinLipSyncActor::OnGeneratedBuffer(const float *AudioBuffer, const int32 NumSamples, const int32 NumChannels)
{
	TArray<uint8> monoPcm = ConvertToMonoPCM(AudioBuffer, NumSamples);
	LipSyncComp->FeedAudio(monoPcm);
}


// Called every frame
void AOdinLipSyncActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOdinLipSyncActor::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

TArray<uint8> AOdinLipSyncActor::ConvertToMonoPCM(const float *interleavedBuffer, size_t bufferLen)
{
	TArray<uint8> monoPcm;
	monoPcm.Reserve(bufferLen / 2 * sizeof(int16_t));
    
	for(size_t i = 0; i < bufferLen; i += 2) {
		float left = interleavedBuffer[i];
		float right = interleavedBuffer[i + 1];
		float mono = (left + right) / 2.0f;  // convert to mono

		int16_t pcmValue = static_cast<int16_t>(mono * kMaxInt16AsFloat);  // scale float to 16 bit integer
		monoPcm.Append(reinterpret_cast<uint8*>(&pcmValue), sizeof(int16_t));
	}
    
	return monoPcm;
}

