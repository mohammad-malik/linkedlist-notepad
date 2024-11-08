#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ncurses.h>
using namespace std;

struct Character
{
    char data;
    Character *next;
    Character *previous;

    Character(char data = '\0')
    {
        this->data = data;
        this->next = nullptr;
        this->previous = nullptr;
    }

    void setNext(Character *next)
    {
        this->next = next;
    }
    void setPrevious(Character *previous)
    {
        this->previous = previous;
    }
    Character *getNext() const
    {
        return this->next;
    }
    Character *getPrevious() const
    {
        return this->previous;
    }

    Character &operator=(const Character *other)
    {
        this->data = other->data;
        this->next = nullptr;
        this->previous = nullptr;
        return *this;
    }
    Character &operator=(const char other)
    {
        this->data = other;
        return *this;
    }
};

struct Line // a list of characters
{
    Character *firstChar, *lastChar; // head, tail of list of characters

    Line *above; // next of each line node
    Line *below; // previous of each line node

    Line()
    {
        this->firstChar = nullptr;
        this->lastChar = nullptr;
        this->above = nullptr;
        this->below = nullptr;
    }

    int getCharacterCount() const // returns the number of characters in the list
    {
        int count = 0;

        for (Character *current = this->firstChar; current != nullptr; count++, current = current->next);

        return count;
    }

    bool isEmptyLine() const
    {
        return this->firstChar == nullptr;
    }

    void clearLineContents()
    {
        Character *current = this->firstChar;

        while (current != nullptr)
        {
            Character *temp = current;
            current = current->next;
            delete temp;
        }

        this->firstChar = nullptr;
        this->lastChar = nullptr;
    }

    void addCharacterAtEnd(Character *newCharacter)
    {
        if (this->isEmptyLine())
        {
            this->firstChar = newCharacter;
            this->lastChar = newCharacter;
        }
        else
        {
            newCharacter->setPrevious(this->lastChar);
            this->lastChar->setNext(newCharacter);
            this->lastChar = newCharacter;
        }
    }

    void addCharacterInLocation(Character *newCharacter, int characterNumber)
    {
        if (this->isEmptyLine())
        {
            this->firstChar = newCharacter;
            this->lastChar = newCharacter;
        }
        else
        {
            if (characterNumber == 0)
            {
                newCharacter->setNext(this->firstChar);
                this->firstChar->setPrevious(newCharacter);
                this->firstChar = newCharacter;
            }
            else if (characterNumber == this->getCharacterCount())
            {
                addCharacterAtEnd(newCharacter);
            }
            else
            {
                Character *currentChar = this->firstChar;
                for (int i = 0; i < characterNumber - 1; i++, currentChar = currentChar->next);

                newCharacter->setNext(currentChar->next);
                newCharacter->setPrevious(currentChar);
                currentChar->next->setPrevious(newCharacter);
                currentChar->next = newCharacter;
            }
        }
    }

    void replaceCharactersInLine(Line *charactersToReplace, int wordLength, int positionTillWord)
    {
        // going to the point where the word starts
        Character *currentChar = firstChar;
        for (int i = 0; i < positionTillWord; i++)
            currentChar = currentChar->next;

        // replacing the characters
        for (int i = 0; i < wordLength; i++, currentChar = currentChar->next)
            currentChar->data = (*charactersToReplace)[i].data;
    }

    void deleteCharacter(int characterNumber)
    {
        if (this->isEmptyLine())
        {
            cout << "Line is empty";
            return;
        }

        if (characterNumber < 0 || characterNumber > this->getCharacterCount())
        {
            cout << "Invalid character number";
            return;
        }

        if (this->getCharacterCount() == 1)
        {
            delete this->firstChar;
            this->firstChar = nullptr;
            this->lastChar = nullptr;
        }
        else
        {
            if (characterNumber == 0)
            {
                Character *temp = this->firstChar;
                this->firstChar = this->firstChar->next;
                this->firstChar->previous = nullptr;
                delete temp;
            }
            else if (characterNumber == this->getCharacterCount() - 1)
            {
                Character *temp = this->lastChar;
                this->lastChar = this->lastChar->previous;
                this->lastChar->next = nullptr;
                delete temp;
            }
            else
            {
                Character *currentChar = this->firstChar;
                for (int i = 0; i < characterNumber; i++, currentChar = currentChar->next);

                currentChar->previous->next = currentChar->next;
                currentChar->next->previous = currentChar->previous;

                delete currentChar;
            }
        }
    }

