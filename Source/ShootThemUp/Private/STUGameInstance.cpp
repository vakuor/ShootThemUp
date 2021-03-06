// Shoot Them Up Game. All rights reserved!


#include "STUGameInstance.h"

#include "Sound/STUSoundFunctionLibrary.h"

void USTUGameInstance::ToggleVolume()
{
	USTUSoundFunctionLibrary::ToggleSoundClassVolume(MasterSoundClass);
}
