// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
 
#include "unWorldGameMode.h"
#include "unWorldCharacter.h"
#include "UObject/ConstructorHelpers.h"

AunWorldGameMode::AunWorldGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}
}
