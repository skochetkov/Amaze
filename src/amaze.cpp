#include <iostream>
#include "math.h"
#include "Amaze.h"
#include <stdlib.h>
#include <time.h>


using namespace std;

const int window_width = 390;
const int window_height = 390;
const int cellWidth = 40;
const int cellHeight = 40;
const int m_nColumns = 8;
const int m_nRows = 8;
const int m_offset = 1;
const int m_left_offset = 30;
const int m_top_offset = 30;


void keyboard(unsigned char key, int x, int y);
void display(void);

MazeController controller;


//Cell constructor definition
Cell::Cell(Index i)
{
	index = i;
	_w = cellWidth;
	_h = cellHeight;

	//rest is initialized based on index
	GLfloat x = m_left_offset + (i.getCol() * (_w + m_offset));
	GLfloat y = m_top_offset + i.getRow() * (_h + m_offset);

	points.push_back(CellPoint(x, y));
	points.push_back(CellPoint(x + _w, y));
	points.push_back(CellPoint(x + _w , y + _h));
	points.push_back(CellPoint(x, y + _h ));
	visitor_state = VisitorState::NONE;
	_p = CellPoint(x+_w/2,y+_h/2);
    visited = false;
};

Cell::Cell(CellPoint p) 
{
	_p = p;
	//rest is initialized with default constants
	_w  = cellWidth;
	_h = cellHeight;
	index.setCol(-1);
	index.setRow(-1);
	visitor_state = VisitorState::NONE;
    visited = false;
};

Cell::Cell() 
{
	_w  = cellWidth;
	_h = cellHeight;
	index.setCol(-1);
	index.setRow(-1);
	visitor_state = VisitorState::NONE;
    visited = false;
};

Cell::Cell(const Cell& cell) 
{
	_w  = cell.getCellWidth();
	_h = cell.getCellHeight();
    index = cell.getIndex();
	visitor_state = cell.getVisitorState();
    points = cell.getPoints();
    _p = cell.getCellPoint();
    visited = cell.getVisited();
};

Cell Cell::operator=(const Cell& cell) 
{
	_w  = cell.getCellWidth();
	_h = cell.getCellHeight();
    index = cell.getIndex();
	visitor_state = cell.getVisitorState();
    points = cell.getPoints();
    _p = cell.getCellPoint();
    visited = cell.getVisited();

    return *this;
};

const Cell* Cell::getCell() const { return this; };

CellPoint Cell::getCellPoint() const {return _p;};

int Cell::getCellWidth() const {return _w;};

int Cell::getCellHeight() const {return _h;};

void Cell::setIndex(int col, int row)
{
	index.setCol(col);
	index.setRow(row);
};

void Cell::setIndex(Index i)
{
	index = i;
};

Index Cell::getIndex() const
{
	return index;
};

vector<CellPoint> Cell::getPoints() const
{
	return points;
};

bool Cell::isIt(int x, int y)
{
	if(x >= points[0].getX() && x <= points[1].getX()
			&& y >= points[0].getY() && y <= points[2].getY())
	{
		return true;
	}
	else
	{
		return false;
	}
};

void Cell::setVisitorState(VisitorState s)
{
	visitor_state = s;
};

VisitorState Cell::getVisitorState() const
{
	return visitor_state;
};

void Cell::setPathState(PathState s)
{
    path_state = s;
};

PathState Cell::getPathState() const
{
    return path_state;
};

void Cell::setVisited(bool v)
{
    visited = v;
};

bool Cell::getVisited() const
{
    return visited;
};

bool  Cell::getNorth()
{
    return isNorth;
};

void  Cell::setNorth(bool n)
{
    isNorth = n;
};
bool  Cell::getSouth()
{
    return isSouth;
};

void  Cell::setSouth(bool s)
{
    isSouth = s;
};

bool  Cell::getWest()
{
    return isWest;
};

void  Cell::setWest(bool w)
{
    isWest = w;
};

bool  Cell::getEast()
{
    return isEast;
};

void  Cell::setEast(bool e)
{
    isEast = e;
};

bool operator==(const Index& lhs, const Index& rhs)
{ 
    return (lhs.getCol() == rhs.getCol() && lhs.getRow() == rhs.getRow()); 
};

bool operator!=(const Index& lhs, const Index& rhs)
{ 
    return !(lhs.getCol() == rhs.getCol() && lhs.getRow() == rhs.getRow()); 
};

