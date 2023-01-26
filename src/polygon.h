#include <SFML/Graphics.hpp>

const float PI = 3.14159;

void CreateRegularPolygon(sf::ConvexShape& polygon, const float radius) {

    const int nPoints = polygon.getPointCount();
    
    const float dTheta = 2 * PI / nPoints;

    for(int i = 0; i < nPoints; i++) {
        const float theta = i * dTheta;
        polygon.setPoint(i, sf::Vector2f(radius * cos(theta), radius * sin(theta)));
    }
}