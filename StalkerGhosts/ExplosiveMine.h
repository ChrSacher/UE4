// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Grenade.h"
#include "GameFramework/Actor.h"
#include "ExplosiveMine.generated.h"

UCLASS()
class STALKERGHOSTS_API AExplosiveMine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosiveMine();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* stepOnSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* stepOffSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		 USphereComponent* triggerLocation; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<AGrenade> explosiveTemplate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UStaticMeshComponent* groundMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
		bool isAboutToExplode = false;
	UFUNCTION(BlueprintCallable, Category = "Event")
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void explode();
};
