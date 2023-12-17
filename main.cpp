#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace sf;

int main()
{
    // SFML window setup
    RenderWindow window(VideoMode(1000, 800), "Hangman Game");
    window.setFramerateLimit(60);

    // Font loading
    Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cerr << "Error: Font loading failed." << std::endl;
        return EXIT_FAILURE;
    }

    // Text setup
    Text welcomeText("Press Enter to Start", font, 30);
    welcomeText.setPosition(250, 250);
    welcomeText.setFillColor(Color::Black);

    Text wordText("", font, 40);
    wordText.setPosition(250, 350);
    wordText.setFillColor(Color::Black);

    Text inputText("Enter a letter: ", font, 30);
    inputText.setPosition(250, 450);
    inputText.setFillColor(Color::Black);

    // Hangman images setup
    Texture hangmanTextures[7];
    for (int i = 0; i < 7; ++i) {
        std::string textureFile = "hangman" + std::to_string(i) + ".jpeg";
        if (!hangmanTextures[i].loadFromFile(textureFile)) {
            std::cerr << "Error: Image loading failed." << std::endl;
            return EXIT_FAILURE;
        }
    }

    Sprite hangmanSprite(hangmanTextures[0]);
    hangmanSprite.setPosition(500, 50);

    // Load words from a file
    std::ifstream wordsFile("Words.txt");
    if (!wordsFile.is_open()) {
        std::cerr << "Error: Could not open words file." << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<std::string> words;
    std::string word;
    while (wordsFile >> word) {
        words.push_back(word);
    }
    wordsFile.close();

    if (words.empty()) {
        std::cerr << "Error: No words in the file." << std::endl;
        return EXIT_FAILURE;
    }

    // Randomly select a word
    srand(static_cast<unsigned>(time(nullptr)));
    int randomIndex = rand() % words.size();
    std::string selectedWord = words[randomIndex];
    std::cout << selectedWord << std::endl;

    // Initialize game state
    std::string displayedWord(selectedWord.length(), '_');
    int incorrectGuesses = 0;
    bool displayWelcome = true;
    bool gameOver = false;

    while (window.isOpen() && !gameOver)
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.key.code == Keyboard::Escape)
            {
                exit(0);
            }

            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Enter)
                {
                    // Start the game loop and display the input field
                    if (displayWelcome)
                        displayWelcome = false;
                    else
                        gameOver = true;
                }
                else if (event.key.code >= Keyboard::A && event.key.code <= Keyboard::Z)
                {
                    char guess = static_cast<char>(event.key.code);
                    bool correctGuess = false;

                    for (size_t i = 0; i < selectedWord.length(); ++i) {
                        if (selectedWord[i] == guess) {
                            displayedWord[i] = guess;
                            correctGuess = true;
                        }
                    }

                    if (!correctGuess) {
                        // Incorrect guess
                        ++incorrectGuesses;
                        if (incorrectGuesses < 7) {
                            hangmanSprite.setTexture(hangmanTextures[incorrectGuesses]);
                        }
                    }

                    if (selectedWord == displayedWord) {
                        // Word guessed correctly
                        gameOver = true;
                    }
                }
            }
        }

        window.clear(Color::White);

        if (displayWelcome) {
            window.draw(welcomeText);
        }
        else {
            wordText.setString(displayedWord);
            window.draw(wordText);

            inputText.setString("Enter a letter: ");
            window.draw(inputText);

            if (incorrectGuesses < 7) {
                window.draw(hangmanSprite);
            }
            else {
                Text gameOverText("Game Over! The word was: " + selectedWord, font, 30);
                gameOverText.setPosition(250, 250);
                gameOverText.setFillColor(Color::Black);
                window.draw(gameOverText);
                // Display congratulatory message when the word is guessed correctly
                if (selectedWord == displayedWord) {
                    Text congratsText("Congratulations! You've got the word correct: " + selectedWord, font, 30);
                    congratsText.setPosition(250, 300);
                    congratsText.setFillColor(Color::Black);
                    window.draw(congratsText);
                }
            }
        }

        window.display();
    }

    return 0;
}
