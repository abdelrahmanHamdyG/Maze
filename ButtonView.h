#pragma once
#include<string>
#include <SFML/Graphics.hpp>
class ButtonView
{
public:
	float x;
	float y;
	float width;
	bool isHovered;
	sf::RectangleShape button;
	sf::Text buttonText;
	float height;
	int idx;
	sf::Font font;
	
	std::string text;


	ButtonView(float x, float y, float width, float heigth, int idx, std::string text);

	int handleEvent(sf::Event& event);

	ButtonView();
	void draw(sf::RenderWindow& window);
	void update();



};

