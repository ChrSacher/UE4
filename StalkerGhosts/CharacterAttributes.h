// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Buff.h"
#include "CharacterAttributes.generated.h"

class UCharacterAttributes;

UCLASS()
class UBaseAttribute : public UObject
{
	GENERATED_BODY()
public:
	UBaseAttribute();
	~UBaseAttribute();

	
protected:
	friend class UCharacterAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
		float baseValue = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
		float finalValue = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
		TArray<UBuff*> currentBuffs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	UCharacterAttributes* parent;
public:
	float getRaw();
	void setRaw(float x);
	void addRaw(float x);
	void calculate();
	float getFinal();

	void addBuff(UBuff* buff);
	void removeBuff(UBuff* buff);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	AttributeType type;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STALKERGHOSTS_API UCharacterAttributes : public UActorComponent
{
	friend class UBaseAttribut;
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterAttributes();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
protected:
	UPROPERTY()
		TMap<AttributeType, UBaseAttribute*> attributes;

	UPROPERTY()
		UBaseAttribute* nullAttrib;
public:

	UBaseAttribute* getAttrib(AttributeType w);
	void addBuff(UBuff* buff);

	void removeBuff(UBuff* buff);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		bool isStaminaRegenerable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		float currentSpeed = 100;
};
