#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

float DegToRad(float deg) {
    return deg * 3.14159 / 180.0f;
}

sf::Vector2f CalculateNormal(sf::Vector2f pointA, sf::Vector2f pointB) {
    // get vector pointing from one point to the other
    sf::Vector2f directionVector = pointB - pointA;
    // swap co-efficents and negate one to calculate normal (not normalized)
    return sf::Vector2f(-directionVector.y, directionVector.x);
}

std::vector<sf::Vector2f> CalculateAllNormals(const std::vector<sf::Vector2f>& vertices) {
    // a polygon has as many sides and vertices, thus as many normals
    std::vector<sf::Vector2f> normals(vertices.size());

    for(size_t i = 0; i < vertices.size() - 1; i++) {
        normals[i] = CalculateNormal(vertices[i + 1], vertices[i]);
    }
    normals[vertices.size() - 1] = CalculateNormal(vertices[vertices.size() - 1], vertices[0]);
    return normals;
}

sf::Vector2f TransformPoint(sf::Vector2f point, float rotation, sf::Vector2f translation) {
    
    sf::Vector2f transformedPoint(
        point.x * cos(DegToRad(rotation)) - point.y * sin(DegToRad(rotation)),
        point.y * cos(DegToRad(rotation)) + point.x * sin(DegToRad(rotation))
    );

    transformedPoint += translation;
    return transformedPoint;
}

float Vector2fDot (sf::Vector2f a, sf::Vector2f b) {
    return (a.x * b.x + a.y * b.y);
}

void DrawNormals(const std::vector<sf::Vector2f>& vertices, const std::vector<sf::Vector2f>& normals, sf::RenderWindow& window) {
    for(size_t i = 0; i < vertices.size() - 1; i++) {
        sf::Vector2f midPoint = (vertices[i] + vertices[i + 1]) / 2.0f;
        sf::CircleShape point(5.0f, 10);
        point.setOrigin(sf::Vector2f(2.5f, 2.5f));
        point.setPosition(midPoint + normals[i]);
        window.draw(point);
    }

    sf::Vector2f midPoint = (vertices[vertices.size() - 1] + vertices[0]) / 2.0f;
    sf::CircleShape point(5.0f, 10);
    point.setOrigin(sf::Vector2f(2.5f, 2.5f));
    point.setPosition(midPoint + normals[normals.size() - 1]);
    window.draw(point);
}

bool SATPolygonCollision(const sf::ConvexShape& polygonA, const sf::ConvexShape& polygonB, sf::RenderWindow& window) {

    const size_t nPointsA = polygonA.getPointCount(), nPointsB = polygonB.getPointCount();
    std::vector<sf::Vector2f> verticesA(nPointsA), verticesB(nPointsB);

    for(size_t i = 0; i < nPointsA; i++) {
        verticesA[i] = TransformPoint(polygonA.getPoint(i), polygonA.getRotation(), polygonA.getPosition());
    }

    for(size_t i = 0; i < nPointsB; i++) {
        verticesB[i] = TransformPoint(polygonB.getPoint(i), polygonB.getRotation(), polygonB.getPosition());
    }

    // calculate normals for each side of polygons
    auto polygonANormals = CalculateAllNormals(verticesA);
    DrawNormals(verticesA, polygonANormals, window);
    auto polygonBNormals = CalculateAllNormals(verticesB);
    DrawNormals(verticesB, polygonBNormals, window);

    // combine normals of the two polygon in one array to loop over
    std::vector<sf::Vector2f> normals;
    normals.insert(normals.end(), polygonANormals.begin(), polygonANormals.end());
    normals.insert(normals.end(), polygonBNormals.begin(), polygonBNormals.end());

    bool isColliding = true;
    for(sf::Vector2f normal : normals) {
        
        // min and max projection of polygon A
        float paMin = FLT_MAX, paMax = FLT_MIN;

        for(sf::Vector2f vertex : verticesA) {
            float projection = Vector2fDot(vertex, normal);
            paMin = std::min(paMin, projection);
            paMax = std::max(paMax, projection);
        }

        float pbMin = FLT_MAX, pbMax = FLT_MIN;

        for(sf::Vector2f vertex : verticesB) {
            float projection = Vector2fDot(vertex, normal);
            pbMin = std::min(pbMin, projection);
            pbMax = std::max(pbMax, projection);
        }

        if(paMax < pbMin || pbMax < paMin) {
            isColliding = false;
        }
        else {
            // std::cout << "Collision";
        }
    }

    return isColliding;

}