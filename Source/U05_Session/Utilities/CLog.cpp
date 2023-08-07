#include "CLog.h"

DEFINE_LOG_CATEGORY_STATIC(GameProject, Warning, All);

void CLog::Print(int32 InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, FString::FromInt(InValue));
}

void CLog::Print(float InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, FString::SanitizeFloat(InValue));
}

void CLog::Print(const FString& InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, InValue);
}

void CLog::Print(const FVector& InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, InValue.ToString());
}

void CLog::Print(const FRotator& InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, InValue.ToString());
}

void CLog::Log(int32 InValue)
{
	UE_LOG(GameProject, Warning, L"%d", InValue);
}

void CLog::Log(float InValue)
{
	UE_LOG(GameProject, Warning, L"%.3f", InValue);
}

void CLog::Log(const FString& InValue)
{
	UE_LOG(GameProject, Warning, L"%s", *InValue);
}

void CLog::Log(const FVector& InValue)
{
	UE_LOG(GameProject, Warning, L"%s", *InValue.ToString());
}

void CLog::Log(const FRotator& InValue)
{
	UE_LOG(GameProject, Warning, L"%s", *InValue.ToString());
}

void CLog::Log(const UObject* InObject)
{
	FString str;

	if (!!InObject)
	{
		str.Append(InObject->GetName());
		str.Append(" is Valid");
		UE_LOG(GameProject, Warning, L"%s", *str);
	}
	else
		UE_LOG(GameProject, Error, L"NULL");
}

void CLog::Log(const FString& InFuncName, int32 InLineNumber)
{
	FString str;
	str.Append(InFuncName);
	str.Append(", ");
	str.Append(FString::FromInt(InLineNumber));

	UE_LOG(GameProject, Warning, L"%s", *str);
}

