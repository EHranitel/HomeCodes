#include <math.h>
#include <cassert>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Vector2f
{
    public:

        float x = 0;
        float y = 0;

        Vector2f (float x, float y)
        {
            this->x = x;
            this->y = y;  
        }

        Vector2f operator+ (Vector2f v)
        {
            return Vector2f(x + v.x, y + v.y);
        }

        Vector2f operator- (Vector2f v)
        {
            return Vector2f(x - v.x, y - v.y);
        }

        Vector2f operator* (float n)
        {
            return Vector2f(x * n, y * n);
        }

        Vector2f operator/ (float n)
        {
            return Vector2f(x / n, y / n);
        }

        float operator* (Vector2f v)
        {
            return x * v.x + y * v.y;
        }

        float length()
        {
            return sqrt(pow(x, 2) + pow(y, 2));
        }

        Vector2f normalize()
        {
            return Vector2f(x / length(), y / length());
        }
};

class Rigid
{
    public:
        
        float rigidity = 0;
        float firstLength = 0;
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

        void move(int dT)
        {
            x = x + speedX * dT + accelerationX * dT * dT / 2;
            y = y + speedY * dT + accelerationY * dT * dT / 2;
        }
};

void drawLine(Vector2f v1, Vector2f v2, sf::RenderWindow* window)
{
    sf::VertexArray line(sf::Lines, 2);

    line[0].position = sf::Vector2f(v1.x, v1.y);
    line[1].position = sf::Vector2f(v2.x, v2.y);

    window->draw(line);
}

void drawAllSphereConnections(Sphere* spheres, int sphereNumber, sf::RenderWindow* window)
{
    for (int i = 0; i < sphereNumber; i++)
        {
            for (int j = i + 1; j < sphereNumber; j++)
            {
                assert(spheres);

                Vector2f v1(spheres[i].x + spheres[i].radius, spheres[i].y + spheres[i].radius);
                Vector2f v2(spheres[j].x + spheres[j].radius, spheres[j].y + spheres[j].radius);
                drawLine(v1, v2, window);
            }
        }
}

void changeRigidAcceleration(Sphere* sphere1, Sphere* sphere2, Rigid rigid)
{
    Vector2f radiusVector1(sphere1->x, sphere1->y);
    Vector2f radiusVector2(sphere2->x, sphere2->y);
    Vector2f deltaTwoSpheres = radiusVector1 - radiusVector2;
    Vector2f deltaRigid = deltaTwoSpheres.normalize() * (rigid.firstLength - deltaTwoSpheres.length());

    Vector2f acceleration2 = deltaRigid * (- (rigid.rigidity / sphere2->mass));
    Vector2f acceleration1 = deltaRigid * (rigid.rigidity / sphere1->mass);

    sphere1->accelerationX += acceleration1.x;
    sphere1->accelerationY += acceleration1.y;
    sphere2->accelerationX += acceleration2.x;
    sphere2->accelerationY += acceleration2.y;
}

void changeAllSpheresRigidAcceleration(Sphere* spheres, int sphereNumber, Rigid rigids[4][4])
{
    for (int i = 0; i < sphereNumber; i++)
        {
            for (int j = i + 1; j < sphereNumber; j++)
            {
                assert(spheres);
                
                changeRigidAcceleration(&spheres[i], &spheres[j], rigids[i][j]);
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

void moveAllSpheres(Sphere* spheres, int sphereNumber, int dT)
{
    for (int i = 0; i < sphereNumber; i++)
        {
            assert(spheres);

            spheres[i].move(dT);
        }
}

int main()
{
    Sphere spheres[4];
    Rigid rigids[4][4]; 

    for (int i = 0; i < 4; i++)
    {   
        for (int j = i + 1; j < 4; j++)
        {
            assert(rigids);

            rigids[i][j].firstLength = 400;
            rigids[i][j].rigidity = 0.5;
        }
    }
    
    for (int i = 0; i < 4; i++)
    {   
        assert(spheres);

        spheres[i].mass = 25;
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

    int dT = 1;

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

        drawAllSpheres(spheres, 4, &window);
        drawAllSphereConnections(spheres, 4, &window);

        window.display();

        changeAllSpheresRigidAcceleration(spheres, 4, rigids);

        moveAllSpheres(spheres, 4, dT);
    }

    return 0;
}