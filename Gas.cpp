#include <iostream>
#include <cstdlib>
#include <math.h>
#include <cassert>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Sphere.h"

void drawAllParticles(Sphere* particles, int sphereNumber, sf::RenderWindow* window)
{
    for (int i = 0; i < sphereNumber; i++)
        {
            assert(particles);

            particles[i].draw(window);
        }
}

void moveAllParticles(Sphere* particles, int sphereNumber, float dT)
{
    for (int i = 0; i < sphereNumber; i++)
    {
        for (int j = i + 1; j < sphereNumber; j++)
        {
            assert(particles);

            particles[i].collideWithSphere(&particles[j]);
        }
    }

    for (int i = 0; i < sphereNumber; i++)
    {
        assert(particles);

        particles[i].collideWithWall();
    }

    for (int i = 0; i < sphereNumber; i++)
    {
        assert(particles);

        particles[i].move(dT);
    }
}

int main()
{
    int sphereNumber = 500;
    Sphere* particles = new Sphere[sphereNumber];

    int dT = 1;

    sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML window");

    for (int i = 0; i < sphereNumber; i++)
    {   
        assert(particles);

        particles[i].x = 20 + rand()%(980-20+1);
        particles[i].y = 20 + rand()%(780-20+1);
        particles[i].radius = 5;
        particles[i].speedY = -10 + rand()%(10+10+1);
        particles[i].speedX = -10 + rand()%(10+10+1);
        particles[i].colorRed = 255;
        particles[i].colorGreen = 255;
        particles[i].colorBlue = 255;
        particles[i].detailCirclesNum = 5;
    }

    while (window.isOpen())
    {   
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();

        drawAllParticles(particles, sphereNumber, &window);

        window.display();

        moveAllParticles(particles, sphereNumber, dT);
    }

    delete[] particles;
    return 0;
}
