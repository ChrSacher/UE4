// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Buff.h"
#include "CharacterAttributes.generated.h"

class UCharacterAttributes;

USTRUCT(Blueprintable, BlueprintType)
struct STALKERGHOSTS_API FCharacterAttributesInit
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float baseValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		AttributeType type;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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
	UFUNCTION(BlueprintCallable, Category = "Event")
		float getRaw();
	UFUNCTION(BlueprintCallable, Category = "Event")
		void setRaw(float x);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void addRaw(float x);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void calculate();
	UFUNCTION(BlueprintCallable, Category = "Event")
		float getFinal();
	UFUNCTION(BlueprintCallable, Category = "Event")
		void addBuff(UBuff* buff);
	UFUNCTION(BlueprintCallable, Category = "Event")
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TArray<UBaseAttribute*> attributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		TArray<FCharacterAttributesInit> initVars;

	UPROPERTY()
		UBaseAttribute* nullAttrib;
public:
	UFUNCTION(BlueprintCallable, Category = "Event")
		UBaseAttribute* getAttrib(AttributeType w);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void addBuff(UBuff* buff);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void removeBuff(UBuff* buff);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		bool isStaminaRegenerable = true;

	void setup();
	
};
