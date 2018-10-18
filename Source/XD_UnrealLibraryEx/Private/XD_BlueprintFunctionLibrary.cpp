// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_BlueprintFunctionLibrary.h"
#include "XD_UnrealLibraryExUtility.h"
#include <Navigation/PathFollowingComponent.h>
#include <Animation/AnimMetaData.h>
#include <Engine/StaticMeshActor.h>
#include <AI/NavigationSystemBase.h>
#include <NavigationSystem.h>
#include <Kismet/GameplayStatics.h>
#include <Animation/AnimMontage.h>

bool UXD_BlueprintFunctionLibrary::IsValid(const FBPDelegateHandle& Handle)
{
	return Handle.DelegateHandle.IsValid();
}

FBPDelegateHandle UXD_BlueprintFunctionLibrary::AddTicker(const FBPTickerDelegate& BPTickerDelegate, float InDelay /*= 0.f*/)
{
	return FBPDelegateHandle(FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateStatic(&UXD_BlueprintFunctionLibrary::TickerTick, BPTickerDelegate)));
}

void UXD_BlueprintFunctionLibrary::RemoveTicker(const FBPDelegateHandle& BPDelegateHandle)
{
	FTicker::GetCoreTicker().RemoveTicker(BPDelegateHandle.DelegateHandle);
}

bool UXD_BlueprintFunctionLibrary::TickerTick(float DeltaSeconds, FBPTickerDelegate BPTickerDelegate)
{
	if (BPTickerDelegate.IsBound())
	{
		BPTickerDelegate.Execute(DeltaSeconds);
		return true;
	}
	return false;
}

bool UXD_BlueprintFunctionLibrary::TickerTick(float DeltaSeconds, FBPTickerDelegateWithReturn BPTickerDelegate)
{
	if (BPTickerDelegate.IsBound())
	{
		bool IsContinueTick;
		BPTickerDelegate.ExecuteIfBound(DeltaSeconds, IsContinueTick);
		return IsContinueTick;
	}
	return false;
}

FBPDelegateHandle UXD_BlueprintFunctionLibrary::AddOnActorSpawnedInWorldEvent(const UObject* WorldContextObject, const FBPActorSpawnedinWorldDelegate& OnActorSpawnedEvent)
{
	return FBPDelegateHandle(WorldContextObject->GetWorld()->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateLambda([=](AActor* Actor)
	{
		OnActorSpawnedEvent.ExecuteIfBound(Actor);
	})));
}

void UXD_BlueprintFunctionLibrary::RemoveOnActorSpawnedInWorldEvent(const UObject* WorldContextObject, const FBPDelegateHandle& DelegateHandle)
{
	WorldContextObject->GetWorld()->RemoveOnActorSpawnedHandler(DelegateHandle.DelegateHandle);
}

FBPDelegateHandle UXD_BlueprintFunctionLibrary::AddTickerWithReturn(const FBPTickerDelegateWithReturn& BPTickerDelegate, float InDelay /*= 0.f*/)
{
	return FBPDelegateHandle(FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateStatic(&UXD_BlueprintFunctionLibrary::TickerTick, BPTickerDelegate)));
}

float UXD_BlueprintFunctionLibrary::GetBlendInTime(UAnimMontage* Montage)
{
	return Montage->BlendIn.GetBlendTime();
}

float UXD_BlueprintFunctionLibrary::GetBlendOutTime(UAnimMontage* Montage)
{
	return Montage->BlendOut.GetBlendTime();
}

const TArray<class UAnimMetaData*>& UXD_BlueprintFunctionLibrary::GetMetaData(const UAnimationAsset* AnimationAsset)
{
	return AnimationAsset->GetMetaData();
}

bool UXD_BlueprintFunctionLibrary::IsContainMetaDataType(const UAnimationAsset* AnimationAsset, TSubclassOf<class UAnimMetaData> AnimMetaDataType)
{
	return AnimationAsset ? AnimationAsset->GetMetaData().ContainsByPredicate([&](class UAnimMetaData* E) {return E ? E->IsA(AnimMetaDataType) : false; }) : false;
}


