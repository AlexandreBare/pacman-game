#include "Grid.hpp"
#include "constants.hpp"
#include "Food.hpp"
#include "Treat.hpp"
#include "PowerPill.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>

using namespace std;
using namespace sf;

static const sf::Color WALL_COLOR(160, 160, 160, 255);
static const int OFF_SCREEN_TILES_SIZE = 50;


//
// Public static function
//

void Grid::getTileIndexesFromPosition(const Coordinates& position, int res[]){
    
    res[0] = (int)(position.x / CELL_SIZE);
    res[1] = (int)(position.y / CELL_SIZE);
}

int Grid::getTileDistance(const Tile& source, const Tile& destination){
    int sourceIndexes[2];
    getTileIndexesFromPosition(source.getPosition(), sourceIndexes);
    int destinationIndexes[2];
    getTileIndexesFromPosition(destination.getPosition(), destinationIndexes);
    return abs(sourceIndexes[0] - destinationIndexes[0]) + abs(sourceIndexes[1] - destinationIndexes[1]);
}

//
// Private function
//

bool Grid::isOffScreenTile(int row, int col) const{
    return row < 0 || col < 0 || row >= GRID_SIZE[0] || col >= GRID_SIZE[1];
}

const Tile& Grid::getTile(int row, int col){
    if(isOffScreenTile(row, col)){
        head = ++head % offScreenTiles.size(); // cyclic index
        offScreenTiles[head] = (Tile(TILE_TYPE::SLOW, Coordinates(CELL_SIZE / 2 + CELL_SIZE * row, CELL_SIZE / 2 + CELL_SIZE * col)));
        return offScreenTiles[head];
    }
    return getOnScreenTile(row, col);
}

const Tile& Grid::getOnScreenTile(int row, int col) const{
    return onScreenTiles[row][col];
}

//
// Public functions
//

void Grid::update(sf::Time elapsed){
    for(decltype(onScreenTiles.size()) i = 0; i < onScreenTiles.size(); i++){
        for(decltype(onScreenTiles[i].size()) j = 0; j < onScreenTiles[i].size(); j++){
            onScreenTiles[i][j].update(elapsed);
        }
    }
}

const Tile& Grid::operator()(int row, int col){
    return getTile(row, col);
}

const Tile& Grid::operator()(const int rowcol[]){
    return getTile(rowcol[0], rowcol[1]);
}

const Tile& Grid::getTileFromPosition(const Coordinates& position){
    int indexes[2];
    getTileIndexesFromPosition(position, indexes);
    return getTile(indexes[0], indexes[1]);
    
}

void Grid::drawOnWindow(sf::RenderWindow& window) const{
    for(const auto& tileColumn : onScreenTiles){
        for(const Tile& tile : tileColumn){
            tile.drawOnWindow(window);
        }
    }
    
    drawWalls(window);
    if(DEBUG == true)
        drawLines(window);
}

bool Grid::hasFoodOnTile(const Tile &tile) const{
    int indexes[2];
    getTileIndexesFromPosition(tile.getPosition(), indexes);
    
    if(isOffScreenTile(indexes[0], indexes[1]))
        return false;
    return getOnScreenTile(indexes[0], indexes[1]).hasFood();
}

int Grid::eatFoodOnTile(const Tile& tile){
    if(!hasFoodOnTile(tile))
        throw invalid_argument("No piece of food to eat on the given tile");
    
    int indexes[2];
    getTileIndexesFromPosition(tile.getPosition(), indexes);
    if(isOffScreenTile(indexes[0], indexes[1]))
        throw invalid_argument("An off-screen tile can't have a piece of food");
    return onScreenTiles[indexes[0]][indexes[1]].eatFood();
}

bool Grid::hasContaminatedFoodOnTile(const Tile& tile) const{
    if(!hasFoodOnTile(tile))
        return false;
    
    int indexes[2];
    getTileIndexesFromPosition(tile.getPosition(), indexes);
    if(isOffScreenTile(indexes[0], indexes[1]))
        throw invalid_argument("An off-screen tile can't have a piece of food");
    return onScreenTiles[indexes[0]][indexes[1]].hasContaminatedFood();
}

