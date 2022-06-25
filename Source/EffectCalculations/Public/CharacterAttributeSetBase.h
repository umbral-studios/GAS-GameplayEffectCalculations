// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)           \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class EFFECTCALCULATIONS_API UCharacterAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()
	
public:

    UPROPERTY(BlueprintReadOnly, Category = "Health")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Health)

    UPROPERTY(BlueprintReadOnly, Category = "Strength")
    FGameplayAttributeData Strength;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Strength)

    UPROPERTY(BlueprintReadOnly, Category = "Armor")
    FGameplayAttributeData Armor;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Armor)
};
