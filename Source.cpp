// Raj Asarpota FINAL PROJECT 8/8/19 CS 2410-40462

#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>
#include<time.h>
using namespace std;

const int WorldSize = 20;
const int intAnts = 100;
const int intBugs = 5;
const int doodleBug = 1;
const int ant = 2;
const int antBreed = 3;
const int bugBreed = 8;
const int bugStarve = 3;

class Organism;  //forward declarations of organism classes
class Doodlebug;
class Ant;

class World		
{
	friend class Organism;
	friend class Doodlebug;
	friend class Ant;

public:
	World(); //constructor
	~World(); //destructor
	Organism* getAt(int x, int y);
	void setAt(int x, int y, Organism *org);
	void Display();
	void stepSimulation();

private:
	Organism * grid[WorldSize][WorldSize];
};

class Organism   // def for Organism base class
{
	friend class World;    //Allows world to affect organism

public:
	Organism();  //constructors
	Organism(World *world, int x, int y);
	~Organism(); //destructor
	 
	virtual void breed() = 0;	//methods 
	virtual void move() = 0;
	virtual int getType() = 0;
	virtual bool starve() = 0;
protected:
	int x, y;
	bool moved;
	int breedTicks;
	World *world;
};

World::World() //constructor def
{
	for (int i = 0; i < WorldSize; i++)		//initializes world to empty spaces
	{
		for (int j = 0; j < WorldSize; j++)
		{
			grid[i][j] = NULL;
		}
	}
	
}

World::~World() //destructor def
{
	for (int i = 0; i < WorldSize; i++)		//releases any allocated memory
	{
		for (int j = 0; j < WorldSize; j++)
		{
			if (grid[i][j] != NULL)
				delete grid[i][j];
		}
	}
}

//Member function definitions

Organism* World::getAt(int x, int y)
{
	if ((x >= 0) && (x < WorldSize) && (y >= 0) && (y < WorldSize))
		return grid[x][y];
	return NULL;
}

void World::setAt(int x, int y, Organism *org)
{
	if ((x >= 0) && (x < WorldSize) && (y >= 0) && (y < WorldSize))
		grid[x][y] = org;
}

void World::Display()
{
	cout << endl << endl;
	for (int j = 0; j < WorldSize; j++)
	{
		for (int i = 0; i < WorldSize; i++)
		{
			if (grid[i][j] == NULL)
				cout << ".";
			else if (grid[i][j]->getType() == ant)
				cout << "O";
			else
				cout << "X";
		}
		cout << endl;
	}
}

void World::stepSimulation()
{
	int i, j;
	
	for (i = 0; i < WorldSize; i++)	//resets organisms to not moved
		for (j = 0; j < WorldSize; j++)
		{
			if (grid[i][j] != NULL)
				grid[i][j]->moved = false;
		}
	
	
			
	for(i = 0; i < WorldSize; i++)		//loops through the cells in order and moves if it's a doodlebug
		for (j = 0; j < WorldSize; j++)
		{
			if ((grid[i][j] != NULL) && (grid[i][j]->getType() == doodleBug))
			{
				if (grid[i][j]->moved == false)
				{
					grid[i][j]->moved = true;
					grid[i][j]->move();
				}
			}
		}

				
	for (i = 0; i < WorldSize; i++)		//loops through the cells in order and moves if it's a ant
		for (j = 0; j < WorldSize; j++)
		{
			if ((grid[i][j] != NULL) && (grid[i][j]->getType() == ant))
			{
				if (grid[i][j]->moved == false)
				{
					grid[i][j]->moved = true;
					grid[i][j]->move();
				}
			}
		}

	for(i = 0; i < WorldSize; i++)		//loops through the cells in order and checks if doodlebugs should breed
		for (j = 0; j < WorldSize; j++)
		{
			if ((grid[i][j] != NULL) && (grid[i][j]->getType() == doodleBug))
			{
			if (grid[i][j]->starve())
			{
				delete(grid[i][j]);
				grid[i][j] = NULL;
			}
			}
		}

		for (i = 0; i < WorldSize; i++)	//loops through the cells in order and checks if any should breed
		{
			for (j = 0; j < WorldSize; j++)
			{
				if ((grid[i][j] != NULL) && (grid[i][j]->moved == true))
				{
					grid[i][j]->breed();
				}
			}
		}
}

