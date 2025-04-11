// Copyright Villains, Inc. All Rights Reserved.


#include "Utility/LGDebug.h"


LGDebug::LGDebug()
{
}

LGDebug::~LGDebug()
{
}

void LGDebug::Log(const FString& LogMsg, const bool bPrintOnScreen)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *LogMsg);
	if (bPrintOnScreen && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, LogMsg);
	}
}

void LGDebug::Warning(const FString& WarningMsg, const bool bPrintOnScreen)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *WarningMsg);
	if (bPrintOnScreen && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, WarningMsg);
	}
}

void LGDebug::Error(const FString& ErrorMsg, const bool bPrintOnScreen)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMsg);
	if (bPrintOnScreen && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ErrorMsg);
	}
}
