// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class LGUEDK_API LGDebug
{
public:
	LGDebug();
	~LGDebug();

	static void Log(const FString& LogMsg, const bool bPrintOnScreen = false);
	static void Warning(const FString& WarningMsg, const bool bPrintOnScreen = false);
	static void Error(const FString& ErrorMsg, const bool bPrintOnScreen = false);
};