Organism::Organism()	//default constructor for organism
{
	world = NULL;
	moved = false;
	breedTicks = 0;
	x = 0;
	y = 0;
}

Organism::Organism(World *wld, int x, int y)  //the constructor with parameters
{
	this->world = wld;
	moved = false;
	breedTicks = 0;
	this->x = x;
	this->y = y;
	wld->setAt(x, y, this);
}

Organism::~Organism()	//destructor
{

}

class Ant : public Organism
{
	friend class World;

public:
	Ant();	//constructors
	Ant(World *world, int x, int y);

	void breed();	//methods
	void move();
	int getType();
	bool starve()
	{
		return false;
	}
};

Ant::Ant() : Organism()		//default constructor
{

}

Ant::Ant(World *world, int x, int y) : Organism(world, x, y)	//the constructor with parameters
{

}

void Ant::move()
{
	int direction = rand() % 4;		//picks a random direction to move

	if (direction == 0)  //Trying to move up
	{
		if ((y > 0) && (world->getAt(x, y - 1) == NULL))
		{
			world->setAt(x, y - 1, world->getAt(x, y));
			world->setAt(x, y, NULL);
			y--;
		}
	}

	else if (direction == 1) // Trying to move down
	{
		if ((y < WorldSize - 1) && (world->getAt(x, y + 1) == NULL))
		{
			world->setAt(x, y + 1, world->getAt(x, y));

			world->setAt(x, y, NULL);
			y++;
		}
	}

	else if (direction == 2) //Trying t move left
	{
		if ((x > 0) && (world->getAt(x - 1, y)) == NULL)
		{
			world->setAt(x - 1, y, world->getAt(x, y));
			world->setAt(x, y, NULL);
			x--;
		}
	}

	else
	{
		if ((x < WorldSize - 1) && (world->getAt(x + 1, y) == NULL))
		{
			world->setAt(x + 1, y, world->getAt(x, y));

			world->setAt(x, y, NULL);
			x++;
		}
	}
}

int Ant::getType()
{
	return ant;
}

void Ant::breed()
{
	breedTicks++;
	if (breedTicks == antBreed)
	{
		breedTicks = 0;		//tries to make a new ant either above, left, right, or down

		if ((y > 0) && (world->getAt(x, y - 1) == NULL))
		{
			Ant *newAnt = new Ant(world, x, y - 1);
		}

		else if((y < WorldSize - 1) && (world->getAt(x, y + 1) == NULL))
		{
			Ant *newAnt = new Ant(world, x, y + 1);
		}

		if((x > 0) && (world->getAt(x - 1, y) == NULL))
		{
			Ant *newAnt = new Ant(world, x - 1, y);
		}

		else if ((x < WorldSize - 1) && (world->getAt(x + 1, y) == NULL))
		{
			Ant *newAnt = new Ant(world, x + 1, y);
		}

	}
}


class Doodlebug : public Organism
{
	friend class World;
public:
	Doodlebug();
	Doodlebug(World *world, int x, int y);
	
	void breed();
	void move();
	int getType();
	bool starve();

private:
	int starveTicks;
};

Doodlebug::Doodlebug() : Organism()		//default constructor
{
	starveTicks = 0;
}

Doodlebug::Doodlebug(World *world, int x, int y) : Organism(world, x, y)	//constructor with parameters
{
	starveTicks = 0;
}

//Member functions

