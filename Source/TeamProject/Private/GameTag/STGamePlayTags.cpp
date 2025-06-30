// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTag/STGamePlayTags.h"

namespace STGamePlayTags
{
	// player Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner, "Player.Runner")
	UE_DEFINE_GAMEPLAY_TAG(Player_Tagger, "Player.Tagger")
	
	// Input Tags
	UE_DEFINE_GAMEPLAY_TAG(Input_Move, "Input.Move")
    UE_DEFINE_GAMEPLAY_TAG(Input_Look, "Input.Look")
	UE_DEFINE_GAMEPLAY_TAG(Input_Hold, "Input.Hold")
	UE_DEFINE_GAMEPLAY_TAG(Input_Hold_Run, "Input.Hold.Run")

		
	// Runner Status Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Runner_Status_Running, "Player.Runner.Status.Running")
	

	// Input Ability
	UE_DEFINE_GAMEPLAY_TAG(Input_QAbility, "Input.QAbility")
    UE_DEFINE_GAMEPLAY_TAG(Input_EAbility, "Input.EAbility")
	
	

}

