// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ObjectInfo/ObjectInfoSection.h"

#include "Components/TextBlock.h"
#include "Map/Object/Actor/BaseObject.h"

void UObjectInfoSection::InitObjectInfo(ABaseObject* Object)
{
	if (!Object)
		return;
	
	Cached_Object = Object;
	SetObjectName();
	SetDescription();
}

void UObjectInfoSection::UpdateUI()
{
	SetObjectName();
	SetDescription();
}

void UObjectInfoSection::SetObjectName()
{
	if (ObjectNameTextBlock && Cached_Object.IsValid())
	{
		ObjectNameTextBlock->SetText(Cached_Object->GetObjectName());
	}

}

void UObjectInfoSection::SetDescription()
{
	if (DescriptionTextBlock && Cached_Object.IsValid())
		DescriptionTextBlock->SetText(Cached_Object->GetDescription());
}
