//
//  FBullCowGame.hpp
//  BullCowGame
//
//  Created by Duong Pham on 1/8/19.
//  Copyright © 2019 Duong Pham. All rights reserved.
//

#ifndef FBullCowGame_hpp
#define FBullCowGame_hpp

#pragma once

#include <map>
#include <stdio.h>
#include <string>
#include <iostream>

// make the syntax Unreal friendly
using int32 = int;
using FString = std::string;

struct FBullCowCount
{
    int32 Bull;
    int32 Cow;
};

enum class EGuessStatus
{
    INVALID,
    NOT_ISOGRAM,
    NOT_LOWERCASE,
    INCORRECT_LENGTH,
    OK
};

class FBullCowGame
{
public:
    FBullCowGame(); // constructor
    ~FBullCowGame(); // destructor

    void Reset();

    int32 GetMaxTries() const;
    int32 GetCurrentTries() const;
    int32 GetHiddenWordLength() const;

    void IncreaseCurrentTries(int val = 1);
    bool SubmitGuess(FString, FBullCowCount&);

private:
    int32 MyMaxTries;
    int32 MyCurrentTries;
    FString MyHiddenWord;

    EGuessStatus CheckStatus(FString) const;
    bool IsLowercase(FString) const;
    bool IsIsogram(FString) const;

    void FeedBack(EGuessStatus);
};

#endif /* FBullCowGame_hpp */

