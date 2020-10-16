// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);

    Isogram = GetValidWords(Words);
    SetUpGame();

    //PrintLine(TEXT("Number of possible words is %i."), Words.Num());
    //PrintLine(TEXT("Number of possible valid words is %i."),  GetValidWords(Words).Num());
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if(bGameOver)
    {
        ClearScreen();
        SetUpGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetUpGame()
{   
    PrintLine(TEXT("Well hello stranger! \nWelcome to BullCow Game"));

    HiddenWord = Isogram[FMath::RandRange(0, Isogram.Num() - 1)];
    Lives = HiddenWord.Len() * 2;
    bGameOver = false;

    PrintLine(TEXT("Hidden Word is %i characters long"), HiddenWord.Len());
    PrintLine(TEXT("You start with %i lives" ), Lives);
    PrintLine(TEXT("Try guessing the hidden word"));
    //PrintLine(TEXT("The Hidden Word is: %s."), *HiddenWord); //debug line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again"));
    
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    
    if(Guess == HiddenWord)
    {            
        PrintLine(TEXT("Well Done You Win"));
        EndGame();
        return;
    }

    if(Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Unlucky Try Again" ));
        PrintLine(TEXT("Hidden Word is %i characters long"), HiddenWord.Len());
        return;
    }

    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again."));
        return;
    }
        
    PrintLine(TEXT("You have lost a life"));
    --Lives;
        
    if(Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You Lose!"));
        PrintLine(TEXT("The Hidden Word was: %s."), *HiddenWord);
        EndGame();
    }

    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("You have %i lives left." ), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for(int32 Index = 0; Index < Word.Len(); Index++)
    {
        for(int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if(Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for(FString Word : WordList)
    {
        if(Word.Len() >= 4 && Word.Len() <= 8)
        {
            if(IsIsogram(Word))
            {
                ValidWords.Emplace(Word);
            }
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
   FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if(Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
    
    
        for(int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if(Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
               Count.Cows++;
            }
        }
    }
    return Count;
}