// Fill out your copyright notice in the Description page of Project Settings.


#include "NativeTags/Tags.h"
namespace MMGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Crouch, "InputTag.Crouch", "Crouch input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Jump, "InputTag.Jump", "Jump input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Attack, "InputTag.Attack", "Attack input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Block, "InputTag.Block", "Block input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SwapWeapon, "InputTag.SwapWeapon", "Swap Weapon input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SwapEquipping, "InputTag.SwapEquipping", "Swap Equipping input. If weapon is euipped pressing this key wil uneguip.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayAbility_Jump, "GameplayAbility.Jump", "Ability jump");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayAbility_Attack, "GameplayAbility.Attack", "Ability attack");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayAbility_Attack_Weapon, "GameplayAbility.Attack.Weapon", "Ability acctack with weapon");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayAbility_Block, "GameplayAbility.Block", "Ability block");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayAbility_Block_Weapon, "GameplayAbility.Block.Weapon", "Ability block with weapon");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayAbility_SwapEquipping, "GameplayAbility.SwapEquipping", "Ability swap equipping");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayAbility_SwapWeapon, "GameplayAbility.SwapWeapon", "Ability swap weapon");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayAbility_Behavior_SurvivesDeath, "GameplayAbility.Behavior.SurvivesDeath", "An ability with this type tag should not be canceled due to death.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Death, "GameplayEvent.Death", "Event that fires on death.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Reset, "GameplayEvent.Reset", "Event that fires once a player reset is executed.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_RequestReset, "GameplayEvent.RequestReset", "Event to request a player's pawn to be instantly replaced with a new one at a valid spawn location.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_SwapWeapon, "GameplayEvent.SwapWeapon", "Event that fires when a weapon should be swapped.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_SwapEquipping, "GameplayEvent.SwapEquipping", "Event that fires when a player wants to swap equping of weapon. Only for actiwation from code");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Damage, "SetByCaller.Damage", "SetByCaller tag used by damage gameplay effects.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Heal, "SetByCaller.Heal", "SetByCaller tag used by healing gameplay effects.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Crouching, "Status.Crouching", "Target is crouching.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death, "Status.Death", "Target has the death status.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death_Dying, "Status.Death.Dying", "Target has begun the death process.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death_Dead, "Status.Death.Dead", "Target has finished the death process.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Weapon_Equipped, "Status.Weapon.Equipped", "Target has equipped weapon.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Weapon_Equipping, "Status.Weapon.Equipping", "Target is equipping weapon.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Attacking, "Status.Attacking", "Target is attacking.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Blocking, "Status.Blocking", "Target is blocking.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Gameplay_DamageImmunity, "Gameplay.DamageImmunity", "Target has immunity, for example when is blocking. ");

	// These are mapped to the movement modes inside GetMovementModeTagMap()
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Walking, "Movement.Mode.Walking", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_NavWalking, "Movement.Mode.NavWalking", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Falling, "Movement.Mode.Falling", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Swimming, "Movement.Mode.Swimming", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Flying, "Movement.Mode.Flying", "Default Character movement tag");

	const TMap<uint8, FGameplayTag> MovementModeTagMap =
	{
		{ MOVE_Walking, Movement_Mode_Walking },
		{ MOVE_NavWalking, Movement_Mode_NavWalking },
		{ MOVE_Falling, Movement_Mode_Falling },
		{ MOVE_Swimming, Movement_Mode_Swimming },
		{ MOVE_Flying, Movement_Mode_Flying }
	};
}


