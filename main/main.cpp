#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

const int MAX_DIMENSION = 99;
const int COL_SIZE = 2;
const int DISPLAY_LENGTH = 50;
const int SPACE_B4_COMMAND = 2;
const int SPACE_B4_DESCRIPTION = 10;
const string QUESTION_OPTION = "question";
const string ANSWER_OPTION = "answer";
const string LINK_OPTION = "link";

void menu(string questionArray[], int& questionSize, string answerArray[], int& answerSize, string linkArray[][COL_SIZE], int& linkSize);
//function to display options to the user and ask the user to choose

void open(ifstream& fin, string fileName);
//function to open an input file
void close(ifstream& fin);
//function to close an input file

void open(ofstream& fout, string fileName);
//function to open an output file
void close(ofstream& fout);
//function to close an input file
string format_QandA(string text, string QorA);
//function to format answer or question that suits the plist file

void findQuestion(ifstream& fin);
//loops through the file from the beginning and looks for "<key>questions</key>"
//since there will always be a "<key>text</key>" at the line before the question, look for that line
//get the next line and extract the text using the function extractText();

string extractText(string& xml);
//takes "<tag>TEXT</tag>" as the argument and returns "TEXT"
//works by ignoring everything from '<' to '>'

int fillArray(string fileName, string textArray[]);
//function to pull answers/questions from the file indicated by fileName
//put the answers/questions into textArray[]

int fill2DArray(string fileName, string textArray[][COL_SIZE]);
//function to pull answers/questions from the file indicated by fileName
//put the answers/questions into textArray[]

void userView(string textArray[], int arraySize);
//function to format and display the questions and answers files to the user
//This function is different from displayFile because the other one displays everything in a file as it is
//whereas this function formats the content of the file before outputting it
//This function should only be used for "answers.txt" and "questions.txt"

void user2DView(string linkArray[][COL_SIZE], int linkSize, string questionArray[], int questionSize, string answerArray[], int answerSize);

void addQorA(string textArray[], int& arraySize, string whatToAdd);
//function to append another string at the end of an array

void delQorA(string textArray[], int& arraySize, int whereToDelete);
//function to delete something in an array

void stringToLower(string& text);
//function to convert a string into all lowercase

string parseCommandLine(string choice, unsigned int& start);
//reads the string "choice", find the first word, store it in a char vector and return it

bool spaceOnly(string text);
//return true if text only contains spaces, false otherwise

bool digitOnly(string text);
//return true if text only contains digits, false otherwise

bool containsSpace(string text);
//return true if text contains at least one space, false otherwise

bool containsDigit(string text);
//return true if text contains at least one digit, false otherwise

bool digitnSpaceOnly(string text);
//return true if text only contains spaces or digits, false otherwise

void checknDelete(vector<int> indicesToDelete, string textArray[], int& textSize);

string formatUserInput(string text);

void append(vector<int>& indicesToDelete, string choice, unsigned int& start);

void sortDesc(vector<int>& vector);
//sort a vector in descending order

unsigned int findBiggest(vector<int> vector, unsigned int start);
//return the index of the biggest number in a vector

void link(string choice, string linkArray[][COL_SIZE], int& linkSize);

int main()
{
    ifstream fin;
    ofstream fout;
    string questionArray[MAX_DIMENSION];
    string answerArray[MAX_DIMENSION];
    string linkArray[MAX_DIMENSION][COL_SIZE];
    int questionSize;
    int answerSize;
    int linkSize;

    string test = "testing";

    //initialization
    questionSize = fillArray("questions.txt", questionArray);
    answerSize = fillArray("answers.txt", answerArray);
    linkSize = fill2DArray("link.txt", linkArray);

    //menu
    menu(questionArray, questionSize, answerArray, answerSize, linkArray, linkSize);

    return 0;
}

