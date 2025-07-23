// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTag/STGamePlayTags.h"

namespace STGamePlayTags
{

	// Input Tags
	UE_DEFINE_GAMEPLAY_TAG(Input_Move, "Input.Move")
	UE_DEFINE_GAMEPLAY_TAG(Input_Look, "Input.Look")
	UE_DEFINE_GAMEPLAY_TAG(Input_Jump, "Input.Jump")
	UE_DEFINE_GAMEPLAY_TAG(Input_Hold, "Input.Hold")
	UE_DEFINE_GAMEPLAY_TAG(Input_Use, "Input.Use")
	UE_DEFINE_GAMEPLAY_TAG(Input_Shoot, "Input.Shoot")
	UE_DEFINE_GAMEPLAY_TAG(Input_UnEquip, "Input.UnEquip")
	UE_DEFINE_GAMEPLAY_TAG(Input_Hold_Run, "Input.Hold.Run")
	UE_DEFINE_GAMEPLAY_TAG(Input_Hold_Aiming, "Input.Hold.Aiming")
	UE_DEFINE_GAMEPLAY_TAG(Input_Hold_Crunch, "Input.Hold.Crunch")
	UE_DEFINE_GAMEPLAY_TAG(Input_Shoot_WaterGun, "Input.Shoot.WaterGun")
	UE_DEFINE_GAMEPLAY_TAG(Input_Recharge_WaterGun, "Input.Recharge.WaterGun")

	// player Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner, "Player.Runner")
	UE_DEFINE_GAMEPLAY_TAG(Player_Tagger, "Player.Tagger")

	// Runner Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_Equip, "Player.Runner.Ability.Equip")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_UnEquip, "Player.Runner.Ability.UnEquip")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_Equip_WaterGun, "Player.Runner.Ability.Equip.WaterGun")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_Recharge_WaterGun, "Player.Runner.Ability.Recharge.WaterGun")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_Death, "Player.Runner.Ability.Death")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_HitReact, "Player.Runner.Ability.HitReact")

	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_Shoot_WaterGun, "Player.Runner.Ability.Shoot.WaterGun")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_Use_WaterGun, "Player.Runner.Ability.Use.WaterGun")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_Aiming, "Player.Runner.Ability.Aiming")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Ability_Crunch, "Player.Runner.Ability.Crunch")

	// Tagger Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Tagger_Ability_Crunch, "Player.Tagger.Ability.Crunch")
	UE_DEFINE_GAMEPLAY_TAG(Player_Tagger_Ability_HitReact, "Player.Tagger.Ability.HitReact")
	
	// Runner Weapon Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Weapon_Watergun, "Player.Runner.Weapon.WaterGun")

	// Runner Event Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Event_WaterGunEquip, "Player.Runner.Event.WaterGunEquip")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Event_WaterGunUnEquip, "Player.Runner.Event.WaterGunUnEquip")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Event_Use, "Player.Runner.Event.Use")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Event_HitReact, "Player.Runner.Event.HitReact")

	// Tagger Event Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Tagger_Event_HitReact, "Player.Tagger.Event.HitReact")
	
	// Runner Status Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Status_Running, "Player.Runner.Status.Running")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Status_Aiming, "Player.Runner.Status.Aiming")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Status_Using, "Player.Runner.Status.Using")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Status_Crouching, "Player.Runner.Status.Crouching")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Status_Equipping, "Player.Runner.Status.Equipping")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Status_UnEquipping, "Player.Runner.Status.UnEquipping")
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Status_Dead, "Player.Runner.Status.Dead")

	// Tagger Status Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Tagger_Status_Crouching, "Player.Tagger.Status.Crouching")
	UE_DEFINE_GAMEPLAY_TAG(Player_Tagger_Status_Running, "Player.Tagger.Status.Running")

	// Input Ability
	UE_DEFINE_GAMEPLAY_TAG(Input_QAbility, "Input.QAbility")
    UE_DEFINE_GAMEPLAY_TAG(Input_EAbility, "Input.EAbility")

	// Object Tags
	UE_DEFINE_GAMEPLAY_TAG(Object_Actor_None, "Object.Actor.None")
	UE_DEFINE_GAMEPLAY_TAG(Object_Actor_Door, "Object.Actor.Door")
	UE_DEFINE_GAMEPLAY_TAG(Object_Actor_WaterGun, "Object.Actor.WaterGun")
	UE_DEFINE_GAMEPLAY_TAG(Object_Actor_Blackboard, "Object.Actor.Blackboard")
	UE_DEFINE_GAMEPLAY_TAG(Object_Actor_Graffiti, "Object.Actor.Graffiti")
	UE_DEFINE_GAMEPLAY_TAG(Object_Actor_Hammer, "Object.Actor.Hammer")
	UE_DEFINE_GAMEPLAY_TAG(Object_Actor_WaterDispenser, "Object.Actor.WaterDispenser")

	// Object Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Object_Ability_Splash, "Object.Ability.Splash")
	

	// Event Tag
	UE_DEFINE_GAMEPLAY_TAG(Event_OnSplashHit, "Event.OnSplashHit")
	UE_DEFINE_GAMEPLAY_TAG(Event_OnHammerHit, "Event.OnHammerHit")
	UE_DEFINE_GAMEPLAY_TAG(Event_OnWaterGunRecharge, "Event.OnWaterGunRecharge")
}

