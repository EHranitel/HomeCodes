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

int main()
{
    Sphere particles[4];
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
        assert(particles);

        particles[i].mass = 25;
        particles[i].x = 35 + rand()%(895 + 1);
        particles[i].y = 35 + rand()%(695 + 1);
        particles[i].radius = 35;
        particles[i].speedY = 0;
        particles[i].speedX = 0;
        particles[i].accelerationX = 0;
        particles[i].accelerationY = 0;
        particles[i].colorRed = 255;
        particles[i].colorGreen = 255;
        particles[i].colorBlue = 255;
        particles[i].detailCirclesNum = 50;
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

        for (int i = 0; i < 4; i++)
        {
            assert(particles);

            particles[i].draw(&window);
        }

        for (int i = 0; i < 4; i++)
        {
            for (int j = i + 1; j < 4; j++)
            {
                assert(particles);

                Vector2f v1(particles[i].x + particles[i].radius, particles[i].y + particles[i].radius);
                Vector2f v2(particles[j].x + particles[j].radius, particles[j].y + particles[j].radius);
                drawLine(v1, v2, &window);
            }
        }

        window.display();

        for (int i = 0; i < 4; i++)
        {
            for (int j = i + 1; j < 4; j++)
            {
                assert(particles);
                
                changeRigidAcceleration(&particles[i], &particles[j], rigids[i][j]);
            }
        }

        for (int i = 0; i < 4; i++)
        {
            assert(particles);

            particles[i].move(dT);
        }
    }

    return 0;
}