void menu(string questionArray[], int& questionSize, string answerArray[], int& answerSize, string linkArray[][COL_SIZE], int& linkSize)
{
    cout.setf(ios::left);

    string choice;
    while (true)
    {
        cout << "CS2_honors:~ vhtoolkit$ ";
        getline(cin, choice);
        //stringToLower(choice);
        //use getline to get the whole line of user input

        if (spaceOnly(choice))
            continue;

        choice = formatUserInput(choice) + " ";

        unsigned int start = 0;
        string command = parseCommandLine(choice, start);
        if (command == "display") //first layer
        {
            string option = parseCommandLine(choice, start);
            if (option == ANSWER_OPTION)  //second layer
            {
                userView(answerArray, answerSize);
            }
            else if (option == QUESTION_OPTION)  //second layer
            {
                userView(questionArray, questionSize);
            }
            else if (option == LINK_OPTION)  //second layer
            {
                user2DView(linkArray, linkSize, questionArray, questionSize, answerArray, answerSize);
            }
            else
            {
                cout << "Option not found.\n"
                     << "Available options:\n"
                     << setw(SPACE_B4_COMMAND) << " " << QUESTION_OPTION << endl
                     << setw(SPACE_B4_COMMAND) << " " << ANSWER_OPTION << endl
                     << setw(SPACE_B4_COMMAND) << " " << LINK_OPTION << endl;

                cout << endl
                     << "Usage: \n"
                     << setw(SPACE_B4_COMMAND) << " " << "display " << "[option]"
                     << endl << endl;
            }
        }
        else if (command == "add") //first layer
        {
            string option = parseCommandLine(choice, start);
            if (option == QUESTION_OPTION)  //second layer
            {
                string question = choice.substr(start);
                addQorA(questionArray, questionSize, question);
                cout << "\"" << question << "\" added to questions\n";
            }
            else if (option == ANSWER_OPTION)  //second layer
            {
                string answer = choice.substr(start);
                addQorA(answerArray, answerSize, answer);
                cout << "\"" << answer << "\" added to answers\n";
            }
            else if (option == LINK_OPTION)  //second layer
            {
                string link = choice.substr(start);


            }
            else  //second layer
            {
                cout << "Option not found.\n"
                     << "Available options:\n"
                     << setw(SPACE_B4_COMMAND) << " " << QUESTION_OPTION << endl
                     << setw(SPACE_B4_COMMAND) << " " << ANSWER_OPTION << endl;

                cout << endl
                     << "Usage: \n"
                     << setw(SPACE_B4_COMMAND) << " " << "add " << "[option] " << "[text to add]"
                     << endl << endl;
            }
        }
        else if (command == "del") //first layer
        {
            string option = parseCommandLine(choice, start);
            if (option == "question")  //second layer
            {
                string whereToDelete = choice.substr(start);
                if (digitnSpaceOnly(whereToDelete))
                {
                    if (containsSpace(whereToDelete))
                    {
                        vector<int> indicesToDelete;
                        while (containsSpace(choice.substr(start)))
                        {
                            append(indicesToDelete, choice, start);
                            if (!containsDigit(choice.substr(start)))
                                break;
                        }
                        sortDesc(indicesToDelete);
                        checknDelete(indicesToDelete, questionArray, questionSize);
                    }
                    else //doesn't contain space, aka one index only
                    {
                        if (stoi(whereToDelete) < questionSize)
                        {
                            delQorA(questionArray, questionSize, stoi(whereToDelete));
                            cout << "Question no." << whereToDelete << " deleted from questions\n";
                        }
                        else
                        {
                            cout << "INDEX ERROR: index \"" << whereToDelete << "\" out of bound\n";
                        }
                    }
                }
                else
                {
                    cout << "INDEX ERROR: invalid index \"" << whereToDelete << "\"\n";
                }
            }
            else if (option == "answer")  //second layer
            {
                string whereToDelete = choice.substr(start);
                if (digitnSpaceOnly(whereToDelete))
                {
                    if (containsSpace(whereToDelete))
                    {
                        vector<int> indicesToDelete;
                        while (containsSpace(choice.substr(start)))
                        {
                            append(indicesToDelete, choice, start);
                            if (!containsDigit(choice.substr(start)))
                                break;
                        }
                        sortDesc(indicesToDelete);
                        checknDelete(indicesToDelete, questionArray, questionSize);
                    }
                    else //doesn't contain space, aka one index only
                    {
                        if (stoi(whereToDelete) < questionSize)
                        {
                            delQorA(questionArray, questionSize, stoi(whereToDelete));
                            cout << "Question no." << whereToDelete << " deleted from questions\n";
                        }
                        else
                        {
                            cout << "INDEX ERROR: index \"" << whereToDelete << "\" out of bound\n";
                        }
                    }
                }
                else
                {
                    cout << "INDEX ERROR: invalid index \"" << whereToDelete << "\"\n";
                }
            }
            else  //second layer
            {
                cout << "Option not found.\n"
                     << "Available options:\n"
                     << setw(SPACE_B4_COMMAND) << " " << "question" << endl
                     << setw(SPACE_B4_COMMAND) << " " << "answer" << endl;

                cout << endl
                     << "Usage: \n"
                     << setw(SPACE_B4_COMMAND) << " " << "del " << "[option] " << "[indices of question/answer to delete]"
                     << endl << endl;
            }
        }
        else if (command == "link") //first layer
        {
            vector<int> linkVector;
            string argument = choice.substr(start);
            while (containsSpace(choice.substr(start)))
            {
                append(linkVector, choice, start);
                if (!containsDigit(choice.substr(start)))
                    break;
            }
            if (linkVector.size() > 2)
            {
                cout << "ARGUMENT ERROR: Invalid argument \"" << argument << "\"\n";
            }
            else
            {
                linkArray[linkSize][0] = to_string(linkVector[0]);
                linkArray[linkSize][1] = to_string(linkVector[1]);
                linkSize++;
                cout << "Question no." << linkVector[0] << " linked to Answer no." << linkVector[1] << endl;
            }
        }
        else if (command == "vhtoolkit") //first layer
        {
            cout << endl
                 << "Usage: \n"
                 << setw(SPACE_B4_COMMAND) << " " << setw(SPACE_B4_DESCRIPTION) << "<command>" << "[options]"
                 << endl << endl;


            cout << "Available commands:\n"
                 << setw(SPACE_B4_COMMAND) << " " << setw(SPACE_B4_DESCRIPTION) << "display" << "Display either the questions or answers in the vhtoolkit" << endl
                 << setw(SPACE_B4_COMMAND) << " " << setw(SPACE_B4_DESCRIPTION) << "add" << "Add a question or an answer to the vhtoolkit" << endl
                 << setw(SPACE_B4_COMMAND) << " " << setw(SPACE_B4_DESCRIPTION) << "del" << "Delete a question or an answer in the vhtoolkit" << endl
                 << setw(SPACE_B4_COMMAND) << " " << setw(SPACE_B4_DESCRIPTION) << "quit" << "Quit the program" << endl << endl;
        }
        else if (command == "quit") //first layer
        {
           break;
        }
        else
        {
            cout << "Unknown command: " << choice <<  endl << endl
                 << "Use \"vhtoolkit\" to see available  commands\n";
        }
    }
}

