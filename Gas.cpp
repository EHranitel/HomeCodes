#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

struct Sphere
{
    int x;
    int y;
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
    return (sphere1.x - sphere2.x) * (sphere1.x - sphere2.x) + (sphere1.y - sphere2.y) * (sphere1.y - sphere2.y) <= (sphere1.radius + sphere2.radius) * (sphere1.radius + sphere2.radius);
}

void collideWithWall(Sphere* sphere)
{
    if((sphere->x + 2 * sphere->radius >= 1920 && sphere->speedX > 0) || (sphere->x <= 0 && sphere->speedX < 0))
    {
        sphere->speedX = - sphere->speedX;
    }

    if((sphere->y + 2 * sphere->radius >= 1080 && sphere->speedY > 0) || (sphere->y <= 0 & sphere->speedY < 0))
    {
        sphere->speedY = - sphere->speedY;
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

    sphere1->speedX = speedX2BeforeCollision;
    sphere1->speedY = speedY2BeforeCollision;
    sphere2->speedX = speedX1BeforeCollision;
    sphere2->speedY = speedY1BeforeCollision;

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
    Sphere particles[100];

    int dT = 1;

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML window");

    for (int i = 0; i < 100; i++)
    {   
        particles[i].x = 20 + rand()%(1900-20+1);
        particles[i].y = 20 + rand()%(1060-20+1);
        particles[i].radius = 10;
        particles[i].speedY = -10 + rand()%(20+10+1);
        particles[i].speedX = -10 + rand()%(20+10+1);
        particles[i].colorRed = 0;
        particles[i].colorGreen = 0;
        particles[i].colorBlue = 255;
        particles[i].detailCirclesNum = 30;
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

        for (int i = 0; i < 100; i++)
        {
            drawSphere(particles[i], &window);
        }

        drawSphere(particles[0], &window);

        window.display();

        for (int i = 0; i < 100; i++)
        {
            for (int j = i + 1; j < 100; j++)
            {
                collideTwoSpheres(&particles[i], &particles[j]);
            }
        }

        for (int i = 0; i < 100; i++)
        {
            collideWithWall(&particles[i]);
        }

        for (int i = 0; i < 100; i++)
        {
            moveSphere(&particles[i], dT);
        }
    }

    return 0;
}