UObject* UXD_BlueprintFunctionLibrary::FindItem(const TArray<UObject*>& Array, const FFindElementFunction& FindElementFunction)
{
	UObject* const * Find = Array.FindByPredicate([&](UObject* A) {
		bool Result = false;
		FindElementFunction.Execute(A, Result);
		return A;
	});
	return Find ? *Find : nullptr;
}

void UXD_BlueprintFunctionLibrary::Sort(TArray<UObject*>& Array, const FSortFunction& SortFunction)
{
	Array.Sort([&](UObject& A, UObject& B) {
		bool Result;
		SortFunction.Execute(&A, &B, Result);
		return Result;
	});
}

void UXD_BlueprintFunctionLibrary::SortCaptureOneParameter(UPARAM(Ref)TArray<UObject*>& Array, UObject* CaptureParameter, const FSortCaptureOneParameterFunction& SortFunction)
{
	Array.Sort([&](UObject& A, UObject& B) {
		bool Result;
		SortFunction.Execute(&A, &B, CaptureParameter, Result);
		return Result;
	});
}

int32 UXD_BlueprintFunctionLibrary::Find(UPARAM(Ref)TArray<UObject*>& Array, const FFindFunction& FindFunction)
{
	int32 Index = Array.IndexOfByPredicate(
		[&](UObject* A)
	{
		bool ReturnValue;
		FindFunction.Execute(A, ReturnValue);
		return ReturnValue;
	});
	return Index;
}

void UXD_BlueprintFunctionLibrary::Generic_SortStructArray_UserDefined(void* TargetArray, const UArrayProperty* ArrayProp, UObject* OwnerObject, FName FunctionName)
{
	if (!OwnerObject)
	{
		UnrealLibraryEx_Error_Log("FunctionFromObject为空");
		return;
	}
	if (!TargetArray)
	{
		UnrealLibraryEx_Error_Log("排序数组为空");
		return;
	}
	UFunction* SortRuleFunc = OwnerObject->FindFunction(FunctionName);
	ensure(SortRuleFunc && SortRuleFunc->NumParms == 3);
	if (!SortRuleFunc || SortRuleFunc->NumParms != 3)
	{
		UnrealLibraryEx_Error_Log("[%s]中没找到排序函数[%s]", *OwnerObject->GetName(), *FunctionName.ToString());
		return;
	}
	UBoolProperty* ReturnParam = Cast<UBoolProperty>(SortRuleFunc->GetReturnProperty());
	if (!ReturnParam)
	{
		UnrealLibraryEx_Error_Log("[%s]中排序函数[%s]类型不为bool", *OwnerObject->GetName(), *FunctionName.ToString());
		return;
	}
	//begin Sort Array
	{
		FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
		UProperty* InnerProp = ArrayProp->Inner;
		const int32 Len = ArrayHelper.Num();
		const int32 PropertySize = InnerProp->ElementSize * InnerProp->ArrayDim;
		uint8* Parameters = (uint8*)FMemory::Malloc(PropertySize * 2 + 1);
		for (int32 i = 0; i < Len; i++)
		{
			for (int32 j = 0; j < Len - i - 1; j++)
			{
				FMemory::Memzero(Parameters, PropertySize * 2 + 1);
				InnerProp->CopyCompleteValueFromScriptVM(Parameters, ArrayHelper.GetRawPtr(j));
				InnerProp->CopyCompleteValueFromScriptVM(Parameters + PropertySize, ArrayHelper.GetRawPtr(j + 1));
				OwnerObject->ProcessEvent(SortRuleFunc, Parameters);
				if (ReturnParam && ReturnParam->GetPropertyValue(Parameters + PropertySize * 2))
				{
					ArrayHelper.SwapValues(j, j + 1);
				}
			}
		}
		FMemory::Free(Parameters);
	}
	//end Sort Array
}

