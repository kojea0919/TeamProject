// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ObjectInfo/ObjectInfoPanel.h"

#include "Components/VerticalBox.h"
#include "UI/ObjectInfo/ObjectInfoSection.h"

void UObjectInfoPanel::InitObjectInfoPanel()
{
	OnCurrentInteractedObjectChanged.AddUObject(this, &UObjectInfoPanel::UpdateObjectInfo);
}

void UObjectInfoPanel::UpdateObjectInfo(ABaseObject* Object)
{
	if (!ObjectInfoSectionClass || !W_ObjectInfoPanelVerticalBox)
	{
		return;
	}

	// 전달받은 Object가 null인 경우 모든 섹션 제거
	if (!Object || !Object->Interactable)
	{
		W_ObjectInfoPanelVerticalBox->ClearChildren();
		return;
	}

	// 기존 자식 ObjectInfoSection들 중에 같은 Object를 가진 것이 있는지 확인
	TArray<UWidget*> Children = W_ObjectInfoPanelVerticalBox->GetAllChildren();
    
	for (UWidget* Child : Children)
	{
		UObjectInfoSection* Section = Cast<UObjectInfoSection>(Child);
		if (Section)
		{
			// Cached_Object가 전달받은 Object와 같은지 확인
			if (Section->GetCachedObject() == Object)
			{
				// 이미 같은 오브젝트를 표시하는 섹션이 있음 - 아무것도 안함
				return;
			}
		}
	}
    
	// 모든 자식 제거
	W_ObjectInfoPanelVerticalBox->ClearChildren();
    
	// 새 섹션 생성
	UObjectInfoSection* Section = CreateWidget<UObjectInfoSection>(this, ObjectInfoSectionClass);
    
	if (Section)
	{
		W_ObjectInfoPanelVerticalBox->AddChild(Section);
		Section->InitObjectInfo(Object);
		Section->UpdateUI();
	}
}

void UObjectInfoPanel::UpdateUI()
{
	TArray<UWidget*> Children = W_ObjectInfoPanelVerticalBox->GetAllChildren();
    
	for (UWidget* Child : Children)
	{
		UObjectInfoSection* Section = Cast<UObjectInfoSection>(Child);
		if (Section)
		{
			Section->UpdateUI();
		}
	}
}
