#ifndef Sphere_h
#define Sphere_h
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
        float accelerationX;
        float accelerationY;
        int colorRed = 0;
        int colorGreen = 0;
        int colorBlue = 0;
        int detailCirclesNum = 0;

        void move(float dT)
        {
            x = x + speedX * dT + accelerationX * dT * dT / 2;
            y = y + speedY * dT + accelerationX * dT * dT / 2;

            speedX = speedX + accelerationX * dT;
            speedY = speedY + accelerationY * dT;
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

#endif