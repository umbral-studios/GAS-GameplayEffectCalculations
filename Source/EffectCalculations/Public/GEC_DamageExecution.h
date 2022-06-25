// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEC_DamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class EFFECTCALCULATIONS_API UGEC_DamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:

	UGEC_DamageExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