void Grid::contaminateFoodOnTile(const Tile& tile){
    if(!hasFoodOnTile(tile))
        throw invalid_argument("No piece of food to contaminate on the given tile");
    
    int indexes[2];
    getTileIndexesFromPosition(tile.getPosition(), indexes);
    if(isOffScreenTile(indexes[0], indexes[1]))
        throw invalid_argument("An off-screen tile can't have a piece of food");
    onScreenTiles[indexes[0]][indexes[1]].contaminateFood();
}

Grid::Grid(){
    offScreenTiles.resize(OFF_SCREEN_TILES_SIZE);
    onScreenTiles.resize(GRID_SIZE[0]);
    
    for(int j = 0; j < GRID_SIZE[0]; j++){ // Columns
        
        onScreenTiles[j].reserve(GRID_SIZE[1]);
        for(int i = 0; i < GRID_SIZE[1]; i++){ // Rows
            Coordinates currentPos = Coordinates(CELL_SIZE / 2 + CELL_SIZE * j, CELL_SIZE / 2 + CELL_SIZE * i);
            Treat treat(currentPos);
            //Food food(currentPos,CELL_SIZE/6,10);
            if((i>=0 && i<3) || i==34 || i==35)
                onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos));
            else if(i==3 || i==33)
                onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
            else if(i==4){
                if(j==0 || j== 13 || j==14 || j==27)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==5){
                if(j==0 || (j>1 && j<6) || (j>6 && j<12) || j==13
                    || j==14 || (j>15 && j<21) || (j>21 && j<26) || j==27)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==6){
                if(j==0 || (j>1 && j<6) || (j>6 && j<12) || j==13
                    || j==14 || (j>15 && j<21) || (j>21 && j<26) || j==27)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else if(j==1 || j==26){
                    PowerPill powerPill(currentPos);
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, powerPill));
                }else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==7){
                if(j==0 || (j>1 && j<6) || (j>6 && j<12) || j==13
                    || j==14 || (j>15 && j<21) || (j>21 && j<26) || j==27)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==8){
                if(j==0 || j==27)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==9 || i==10){
                if(j==0 || (j>1 && j<6) || j==7 || j==8 || (j>9 && j<18)
                    || j==19 || j==20 || (j>21 && j<26) || j==27)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==11){
                if(j==0 || j==7 || j==8 || j==13 || j==14 || j==19 || j==20 || j==27)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==12){
                if((j>=0 && j<6) || (j>6 && j<12) || j==13 || j==14
                    || (j>15 && j<21) || (j>21 && j<=27))
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else if(j==12 || j==15)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==13){
                if(j==5 || (j>6 && j<12) || j==13 || j==14 || (j>15 && j<21) || j==22)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else if(j==6 || j==21)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos));
            }
            else if(i==14){
                if(j==5 || j==7 || j==8 || j==19 || j==20 || j==22)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else if(j==12 || j==15)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::ESCAPE, currentPos));
                else if(j==6 || j==21)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos));
            }
            else if(i==15){
                if(j==5 || j==7 || j==8 || (j>9 && j<13) || (j>14 && j<18) || j==19
                    || j==20 || j== 22)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else if(j==6 || j==21)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
                else if(j==13 || j==14)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos));
            }
            else if(i==16){
                if((j>=0 && j<6) || j==7 || j==8 || j==10 || j==17 || j==19
                    || j==20 || (j>21 && j<=27))
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else if(j==6 || j==21)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos));
            }
            else if(i==17){
                if(j==10 || j==17)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else if((j>=0 && j<6) || (j>21 && j<= 27))
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::SLOW,currentPos));
                else if(j==6 || j==21)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos));
            }
            else if(i==18){
                if((j>=0 && j<6) || j==7 || j==8 || j==10 || j==17 || j==19
                    || j==20 || (j>21 && j<=27))
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else if(j==6 || j==21)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos));
            }
            else if(i==19){
                if(j==5 || j==7 || j==8 || (j>9 && j<18) || j==19 || j==20 || j== 22)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else if(j==6 || j==21)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos));
            }
            else if(i==20){
                if(j==5 || j==7 || j==8 || j==19 || j==20 || j==22)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else if(j==6 || j==21)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos));
            }
            else if(i==21){
                if(j==5 || j==7 || j==8 || (j>9 && j<18) || j==19 || j==20 || j==22)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else if(j==6 || j==21)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos));
            }
            else if(i==22){
                if((j>=0 && j<6) || j==7 || j==8 || (j>9 && j<18) || j==19 || j==20
                    || (j>21 && j<= 27))
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else if(j==6 || j==21)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos));
            }
            else if(i==23){
                if(j==0 || j==13 || j==14 || j==27)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==24){
                if(j==0 || (j>1 && j<6) || (j>6 && j<12) || j==13 || j==14
                    || (j>15 && j<21) || (j>21 && j<26) || j==27)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==25){
                if(j==0 || (j>1 && j<6) || (j>6 && j<12) || j==13 || j==14
                    || (j>15 && j<21) || (j>21 && j<26) || j==27)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==26){
                PowerPill pill(currentPos);
                if(j==0 || j==4 || j==5 || j==22 || j==23 || j==27)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else if(j==12 || j==15)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::ESCAPE, currentPos, treat));
                else if(j==13 || j==14)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos));
                else if(j==1 || j==26)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, pill));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==27){
                if((j>=0 && j<3) || j==4 || j==5 || j==7 || j==8
                    || (j>9 && j<18) || j==19 || j==20 || j==22 || j==23 || (j>24 && j<=27))
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==28){
                if((j>=0 && j<3) || j==4 || j==5 || j==7 || j==8
                    || (j>9 && j<18) || j==19 || j==20 || j==22 || j==23 || (j>24 && j<=27))
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==29){
                if(j==0 || j==7 || j==8 || j==13 || j==14 || j==19 || j==20 || j==27)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==30){
                if(j==0 || (j>1 && j<12) || j==13 || j==14 || (j>15 && j<26) || j==27)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==31){
                if(j==0 || (j>1 && j<12) || j==13 || j==14 || (j>15 && j<26) || j==27)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
            else if(i==32){
                if(j==0 || j==27)
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::WALL, currentPos));
                else
                    onScreenTiles[j].push_back(Tile(TILE_TYPE::NORMAL, currentPos, treat));
            }
        }
    }
    
}

