#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

enum GameState
{
    NotStarted,
    Started,
    Finished,
};
struct Sphere
{
    int mass;
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
    if((sphere->x + 2 * sphere->radius >= 800 && sphere->speedX > 0) || (sphere->x <= 0 && sphere->speedX < 0))
    {
        sphere->speedX = - sphere->speedX;
    }

    if((sphere->y + 2 * sphere->radius >= 600 && sphere->speedY > 0) || (sphere->y <= 0 & sphere->speedY < 0))
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

    if (sphere1->mass == sphere2->mass)
    {
        sphere1->speedX = speedX2BeforeCollision;
        sphere1->speedY = speedY2BeforeCollision;
        sphere2->speedX = speedX1BeforeCollision;
        sphere2->speedY = speedY1BeforeCollision;
    }

    else if (sphere1->mass > sphere2->mass)
    {
        sphere2->speedX = -speedX2BeforeCollision + 2 * speedX1BeforeCollision;
        sphere2->speedY = -speedY2BeforeCollision + 2 * speedY1BeforeCollision;   
    }

    else
    {
        sphere1->speedX = -speedX1BeforeCollision + 2 * speedX2BeforeCollision;
        sphere1->speedY = -speedY1BeforeCollision + 2 * speedY2BeforeCollision;
    }
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

void printText(int x, int y, sf::String textText, sf::Font textFont, int textSize, sf::Color textColor, sf::RenderWindow* window)
{
    sf::Text text(textText, textFont, textSize);
    text.setPosition(x, y);
    text.setFillColor(textColor);
    window->draw(text);
}

bool clickStartButton(sf::Vector2i mousePosition, sf::RenderWindow* window)
{
    if (300 < mousePosition.x && mousePosition.x < 500 && 200 < mousePosition.y && mousePosition.y < 400 &&  sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        return true;
    }

    return false;
}

void setControlledSphereParametrs(Sphere* sphere, int dT, sf::RenderWindow* window)
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

    sphere->speedX = (mousePosition.x - sphere->radius - sphere->x) / dT;
    sphere->speedY = (mousePosition.y - sphere->radius - sphere->y) / dT;
    
    sphere->x = mousePosition.x - sphere->radius;
    sphere->y = mousePosition.y - sphere->radius;    
}

bool checkWinConditions(Sphere* sphereYou, Sphere* sphereFriend)
{
    if (checkCollision(*sphereYou, *sphereFriend) || sphereFriend->colorRed == 255)
    {
        sphereFriend->colorRed = 255;
        return true;
    }

    return false;
}

bool checkLoseConditions(Sphere sphereYou, Sphere sphereEnemy)
{
    return checkCollision(sphereYou, sphereEnemy);
}

