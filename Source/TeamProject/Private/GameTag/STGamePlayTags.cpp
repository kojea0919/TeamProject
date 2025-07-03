// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTag/STGamePlayTags.h"

namespace STGamePlayTags
{

	// Input Tags
	UE_DEFINE_GAMEPLAY_TAG(Input_Move, "Input.Move")
	UE_DEFINE_GAMEPLAY_TAG(Input_Look, "Input.Look")
	UE_DEFINE_GAMEPLAY_TAG(Input_Hold, "Input.Hold")
	UE_DEFINE_GAMEPLAY_TAG(Input_Hold_Run, "Input.Hold.Run")
	UE_DEFINE_GAMEPLAY_TAG(Input_Hold_Aiming, "Input.Hold.Aiming")

	// player Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner, "Player.Runner")
	UE_DEFINE_GAMEPLAY_TAG(Player_Tagger, "Player.Tagger")

	// Runner Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_Equip, "Player.Runner.Ability.Equip")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_UnEquip, "Player.Runner.Ability.UnEquip")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_Equip_WaterGun, "Player.Runner.Ability.Equip.WaterGun")
	
	// Runner Weapon Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Weapon_Watergun, "Player.Runner.Weapon.WaterGun")

	// Runner Status Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Event_WaterGunEquip, "Player.Runner.Event.WaterGunEquip")
	
	// Runner Status Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Status_Running, "Player.Runner.Status.Running")
	

	// Input Ability
	UE_DEFINE_GAMEPLAY_TAG(Input_QAbility, "Input.QAbility")
    UE_DEFINE_GAMEPLAY_TAG(Input_EAbility, "Input.EAbility")

	// Object Tags
	UE_DEFINE_GAMEPLAY_TAG(Object_Door, "Object.Door")
	UE_DEFINE_GAMEPLAY_TAG(Object_WaterGun, "Object.WaterGun")
	UE_DEFINE_GAMEPLAY_TAG(Object_Blackboard, "Object.Blackboard")
	

}