//
// Private static functions
//

void Grid::drawLines(RenderWindow& window){
    for(float i=0;i<36;i++){
        sf::Vertex line[] = {Vertex(Vector2f(0.f,i*CELL_SIZE)),
                            Vertex(Vector2f(28*CELL_SIZE,i*CELL_SIZE))};
        window.draw(line,2,sf::Lines);
    }
    for(float i=0;i<28;i++){
        sf::Vertex line[] = {Vertex(Vector2f(i*CELL_SIZE,0.f)),
                            Vertex(Vector2f(i*CELL_SIZE,36*CELL_SIZE))};
        window.draw(line,2,Lines);
    }
}


//Trace a long grey rectangle at the at the [i,y] position in the window "window"
void Grid::drawWall(float length, float large, float i, float y, RenderWindow& window){
    RectangleShape rectangle(Vector2f(length,large));
    rectangle.setFillColor(WALL_COLOR);
    rectangle.setPosition(y*CELL_SIZE,i*CELL_SIZE);
    window.draw(rectangle);
}

//Given a window, all the walls of the labyrinth in this window
void Grid::drawWalls(RenderWindow& window){
    for(float i=3;i<34;i++){
        for(float y=0;y<28;y++){
            if(i==3){
                if(y==0 || y ==14){
                    drawWall(CELL_SIZE ,CELL_SIZE / 2,i,y,window);
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                }else if(y== 13 || y==27){
                    drawWall(CELL_SIZE ,CELL_SIZE / 2,i,y,window);
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                }else{
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
                }
            }
            else if(i==4){
                if(y==0 || y==14)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==13 || y==27)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
            }
            else if(i==5){
                if(y==0 || y==14)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==13 || y==27)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==2 || y==7 || y==16 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y+0.5,window);
                else if(y==5 || y==11 || y==20 ||y==25)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y,window);
                else if(y==3 || y==4 || y==8 || y==9 || y==10
                        || y==17 || y== 18 || y==19 || y==23 || y==24)
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
            }
            else if(i==6){
                if(y==0 || y==14)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==13 || y==27)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==2 || y==7 || y==16 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==5 || y==11 || y==20 ||y==25)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==3 || y==4 || y==8 || y==9 || y==10
                        || y==17 || y== 18 || y==19 || y==23 || y==24)
                    drawWall(CELL_SIZE,CELL_SIZE,i,y,window);
            }
            else if(i==7){
                if(y==0)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==27)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==2 || y==7 || y==13 || y==16 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y+0.5,window);
                else if(y==5 || y==11 || y==14 || y==20 ||y==25)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y,window);
                else if(y==3 || y==4 || y==8 || y==9 || y==10
                        || y==17 || y== 18 || y==19 || y==23 || y==24)
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
            }
            else if(i==8){
                if(y==0)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==27)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
            }
            else if(i==9){
                if(y==0)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==27)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==2 || y==7 || y==10 || y==19 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y+0.5,window);
                else if(y==5 || y==8 || y==17 || y==20 || y==25)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y,window);
                else if (y==3 || y==4 || (y>10 && y<17) || y==23 || y==24)
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
            }
            else if(i==10){
                if(y==0 || y==8 || y==20)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==27 || y==7 || y==19)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==2 || y==10 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y+0.5,window);
                else if(y==5 || y==17 || y==25)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y,window);
                else if ((y>2 && y<5) || (y>10 && y<13) || (y>14 && y<17) || (y>22 && y<25))
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
                else if(y==13){
                    drawWall(CELL_SIZE ,CELL_SIZE / 2,i,y,window);
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                }else if(y==14){
                    drawWall(CELL_SIZE ,CELL_SIZE / 2,i,y,window);
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                }
            }
            else if(i==11){
                if(y==0 || y==8 || y==14 || y==20)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==27 || y==7 || y==13 || y==19)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
            }
            else if (i==12){
                if(y==0 || y== 8){
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
                }
                else if(y==19 || y==27){
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
                }
                else if(y==7 || y==13)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==14 ||y==20)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if((y>0 && y<5) || (y>22 && y<27) || (y>8 && y<11) ||(y>16 && y<19))
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
                else if(y==5 || y==11)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y,window);
                else if(y==16 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y+0.5,window);
            }else if(i==13){
                if(y==5 || y==20)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==7 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==8){
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                }else if(y==19){
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                }else if((y>8 && y<11) || (y>16 && y<19))
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
                else if(y==11)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y,window);
                else if(y==16)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y+0.5,window);
            }
            else if(i==14){
                if(y==5 || y==8 || y==20)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==7 || y==19 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
            }
            else if(i==15){
                if(y==5 || y==8 || y==20)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==7 || y==19 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==10)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y+0.5,window);
                else if(y==17)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y,window);
                else if(y==11 || y==12 || y== 15 || y==16)
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
                else if(y==13 || y==14){//door tiles
                    RectangleShape rectangle(Vector2f(CELL_SIZE,CELL_SIZE*0.25));
                    rectangle.setFillColor(sf::Color::White);
                    rectangle.setPosition(y*CELL_SIZE,(i+0.625)*CELL_SIZE);
                    window.draw(rectangle);
                }
            }
            else if(i==16){
                if((y>=0 && y<5) || (y>22 && y<=27))
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
                else if(y==5 || y==8 ||y==20)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y,window);
                else if(y==7 || y==19 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y+0.5,window);
                else if(y==10)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==17)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);

            }
            else if(i==17){
                if(y==10)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==17)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
            }
            else if(i==18){
                if((y>=0 && y<5) || (y>22 && y<=27))
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
                else if(y==5 || y==8 ||y==20)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y,window);
                else if(y==7 || y==19 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y+0.5,window);
                else if(y==10)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==17)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
            }
            else if(i==19){
                if(y==5 || y==8 || y==20)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==7 || y==19 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==10)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y+0.5,window);
                else if(y==17)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y,window);
                else if(y>10 && y<17)
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
            }
            else if(i==20){
                if(y==5 || y==8 || y==20)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==7 || y==19 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
            }
            else if(i==21){
                if(y==5 || y==8 || y==20)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==7 || y==19 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==10)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y+0.5,window);
                else if(y==17)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y,window);
                else if(y>10 && y<17)
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
            }
            else if(i==22){
                if(y==0 || y==14){
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                }
                else if(y==27 || y==13){
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                }
                else if(y==5 || y==8 || y==17 || y==20)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y,window);
                else if(y==7 || y==10 || y==19 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y+0.5,window);
                else if((y>0 && y<5) || (y>10 && y<13) || (y>14 && y<17) || (y>22 && y<27))
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
            }
            else if(i==23){
                if(y==0 || y==14)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==13 || y==27)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
            }
            else if(i==24){
                if(y==0 || y==14)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==13 || y==27)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==2 || y==7 || y==16 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y+0.5,window);
                else if(y==5 || y==11 || y==20 ||y==25)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y,window);
                else if(y==3 || y==4 || y==8 || y==9 || y==10
                        || y==17 || y== 18 || y==19 || y==23 || y==24)
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
            }
            else if(i==25){
                if(y==0 || y==5)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==22 || y==27)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==2 || y==7 || y==13 || y==16)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y+0.5,window);
                else if(y==11 || y==14 || y==20 || y==25)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y,window);
                else if(y==3 ||(y>7 && y<11) || (y>16 && y<20) || y==24)
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
                else if(y==4){
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                }else if(y==23){
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                }
            }
            else if(i==26){
                if(y==0 || y==5 || y==23)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==4 || y==22 || y==27)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
            }
            else if(i==27){
                if(y==0){
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
                }else if(y==27){
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
                }else if(y==2 || y==8 || y==17 || y==20)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y,window);
                else if(y==7 || y==10 || y==19 || y==25)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y+0.5,window);
                else if(y==1 || (y>10 && y<17) || y==26)
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
                else if(y==4 || y==22)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==5 ||y==23)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
            }
            else if(i==28){
                if(y==0 || y==14){
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                }else if(y==13 || y==27){
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                }else if(y==2 || y==5 || y==17 || y==23)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y,window);
                else if(y==4 || y==10 || y==22 || y==25)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y+0.5,window);
                else if(y==1 || (y>10 && y<13) || (y>14 && y<17) || y==26)
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
                else if(y==7 || y==19)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==8 || y==20)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
            }
            else if(i==29){
                if(y==0 || y==8 || y==14 || y==20)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==7 || y==13 || y==19 || y==27)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
            }
            else if(i==30){
                if(y==0 || y==14)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==13 || y==27)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==2 || y==16)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y+0.5,window);
                else if(y==11 || y==25)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i+0.5,y,window);
                else if(y==7 || y==19){
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                }else if(y==8 || y==20){
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
                }else if((y>2 && y<7) || (y>8 && y<11) || (y>16 && y<19) || (y>20 && y<25))
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
            }
            else if(i==31){
                if(y==0)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==27)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                else if(y==2 || y==13 || y==16)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y+0.5,window);
                else if(y==11 || y==14 || y==25)
                    drawWall(CELL_SIZE/2,CELL_SIZE/2,i,y,window);
                else if((y>2 && y<12) || (y>16 && y< 25))
                    drawWall(CELL_SIZE,CELL_SIZE/2,i,y,window);
            }
            else if(i==32){
                if(y==0)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                else if(y==27)
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
            }else if(i==33){
                if(y==0){
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y,window);
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
                }else if(y==27){
                    drawWall(CELL_SIZE/2,CELL_SIZE,i,y+0.5,window);
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
                }else
                    drawWall(CELL_SIZE,CELL_SIZE/2,i+0.5,y,window);
            }
        }
    }
}