int main()
{
    Sphere sphereEnemy1 = {10, 20, 20, 35, 7, 8, 255, 0, 0, 100};
    Sphere sphereEnemy2 = {10, 710, 510, 35, 5, 9, 255, 0, 0, 100};
    Sphere sphereFriend1 = {10, 20, 510, 35, -6, 6, 0, 255, 0, 100};
    Sphere sphereFriend2 = {10, 710, 20, 35, 5, -7, 0, 255, 0, 100};
    Sphere sphereNeutral1 = {10, 50, 265, 35, -3, -10, 255, 255, 0, 100};
    Sphere sphereNeutral2 = {10, 680, 265, 35, 9, -8, 255, 255, 0, 100};
    Sphere sphereYou = {100, 300, 200, 100, 0, 0, 255, 255, 255, 100};

    int dT = 1;

    sf::Font textFont;
    if (!textFont.loadFromFile("TimesNewRoman.ttf"))
    {
        return EXIT_FAILURE;
    }
    sf::Color textColorStartScreen = sf::Color::White;
    sf::Color textColorWin = sf::Color::Green;
    sf::Color textColorLose = sf::Color::Red;
    int textSize = 200;
    int textSizeLose = 150;
    int textSizeWin = 150; 

    GameState state = GameState::NotStarted;

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    window.clear();

    drawSphere(sphereEnemy1, &window);
    drawSphere(sphereEnemy2, &window);
    drawSphere(sphereFriend1, &window);
    drawSphere(sphereFriend2, &window);
    drawSphere(sphereNeutral1, &window);
    drawSphere(sphereNeutral2, &window);
    drawSphere(sphereYou, &window);

    printText(100, -30, "CLICK", textFont, textSize, textColorStartScreen, &window);
    printText(185, 170, "T", textFont, textSize, textColorStartScreen, &window);
    printText(500, 170, "O", textFont, textSize, textColorStartScreen, &window);
    printText(100, 360, "START", textFont, textSize, textColorStartScreen, &window);    

    window.display();

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

        if (state == GameState::Finished && sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            return 0;
        }

        if (state == GameState::Finished)
        {
            continue;
        }

        if (state == GameState::NotStarted && clickStartButton(sf::Mouse::getPosition(window), &window))
        {
            state = GameState::Started;

            for (int i = 101; i > 25; i--)
            {
                sphereYou.radius = i;
                sphereYou.x += 1;
                sphereYou.y += 1;
                drawSphere(sphereYou, &window);

                window.display();
            }
        }

        if (state == GameState::NotStarted)
        {
            continue;
        }

        window.clear();

        setControlledSphereParametrs(&sphereYou, dT, &window);

        drawSphere(sphereEnemy1, &window);
        drawSphere(sphereEnemy2, &window);
        drawSphere(sphereFriend1, &window);
        drawSphere(sphereFriend2, &window);
        drawSphere(sphereNeutral1, &window);
        drawSphere(sphereNeutral2, &window);
        drawSphere(sphereYou, &window);

        window.display();

        if (checkLoseConditions(sphereYou, sphereEnemy1) || checkLoseConditions(sphereYou, sphereEnemy2))
        {
            window.clear();

            printText(35, 200, "YOU LOSE", textFont, textSizeLose, textColorLose, &window);

            window.display();
            
            state = GameState::Finished;

            continue;
        }

        setControlledSphereParametrs(&sphereYou, dT, &window);
        checkWinConditions(&sphereYou, &sphereFriend1);
        checkWinConditions(&sphereYou, &sphereFriend2);
        if (checkWinConditions(&sphereYou, &sphereFriend1) && checkWinConditions(&sphereYou, &sphereFriend2))
        {
            window.clear();

            printText(75, 200, "YOU WIN", textFont, textSizeLose, textColorWin, &window);

            window.display();

            state = GameState::Finished;

            continue;
        }

        collideWithWall(&sphereEnemy1);
        collideWithWall(&sphereEnemy2);
        collideWithWall(&sphereFriend1);
        collideWithWall(&sphereFriend2);
        collideWithWall(&sphereNeutral1);
        collideWithWall(&sphereNeutral2);

        collideTwoSpheres(&sphereYou, &sphereFriend1);
        collideTwoSpheres(&sphereYou, &sphereFriend2);
        collideTwoSpheres(&sphereYou, &sphereNeutral1);
        collideTwoSpheres(&sphereYou, &sphereNeutral2);

        collideTwoSpheres(&sphereEnemy1, &sphereEnemy2);
        collideTwoSpheres(&sphereEnemy1, &sphereFriend1);
        collideTwoSpheres(&sphereEnemy1, &sphereFriend2);
        collideTwoSpheres(&sphereEnemy1, &sphereNeutral1);
        collideTwoSpheres(&sphereEnemy1, &sphereNeutral2);
        collideTwoSpheres(&sphereEnemy2, &sphereFriend1);
        collideTwoSpheres(&sphereEnemy2, &sphereFriend2);
        collideTwoSpheres(&sphereEnemy2, &sphereNeutral1);
        collideTwoSpheres(&sphereEnemy2, &sphereNeutral2);
        collideTwoSpheres(&sphereFriend1, &sphereFriend2);
        collideTwoSpheres(&sphereFriend1, &sphereNeutral1);
        collideTwoSpheres(&sphereFriend1, &sphereNeutral2);
        collideTwoSpheres(&sphereFriend2, &sphereNeutral1);
        collideTwoSpheres(&sphereFriend2, &sphereNeutral2);
        collideTwoSpheres(&sphereNeutral1, &sphereNeutral2);


        moveSphere(&sphereEnemy1, dT);
        moveSphere(&sphereEnemy2, dT);
        moveSphere(&sphereFriend1, dT);
        moveSphere(&sphereFriend2, dT);
        moveSphere(&sphereNeutral1, dT);
        moveSphere(&sphereNeutral2, dT);
    }

    return 0;
}



