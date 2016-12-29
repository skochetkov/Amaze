#include <vector>
#include "GL/glut.h"

using std::vector;
using std::pair;

extern const int cellWidth;
extern const int cellHeight;
extern const int boardCellsHorzontal;
extern const int boardCellsVertical;

enum class VisitorState
{
	NONE, PLAYER
};

enum class PathState
{
	NONE, PATH
};

enum class ObstclState
{
    NONE, WATER
};

/*
* Initial point from which the cell is built (bottom/left corner) 
*/
class CellPoint
{
public:
	CellPoint():x(0), y(0){};
	CellPoint(int xx, int yy): x(xx), y(yy){};
	int getX(){ return x;};
	int getY(){ return y;};
private:
	int x, y;
};

class CellLine
{
public:
    CellLine():_p1(), _p2(){}
    CellLine(CellPoint p1, CellPoint p2): _p1(p1), _p2(p2){};
private:
    CellPoint _p1, _p2;
};
/*
 * Index class that indicates what position is in the board
 */
class Index
{
public:
	Index(): col(0), row(0){};
	Index(int c, int r): col(c), row(r){};
	int getCol() const {return col;};
	int getRow() const {return row;};
	void setCol(int c) {col = c;};
	void setRow(int r) {row = r;};

private:
	int col, row;
};

class Cell
{
public:
	Cell();
    Cell(const Cell& c);
	Cell(const Index i);
	Cell(CellPoint p);
    Cell(CellLine n, CellLine s, CellLine w, CellLine e);
	CellPoint getCellPoint() const;
	int getCellWidth() const;
	int getCellHeight() const;
	void setIndex(int col, int row);
	void setIndex(Index i);
	Index getIndex() const;
    vector<CellPoint> getPoints() const;
	bool isIt(int x, int y);
	void setVisitorState(VisitorState s);
	VisitorState getVisitorState() const;
    void setPathState(PathState s);
	PathState getPathState() const;
    const Cell* getCell() const;
    Cell Cell::operator=(const Cell& cell);
    void setVisited(bool v);
    bool getVisited() const;
    bool getNorth();
    void setNorth(bool n);
    bool getSouth();
    void setSouth(bool s);
    bool getWest();
    void setWest(bool w);
    bool getEast();
    void setEast(bool e);

private:
	CellPoint _p;
	int _w, _h;
	Index index;
	VisitorState visitor_state;
    PathState path_state;
    vector<CellPoint> points;
    bool visited;
    bool isNorth, isSouth, isWest, isEast;
};


class MazeLevel
{
public:
	vector<Cell> getVertices();
	void setVertex(Cell cell);
	vector<pair<Cell, Cell>> getEdges();
	void setEdge(pair<Cell, Cell> edge);
	bool isInLevel(Cell cell);
private:
	vector<Cell> vertices;
	vector<pair<Cell, Cell>> edges;
};

/*
 * Logical and data representation of the maze
 */
class MazeModel
{
public:
	void init();
	vector<Cell> getMazeBlocks();
	Cell* find(int x, int y);
	//checks if a move affected something
	void checkForConnections(Cell* cell);
	MazeLevel levelGenerator();
	MazeLevel getCurrentLevel();
	void setCurrentLevel(MazeLevel level);

private:
	vector<Cell> mazeBlocks;
	MazeLevel currentLevel;

};

/*
 * Visual representation of the maze
 */
class MazeView
{
public:

private:
};

/*
 * Maze controller that manages maze creation, user interaction (input and output)
 */
class MazeController
{
public:
	MazeController();
	void init();
	MazeView getView();
	MazeModel getModel();
	void onMouseButton(int button, int state, int x, int y);

private:
	MazeView view;
	MazeModel model;
};


