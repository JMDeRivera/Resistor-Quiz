#include <iostream>
#include <map>
#include <math.h>
#include <SFML/Graphics.hpp>

class QuizHandler
{
    // the maps are for the sf::Color attributes used in displaying the band colors
    std::map<int, sf::Color> itocol_map = {{0, sf::Color::Black}, {1, sf::Color{139, 69, 19}}, {2, sf::Color::Red}, {3, sf::Color{240, 128, 0}}, {4, sf::Color::Yellow}, {5, sf::Color::Green}, {6, sf::Color::Blue}, {7, sf::Color{127, 0, 255}}, {8, {128, 128, 128}}, {9, sf::Color::White}};
    std::map<int, sf::Color> itotolcol_map = {{0, sf::Color{255, 215, 0}}, {1, sf::Color{192, 192, 192}}};
    unsigned int tol_array[2] = {5, 10};

    // tolerance is the randomly-generated tol_array index value
    // correct_tolerance is the actual tol %
    // input_tolerance is the user input
    unsigned int first_num, second_num, multiplier, tolerance, correct_tolerance, input_tolerance, points = 0, items = 0;
    unsigned long long correct_answer, input_answer;

public:
    bool stop_program = false;

    void generate_rand()
    {
        srand(time(0));
        first_num = rand() % 10;
        second_num = rand() % 10;
        multiplier = rand() % 10;
        tolerance = rand() % 2;
        correct_answer = ((first_num * 10) + second_num) * pow(10, multiplier);
        correct_tolerance = tol_array[tolerance];
    }

    void check_answers(unsigned long long input_answer, unsigned long long input_tolerance)
    {
        if (input_answer == correct_answer && input_tolerance == correct_tolerance)
        {
            std::cout << "\nCorrect!\n";
            points++;
        }
        else
        {
            std::cout << "\nIncorrect! The correct answer is " << correct_answer << " +- " << correct_tolerance << "%\n";
        }
        items++;
        if (items == 10)
        {
            std::cout << "\nYou scored " << points << " out of 10!";
            stop_program = true;
        }
    }

    sf::Color first_color() {return itocol_map[first_num];}
    sf::Color second_color(){return itocol_map[second_num];}
    sf::Color multiplier_color(){return itocol_map[multiplier];}
    sf::Color tolerance_color(){return itotolcol_map[tolerance];}
};

int main()
{
    QuizHandler quiz;

    sf::Image icon;
    icon.loadFromFile("images/icon.png");

    sf::RenderWindow window(sf::VideoMode(500, 500), "Resistor Quiz");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    sf::RectangleShape bg(sf::Vector2f(window.getSize().x, window.getSize().y));
    bg.setFillColor(sf::Color(255, 255, 240));

    sf::RectangleShape lead(sf::Vector2f(400, 20));
    lead.setFillColor(sf::Color{192, 192, 192});
    lead.move(50, 240);

    sf::ConvexShape resistor_body;
    resistor_body.setPointCount(16);
    resistor_body.setPoint(0, sf::Vector2f(95, 220));
    resistor_body.setPoint(1, sf::Vector2f(95, 280));
    resistor_body.setPoint(2, sf::Vector2f(115, 305));
    resistor_body.setPoint(3, sf::Vector2f(155, 305));
    resistor_body.setPoint(4, sf::Vector2f(175, 280));
    resistor_body.setPoint(5, sf::Vector2f(325, 280));
    resistor_body.setPoint(6, sf::Vector2f(345, 305));
    resistor_body.setPoint(7, sf::Vector2f(385, 305));
    resistor_body.setPoint(8, sf::Vector2f(405, 280));
    resistor_body.setPoint(9, sf::Vector2f(405, 220));
    resistor_body.setPoint(10, sf::Vector2f(385, 195));
    resistor_body.setPoint(11, sf::Vector2f(345, 195));
    resistor_body.setPoint(12, sf::Vector2f(325, 220));
    resistor_body.setPoint(13, sf::Vector2f(175, 220));
    resistor_body.setPoint(14, sf::Vector2f(155, 195));
    resistor_body.setPoint(15, sf::Vector2f(115, 195));
    resistor_body.setFillColor(sf::Color{234, 221, 202});

    sf::RectangleShape first_band(sf::Vector2f(20, 110));
    first_band.move(125, 195);

    sf::RectangleShape second_band(sf::Vector2f(20, 60));
    second_band.move(200, 220);

    sf::RectangleShape multiplier_band(sf::Vector2f(20, 60));
    multiplier_band.move(240, 220);

    sf::RectangleShape tolerance_band(sf::Vector2f(20, 110));
    tolerance_band.move(355, 195);

    // for managing the console input/output
    std::string user_input = "";
    unsigned long long input_answer;
    unsigned long long input_tolerance;
    bool to_enter_res = true;
    bool to_enter_tol = false;
    bool print_res_prompt = true;
    bool print_tol_prompt = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::TextEntered && !quiz.stop_program)
            {
                if (48 <= event.text.unicode && event.text.unicode <= 57) // accepts 0-9 only
                {
                    char char_input = static_cast<char>(event.text.unicode); // turns numerical unicode value to corres. char
                    user_input.push_back(char_input);
                    std::cout << char_input;
                }
            }
            else if (event.type == sf::Event::KeyPressed && user_input != "") // does not recognize ENTER when input is empty
            {
                if (event.key.code == sf::Keyboard::Enter)
                {
                    if (to_enter_res)
                    {
                        input_answer = stoull(user_input);
                        to_enter_res = false;
                        to_enter_tol = true;
                        print_tol_prompt = true;
                    }
                    else if (to_enter_tol)
                    {
                        input_tolerance = stoull(user_input);
                        to_enter_tol = false;
                        to_enter_res = true;
                        print_res_prompt = true;
                        quiz.check_answers(input_answer, input_tolerance);
                    }
                    std::cout << std::endl;
                    user_input = "";
                }
                else if (event.key.code == sf::Keyboard::Backspace)
                {
                    user_input.pop_back();
                    std::cout << "\b \b"; // goes back by a single character on the console and replaces the character
                }
            }
        }

        window.clear();
        window.draw(bg);
        window.draw(lead);
        window.draw(resistor_body);
        window.draw(first_band);
        window.draw(second_band);
        window.draw(multiplier_band);
        window.draw(tolerance_band);
        window.display();

        // this generates new res values and refreshes displayed bands
        if (to_enter_res && print_res_prompt && !quiz.stop_program)
        {
            quiz.generate_rand();
            first_band.setFillColor(quiz.first_color());
            second_band.setFillColor(quiz.second_color());
            multiplier_band.setFillColor(quiz.multiplier_color());
            tolerance_band.setFillColor(quiz.tolerance_color());
            std::cout << "Resistance (in Ohms): ";
            print_res_prompt = false;
        }
        if (to_enter_tol && print_tol_prompt)
        {
            std::cout << "Tolerance (in %): ";
            print_tol_prompt = false;
        }
    }

    return 0;
}