    void addWordToLine(Line *word, int positionToAddTo)
    {
        // going to the point where the word starts
        Character *currentChar = firstChar;
        for (int i = 0; i < positionToAddTo; i++)
            currentChar = currentChar->next;

        // adding the characters
        for (int i = 0; i < word->getCharacterCount(); i++)
        {
            if (currentChar == nullptr)
                this->addCharacterAtEnd(new Character((*word)[i].data));
            else
                this->replaceCharactersInLine(word, word->getCharacterCount(), positionToAddTo);
            currentChar = currentChar->next;
        }
    }

    vector<char> toVector() const 
    {
        vector<char> line;
        Character* currentChar = firstChar;
        while (currentChar != nullptr) 
        {
            line.push_back(currentChar->data);
            currentChar = currentChar->next;
        }
        return line;
    }

    Character &operator[](const int characterNumber)
    {
        Character *current = this->firstChar;
        for (int i = 0; i < characterNumber; i++, current = current->next);

        return *current;
    }

    bool operator==(const Line &otherLine) const
    {
        if (this->getCharacterCount() != otherLine.getCharacterCount())
            return false;

        Character *current = this->firstChar;
        Character *otherCurrent = otherLine.firstChar;
        for (int i = 0; i < this->getCharacterCount(); i++, current = current->next, otherCurrent = otherCurrent->next)
        {
            if (current->data != otherCurrent->data)
                return false;
        }
        return true;
    }

    bool operator==(const vector<char> vectorLine) const
    {
        if (this->getCharacterCount() != vectorLine.size())
            return false;

        Character *current = this->firstChar;
        for (int i = 0; i < this->getCharacterCount(); i++, current = current->next)
        {
            if (current->data != vectorLine[i])
                return false;
        }
        return true;
    }
};

void reverseWord(Line* word) 
{
    Character *current = word->firstChar;
    Character *temp = nullptr;

    // Swap next and prev for all nodes
    while (current != nullptr) {
        temp = current->previous;
        current->previous = current->next;
        current->next = temp;
        current = current->previous;
    }

    // Change head
    if (temp != nullptr) 
        word->firstChar = temp->previous;
}

int compare(const vector<char> &a, const vector<char> &b) 
{
    for (int i = 0; i < min(a.size(), b.size()); i++) 
    {
        if (a[i] < b[i]) 
            return -1;
        if (a[i] > b[i]) 
            return 1;
    }
    if (a.size() < b.size()) 
        return -1;
    if (a.size() > b.size()) 
        return 1;
    
    return 0;
}