void Generic_FindStructArray_UserDefined(void* TargetArray, const UArrayProperty* ArrayProp, UObject* OwnerObject, FName FunctionName, int32& FindElement, bool& Result)
{

}

bool UXD_BlueprintFunctionLibrary::Less_StringString(const FString& A, const FString& B)
{
	return A < B;
}

bool UXD_BlueprintFunctionLibrary::Greater_StringString(const FString& A, const FString& B)
{
	return A > B;
}

void UXD_BlueprintFunctionLibrary::DestroyComponent(UActorComponent* Component)
{
	if (Component)
	{
		Component->DestroyComponent();
	}
}

void UXD_BlueprintFunctionLibrary::SetLightingChannels(UPrimitiveComponent* PrimitiveComponent, bool Channel_0, bool Channel_1, bool Channel_2)
{
	if (PrimitiveComponent)
	{
		PrimitiveComponent->LightingChannels.bChannel0 = Channel_0;
		PrimitiveComponent->LightingChannels.bChannel1 = Channel_1;
		PrimitiveComponent->LightingChannels.bChannel2 = Channel_2;
	}
}

void UXD_BlueprintFunctionLibrary::PauseMove(class UPathFollowingComponent* PathFollowingComponent)
{
	PathFollowingComponent->PauseMove(FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Reset);
}

void UXD_BlueprintFunctionLibrary::ResumeMove(class UPathFollowingComponent* PathFollowingComponent)
{
	PathFollowingComponent->ResumeMove();
}

void UXD_BlueprintFunctionLibrary::BreakNavigationLink(const FNavigationLink& NavigationLink, FVector& Left, FVector& Right)
{
	Left = NavigationLink.Left;
	Right = NavigationLink.Right;
}

bool UXD_BlueprintFunctionLibrary::IsAreaExistNavigationMesh(UObject* WorldContextObject, FVector TestLoc)
{
	UNavigationSystemV1* const NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(WorldContextObject);
	if (!NavSys)
	{
		return false;
	}
	const ANavigationData* NavData = NavSys->GetNavDataForProps(FNavAgentProperties::DefaultProperties);
	if (!NavData)
	{
		return false;
	}
	FPathFindingQuery Query(WorldContextObject, *NavData, TestLoc, TestLoc);
	return NavSys->TestPathSync(Query);
}

FVector UXD_BlueprintFunctionLibrary::GetCurrentTargetLocation(UPathFollowingComponent* PathFollowingComponent)
{
	return PathFollowingComponent ? PathFollowingComponent->GetCurrentTargetLocation() : FNavigationSystem::InvalidLocation;
}

AActor* UXD_BlueprintFunctionLibrary::SpawnActorFromClass(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, const FTransform& Transform, AActor* Owner, APawn* Instigator, ESpawnActorCollisionHandlingMethod CollisionHandling /*= ESpawnActorCollisionHandlingMethod::Undefined*/)
{
	if (UWorld* World = WorldContextObject->GetWorld())
	{
		FActorSpawnParameters ActorSpawnParameters;
		if (Owner)
		{
			ActorSpawnParameters.Owner = Owner;
			ActorSpawnParameters.OverrideLevel = Owner->GetLevel();
		}
		ActorSpawnParameters.Instigator = Instigator;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = CollisionHandling;
		return World->SpawnActor<AActor>(ActorClass, Transform, ActorSpawnParameters);
	}
	return nullptr;
}

AActor* UXD_BlueprintFunctionLibrary::SpawnActorInLevel(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, const FTransform& Transform, AActor* InLevelActor, APawn* Instigator)
{
	if (InLevelActor)
	{
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.OverrideLevel = InLevelActor->GetLevel();
		ActorSpawnParameters.Instigator = Instigator;
		return WorldContextObject->GetWorld()->SpawnActor<AActor>(ActorClass, Transform, ActorSpawnParameters);
	}
	return nullptr;
}