void open(ifstream& fin, string fileName)
{
    fin.open(fileName);
    if (fin.fail())
    {
        cout << "Failed to open " << fileName << endl;
        exit(1);
    }
}

void close(ifstream& fin)
{
    fin.close();
}

void open(ofstream& fout, string fileName)
{
    fout.open(fileName);
    if (fout.fail())
    {
        cout << "Failed to open " << fileName << endl;
        exit(1);
    }
}

void close(ofstream& fout)
{
    fout.close();
}

void findQuestion(ifstream& fin)
{
    string line;
    bool doubleBreak = false;
    while (!fin.eof())
    {
        getline(fin, line);
        if (line == "<key>questions</key>") //after finding the questions tag, loop until answer tag is seen
        {
            while (!fin.eof()) //keep looping through the file
            {
                getline(fin, line);
                if (line == "<key>text</key>")
                {
                    getline(fin, line);
                    cout << extractText(line) << endl;
                }
                else if (line == "<key>answers</key>")
                {
                    doubleBreak = true;
                    break;
                }
            }
        }
        else if (doubleBreak)
            break;
    }
}

string extractText(string& xml)
{
    vector<char> text;
    bool tag = false;
    for (unsigned int i = 0; i < xml.length(); i++)
    {
        if (xml[i] == '<')
        {
            tag = true;
            continue;
        }
        else if (tag)
        {
            if (xml[i] == '>')
                tag = false;
            continue;
        }
        text.push_back(xml[i]);
    }
    string str(text.begin(), text.end());
    return str;
}

int fillArray(string fileName, string textArray[])
{
    ifstream fin;

    open(fin, fileName);

    string line;
    int index = 0;
    while (!fin.eof())
    {
        getline(fin, line);
        textArray[index] = line;
        index++;
    }

    close(fin);

    return index;
}

int fill2DArray(string fileName, string textArray[][COL_SIZE])
{
    ifstream fin;

    open(fin, fileName);

    string line;
    int index = 0;
    while (!fin.eof())
    {
        string question, answer;

        fin >> question;
        fin >> answer;

        textArray[index][0] = question;
        textArray[index][1] = answer;
        index++;
    }

    close(fin);

    return index;
}

void userView(string textArray[], int arraySize)
{
    for (int i = 0; i < arraySize; i++)
    {
        if (textArray[i].length() <= DISPLAY_LENGTH)
        {
            cout << i << " " << textArray[i] << endl << endl;
        }
        else
        {
            cout << i << " ";
            for (unsigned int j = 0; j < DISPLAY_LENGTH; j++)
            {
                cout << textArray[i][j];
            }
            cout << "..." << endl << endl;
        }
    }
}

