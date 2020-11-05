#include <cstdio>
#include <math.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "String.h"

int main()
    {

    FILE* f = fopen("String.h", "r");

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* buffer = (char*) malloc(sizeof(char) * size);

    fread(buffer, 1, size, f); 

    String strBuffer = String(buffer);

    fclose(f);

    sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML window");
    window.setKeyRepeatEnabled(false);

    sf::Font Font;

    if (!Font.loadFromFile("arial.ttf"))
    {
        return EXIT_FAILURE;
    }

    sf::Text text("", Font, 20);
    text.setPosition(0, 0);
    text.setFillColor(sf::Color::Green);

    float count = 0;
    int lineNumber = 1;
    int portionLen = 5;
    int deletedCharNumber = 0;

    int len = strBuffer.length();

    char* changingChar = new char[len];

    String changingText(changingChar);

    while (window.isOpen())
    {   
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            else if(event.type == sf::Event::KeyPressed) 
            {
                for (int i = count; i < count + portionLen; i++)
                {
                    if (strBuffer.text[i] == '\n')
                    {
                        lineNumber++;
                    }
                    
                    //std::cout << changingText.text << std::endl;
                    //std::cout << strBuffer[i] << std::endl;
                    //std::cout << changingText[i - deletedCharNumber] << " " << "1"  << std::endl;

                    changingText.text[i] = strBuffer.text[i];

                    //std::cout << changingText[i - deletedCharNumber] << std::endl;
                }

                count += portionLen;

                while (lineNumber > 34)
                {
                    lineNumber--;

                    int lenOfFIrstLine = changingText.findFirstOf('\n');

                    String* splits = changingText.split(lenOfFIrstLine);
                    std::cout << splits[0].text << "\n\n\n" << splits[1].text << std::endl;

                    delete[]changingText.text;

                    changingText.text = splits[1].text;

                    delete[] splits[0].text;
                }
            }
        }        

        text.setString(changingText.text);

        window.clear();

        window.draw(text);

        window.display();

        if (count >= len)
        {
            return 0;
        }
    }
}