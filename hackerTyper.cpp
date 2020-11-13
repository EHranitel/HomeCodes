#include <cstdio>
#include <math.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "String.h"

int main()
    {

    FILE* f = fopen("TESTLONGCODE.cpp", "r");

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* buffer = (char*) malloc(sizeof(char) * size);

    fread(buffer, 1, size, f); 

    String strBuffer = String(buffer);

    fclose(f);

    sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML window");

    sf::Font Font;

    if (!Font.loadFromFile("arial.ttf"))
    {
        return EXIT_FAILURE;
    }

    sf::Text text("", Font, 20);
    text.setPosition(0, 0);
    text.setFillColor(sf::Color::Green);

    int count = 0;
    int lineNumber = 1;
    int portionLen = 5;
    int deletedCharNum = 0;

    int len = strBuffer.length();

    char* changingChar = new char[len];
    
    changingChar[0] = '\0';

    String changingText = String();
    changingText.text = changingChar;

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
                if (count + portionLen >= len)
                {
                    delete[] changingChar;

                    return 0;
                }

                for (int i = count; i < count + portionLen; i++)
                {
                    if (strBuffer.text[i] == '\n')
                    {
                        lineNumber++;
                    }

                    changingText.text[i - deletedCharNum] = strBuffer.text[i];
                }

                count += portionLen;

                changingText.text[count - deletedCharNum] = '\0';

                while (lineNumber > 34)
                {
                    lineNumber--;

                    int lenOfFIrstLine = changingText.findFirstOf('\n');

                    deletedCharNum += lenOfFIrstLine + 1;

                    String* splits = changingText.split(lenOfFIrstLine + 1);
                    
                    delete[] splits[0].text;

                    delete[] changingChar;

                    char* changingChar = new char[len];

                    int leng = splits[1].length();

                    for (int i = 0; i < leng; i++)
                    {
                        changingChar[i] = splits[1].text[i];
                    }

                    changingChar[leng] = '\0';
                        
                    changingText.text = changingChar;   

                    delete[] splits[1].text;
                }
            }
        }        

        text.setString(changingText.text);

        window.clear();

        window.draw(text);

        window.display();
    }
}