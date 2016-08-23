#include <iostream>
#include <SFML/Graphics.hpp>


class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles[i + j * width];

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};


int main()
{
    // create the window
    using namespace std;
    sf::RenderWindow window(sf::VideoMode(1280, 768), "TileMap");



    // set the shape color to green
    int level[2100];
    int midDistance = 0;
    int nextRow = 60;
    int current = 30;
    int left = 0;
    int right = 0;
    int fontSize = 12;
    int half = fontSize/2 + 1;
    int thickness = 1;
    int i;


    for(int i = 0; i < 2100; i++){
        level[i] = 0;
    }

    for(int i = 0; i < fontSize; i++){
        cout << current << endl;
        cout << left << endl;
        cout << right << endl;
        if(midDistance != 0){
            current += nextRow;
            left = current - midDistance;
            right = current + midDistance;
            level[left] = 3;
            level[right] = 3;

            if(i == half){
                for(int j = left; j <= right; j++){
                    level[j] = 3;
                }
            }

            if(i == half+1 && (fontSize != 12 && fontSize > 12)){
                for(int j = left; j <= right; j++){
                    level[j] = 3;
                }
            }
            else{
                for(int widen = 1; widen <= thickness; widen++){
                    level[left + widen] = 3;
                    level[right - widen] = 3;
                }
            }
            midDistance += 1;
        }
        else{
            level[current] = 3;
            midDistance += 1;
        }
    }

    // create the tilemap from the level definition
    TileMap map;
    if (!map.load("tileset.png", sf::Vector2u(20, 20), level, 60, 35))
        return -1;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.text.unicode < 128)
                    std::cout << "Enter resize percentage (Max: 200%): " << std::endl;
                    std::cin >> i;
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);
        window.clear();
        window.draw(map);
        // draw everything here...
        // window.draw(...);
        // end the current frame
        window.display();
    }

    return 0;
}