void Doodlebug::move()
{
	if ((y > 0) && (world->getAt(x, y - 1) != NULL) && (world->getAt(x, y - 1)->getType() == ant))
	{
		delete(world->grid[x][y - 1]);
		world->grid[x][y - 1] = this;
		world->setAt(x, y, NULL);
		starveTicks = 0;
		y--;
		return;
	}

	else if ((y < WorldSize - 1) && (world->getAt(x, y + 1) != NULL) && (world->getAt(x, y + 1)->getType() == ant))
	{
		delete(world->grid[x][y + 1]);
		world->grid[x][y + 1] = this;
		world->setAt(x, y, NULL);
		starveTicks = 0;
		y++;
		return;
	}

	else if ((x > 0) && (world->getAt(x - 1, y) != NULL) && (world->getAt(x - 1, y)->getType() == ant))
	{
		delete(world->grid[x-1][y]);
		world->grid[x-1][y] = this;
		world->setAt(x, y, NULL);
		starveTicks = 0;
		x--;
		return;
	}

	else if ((x < WorldSize - 1) && (world->getAt(x + 1, y) != NULL) && (world->getAt(x + 1, y)->getType() == ant))
	{
		delete(world->grid[x + 1][y]);
		world->grid[x + 1][y] = this;
		world->setAt(x, y, NULL);
		starveTicks = 0;
		x++;
		return;
	}


	int direction = rand() % 4;		//selects a random direction

	if (direction == 0)  //Trying to move up
	{
		if ((y > 0) && (world->getAt(x, y - 1) == NULL))
		{
			world->setAt(x, y - 1, world->getAt(x, y));
			world->setAt(x, y, NULL);
			y--;
		}
	}

	else if (direction == 1) // Trying to move down
	{
		if ((y < WorldSize - 1) && (world->getAt(x, y + 1) == NULL))
		{
			world->setAt(x, y + 1, world->getAt(x, y));

			world->setAt(x, y, NULL);
			y++;
		}
	}

	else if (direction == 2) //Trying t move left
	{
		if ((x > 0) && (world->getAt(x - 1, y)) == NULL)
		{
			world->setAt(x - 1, y, world->getAt(x, y));
			world->setAt(x, y, NULL);
			x--;
		}
	}

	else
	{
		if ((x < WorldSize - 1) && (world->getAt(x + 1, y) == NULL)) //trying to move right
		{
			world->setAt(x + 1, y, world->getAt(x, y));

			world->setAt(x, y, NULL);
			x++;
		}
	}
	
	starveTicks++;		//increment starveTick since the doodlebug didnt eat this turn
}

int Doodlebug::getType()
{
	return doodleBug;
}


void Doodlebug::breed()
{
	breedTicks++;
	if (breedTicks == bugBreed)
	{
		breedTicks = 0; //tries to make a new doodlebug either above, left, right, or down

		if ((y > 0) && (world->getAt(x, y - 1) == NULL))
		{
			Doodlebug *newBug = new Doodlebug(world, x, y - 1);
		}

		else if ((y < WorldSize - 1) && (world->getAt(x, y + 1) == NULL))
		{
			Doodlebug *newBug = new Doodlebug(world, x, y + 1);
		}

		if ((x > 0) && (world->getAt(x - 1, y) == NULL))
		{
			Doodlebug *newBug = new Doodlebug(world, x - 1, y);
		}

		else if ((x < WorldSize - 1) && (world->getAt(x + 1, y) == NULL))
		{
			Doodlebug *newBug = new Doodlebug(world, x + 1, y);
		}

	}
}


bool Doodlebug::starve()
{
	if (starveTicks > bugStarve)		//starves if no food is eaten in the last doodlestarve time ticks
		return true;
	else
		return false;
}

int main()
{
	string s;
	srand(time(NULL));		//random number generator
	World w;

	int antCount = 0;	//randomly creates and places 100 ants

	while (antCount < intAnts)
	{
		int x = rand() % WorldSize;
		int y = rand() % WorldSize;

		if (w.getAt(x, y) == NULL)
		{
			antCount++;
			Ant *a = new Ant(&w, x, y);
		}
	}

	int doodleCount = 0;	//randomly creates and places 5 doodlebugs

	while (doodleCount < intBugs)
	{
		int x = rand() % WorldSize;
		int y = rand() % WorldSize;

		if (w.getAt(x, y) == NULL)
		{
			doodleCount++;
			Doodlebug *d = new Doodlebug(&w, x, y);
		}
	}

	while (true)		//endlessly runs simulation until the user decides to cancel
	{
		w.Display();
		w.stepSimulation();
		cout << endl << "Press enter for the next step" << endl;
		getline(cin, s);
	}

	return 0;
}