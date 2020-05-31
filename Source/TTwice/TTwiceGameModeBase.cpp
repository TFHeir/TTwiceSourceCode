// Fill out your copyright notice in the Description page of Project Settings.


#include "TTwiceGameModeBase.h"
#include "Test2DDummy.h"

ATTwiceGameModeBase::ATTwiceGameModeBase()
{
    DefaultPawnClass = ATest2DDummy::StaticClass();
}