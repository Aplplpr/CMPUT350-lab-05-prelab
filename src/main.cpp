#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;
const int FRAMES_PER_ANIMATION = 90;

// global tween function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        }

        // ====== ====== ======
        // TODO: (Q2)
        //  implement key presses (1-9) that replace the tween function
        //  with different alternate tween functions.
        //  Functions can be from lecture or from https://easings.net/#
        // ====== ====== ======
        if (const auto* keyPressed =
                event->getIf<sf::Event::KeyPressed>()) { 

            if (keyPressed->code == sf::Keyboard::Key::Num1) {//linear
                tween = [](float a, float b, float t) {
                    return (1 - t) * a + t * b;
                };
            }

            else if (keyPressed->code == sf::Keyboard::Key::Num2) {//ease in quad
                tween = [](float a, float b, float t) {
                    float easedT = t * t;
                    return (1 - easedT) * a + easedT * b;
                };
            }

            else if (keyPressed->code == sf::Keyboard::Key::Num3) {//ease out quad
                tween = [](float a, float b, float t) {
                    float easedT = 1 - (1 - t) * (1 - t);
                    return (1 - easedT) * a + easedT * b;
                };
            }

            else if (keyPressed->code == sf::Keyboard::Key::Num4) {//ease in-out quad
                tween = [](float a, float b, float t) {
                    float easedT;

                    if (t < 0.5f) {
                        easedT = 2 * t * t;
                    } else {
                        float x = -2 * t + 2;
                        easedT = 1 - (x * x) / 2;
                    }

                    return (1 - easedT) * a + easedT * b;
                };
            }

            else if (keyPressed->code == sf::Keyboard::Key::Num5) {// ease in cubic
                tween = [](float a, float b, float t) {
                    float easedT = t * t * t;
                    return (1 - easedT) * a + easedT * b;
                };
            }

            else if (keyPressed->code == sf::Keyboard::Key::Num6) {//ease out cubic
                tween = [](float a, float b, float t) {
                    float x = 1 - t;
                    float easedT = 1 - x * x * x;

                    return (1 - easedT) * a + easedT * b;
                };
            }

            else if (keyPressed->code == sf::Keyboard::Key::Num7) {//ease in cubic
                tween = [](float a, float b, float t) {
                    float easedT;

                    if (t < 0.5f) {
                        easedT = 4 * t * t * t;
                    } else {
                        float x = -2 * t + 2;
                        easedT = 1 - (x * x * x) / 2;
                    }

                    return (1 - easedT) * a + easedT * b;
                };
            }

            else if (keyPressed->code == sf::Keyboard::Key::Num8) {//ease in quart
                tween = [](float a, float b, float t) {
                    float easedT = t * t * t * t;
                    return (1 - easedT) * a + easedT * b;
                };
            }

            else if (keyPressed->code == sf::Keyboard::Key::Num9) {// ease out quart
                tween = [](float a, float b, float t) {
                    float x = 1 - t;
                    float easedT = 1 - x * x * x * x;

                    return (1 - easedT) * a + easedT * b;
                };
            }
        }
    }
}

void render(sf::RenderWindow& window) {
    // Clear with blue background (sky)
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Q1) Draw circle that moves between
    // the left/right half of the screen.
    // Movement should be governed by the tween function.
    // ====== ====== ======
    static int frame = 0;

    float t =
        static_cast<float>(frame) / (FRAMES_PER_ANIMATION - 1);

    float leftX = 100.0f;
    float rightX = 700.0f;
    float y = WINDOW_HEIGHT / 3.0f;

    float x = tween(leftX, rightX, t);

    sf::CircleShape circle(20.0f);
    circle.setOrigin({20.0f, 20.0f});
    circle.setPosition({x, y});
    circle.setFillColor(sf::Color::White);

    window.draw(circle);

    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======

    const float graphLeft = 100.0f;
    const float graphTop = 500.0f;
    const float graphWidth = 600.0f;
    const float graphHeight = 200.0f;

    // x-axis
    sf::RectangleShape xAxis({graphWidth, 2.0f});
    xAxis.setPosition(
        {graphLeft, graphTop + graphHeight}
    );
    xAxis.setFillColor(sf::Color::White);
    window.draw(xAxis);

    // y-axis
    sf::RectangleShape yAxis({2.0f, graphHeight});
    yAxis.setPosition({graphLeft, graphTop});
    yAxis.setFillColor(sf::Color::White);
    window.draw(yAxis);

    // tween curve
    const int samples = 100;

    sf::VertexArray curve(
        sf::PrimitiveType::LineStrip
    );

    for (int i = 0; i <= samples; i++) {
        float sampleT =
            static_cast<float>(i) / samples;

        float easedT =
            tween(0.0f, 1.0f, sampleT);

        float graphX =
            graphLeft + sampleT * graphWidth;

        float graphY =
            graphTop + graphHeight
            - easedT * graphHeight;

        curve.append(
            sf::Vertex(
                {graphX, graphY},
                sf::Color::Green
            )
        );
    }

    window.draw(curve);

    // current position on curve
    float currentTween =
        tween(0.0f, 1.0f, t);

    float dotX =
        graphLeft + t * graphWidth;

    float dotY =
        graphTop + graphHeight
        - currentTween * graphHeight;

    sf::CircleShape dot(6.0f);
    dot.setOrigin({6.0f, 6.0f});
    dot.setPosition({dotX, dotY});
    dot.setFillColor(sf::Color::Red);

    window.draw(dot);


    // Move to next frame only after Q1 and Q3
    // have both used the current t.
    frame++;

    if (frame >= FRAMES_PER_ANIMATION) {
        frame = 0;
    }

    window.display();

}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Tween");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            render(window);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
