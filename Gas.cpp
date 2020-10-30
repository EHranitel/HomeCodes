#include <iostream>
#include <cstdlib>
#include <math.h>
#include <cassert>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Sphere
{
    public:

        float x = 0;
        float y = 0;
        int radius = 0;
        float speedX = 0;
        float speedY = 0;
        int colorRed = 0;
        int colorGreen = 0;
        int colorBlue = 0;
        int detailCirclesNum = 0;

        void move(int dT)
            {
                x = x + speedX * dT;
                y = y + speedY * dT;
            }
        
        bool checkCollision(Sphere sphere)
        {
            return (x - sphere.x) * (x - sphere.x) + (y - sphere.y) * (y - sphere.y) < (radius + sphere.radius) * (radius + sphere.radius);
        }

        void collideWithWall()
        {
            if(x + 2 * radius > 1000)
            {
                speedX = - speedX;

                float deltaX = x + 2 * radius - 1000;
                float deltaY = (speedY / abs(speedY)) * (abs(speedY / speedX) * deltaX);

                x -= deltaX;
                y -= deltaY;
                
            }
            else if(x < 0)
            {
                speedX = - speedX;

                float deltaX = - x;
                float deltaY = (speedY / abs(speedY)) * (abs(speedY / speedX) * deltaX);

                x += deltaX; 
                y -= deltaY; 
            }

            if(y + 2 * radius > 800)
            {
                speedY = - speedY;

                float deltaY = y + 2 * radius - 800;
                float deltaX = (speedX / abs(speedX)) * (abs(speedX / speedY) * deltaY);

                y -= deltaY;
                x -= deltaX;
            }
            else if(y < 0)
            {
                speedY = - speedY;

                float deltaY = - y;
                float deltaX = (speedX / abs(speedX)) * (abs(speedX / speedY) * deltaY);

                y += deltaY; 
                x -= deltaX; 
            }
        }   

        void collideWithSphere(Sphere* sphere)
        {
            if (!checkCollision(*sphere))
            {
                return;
            }

            float speedX1BeforeCollision = speedX;
            float speedY1BeforeCollision = speedY;
            float speedX2BeforeCollision = sphere->speedX;
            float speedY2BeforeCollision = sphere->speedY;

            float distBefore = sqrt((x - sphere->x) * (x - sphere->x) + (y - sphere->y) * (y - sphere->y));

            assert(distBefore);

            float distAfter = radius + sphere->radius;
            float deltaX = abs(x - sphere->x) * distAfter / (2 * distBefore);
            float deltaY = abs(y - sphere->y) * distAfter / (2 * distBefore);

            speedX = speedX2BeforeCollision;
            speedY = speedY2BeforeCollision;
            sphere->speedX = speedX1BeforeCollision;
            sphere->speedY = speedY1BeforeCollision;

            x += ((x - sphere->x) / abs(x - sphere->x)) * deltaX;
            sphere->x += ((sphere->x - x) / abs(sphere->x - x)) * deltaX;
            y += ((y - sphere->y) / abs(y - sphere->y)) * deltaY;
            sphere->y += ((sphere->y - y) / abs(sphere->y - y)) * deltaY;
        }

        void draw(sf::RenderWindow* window)
        {
            for (int i = 0; i < detailCirclesNum; i++)
            {
                    sf::CircleShape circle(radius - i * radius / detailCirclesNum, 100);
                    circle.setPosition(x + i * radius / detailCirclesNum, y + i * radius / detailCirclesNum);
                    circle.setFillColor(sf::Color(colorRed * i / detailCirclesNum, colorGreen * i / detailCirclesNum, colorBlue * i / detailCirclesNum));
                    window->draw(circle);
            }
        }

};

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
    Sphere particles[500];

    int dT = 1;

    sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML window");

    for (int i = 0; i < 500; i++)
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

        drawAllParticles(particles, 500, &window);

        window.display();

        moveAllParticles(particles, 500, dT);
    }

    return 0;
}
