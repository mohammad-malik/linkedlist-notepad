#include "dependencies.cpp"
using namespace std;

int main()
{
    // testing functionality
    ListofLines list;
    list.addLine();
    vector<vector<char> > dictionary = readDictionary();
    Line word;
    ListofLines correctedWords;

    // getting input and showing on screen through <future> and ncurses
    int currentLine = 0, currentCharacter = 0, charactersTillSpace = 0;

    // Initialize ncurses
    initscr();
    cbreak();
    raw();
    noecho();

    clear();
    keypad(stdscr, TRUE);

    int terminalWidth, terminalHeight;
    getmaxyx(stdscr, terminalHeight, terminalWidth);
    int bottomTwentyfivePercent = terminalHeight - (terminalHeight / 4);

    while (true)
    {
        refresh();
    
        int pressedKey = getch();

        if (pressedKey == KEY_UP)
        {
            if (currentLine > 0)
                currentLine--;
            else
                currentCharacter = 0; // going to the start of the line

            clear();
            list.printList();
            mvprintw(currentLine, currentCharacter, "|"); // printing the cursor
        }
        else if (pressedKey == KEY_DOWN)
        {
            if (currentLine < list.getLineCount() - 1)
                currentLine++;
            
            clear();
            list.printList();
            mvprintw(currentLine, currentCharacter, "|"); // printing the cursor
        }
        else if (pressedKey == KEY_LEFT)
        {
            if (currentCharacter > 0)
                currentCharacter--;
            else if (currentLine > 0 && currentCharacter <= 0)
            {
                currentLine--;
                currentCharacter = list[currentLine].getCharacterCount();
            }

            clear();
            list.printList();
            mvprintw(currentLine, currentCharacter, "|"); // printing the cursor
        }
        else if (pressedKey == KEY_RIGHT)
        {
            if (currentCharacter < list[currentLine].getCharacterCount())
                currentCharacter++;
            else if (currentLine < list.getLineCount() - 1 && currentCharacter >= list[currentLine].getCharacterCount())
            {
                currentLine++;
                currentCharacter = 0;
            }

            clear();
            list.printList();
            mvprintw(currentLine, currentCharacter, "|"); // printing the cursor
        }

        else if (pressedKey == KEY_BACKSPACE || pressedKey == 127)
        {
            if (currentCharacter >= 0)
            {
                list.deleteCharacterFromLine(currentLine, currentCharacter);
                currentCharacter--;
            }
            else if (currentLine > 0 && currentCharacter < 0)
            {
                currentLine--;
                list.deleteLine(currentLine + 1);
                currentCharacter = list[currentLine].getCharacterCount();
            }

            // for the word
            if (charactersTillSpace > 0)
            {
                word.deleteCharacter(word.getCharacterCount() - 1);
                charactersTillSpace--;
            }

            clear();
            list.printList();
            mvprintw(currentLine, currentCharacter, "|"); // printing the cursor
        }

        else if (pressedKey == KEY_ENTER || pressedKey == '\n')
        {
            list.addLine();
            currentLine++;
            currentCharacter = 0;

            clear();
            list.printList();
            mvprintw(currentLine, currentCharacter, "|"); // printing the cursor
        }

        // if control + s is pressed
        else if (pressedKey == 19)
        {
            list.saveToFile();
        }

        // if control + l is pressed
        else if (pressedKey == 12)
        {
            list.readFromFile();

            currentLine = list.getLineCount() - 1;

            if (currentLine < 0)
                currentLine = 0;

            currentCharacter = list[currentLine].getCharacterCount();

            if (currentCharacter < 0)
                currentCharacter = 0;

            clear();
            list.printList();
            mvprintw(currentLine, currentCharacter, "|"); // printing the cursor

        }

        // if escape is pressed
        else if (pressedKey == 27)
        {
            cout << "Exiting..." << endl;

            list.clearListContents();
            word.clearLineContents();
            endwin();

            return 1;

        }

        // if any other key is pressed
        else
        {
            clear();
            if (currentCharacter < 0)
                currentCharacter = 0;
            else if (currentCharacter > list[currentLine].getCharacterCount())
                currentCharacter = list[currentLine].getCharacterCount();

            if(pressedKey != ' ')
            {
                Character * character = new Character(pressedKey);
                word.addCharacterAtEnd(character);
                list.addDataToLine(currentLine, currentCharacter, pressedKey);
                
                charactersTillSpace++;
            }
            else if (pressedKey == ' ')
            {
                list.addDataToLine(currentLine, currentCharacter, ' ');

                bool letterSubstitutionSuccessful = (list.attemptLetterSubstitution(dictionary, charactersTillSpace, &word, correctedWords))? true : false;
                bool letterOmissionSuccessful = (list.attemptLetterOmmission(dictionary, charactersTillSpace, &word,correctedWords))? true : false;
                bool letterInsertionSuccessful = (list.attemptLetterInsertion(dictionary, charactersTillSpace, &word,correctedWords))? true : false;
                bool letterReversalSuccessful = (list.attemptLetterReversal(dictionary, charactersTillSpace, &word,correctedWords))? true : false;
                
                if (!isCorrectlySpelt(dictionary, word))
                {
                    if (!letterSubstitutionSuccessful && !letterOmissionSuccessful && !letterInsertionSuccessful && !letterReversalSuccessful)
                        mvaddstr(bottomTwentyfivePercent, 0, "You are a hopeless speller!\n");
                    else
                    {
                        mvaddstr(bottomTwentyfivePercent, 0,"Did you mean: \n");
                        correctedWords.printCorrectWordsList(terminalHeight);
                    }
                }

                charactersTillSpace = 0;

                word.clearLineContents();
                correctedWords.clearListContents();
            }
            currentCharacter++;
            mvaddch(0,0,'\b');
            list.printList();
            mvprintw(currentLine, currentCharacter, "|"); // printing the cursor
        }
    }
}