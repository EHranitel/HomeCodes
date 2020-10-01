#include <iostream>
#include <cstdlib>
#include <math.h>
#include <cassert>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

struct Sphere
{
    float x;
    float y;
    int radius;
    float speedX;
    float speedY;
    int colorRed;
    int colorGreen;
    int colorBlue;
    int detailCirclesNum;
};

void moveSphere(Sphere* sphere, int dT)
{
    sphere->x = sphere->x + sphere->speedX * dT;
    sphere->y = sphere->y + sphere->speedY * dT;
}

bool checkCollision(Sphere sphere1, Sphere sphere2)
{
    return (sphere1.x - sphere2.x) * (sphere1.x - sphere2.x) + (sphere1.y - sphere2.y) * (sphere1.y - sphere2.y) < (sphere1.radius + sphere2.radius) * (sphere1.radius + sphere2.radius);
}

void collideWithWall(Sphere* sphere)
{
    if(sphere->x + 2 * sphere->radius > 1000)
    {
        sphere->speedX = - sphere->speedX;

        float deltaX = sphere->x + 2 * sphere->radius - 1000;
        float deltaY = (sphere->speedY / abs(sphere->speedY)) * (abs(sphere->speedY / sphere->speedX) * deltaX);

        sphere->x -= deltaX;
        sphere->y -= deltaY;
        
    }
    if(sphere->x < 0)
    {
        sphere->speedX = - sphere->speedX;

        float deltaX = - sphere->x;
        float deltaY = (sphere->speedY / abs(sphere->speedY)) * (abs(sphere->speedY / sphere->speedX) * deltaX);

        sphere->x += deltaX; 
        sphere->y -= deltaY; 
    }

    if(sphere->y + 2 * sphere->radius > 800)
    {
        sphere->speedY = - sphere->speedY;

        float deltaY = sphere->y + 2 * sphere->radius - 800;
        float deltaX = (sphere->speedX / abs(sphere->speedX)) * (abs(sphere->speedX / sphere->speedY) * deltaY);

        sphere->y -= deltaY;
        sphere->x -= deltaX;
    }
    if(sphere->y < 0)
    {
        sphere->speedY = - sphere->speedY;

        float deltaY = - sphere->y;
        float deltaX = (sphere->speedX / abs(sphere->speedX)) * (abs(sphere->speedX / sphere->speedY) * deltaY);

        sphere->y += deltaY; 
        sphere->x -= deltaX; 
    }
}

void collideTwoSpheres(Sphere* sphere1, Sphere* sphere2)
{
    if (!checkCollision(*sphere1, *sphere2))
    {
        return;
    }

    float speedX1BeforeCollision = sphere1->speedX;
    float speedY1BeforeCollision = sphere1->speedY;
    float speedX2BeforeCollision = sphere2->speedX;
    float speedY2BeforeCollision = sphere2->speedY;

    float distBefore = sqrt((sphere1->x - sphere2->x) * (sphere1->x - sphere2->x) + (sphere1->y - sphere2->y) * (sphere1->y - sphere2->y));

    assert(distBefore);

    float distAfter = sphere1->radius + sphere2->radius;
    float deltaX = abs(sphere1->x - sphere2->x) * distAfter / (2 * distBefore);
    float deltaY = abs(sphere1->y - sphere2->y) * distAfter / (2 * distBefore);

    sphere1->speedX = speedX2BeforeCollision;
    sphere1->speedY = speedY2BeforeCollision;
    sphere2->speedX = speedX1BeforeCollision;
    sphere2->speedY = speedY1BeforeCollision;

    sphere1->x += ((sphere1->x - sphere2->x) / abs(sphere1->x - sphere2->x)) * deltaX;
    sphere2->x += ((sphere2->x - sphere1->x) / abs(sphere2->x - sphere1->x)) * deltaX;
    sphere1->y += ((sphere1->y - sphere2->y) / abs(sphere1->y - sphere2->y)) * deltaY;
    sphere2->y += ((sphere2->y - sphere1->y) / abs(sphere2->y - sphere1->y)) * deltaY;
}

void drawSphere(Sphere sphere, sf::RenderWindow* window)
{
    for (int i = 0; i < sphere.detailCirclesNum; i++)
    {
            sf::CircleShape circle(sphere.radius - i * sphere.radius / sphere.detailCirclesNum, 100);
            circle.setPosition(sphere.x + i * sphere.radius / sphere.detailCirclesNum, sphere.y + i * sphere.radius / sphere.detailCirclesNum);
            circle.setFillColor(sf::Color(sphere.colorRed * i / sphere.detailCirclesNum, sphere.colorGreen * i / sphere.detailCirclesNum, sphere.colorBlue * i / sphere.detailCirclesNum));
            window->draw(circle);
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

        for (int i = 0; i < 500; i++)
        {
            assert(particles);

            drawSphere(particles[i], &window);
        }

        window.display();

        for (int i = 0; i < 500; i++)
        {
            for (int j = i + 1; j < 500; j++)
            {
                assert(particles);

                collideTwoSpheres(&particles[i], &particles[j]);
            }
        }

        for (int i = 0; i < 500; i++)
        {
            assert(particles);

            collideWithWall(&particles[i]);
        }

        for (int i = 0; i < 500; i++)
        {
            assert(particles);

            moveSphere(&particles[i], dT);
        }
    }

    return 0;
}