void user2DView(string linkArray[][COL_SIZE], int linkSize, string questionArray[], int questionSize, string answerArray[], int answerSize)
{
    cout.setf(ios::left);
    string seenNumber = "";
    cout << "Questions and their linked answers are grouped together in a block\n";
    for (int i = 0; i < linkSize; i++)
    {
        if (seenNumber == linkArray[i][0])
        {
            int Anumber = stoi(linkArray[i][1]);
            if (Anumber >= answerSize)
            {
                cout << "INDEX ERROR: index \"" << Anumber << "\" out of bound\n";
            }
            else if (answerArray[Anumber].length() > DISPLAY_LENGTH)
            {
                    cout << "A" << Anumber << ": " << answerArray[Anumber].substr(0,DISPLAY_LENGTH) << "..." << endl;
            }
            else
                cout << "A" << Anumber << ": " << answerArray[Anumber] << endl;
        }
        else
        {
            cout << endl;
            seenNumber = linkArray[i][0];
            int Qnumber = stoi(linkArray[i][0]);
            int Anumber = stoi(linkArray[i][1]);
            if (Anumber >= answerSize)
            {
                cout << "INDEX ERROR: index \"" << Anumber << "\" out of bound\n";
            }
            else if (Qnumber >= questionSize)
            {
                cout << "INDEX ERROR: index \"" << Qnumber << "\" out of bound\n";
            }
            else
            {
                cout << "Q" << Qnumber << ": " << questionArray[Qnumber] << endl
                     << "A" << Anumber << ": " << answerArray[Anumber].substr(0,DISPLAY_LENGTH) << endl;
            }
        }
    }
}

void addQorA(string textArray[], int& arraySize, string whatToAdd)
{
    textArray[arraySize] = whatToAdd;
    arraySize++;
}

void delQorA(string textArray[], int& arraySize, int whereToDelete)
{
    for (int i = whereToDelete; i < arraySize - 1; i++)
    {
        textArray[i] = textArray[i + 1];
    }
    arraySize -= 1;
}

void stringToLower(string& text)
{
    for (unsigned int i = 0; i < text.length(); i++)
    {
        text[i] = tolower(text[i]);
    }
}

string parseCommandLine(string choice, unsigned int& start)
{
    string inputString;
    for (unsigned int i = start; i < choice.length(); i++)
    {
        if (choice[i] != ' ')
        {
            inputString.push_back(choice[i]);
        }
        else
        { 
            start = i + 1;
            break;
        }
    }

    if (inputString.size() == 0)
    {
        inputString.push_back(' ');
        inputString.resize(1);
        start += 1;
    }
    return inputString;
}

bool spaceOnly(string text)
{
    for (unsigned int i = 0; i < text.length(); i++)
    {
        if (!isspace(text[i]))
            return 0;
    }
    return 1;
}

bool digitOnly(string text)
{
    for (unsigned int i = 0; i < text.length(); i++)
    {
        if (!isdigit(text[i]))
            return 0;
    }
    return 1;
}

bool containsSpace(string text)
{
    for (unsigned int i = 0; i < text.length(); i++)
    {
        if (isspace(text[i]))
        {
            return 1;
        }
    }
    return 0;
}

bool containsDigit(string text)
{
    for (unsigned int i = 0; i < text.length(); i++)
    {
        if (isdigit(text[i]))
        {
            return 1;
        }
    }
    return 0;
}

bool digitnSpaceOnly(string text)
{
    for (unsigned int i = 0; i < text.length(); i++)
    {
        if (!isdigit(text[i]) && !isspace(text[i]))
            return 0;
    }
    return 1;
}

void checknDelete(vector<int> indicesToDelete, string textArray[], int& textSize)
{
    for (unsigned int i = 0; i < indicesToDelete.size(); i++)
    {
        int index = indicesToDelete[i];
        if (index < textSize)
        {
            delQorA(textArray, textSize, index);
            cout << "Question no." << index << " deleted from questions\n";
        }
        else
        {
            cout << "INDEX ERROR: index \"" << index << "\" out of bound\n";
        }
    }
}

string formatUserInput(string text)
{
    string formatted;
    bool space = false;
    for (unsigned int i = 0; i < text.length(); i++)
    {
        if(!isspace(text[i]))
        {
            formatted += text[i];
            space = false;
        }
        else if (!space)
        {
            formatted += " ";
            space = true;
        }
    }
    return formatted;
}

void append(vector<int>& indicesToDelete, string choice, unsigned int& start)
{
    indicesToDelete.push_back(stoi(parseCommandLine(choice,start)));
}

void sortDesc(vector<int>& vector)
{
    int temp;
    unsigned int biggest_index;
    for (unsigned int i = 0; i < vector.size(); i++)
    {
        biggest_index = findBiggest(vector,i);
        temp = vector[i];
        vector[i] = vector[biggest_index];
        vector[biggest_index] = temp;
    }
}

unsigned int findBiggest(vector<int> vector, unsigned int start)
{
    int value = vector[start];
    unsigned int index = start;
    for (unsigned int i = start + 1; i < vector.size(); i++)
    {
        if (vector[i] >= value)
        {
            value = vector[i];
            index = i;
        }
    }
    return index;
}
