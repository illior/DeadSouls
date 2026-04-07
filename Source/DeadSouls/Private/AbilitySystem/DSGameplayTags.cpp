// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/DSGameplayTags.h"

namespace DSGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayAbility_Interact, "GameplayAbility.Interact", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayAbility_Sprint, "GameplayAbility.Sprint", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayAbility_ChangeWeapon, "GameplayAbility.ChangeWeapon", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayAbility_Aim, "GameplayAbility.Aim", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayAbility_Fire, "GameplayAbility.Fire", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_State_Sprint, "Character.State.Sprint", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_State_Aim, "Character.State.Aim", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_State_DisableMovement, "Character.State.DisableMovement", "");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Death, "GameplayEvent.Death", "Event that fires on death.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_ChangeWeapon, "GameplayEvent.ChangeWeapon", "");
}