bool operator==(const Cell& lc, const Cell& rc)
{
    return (lc.getIndex() == rc.getIndex());
};

//Controller
MazeController::MazeController()
{
	init();
};

void MazeController::init()
{
	//cout << "controller init" << endl;
	model.init();
};

MazeView MazeController::getView()
{
	return view;
};

MazeModel MazeController::getModel()
{

	//cout << "getModel" << endl;
	return model;
};

void MazeController::onMouseButton(int button, int state, int x, int y)
{
    /*if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
           Cell *cell = model.find(x, y);
           //auto *list = controller.getModel().getBoardMembers();

           if(cell)
           {
        	   (*cell).setState(CellState::PLAYER);
        	   //cout << "set new state:" << (*cell).getState() << endl;
               model.checkForConnections(cell);
           }

           
    }*/

    glutPostRedisplay();
};

//MODEL

void MazeModel::init()
{
	//std::cout << "model init" << endl;

	//initialize all board cells as vector with indices
	//CellPoint point = CellPoint(0,0);
	Index index;
	//Cell cell = Cell(NULL);

	//  Randomly set each square to a color
	for(int row = 0; row < m_nRows; row++)
		for(int col = 0; col < m_nColumns; col++)
	    {
	    	index = Index(col, row);
	    	Cell cell = Cell(index);
	    	mazeBlocks.push_back(cell);
	    }

	setCurrentLevel(levelGenerator());

};


vector<Cell> MazeModel::getMazeBlocks()
{
	return mazeBlocks;
};

Cell* MazeModel::find(int x, int y)
{
	Cell *c = 0;


	for(vector<Cell>::iterator it = mazeBlocks.begin(); it != mazeBlocks.end(); ++it)
	{
		//cout << (*it).getIndex().getCol() << "#" << (*it).getIndex().getRow()  << endl;
		//c = &(*it);
		//cout << c.getIndex().getCol() << "::" << c.getIndex().getRow() << endl;

		if(it->isIt(x, y))
		{
			//cout << c.getIndex().getCol() << "::" << c.getIndex().getRow() << endl;
			c = &(*it);
			//cout << it->getState() << endl;
			//it->setState(CellState::PLAYER);
			//cout << it->getState() << endl;
			return c;
		}
	}

	return c;
};

void MazeModel::checkForConnections(Cell* cell)
{
	Index index = cell->getIndex();

	//TODO needs to be optimized with vector of active pieces only - create a new vector to keep members that have pieces on board
	//explore option of having extra vector empty cells to speed up algorithm on move and collection of figures/lines
	for(vector<Cell>::iterator it = mazeBlocks.begin(); it != mazeBlocks.end(); ++it)
	{
		//if(it->getState() )
		if((index.getCol() - it->getIndex().getCol()) <= 1 && (index.getRow() - it->getIndex().getRow()) <= 1)
		{
			cout << index.getCol() << index.getRow() << it->getIndex().getCol() << it->getIndex().getRow() << endl;
		}
	}
};

MazeLevel MazeModel::levelGenerator()
{
	MazeLevel level;
	srand(time(NULL));
	int r = rand();
	int startCol = r % (m_nColumns-1+1) - 0;

	cout << r << "::" <<  startCol << endl;

	Index start = Index(startCol, m_nRows -1);
	Cell entrance = Cell(start);
	level.setVertex(entrance);

	return level; 
};

MazeLevel MazeModel::getCurrentLevel()
{
	return currentLevel;
};

void MazeModel::setCurrentLevel(MazeLevel level)
{
	currentLevel = level;
};

vector<Cell> MazeLevel::getVertices()
{
	return vertices;
};

void MazeLevel::setVertex(Cell cell)
{
	vertices.push_back(cell);
};

vector<pair<Cell, Cell>> MazeLevel::getEdges()
{
	return edges;
};

void MazeLevel::setEdge(pair<Cell, Cell> edge)
{
	edges.push_back(edge);
};

bool MazeLevel::isInLevel(Cell cell)
{
	for(vector<Cell>::iterator it = vertices.begin(); it != vertices.end(); ++it)
	{
		//if(it->getState() )
		if((cell.getIndex().getCol() - it->getIndex().getCol()) <= 1 && (cell.getIndex().getRow() - it->getIndex().getRow()) <= 1)
		{
			return true;
		}
	}
	return false;
};


