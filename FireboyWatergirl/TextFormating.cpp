#include "TextFormating.h"

void centerText(sf::Text& text, const sf::RenderWindow& window)
{
    // Get the bounding box of the text
    sf::FloatRect textBounds = text.getLocalBounds();

    // Calculate the x-coordinate for centering
    float xPos = (window.getSize().x / 2.f) - (textBounds.width / 2.f) - textBounds.left;

    // Keep the y-coordinate unchanged or adjust as needed
    float yPos = text.getPosition().y;

    // Set the new position of the text
    text.setPosition(xPos, yPos);
}