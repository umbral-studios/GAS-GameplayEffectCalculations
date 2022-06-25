#include "GEC_DamageExecution.h"
#include "CharacterAttributeSetBase.h"
#include "AbilitySystemComponent.h"

// Helper struct for fetching the stats necessary for the calculation.
struct FDamageStatics
{
	// Capturedef declarations for attributes.
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	// Default constructor.
	FDamageStatics()
	{
		// Capturedef definitions for attributes. 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, Strength, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, Damage, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, Health, Target, false);
	}
};

// Static helper function to quickly fetch the damage capture attributes.
static const FDamageStatics& DamageStatics()
{
	static FDamageStatics DmgStatics;
	return DmgStatics;
}

// Add all the capturedef definitions to the list of relevant attributes to capture
UGEC_DamageExecution::UGEC_DamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().StrengthDef);
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}

// Do the damage calculations and modify health accordingly
void UGEC_DamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Obtain references to the target and source Ability System Component. Each copy of a Gameplay Effect will have one of each.
	UAbilitySystemComponent* TargetABSC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetABSC ? TargetABSC->GetAvatarActor() : nullptr;

	UAbilitySystemComponent* SourceABSC = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceABSC ? SourceABSC->GetAvatarActor() : nullptr;

	// Get the owning GameplayEffect Spec so that you can use its variables and tags.
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Aggregator Evaluate Parameters used during the attribute capture.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	//Capturing the weapon damage.

	float BaseDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, BaseDamage);
	BaseDamage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);

	//Capturing Armor
	float Armor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, Armor);

	// Capturing Strength
	float Strength = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().StrengthDef, EvaluationParameters, Strength);

	// Performing the actual damage calculation. 
	float DamageDone = (BaseDamage * Strength) * (1 - (Armor / 100));

	if (DamageDone < 0.f)
	{
		DamageDone = 0.0f;
	}

	// Final execution output. We can add more than one AddOutputModifier to change multiple parameters at a time based on more complicated calculations. Here we apply -DamageDone to to Health. Health itself is clamped in the AttributeSet.
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, -DamageDone));
}