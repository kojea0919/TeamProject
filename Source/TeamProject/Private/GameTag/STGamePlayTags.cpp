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
	UE_DEFINE_GAMEPLAY_TAG(Input_Hold_Crunch, "Input.Hold.Crunch")

	// player Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner, "Player.Runner")
	UE_DEFINE_GAMEPLAY_TAG(Player_Tagger, "Player.Tagger")

	// Runner Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_Equip, "Player.Runner.Ability.Equip")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_UnEquip, "Player.Runner.Ability.UnEquip")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_Equip_WaterGun, "Player.Runner.Ability.Equip.WaterGun")

	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_Shoot_WaterGun, "Player.Runner.Ability.Shoot.WaterGun")
	
	// Runner Weapon Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Weapon_Watergun, "Player.Runner.Weapon.WaterGun")

	// Runner Status Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Event_WaterGunEquip, "Player.Runner.Event.WaterGunEquip")
	
	// Runner Status Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Status_Running, "Player.Runner.Status.Running")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Status_Aiming, "Player.Runner.Status.Aiming")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Status_Crouching, "Player.Runner.Status.Crouching")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Status_Equipping, "Player.Runner.Status.Equipping")
	

	// Input Ability
	UE_DEFINE_GAMEPLAY_TAG(Input_QAbility, "Input.QAbility")
    UE_DEFINE_GAMEPLAY_TAG(Input_EAbility, "Input.EAbility")

	// Object Tags
	UE_DEFINE_GAMEPLAY_TAG(Object_Actor_Door, "Object.Actor.Door")
	UE_DEFINE_GAMEPLAY_TAG(Object_Actor_WaterGun, "Object.Actor.WaterGun")
	UE_DEFINE_GAMEPLAY_TAG(Object_Actor_Blackboard, "Object.Actor.Blackboard")
	UE_DEFINE_GAMEPLAY_TAG(Object_Actor_Graffiti, "Object.Actor.Graffiti")
	UE_DEFINE_GAMEPLAY_TAG(Object_Actor_Hammer, "Object.Actor.Hammer")

	// Object Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Object_Ability_Splash, "Object.Ability.Splash")
	

	// Event Tag
	UE_DEFINE_GAMEPLAY_TAG(Event_OnSplashHit, "Event.OnSplashHit")
	UE_DEFINE_GAMEPLAY_TAG(Event_OnHammerHit, "Event.OnHammerHit")
}

