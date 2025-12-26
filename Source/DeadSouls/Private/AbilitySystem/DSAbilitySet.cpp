// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/DSAbilitySet.h"
#include "AbilitySystem/DSAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/DSGameplayAbility.h"
#include "AttributeSet.h"

void FDSAbilitySetGrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FDSAbilitySetGrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FDSAbilitySetGrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	if (IsValid(Set))
	{
		GrantedAttributeSets.Add(Set);
	}
}

void FDSAbilitySetGrantedHandles::TakeFromAbilitySystem(UDSAbilitySystemComponent* AbilitySystem)
{
	if (!IsValid(AbilitySystem))
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystem->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystem->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		AbilitySystem->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

FDSAbilitySetGrantedHandles UDSAbilitySet::GiveToAbilitySystem(UDSAbilitySystemComponent* AbilitySystem) const
{
	FDSAbilitySetGrantedHandles GrantedHandles;
	
	if (!IsValid(AbilitySystem))
	{
		return GrantedHandles;
	}
	
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); AbilityIndex++)
	{
		const FDSAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			//UE_LOG(LogDSAbilitySystem, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UDSGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UDSGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystem->GiveAbility(AbilitySpec);

		GrantedHandles.AddAbilitySpecHandle(AbilitySpecHandle);
	}
	
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); EffectIndex++)
	{
		const FDSAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			//UE_LOG(LogDSAbilitySystem, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = AbilitySystem->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, AbilitySystem->MakeEffectContext());

		GrantedHandles.AddGameplayEffectHandle(GameplayEffectHandle);
	}
	
	for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); SetIndex++)
	{
		const FDSAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[SetIndex];

		if (!IsValid(SetToGrant.AttributeSet))
		{
			//UE_LOG(LogDSAbilitySystem, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* NewSet = NewObject<UAttributeSet>(AbilitySystem->GetOwner(), SetToGrant.AttributeSet);
		AbilitySystem->AddAttributeSetSubobject(NewSet);

		GrantedHandles.AddAttributeSet(NewSet);
	}
	
	return GrantedHandles;
}
