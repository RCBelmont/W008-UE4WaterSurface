// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterFlotage.h"
#include "WaterEvent.h"
#include "Engine/Engine.h"

// Sets default values
AWaterFlotage::AWaterFlotage()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWaterFlotage::BeginPlay()
{
	Super::BeginPlay();
	InitPos = this->GetActorLocation();
	TestEventHandle = WaterEvent::GetInstance()->OnTestFinish().AddLambda([this](FVector pos)
	{
	
			this->Offset = pos;

		
			
			
			
	});
}

void AWaterFlotage::BeginDestroy()
{
	Super::BeginDestroy();
	
	WaterEvent::GetInstance()->OnTestFinish().Remove(TestEventHandle);
}

// Called every frame
void AWaterFlotage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetActorLocation(InitPos + Offset * 0.8);
}

void AWaterFlotage::Test()
{
	
}