bool binarySearch(const vector<vector<char> >& dictionary, const vector<char> &wordToFind) 
{
    int left = 0;
    int right = dictionary.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = compare(dictionary[mid], wordToFind);

        if (cmp == 0) {
            return true;
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return false;
}

class ListofLines
{
    Line *firstLine, *lastLine; // head, tail of list of lines

public:
    ListofLines()
    {
        this->firstLine = nullptr;
        this->lastLine = nullptr;
    }

    void addLine()
    {
        Line *newLine = new Line();
        if (this->isEmptyList())
        {
            this->firstLine = newLine;
            this->lastLine = newLine;
        }
        else
        {
            newLine->above = this->lastLine;
            this->lastLine->below = newLine;
            this->lastLine = newLine;
        }
    }

    void addLine(Line *newLine)
    {
        if (this->isEmptyList())
        {
            this->firstLine = newLine;
            this->lastLine = newLine;
        }
        else
        {
            newLine->above = this->lastLine;
            this->lastLine->below = newLine;
            this->lastLine = newLine;
        }
    }

    int getLineCount() const
    {
        int count = 0;

        for (Line *current = this->firstLine; current != nullptr; count++, current = current->below);

        return count;
    }

    bool isEmptyList() const
    {
        return this->firstLine == nullptr;
    }

    void addDataToLine(int lineNumber, int characterNumber, char data)
    {
        if (lineNumber < 0 || lineNumber >= this->getLineCount())
        {
            cout << "Invalid line number";
            return;
        }

        Line *current = this->firstLine;
        for (int i = 0; i < lineNumber; i++, current = current->below);

        if (characterNumber < 0 || characterNumber > current->getCharacterCount())
        {
            cout << "Invalid character number";
            return;
        }

        Character *newCharacter = new Character(data);
        if (current->isEmptyLine())
        {
            current->firstChar = newCharacter;
            current->lastChar = newCharacter;
        }
        else
        {
            if (characterNumber == 0)
            {
                newCharacter->next = current->firstChar;
                current->firstChar->previous = newCharacter;
                current->firstChar = newCharacter;
            }
            else if (characterNumber == current->getCharacterCount())
            {
                newCharacter->previous = current->lastChar;
                current->lastChar->next = newCharacter;
                current->lastChar = newCharacter;
            }
            else
            {
                Character *currentChar = current->firstChar;
                for (int i = 0; i < characterNumber - 1; i++, currentChar = currentChar->next);

                newCharacter->next = currentChar->next;
                newCharacter->previous = currentChar;
                currentChar->next->previous = newCharacter;
                currentChar->next = newCharacter;
            }
        }
    }

    void replaceDataInLine(int lineNumber, int characterNumber, char data)
    {
        if (lineNumber < 0 || lineNumber >= this->getLineCount())
        {
            cout << "Invalid line number";
            return;
        }

        Line *current = this->firstLine;
        for (int i = 0; i < lineNumber; i++, current = current->below);

        if (characterNumber < 0 || characterNumber >= current->getCharacterCount())
        {
            cout << "Invalid character number";
            return;
        }

        Character *currentChar = current->firstChar;
        for (int i = 0; i < characterNumber; i++, currentChar = currentChar->next);

        currentChar->data = data;
    }

    void deleteLine(int lineNumber)
    {
        Line *current = this->firstLine;
        for (int i = 0; i < lineNumber && current != nullptr; i++, current = current->below);

        if (current != nullptr)
        {
            if (current->above == nullptr) // first line
                this->firstLine = current->below;
            else
                current->above->below = current->below;

            if (current->below == nullptr) // last line
                this->lastLine = current->above;
            else
                current->below->above = current->above;

            current->clearLineContents();
            delete current;
        }
    }

    void deleteCharacterFromLine(int lineNumber, int characterNumber)
    {
        if (this->isEmptyList())
        {
            cout << "List is empty";
            return;
        }

        if (lineNumber < 0 || lineNumber >= this->getLineCount())
        {
            cout << "Invalid line number";
            return;
        }

        Line *current = this->firstLine;
        for (int i = 0; i < lineNumber; i++, current = current->below);

        if (characterNumber < 0 || characterNumber > current->getCharacterCount())
        {
            cout << "Invalid character number";
            return;
        }

        if (current->isEmptyLine())
        {
            cout << "Line is empty";
            return;
        }

        if (current->getCharacterCount() == 1)
        {
            delete current->firstChar;
            current->firstChar = nullptr;
            current->lastChar = nullptr;
        }
        else
        {
            if (characterNumber == 0)
            {
                Character *temp = current->firstChar;
                current->firstChar = current->firstChar->next;
                current->firstChar->previous = nullptr;
                delete temp;
            }
            else if (characterNumber == current->getCharacterCount())
            {
                Character *temp = current->lastChar;
                current->lastChar = current->lastChar->previous;
                current->lastChar->next = nullptr;
                delete temp;
            }
            else
            {
                Character *currentChar = current->firstChar;
                for (int i = 0; i < characterNumber; i++, currentChar = currentChar->next);

                currentChar->previous->next = currentChar->next;
                currentChar->next->previous = currentChar->previous;

                delete currentChar;
            }
        }
    }

    void saveToFile() const
    {
        ofstream file("save.txt", ios::out | ios::trunc);
        Line *current = this->firstLine;
        while (current != nullptr)
        {
            Character *currentChar = current->firstChar;
            while (currentChar != nullptr)
            {
                file << currentChar->data; // writing each character to the file
                currentChar = currentChar->next;
            }
            if (current->below != nullptr)
                file << endl;
            current = current->below;
        }
        file.close();
    }

    void clearListContents()
    {
        Line *current = this->firstLine;

        while (current != nullptr)
        {
            Character *currentChar = current->firstChar;
            while (currentChar != nullptr)
            {
                Character *temp = currentChar;
                currentChar = currentChar->next;
                delete temp;
            }
            Line *temp = current;
            current = current->below;
            delete temp;
        }

        this->firstLine = nullptr;
        this->lastLine = nullptr;
    }

    void readFromFile()
    {
        ifstream file("save.txt");
        if (!file.is_open())
        {
            cout << "File not found";
            return;
        }
        string line;

        clearListContents();

        while (getline(file, line))
        {
            this->addLine();
            for (int i = 0; i < line.length(); i++)
                this->addDataToLine(this->getLineCount() - 1, i, line[i]);
        }
        if (this->getLineCount() == 0)
            this->addLine();
        file.close();
    }

    void printList() const
    {
        int lineNumber, characterNumber;
        Line *current = this->firstLine;

        for (lineNumber = 0; current != nullptr; lineNumber++)
        {
            Character *currentChar = current->firstChar;
            for (characterNumber = 0; currentChar != nullptr; characterNumber++)
            {
                addch(currentChar->data);
                currentChar = currentChar->next;
            }
            current = current->below;
            if (current != nullptr)
                printw("\n");
            else
                break;
        }
    }

    void printCorrectWordsList(int terminalHeight) const
    {
        int wordNumber, characterNumber;
        Line *current = this->firstLine;

        for (wordNumber = 0; current != nullptr; wordNumber++)
        {
            Character *currentChar = current->firstChar;
            for (characterNumber = 0; currentChar != nullptr; characterNumber++)
            {
                addch(currentChar->data);
                currentChar = currentChar->next;
            }
            current = current->below;
            if (current != nullptr)
                printw(" ");
            else
                break;
        }
    }

    Line &operator[](const int lineNumber) const
    {
        if (lineNumber < 0 || lineNumber >= this->getLineCount())
        {
            cout << "Invalid line number";
            return *(this->firstLine);
        }

        Line *current = this->firstLine;
        for (int i = 0; i < lineNumber; i++, current = current->below);

        return *current;
    }

    bool attemptLetterSubstitution(vector<vector<char> > dictionary, int wordLength, Line *word, ListofLines &correctedWords)
    {
        int wordsAdded = 0;

        // creating a copy of the word
        Line *newWord = new Line();
        for (int i = 0; i < wordLength; i++)
            newWord->addCharacterAtEnd(new Character((*word)[i].data));

        // looping through each character in the word
        for (int characterInWord = 0; characterInWord < wordLength; characterInWord++)
        {
            char originalCharacter = (*newWord)[characterInWord].data;
            // looping through each character in the alphabet
            for (int letter = 97; letter < 123; letter++)
            {
                if (letter == originalCharacter)
                    continue;
                // replacing jth character in the word with k
                (*newWord)[characterInWord] = letter; // using string-like indexing, overloaded operators for Line
                // if the word is found in the dictionary
                if (binarySearch(dictionary, newWord->toVector()))
                {
                    for (int correctedWordsIndex = 0; correctedWordsIndex < wordsAdded; correctedWordsIndex++)
                    {
                        // if the word is already in the list of corrected words
                        if (correctedWords[correctedWordsIndex] == *newWord)
                            continue;
                    }
                    // creating a copy of the new word to store
                    Line *wordToStore = new Line();
                    for (int i = 0; i < wordLength; i++)
                        wordToStore->addCharacterAtEnd(new Character((*newWord)[i].data));
                    correctedWords.addLine(wordToStore);
                    wordsAdded++;
                }
            }
            // restoring the original character
            (*newWord)[characterInWord] = originalCharacter;
        }

    if (wordsAdded > 0)
        return true;
    else
        return false;
    }

    bool attemptLetterOmmission(vector<vector<char> > dictionary, int wordLength, Line *word, ListofLines &correctedWords)
    {
        int wordsAdded = 0;

        // creating a copy of the word
        Line *newWord = new Line();
        for (int i = 0; i < wordLength; i++)
            newWord->addCharacterAtEnd(new Character((*word)[i].data));

        // looping through each character in the word
        for (int characterInWord = 0; characterInWord < wordLength; characterInWord++)
        {
            // removing jth character in the word
            newWord->deleteCharacter(characterInWord);
            // if the word is found in the dictionary
            if (binarySearch(dictionary, newWord->toVector()))
            {
                for (int correctedWordsIndex = 0; correctedWordsIndex < wordsAdded;correctedWordsIndex++)
                    {
                        // if the word is already in the list of corrected words
                        if (correctedWords[correctedWordsIndex] == *newWord)
                            continue;
                    }
                // creating a copy of the new word to store
                Line *wordToStore = new Line();
                for (int i = 0; i < newWord->getCharacterCount(); i++)
                    wordToStore->addCharacterAtEnd(new Character((*newWord)[i].data));
                correctedWords.addLine(wordToStore);
                wordsAdded++;
            }
            // restoring the original character
            newWord->addCharacterInLocation(new Character((*word)[characterInWord].data), characterInWord);

        }

    if (wordsAdded > 0)
        return true;
    else
        return false;
    }

    bool attemptLetterInsertion(vector<vector<char> > dictionary, int wordLength, Line *word, ListofLines &correctedWords)
    {
        int wordsAdded = 0;

        // creating a copy of the word
        Line *newWord = new Line();
        for (int i = 0; i < wordLength; i++)
            newWord->addCharacterAtEnd(new Character((*word)[i].data));

            // looping through each character in the word
        for (int characterInWord = 0; characterInWord < wordLength + 1; characterInWord++)
        {
            // inserting jth character in the word
            newWord->addCharacterInLocation(new Character(' '), characterInWord);
            // looping through each character in the alphabet
            for (int letter = 97; letter < 123; letter++)
            {
                // inserting jth character in the word
                (*newWord)[characterInWord] = letter; // using string-like indexing, overloaded operators for Line
                
                if (binarySearch(dictionary, newWord->toVector()))
                {
                    for (int correctedWordsIndex = 0; correctedWordsIndex < wordsAdded; correctedWordsIndex++)
                    {
                        // if the word is already in the list of corrected words
                        if (correctedWords[correctedWordsIndex] == *newWord)
                            continue;
                    }
                    // creating a copy of the new word to store
                    Line *wordToStore = new Line();
                    for (int i = 0; i < wordLength + 1; i++)
                        wordToStore->addCharacterAtEnd(new Character((*newWord)[i].data));
                    correctedWords.addLine(wordToStore);
                    wordsAdded++;
                }
            }
            // restoring the original character
            newWord->deleteCharacter(characterInWord);
        }  

    if (wordsAdded > 0)
        return true;
    else
        return false;
    }

    bool attemptLetterReversal(vector<vector<char> > dictionary, int wordLength, Line *word, ListofLines &correctedWords)
    {
        int wordsAdded = 0;

        // creating a copy of the word
        Line *newWord = new Line();
        for (int i = 0; i < wordLength; i++)
            newWord->addCharacterAtEnd(new Character((*word)[i].data));

        // looping through each character in the word
        for (int characterInWord = 0; characterInWord < wordLength - 1; characterInWord++)
        {
            reverseWord(newWord);

            // if the word is found in the dictionary
            if (binarySearch(dictionary, newWord->toVector()))
            {
                bool isAlreadyStored = false;
                int currentCount = correctedWords.getLineCount();
                for (int correctedWordsIndex = 0; correctedWordsIndex < currentCount; correctedWordsIndex++)
                {
                    // if the word is already in the list of corrected words
                    if (correctedWords[correctedWordsIndex] == *newWord)
                        isAlreadyStored = true;
                }
                if (!isAlreadyStored)
                {    // creating a copy of the new word to store
                    Line *wordToStore = new Line();
                    for (int i = 0; i < wordLength; i++)
                        wordToStore->addCharacterAtEnd(new Character((*newWord)[i].data));
                    correctedWords.addLine(wordToStore);
                    wordsAdded++;
                }
            }
            // reverting
            reverseWord(newWord);
        }

        if (wordsAdded > 0)
            return true;
        else
            return false;
    }
};

void toLowerCase(vector<vector<char> >& dictionary) 
{
    for (int i = 0; i < dictionary.size(); i++) 
    {
        if (!dictionary[i].empty()) 
        {
            if (dictionary[i][0] >= 'A' && dictionary[i][0] <= 'Z')
                dictionary[i][0] = dictionary[i][0] + ' ';
        }
    }
}

vector<vector<char> > readDictionary()
{
    vector<vector<char> > dictionaryWords;
    ifstream file("dictionary.txt");
    string word;
    while (file >> word)
    {
        vector<char> wordVector;
        for (int i = 0; i < word.length(); i++)
            wordVector.push_back(word[i]);
        dictionaryWords.push_back(wordVector);
    }
    toLowerCase(dictionaryWords);
    return dictionaryWords;
}

bool isCorrectlySpelt(vector<vector<char> > dictionary, Line word)
{
    return binarySearch(dictionary, word.toVector());
}
