// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterFlotage.h"

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
	
}

// Called every frame
void AWaterFlotage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaterFlotage::Test()
{
	UE_LOG(LogTemp, Warning, TEXT("?????"));
	
}