void UXD_BlueprintFunctionLibrary::StartRecordingReplay(const FString& InName, const FString& FriendlyName, const TArray<FString>& AdditionalOptions, UObject* WorldContextObject)
{
	UGameplayStatics::GetGameInstance(WorldContextObject)->StartRecordingReplay(InName, FriendlyName, AdditionalOptions);
}

void UXD_BlueprintFunctionLibrary::StopRecordingReplay(UObject* WorldContextObject)
{
	UGameplayStatics::GetGameInstance(WorldContextObject)->StopRecordingReplay();
}

void UXD_BlueprintFunctionLibrary::PlayReplay(const FString& InName, UWorld* WorldOverride /*= nullptr*/, const TArray<FString>& AdditionalOptions, UObject* WorldContextObject)
{
	UGameplayStatics::GetGameInstance(WorldContextObject)->PlayReplay(InName, WorldOverride, AdditionalOptions);
}

UWorld* UXD_BlueprintFunctionLibrary::GetWorldFromObject(UObject* Object)
{
	return Object ? Object->GetWorld() : nullptr;
}

UObject* UXD_BlueprintFunctionLibrary::GetOuter(UObject* Object)
{
	return Object ? Object->GetOuter() : nullptr;
}

UObject* UXD_BlueprintFunctionLibrary::GetClassDefaultObject(TSubclassOf<UObject> Class)
{
	return Class ? Class->GetDefaultObject() : nullptr;
}

FTimerHandle UXD_BlueprintFunctionLibrary::SetTimerByEvent_WithoutFindHandle(FTimerDynamicDelegate Event, float Time, bool bLooping)
{
	FTimerHandle Handle;
	if (Event.IsBound())
	{
		if (const UWorld* const World = GEngine->GetWorldFromContextObject(Event.GetUObject(), EGetWorldErrorMode::LogAndReturnNull))
		{
			World->GetTimerManager().SetTimer(Handle, Event, Time, bLooping);
		}
	}
	return Handle;
}

float UXD_BlueprintFunctionLibrary::GetRuntimeFloatCurveValue(const FRuntimeFloatCurve& RuntimeFloatCurve, float Time, float DefaultValue /*= 0.f*/)
{
	return RuntimeFloatCurve.GetRichCurveConst()->Eval(Time, DefaultValue);
}

void UXD_BlueprintFunctionLibrary::SetNavigationLinkParameters(UPARAM(Ref)FNavigationLink& NavigationLink, FVector Left, FVector Right, bool UseSnapHeight /*= false*/, float SnapHeight /*= 50.f*/)
{
	NavigationLink.Left = Left;
	NavigationLink.Right = Right;
	NavigationLink.bUseSnapHeight = UseSnapHeight;
	NavigationLink.SnapHeight = SnapHeight;
}

bool UXD_BlueprintFunctionLibrary::IsPlayInEditorPreview(UObject* WorldContextObject)
{
	return WorldContextObject->GetWorld() ? WorldContextObject->GetWorld()->WorldType == EWorldType::EditorPreview : false;
}

bool UXD_BlueprintFunctionLibrary::IsPlayInGame(UObject* WorldContextObject)
{
	return WorldContextObject->GetWorld() ? WorldContextObject->GetWorld()->WorldType == EWorldType::Game : false;
}

bool UXD_BlueprintFunctionLibrary::IsPlayInPIE(UObject* WorldContextObject)
{
	return WorldContextObject->GetWorld() ? WorldContextObject->GetWorld()->WorldType == EWorldType::PIE : false;
}

EEditorWorldType UXD_BlueprintFunctionLibrary::GetWorldType(UObject* WorldContextObject)
{
	return WorldContextObject->GetWorld() ? static_cast<EEditorWorldType>(WorldContextObject->GetWorld()->WorldType) : EEditorWorldType::None;
}


