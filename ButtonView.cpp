#include "ButtonView.h"
#include <iostream>

ButtonView::ButtonView(float x, float y, float width, float height, int idx,std::string text)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->idx = idx;
	this->text = text;

	button.setPosition(x, y);
	button.setFillColor(sf::Color::Red);
	button.setSize(sf::Vector2f(width, height));
	float cornerRadius = 10.0f;
	button.setOutlineThickness(2.0f);
	button.setOutlineColor(sf::Color::White);
	button.setOutlineThickness(-1); 
	 

	font.loadFromFile("fontt.ttf");

	
	//buttonText.setFont(font);
	buttonText.setString(text);
	buttonText.setCharacterSize(9);
	buttonText.setFillColor(sf::Color::White);
	buttonText.setFont(font);
	buttonText.setPosition(x + (width - buttonText.getLocalBounds().width) / 2.0f,
		y + (height - buttonText.getLocalBounds().height-5)/ 2.0f);
	button.getGlobalBounds();

}
int ButtonView::handleEvent(sf::Event& event) {
	if (event.type == sf::Event::MouseMoved) {
		
		isHovered=button.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y);
	}
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		if (isHovered) {
			std::cout << "clicked\n";
			
			return idx;

		}
	}
	return 0;


}


void ButtonView::update() {
	if (isHovered) {
		button.setFillColor(sf::Color(1, 0, 100)); // Change color when hovered
	}
	else {
		button.setFillColor(sf::Color(100, 0,0));
	}
}


ButtonView::ButtonView(){



}

void ButtonView::draw(sf::RenderWindow& window)
{
	window.draw(button);
	window.draw(buttonText);

}