//Generic functions
void onMouseButton(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
    	//temporary turned off
		//controller.onMouseButton(button, state, x, y);
    }

    glutPostRedisplay();
};



int main(int argc, char** argv)
{

	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(window_width,window_height);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Amaze Test"); // Create a window with the given title

	//do i need to get this here and will use it everywhere?
	//members = controller.getModel().getBoardMembers();
	//controller = BoardController();
	//glutKeyboardFunc(&keyboard);

	glutDisplayFunc(&display); // Register display callback handler for window re-paint
	glutMouseFunc(&onMouseButton);
	glutMainLoop();           // Enter the infinitely event-processing loop

	return EXIT_SUCCESS;
};



void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case '\x1B':
			exit(EXIT_SUCCESS);
		break;
	}
};

void drawPiece(float cx, float cy, float r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++)   {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component
        glVertex2f(x + cx, y + cy);//output vertex
    }

    glEnd();

    if(r < 5)
    	return;

    drawPiece(cx, cy, r - 2, num_segments--);
}

void display()
{

	//cout << controller.getModel().getBoardMembers().size() << endl;

	glClearColor( 0, 0, 0, 1 );
	glClear( GL_COLOR_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	double w = glutGet( GLUT_WINDOW_WIDTH );
	double h = glutGet( GLUT_WINDOW_HEIGHT );
	glOrtho(0, w, h, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	vector<Cell> blocksList = controller.getModel().getMazeBlocks();
	MazeLevel currentLevel = controller.getModel().getCurrentLevel();

	for(vector<Cell>::iterator it = blocksList.begin(); it != blocksList.end(); ++it)
	{
	    //Cell c = *it;

	    //cout << "state:" << (*it).getState() << endl;
		glColor3ub( 255, 255, 255 );
	    /*if ((*it).getState() == NUTRAL)
	    {
	    	glColor3ub( 255, 255, 255 );
	    }
	    else if ((*it).getState() == PLAYER)
	    {
	    	glColor3ub( 1, 1, 1 );
	    }
	    else
	    {
	    	glColor3ub( 50, 50 , 50 );
	    }*/

	    //cout << "inside of the vector" << endl;

	    vector<CellPoint> points = (*it).getPoints();

        /*if((*it).getNorth())
        {
            glBegin(GL_QUADS);
	    	//glColor3f(1.0f, 0.0f, 0.0f); // Red

	    	//for(vector<CellPoint>::iterator _it = points.begin(); _it != points.end(); ++_it)
	    	{
	    		//cout << "inside of the second vector:" << (*_it).getX() << ":" << (*_it).getY() << endl;
	    		//glVertex2f((GLfloat)(*_it).getX(), (GLfloat)(*_it).getY());

	    	}
		    glEnd();
        }*/
		//Temporary display current level maze
	    	if(currentLevel.isInLevel((Cell)*it))
                glColor3f(1.0f, 0.0f, 0.0f); // Red
			else
				glColor3f(0.0f, 0.0f, 0.0f); // Red
        glBegin(GL_QUADS);
			


	    	for(vector<CellPoint>::iterator _it = points.begin(); _it != points.end(); ++_it)
	    	{
	    		//cout << "inside of the second vector:" << (*_it).getX() << ":" << (*_it).getY() << endl;
	    		glVertex2f((GLfloat)(*_it).getX(), (GLfloat)(*_it).getY());

	    	}
		glEnd();
		//glClearColor( 0, 0, 0, 1 );
		//glColor3ub( 4, 6, 24 );
		//glColor3f(1,0,0);
		/*if ((*it).getState() == State::NUTRAL)
		{
			//glColor3f( 255, 255, 255 );
		}
		else if ((*it).getState() == State::PLAYER)
		{
			glColor3ub( 1, 1, 1 );
			drawPiece((float)it->getCellPoint().getX(), (float)it->getCellPoint().getY(), 10, 30);
		}
		else
		{
			glColor3ub( 50, 50 , 50 );
			drawPiece((float)it->getCellPoint().getX(), (float)it->getCellPoint().getY(), 10, 30);
		}*/
		//drawPiece((float)it->getCellPoint().getX(), (float)it->getCellPoint().getY(), 10, 30);
		//cout << "inside:" << c.getIndex().getCol() << endl;
	}

	//cout << "outside" << endl;


	glutSwapBuffers();

};


