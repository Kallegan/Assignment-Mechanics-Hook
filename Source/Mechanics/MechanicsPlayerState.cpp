// Fill out your copyright notice in the Description page of Project Settings.


#include "MechanicsPlayerState.h"

void AMechanicsPlayerState::AddLootedOrb_Implementation(int Count)
{
    int PreviousCount = LootedOrbs;
    LootedOrbs += Count;
    LootedOrbs = FMath::Clamp(LootedOrbs, 0, LootedOrbsMax);

    if (LootedOrbs != PreviousCount)
    {
        OnOrbCountChanged.Broadcast(LootedOrbs);
    }
}

void AMechanicsPlayerState::RemoveLootedOrb_Implementation(int Count)
{
    int PreviousCount = LootedOrbs;
    LootedOrbs -= Count;
    LootedOrbs = FMath::Clamp(LootedOrbs, 0, LootedOrbsMax);

    if (LootedOrbs != PreviousCount)
    {
        OnOrbCountChanged.Broadcast(LootedOrbs);
    }
}