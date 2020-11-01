#include <math.h>
#include <cassert>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Vector2f.h"

class Rigid
{
    public:
        
        float rigidity = 0;
        float firstLength = 0;
        float x1 = 0;
        float y1 = 0;
        float x2 = 0;
        float y2 = 0;
        

        void draw(sf::RenderWindow* window)
        {
            sf::VertexArray line(sf::Lines, 2);

            line[0].position = sf::Vector2f(x1, y1);
            line[1].position = sf::Vector2f(x2, y2);

            window->draw(line);
        }
};

class Sphere
{
    public:

        float mass;
        float x;
        float y;
        float radius;
        float speedX;
        float speedY;
        float accelerationX;
        float accelerationY;
        int colorRed;
        int colorGreen;
        int colorBlue;
        int detailCirclesNum;

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

        void move(float dT)
        {
            x = x + speedX * dT + accelerationX * dT * dT / 2;
            y = y + speedY * dT + accelerationX * dT * dT / 2;

            speedX = speedX + accelerationX * dT;
            speedY = speedY + accelerationY * dT;
        }

        void changeRigidAcceleration(Sphere* sphere, Rigid rigid, float dT)
        {
            Vector2f radiusVector1(x, y);
            Vector2f radiusVector2(sphere->x, sphere->y);
            Vector2f deltaTwoSpheres = radiusVector1 - radiusVector2;
            Vector2f deltaRigid = deltaTwoSpheres.normalize() * (rigid.firstLength - deltaTwoSpheres.length());

            Vector2f acceleration2 = deltaRigid * (- (rigid.rigidity / sphere->mass));
            Vector2f acceleration1 = deltaRigid * (rigid.rigidity / mass);

            accelerationX += acceleration1.x;
            accelerationY += acceleration1.y;
            sphere->accelerationX += acceleration2.x;
            sphere->accelerationY += acceleration2.y;
        }
};

void drawAllSphereConnections(Sphere* spheres, Rigid** rigids, int sphereNumber, sf::RenderWindow* window)
{
    for (int i = 0; i < sphereNumber; i++)
        {
            for (int j = i + 1; j < sphereNumber; j++)
            {
                assert(rigids);

                rigids[i][j].x1 = spheres[i].x + spheres[i].radius;
                rigids[i][j].y1 = spheres[i].y + spheres[i].radius;
                rigids[i][j].x2 = spheres[j].x + spheres[i].radius;
                rigids[i][j].y2 = spheres[j].y + spheres[i].radius;

                rigids[i][j].draw(window);
            }
        }
}

void changeAllSpheresRigidAcceleration(Sphere* spheres, int sphereNumber, Rigid** rigids, float dT)
{
    for (int i = 0; i < sphereNumber; i++)
    {
        spheres[i].accelerationX = 0; 
        spheres[i].accelerationY = 0; 
    }
    
    for (int i = 0; i < sphereNumber; i++)
        {
            for (int j = i + 1; j < sphereNumber; j++)
            {
                assert(spheres);

                spheres[i].changeRigidAcceleration(&spheres[j], rigids[i][j], dT);
            }
        }
}

void drawAllSpheres(Sphere* spheres, int sphereNumber, sf::RenderWindow* window)
{
    for (int i = 0; i < sphereNumber; i++)
        {
            assert(spheres);

            spheres[i].draw(window);
        }
}

void moveAllSpheres(Sphere* spheres, int sphereNumber, float dT)
{
    for (int i = 0; i < sphereNumber; i++)
        {
            assert(spheres);

            spheres[i].move(dT);
        }
}

int main()
{
    int sphereNumber = 4;

    Sphere* spheres = new Sphere[sphereNumber];
    Rigid** rigids = new Rigid*[sphereNumber]; 
    for (int i = 0; i < sphereNumber; i++)
    { 
        rigids[i] = new Rigid[sphereNumber];
    }

    for (int i = 0; i < sphereNumber; i++)
    {   
        assert(spheres);

        spheres[i].mass = 400;
        spheres[i].x = 35 + rand()%(895 + 1);
        spheres[i].y = 35 + rand()%(695 + 1);
        spheres[i].radius = 35;
        spheres[i].speedY = 0;
        spheres[i].speedX = 0;
        spheres[i].accelerationX = 0;
        spheres[i].accelerationY = 0;
        spheres[i].colorRed = 255;
        spheres[i].colorGreen = 255;
        spheres[i].colorBlue = 255;
        spheres[i].detailCirclesNum = 50;
    }

    for (int i = 0; i < sphereNumber; i++)
    {   
        for (int j = i + 1; j < sphereNumber; j++)
        {
            assert(rigids);

            rigids[i][j].firstLength = 200;
            rigids[i][j].rigidity = 0.1;
            rigids[i][j].x1 = spheres[i].x + spheres[i].radius;
            rigids[i][j].y1 = spheres[i].y + spheres[i].radius;
            rigids[i][j].x2 = spheres[j].x + spheres[i].radius;
            rigids[i][j].y2 = spheres[j].y + spheres[i].radius;
        }
    }

    float dT = 0.1;

    sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML window");

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

        drawAllSpheres(spheres, sphereNumber, &window);
        drawAllSphereConnections(spheres, rigids, sphereNumber, &window);

        window.display();

        changeAllSpheresRigidAcceleration(spheres, sphereNumber, rigids, dT);

        moveAllSpheres(spheres, sphereNumber, dT);
    }

    delete[] spheres;

    for (int i = 0; i < sphereNumber; i++)
    { 
        delete[] rigids[i];
    }

    return 0;
}