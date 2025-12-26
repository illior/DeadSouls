// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/DSAbilitySystemComponent.h"

void UDSAbilitySystemComponent::PressInputTag(const FGameplayTag InputTag)
{
	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			if (Spec.Ability)
			{
				Spec.InputPressed = true;
				if (Spec.IsActive())
				{
					if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
					{
						ServerSetInputPressed(Spec.Handle);
					}

					AbilitySpecInputPressed(Spec);

					TArray<UGameplayAbility*> Instances = Spec.GetAbilityInstances();
					if (!Instances.IsEmpty())
					{
						const FGameplayAbilityActivationInfo& ActivationInfo = Instances.Last()->GetCurrentActivationInfoRef();
						InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, ActivationInfo.GetActivationPredictionKey());	
					}				
				}
				else
				{
					TryActivateAbility(Spec.Handle);
				}
			}
		}
	}
}

void UDSAbilitySystemComponent::ReleaseInputTag(const FGameplayTag InputTag)
{
	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			Spec.InputPressed = false;
			if (Spec.Ability && Spec.IsActive())
			{
				if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
				{
					ServerSetInputReleased(Spec.Handle);
				}

				AbilitySpecInputReleased(Spec);

				TArray<UGameplayAbility*> Instances = Spec.GetAbilityInstances();
				if (!Instances.IsEmpty())
				{
					const FGameplayAbilityActivationInfo& ActivationInfo = Instances.Last()->GetCurrentActivationInfoRef();
					InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, ActivationInfo.GetActivationPredictionKey());
				}
			}
		}
	}
}
