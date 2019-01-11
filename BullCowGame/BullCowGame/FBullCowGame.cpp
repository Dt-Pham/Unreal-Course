//
//  FBullCowGame.cpp
//  BullCowGame
//
//  Created by Duong Pham on 1/8/19.
//  Copyright © 2019 Duong Pham. All rights reserved.
//

#include <iostream>
#include "FBullCowGame.hpp"

// make the syntax Unreal friendly
#define TMap std::map

FBullCowGame::FBullCowGame() { Reset(); }

FBullCowGame::~FBullCowGame() {}

void FBullCowGame::Reset()
{
    constexpr int32 MAX_TRIES = 8;
    const FString HIDDEN_WORD = "duong";

    MyMaxTries = MAX_TRIES;
    MyHiddenWord = HIDDEN_WORD;
    MyCurrentTries = 1;
    return;
}

int32 FBullCowGame::GetMaxTries() const { return MyMaxTries; }

int32 FBullCowGame::GetCurrentTries() const { return MyCurrentTries; }

int32 FBullCowGame::GetHiddenWordLength() const { return (int)MyHiddenWord.size(); }

void FBullCowGame::IncreaseCurrentTries(int val) { MyCurrentTries += val; }

bool FBullCowGame::SubmitGuess(FString Word, FBullCowCount &BullCowCount)
{
    EGuessStatus Status = CheckStatus(Word);
    FeedBack(Status);
    if (Status != EGuessStatus::OK) return false;

    BullCowCount = {0, 0};
    for (int i = 0; i < GetHiddenWordLength(); i++)
    {
        for (int j = 0; j < (int)Word.size(); j++)
        {
            if (MyHiddenWord[i] != Word[j]) continue;
            if (i == j)
                BullCowCount.Bull++;
            else
                BullCowCount.Cow++;
        }
    }
    return true;
}

EGuessStatus FBullCowGame::CheckStatus(FString Word) const
{
    if(Word.size() != GetHiddenWordLength())
    {
        return EGuessStatus::INCORRECT_LENGTH;
    }
    if (!IsLowercase(Word))
    {
        return EGuessStatus::NOT_LOWERCASE;
    }
    if (!IsIsogram(Word))
    {
        return EGuessStatus::NOT_ISOGRAM;
    }
    return EGuessStatus::OK;
}

bool FBullCowGame::IsIsogram(FString Word) const
{
    TMap<char, bool> Seen; // Check if the character have been seen in word or not
    for (auto letter : Word)
    {
        if (Seen[letter]) return false;
        Seen[letter] = true;
    }
    return true;
}


bool FBullCowGame::IsLowercase(FString Word) const
{
    for (auto letter : Word)
    {
        if ('a' <= letter && letter <= 'z') continue;
        return false;
    }
    return true;
}

void FBullCowGame::FeedBack(EGuessStatus Status)
{
    switch (Status) {
        case EGuessStatus::INCORRECT_LENGTH:
            std::cout << "Please type in a " << GetHiddenWordLength() << " length word\n\n";
            break;
        case EGuessStatus::NOT_ISOGRAM:
            std::cout << "Please type a word without duplicate letter\n\n";
            break;
        case EGuessStatus::NOT_LOWERCASE:
            std::cout << "Please type in all lowercase number\n\n";
            break;
        default:
            break;
    }
}



