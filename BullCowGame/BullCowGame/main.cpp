//
//  main.cpp
//  BullCowGame
//
//  Created by Duong Pham on 1/7/19.
//  Copyright © 2019 Duong Pham. All rights reserved.
//

#include <iostream>
#include <string>
#include "FBullCowGame.hpp"

using namespace std;

// make the syntax Unreal friendly
using FText = std::string;
using int32 = int;

void PrintIntro();
void PlayGame();
string GetGuess(int);
bool AskToPlayAgain();
void SummarizeGame(bool);

FBullCowGame BCGame;

std::ostream &operator<<(std::ostream &o, FBullCowCount val)
{
    o << "Bull = " << val.Bull << ", Cow = " << val.Cow;
    return o;
}

int main() {
    PrintIntro();
    do
    {
        BCGame.Reset();
        PlayGame();
    }
    while (AskToPlayAgain());

    return 0;
}

// introduce the game
void PrintIntro()
{
    constexpr int32 WORD_LENGTH = 5;
    cout << "Welcome to Bull and Cow game.\n";
    cout << "Can you guess the " << WORD_LENGTH;
    cout << " letter isogram I'm thinking of?\n";
    cout << endl;
}

void PlayGame()
{
    FBullCowCount BullCowCount;
    bool IsWin = false;
    while(BCGame.GetCurrentTries() <= BCGame.GetMaxTries())
    {
        string Guess;
        // repeat ask until the user type the correct format.
        do
        {
            Guess = GetGuess(BCGame.GetCurrentTries());
        }
        while(!BCGame.SubmitGuess(Guess, BullCowCount));

        BCGame.IncreaseCurrentTries();
        cout << BullCowCount << endl << endl;

        // if win then break the loop
        if (BullCowCount.Bull == BCGame.GetHiddenWordLength())
        {
            IsWin = true;
            break;
        }
    }

    // game summary
    SummarizeGame(IsWin);
}

string GetGuess(int NumTries)
{
    // Get the guess from player
    string Guess;
    cout << "Try " << NumTries << " of " << BCGame.GetMaxTries();
    cout << ". Enter your guess: ";
    getline(cin, Guess);
    return Guess;
}

bool AskToPlayAgain()
{
    cout << "Do you want to play again? ";
    string Response = "";
    getline(cin, Response);

    return (Response[0] == 'y' || Response[0] == 'Y');
}

void SummarizeGame(bool IsWin)
{
    if (IsWin) cout << "Congratulation.\n\n";
    else cout << "Better luck next time\n\n";
}

