#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

const std::string alphabet[5] = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    "abcdefghijklmnopqrstuvwxyz",
    "АБВГҐДЕЄЖЗИІЇЙКЛМНОПРСТУФХЦЧШЩЬЮЯ",
    "абвгґдеєжзиіїйклмнопрстуфхцчшщьюя",
    ",./!@#$%^&*()-+1234567890 "
};

std::string useAlphabet(char charInput);
void textEncryption(std::string& output);
void fileEncryption(std::string& output);
void rotAlg(std::string input, int move, std::string& output, bool action);
void rotAlgExt(std::string input, std::string& output, bool action);

int main() {
    setlocale(LC_CTYPE, "ukr");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    std::string output;
    std::string choice;
    std::cout << "Введіть 1 для зчитування тексту з консолі або 2 для файлу: ";
    while (true) {
        std::getline(std::cin, choice);
        try {
            switch (std::stoi(choice)) {
            case 1:
                textEncryption(output);
                return 0;
            case 2:
                fileEncryption(output);
                return 0;
            default:
                std::cout << "Sorry, couldn't understand that. Try again\n";
                choice = "0";
            }
        }
        catch (const std::invalid_argument&) {
            std::cout << "Invalid input: not a number. Try again.\n";
        }
        catch (const std::out_of_range&) {
            std::cout << "Invalid input: number is too large. Try again.\n";
        }
    }
}

void fileEncryption(std::string& output) {
    std::fstream inputFile("file.txt", std::ios::in | std::ios::out);
    if (!inputFile.is_open()) {
        std::cout << "Can't open file.txt. Did you forget to create it?\n";
        return;
    }

    bool action = true; //encrypt by default
    int moveNum, choice;
    std::string algChoice;

    std::cout << "Щоб шифрувати файл натисніть 1, щоб дешифрувати файл натисніть 2: ";
    std::cin >> choice;
    std::cin.ignore(); 

    std::cout << "Для звичайного алгоритму шифрування натисність 1, для розширеного - 2: ";
    std::getline(std::cin, algChoice);

    if (stoi(algChoice) == 1) {
        std::cout << "Введіть ключ шифрування: ";
        std::cin >> moveNum;
    }

    if (choice == 2) { action = false; }

    std::ostringstream buffer;
    buffer << inputFile.rdbuf();
    std::string textInput = buffer.str();

    if (stoi(algChoice) == 1) {
        rotAlg(textInput, moveNum, output, action);
    }
    else {
        rotAlgExt(textInput, output, action);
    }

    inputFile.seekp(0);
    inputFile << output;
    inputFile.close();

    std::cout << "\nOperation completed. Result written to file.\n";
}

void textEncryption(std::string& output) {
    int moveNum;
    std::string textInput, algChoice;
    bool action = true;

    std::cout << "Щоб шифрувати текст натисніть 1, щоб дешифрувати текст натисніть 2: ";
    std::getline(std::cin, algChoice);
    if (stoi(algChoice) == 2) {
        action = false;
    }

    std::cout << "Для звичайного алгоритму шифрування натисність 1, для розширеного - 2: ";
    std::getline(std::cin, algChoice);

    std::cout << "Введіть текст: ";
    std::getline(std::cin, textInput);

    try {
        if (stoi(algChoice) == 1) {
            std::cout << "Введіть індекс зміщення: ";
            std::cin >> moveNum;
            rotAlg(textInput, moveNum, output, action);
        }
        else {
            rotAlgExt(textInput, output, action);
        }
        std::cout << "\nResult: " << output << std::endl;
    }
    catch (const std::invalid_argument&) {
        std::cout << "Invalid input: not a number. Try again.\n";
        return;
    }
    catch (const std::out_of_range&) {
        std::cout << "Invalid input: number is too large. Try again.\n";
        return;
    }
}

void rotAlg(std::string input, int move, std::string& output, bool action) {
    output = input;
    if (!action) { move = -move; }

    for (int i = 0; i < input.size(); i++) {
        std::string alphabetStr = useAlphabet(input[i]);
        if (alphabetStr.empty()) {
            output[i] = input[i]; 
            continue;
        }

        size_t modIndex = alphabetStr.find(input[i]);
        if (modIndex == std::string::npos) {
            output[i] = input[i]; 
            continue;
        }

        int replacementIndex = (((int)modIndex + move) % (int)alphabetStr.size() + (int)alphabetStr.size()) % (int)alphabetStr.size();
        output[i] = alphabetStr[replacementIndex];
    }
}

void rotAlgExt(std::string input, std::string& output, bool action) {
    output = input;
    const int BLOCKS = 4;
    const int keys[BLOCKS] = { 2, 12, 23, 55 };

    int baseSize = input.size() / BLOCKS;
    int remainder = input.size() % BLOCKS;

    int pos = 0;

    for (int b = 0; b < BLOCKS; b++) {
        int currentSize = baseSize + (b < remainder ? 1 : 0);
        int move = keys[b];
        if (!action) move = -move; 

        for (int j = 0; j < currentSize && pos < (int)input.size(); j++, pos++) {
            std::string alphabetStr = useAlphabet(input[pos]);
            if (alphabetStr.empty()) {
                output[pos] = input[pos]; 
                continue;
            }

            size_t modIndex = alphabetStr.find(input[pos]);
            if (modIndex == std::string::npos) {
                output[pos] = input[pos]; 
                continue;
            }

            int replacementIndex = (((int)modIndex + move) % (int)alphabetStr.size() + (int)alphabetStr.size()) % (int)alphabetStr.size();
            output[pos] = alphabetStr[replacementIndex];
        }
    }
}

std::string useAlphabet(char charInput) {
    for (const std::string& i : alphabet) {
        if (i.find(charInput) != std::string::npos) return i;
    }
    return "";
}