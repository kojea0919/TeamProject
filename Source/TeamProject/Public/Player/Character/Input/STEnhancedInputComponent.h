// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "STInputConfig.h"
#include "STEnhancedInputComponent.generated.h"


class STInputConfig;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API USTEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const USTInputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent,
		UserObject* ContextObject, CallbackFunc Func)
	{
		checkf(InInputConfig, TEXT("Input Config data asset is null, can not proceed with Binding"));
		if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
		{
			BindAction(FoundAction, TriggerEvent, ContextObject, Func);
		}
	}


	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
		void BindAbilityAction(USTInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,
							   ReleasedFuncType ReleasedFunc)
	{
		check(InputConfig);

		for (const FSTInputAction& Action : InputConfig->AbilityInputActions)
		{
			if (IsValid(Action.InputAction) && Action.InputTag.IsValid())
			{
				if (PressedFunc)
				{
					BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
				}
				if (ReleasedFunc)
				{
					BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
				}
			}
			
		}
	}
};
	

