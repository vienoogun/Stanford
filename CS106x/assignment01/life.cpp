/*
 * File: life.cpp
 * --------------
 *
 * This program creates a simulation of the multiplication of a 2-D bacteria colony based upon a 
 * predetermined set of rules.
 */

#include "genlib.h"
#include "extgraph.h"
#include "lifeGraphics.h"
#include "simpio.h"
#include "random.h"
#include "strutils.h"
#include "grid.h"
#include <string>
#include <iostream>
#include <fstream>

/**
 * Function: DetermineStartingColony()
 * ------------------------------------
 * Provides the user with the option to generate a random colony or to implement a sample
 * colony from a prepared file.
 *
 *@return start, a char representing the user's "yes" or "no" answer.
 */
char DetermineStartingColony()
{
	cout << "You can start your colony with random cells or read from a prepared file." << endl;
	cout << "Do you have a starting file in mind? ";
	string start = ConvertToUpperCase(GetLine());
	while (start != "YES" && start != "NO") {
		cout << "Please answer 'Yes' or 'No'." << endl;
		cout << "Do you have a starting file in mind? ";
		start = ConvertToUpperCase(GetLine());
	}
	return start.at(0);
}

/**
 * Function: DetermineMode()
 * --------------------------
 * Provides the user with the option to select between 3 different boundary conditions.
 *
 *@return mode, a char representing the user's selection of 1 of the 3 boundary conditions.
 */
char DetermineMode()
{
	cout << "You choose how cells on the edges will count their neighbors." << endl;
	cout << "\tP = Plateau Mode (cells on edges just have fewer neighbors)" << endl;
    cout << "\tD = Donut Mode (cells on edge wrap around to find neighbors)" << endl;
    cout << "\tM = Mirror Mode (cells on edge reflect back to find neighbors)" << endl;
	cout << "Your choice: ";
	string mode = ConvertToUpperCase(GetLine());
	while (mode.at(0) != 'P' && mode.at(0) != 'D' && mode.at(0) != 'M') {
		cout << "Acceptable answers are P, D, or M!" << endl;
		cout << "Your choice: ";
		mode = ConvertToUpperCase(GetLine());
	}
	return mode.at(0);
}

/**
 * Function: DetermineSpeed()
 * ----------------------------
 * Provides the user with the option to select between 3 different speeds of the simulation
 * as well as a manual mode that forces him to hit enter between generations.
 *
 *@return speed, an int representing the corresponding pausetime of the selected speed.
 *				the manual mode returns a dummy 5, which is chosen to be greater than 2 and
 *				used to distinguish between the manual mode and the other speeds.
 */
int DetermineSpeed()
{
	cout << "You choose how fast to run the simulation." << endl;
    cout << "\t1 = As fast as this chip can go!" << endl;
	cout << "\t2 = Not too fast, this is a school zone." << endl;
    cout << "\t3 = Nice and slow so I can watch everything that happens." << endl;
    cout << "\t4 = Wait for user to hit enter between generations." << endl;
	cout << "Your choice: ";
	int speed = GetInteger();
	while (speed < 1 || speed > 4) {
        cout << "Please enter an integer between 1 and 4!" << endl;
        cout << "Your choice: ";
        speed = GetInteger();
	}
	switch(speed) {
		case 1: return 0.75;
		case 2: return 1;
		case 3: return 1.25;
		default: return 5;
	} 
}

/**
 * Function: RandomColony()
 * -------------------------
 * Based upon user input, generates a random colony in a predetermined canvas size. 
 *
 *
 *@return randomGrid, a grid with a randomly generated colony. There is a 50/50 chance
 *					as to whether a location will have a live or dead cell.
 */
Grid<int> RandomColony()
{
	cout << "Okay, I will seed your colony randomly." << endl;
	int rowSize = 20;
	int columnSize = 40;
	Grid<int> randomGrid(rowSize, columnSize);
	InitLifeGraphics(rowSize, columnSize);
	for (int i = 0; i < rowSize; i ++) {
		for (int j = 0; j < columnSize; j ++) {
			if (RandomChance(0.50)) {
				randomGrid(i, j) = 0;
			} else {
				randomGrid(i, j) = 1;
				DrawCellAt(i, j, 1);
			}
		}
	}
	return randomGrid;
}

/**
 * Function: AccessStartingColony()
 * ---------------------------------
 * Based upon user input, reads in a file and generates a colony. Assumes that the specified
 * file has proper formatting: 1) all comments are set apart with '#', 2) subsequent 2 integers
 * denote the row and column size of the grid, and 3) '-' denotes empty locations and 'X' denotes
 * locations with live cells.
 *
 *@return fileGrid, a grid with a pre-arranged colony generated from the specifications of
 *					the user-specified file.
 */
Grid<int> AccessStartingColony()
{	
	ifstream startingColony;
	cout << "Please enter the filename: ";
	string start = GetLine();
	startingColony.open(start.c_str());
	while (startingColony.fail()) {
		startingColony.clear();
		cout << "Unable to open file named " << start << ". Please try again." << endl;
		cout << "Please enter the filename: ";
		start = GetLine();
		startingColony.open(start.c_str());
	}
	string str1;
	getline(startingColony, str1);
	while(str1.at(0) == '#') {
		getline(startingColony, str1);
	}
	
	int rowSize = StringToInteger(str1);
	getline(startingColony, str1);
	int columnSize = StringToInteger(str1);
	Grid<int> fileGrid(rowSize, columnSize);
	InitLifeGraphics(rowSize, columnSize);
	
	for (int i = 0; i < rowSize; i ++) {
		getline(startingColony, str1);
		for (int j = 0; j < columnSize; j ++) {
			if (str1.at(j) == '-') {
				fileGrid(i, j) = 0;
			} else {
				fileGrid(i, j) = 1;
				DrawCellAt(i, j, 1);
			}
		}
	}
	startingColony.close();
	return fileGrid;
}

/**
 * Function: MirrorAdjust()
 * -------------------------
 * Adjusts boundary cases in the Mirror boundary condition. References off
 * the grid are bounced back onto the grid, reflecting the cell configuration
 * of the adjacent location.
 *
 *@return an integer that increases or decreases the reference point by 1 so that
 *			it becomes a reflection of the location that was adjacent to it.
 */
int MirrorAdjust(int access, int max)
{
	if (access < 0) {
		return 1;
	} else if (access == max) {
		return -1;
	}
	return 0;
}

/**
 * Function: DonutAdjust()
 * ------------------------
 * Adjusts boundary cases in the Donut boundary condition.  Any reference that
 * disappears off one side is mapped around to the opposite side of the grid.
 *
 *@return an integer that represents the difference between the reference point
 *		and where it is supposed to be mapped around to.
 */
int DonutAdjust(int access, int max)
{	
	if (access < 0) {
		return max;
	} else if (access == max) {
		return -max;
	}
	return 0;
}

/**
 * Function: CountNeighbors()
 * ---------------------------
 * Counts the number of neighbors surrounding a specified location. Goes through
 * the 8 possible neighbor locations on the grid to determine how many neighbors
 * the cell has. Also takes into consideration the user-specified boundary condition
 * and handles boundary cases accordingly.
 *
 *@param row, col two integers specifying the exact location on the grid the function
 *				is looking at.
 *@param mainGrid the grid from which the function is pulling information regarding
 *				the number of neighbors.
 *@param mode a char representation of the user-specified boundary condition that
 *				the functino will refer to in dealing with boundary cases.
 *@return neighbors	int representation of the  number of live neighbors surrounding 
 *				the specified location.
 */
int CountNeighbors(int row, int col, Grid<int> mainGrid, char mode)
{	
	int neighbors = 0;
	int rowAdjust = 0;
	int colAdjust = 0;
	if (mainGrid.getAt(row, col) != 0) neighbors--;
	
	for (int i = row - 1; i <= row + 1; i ++) {
		if (mode == 'D') {
			rowAdjust = DonutAdjust(i, mainGrid.numRows());
		} else if (mode == 'M') {
			rowAdjust = MirrorAdjust(i, mainGrid.numRows());
		}
		
		for (int j = col - 1; j <= col + 1; j++) {
			if (mode == 'D') {
				colAdjust = DonutAdjust(j, mainGrid.numCols());
			} else if (mode == 'M') {
				colAdjust = MirrorAdjust(j, mainGrid.numCols());
			}
			if (mode == 'P') {
				if (i >= 0 && i < mainGrid.numRows() && j >= 0 && j < mainGrid.numCols() && mainGrid.getAt(i, j) != 0) {
					neighbors ++;
				}
			} else if (mainGrid.getAt(i + rowAdjust, j + colAdjust) != 0) {
				neighbors++;
			}
		}
	}	
	return neighbors;
}

/**
 * Function: GenerateNewGeneration()
 * -----------------------------------
 * Based upon the number of neighbors that each location has, this function 
 * determines whether or not the cell will live or die in accordance to the
 * specified rules: 1) locations with 3 neighbors will continue to live or
 * give birth to a new cell, 2) locations with 2 neighbors will remain stable,
 * no changes will take place, but the already-present cell will age, 3) locations
 * with more than 3 or less than 2 neighbors will die or remain empty.
 *
 *@param mainGrid the grid from which the current configuration of cells will be
 *					drawn.
 *@param newGrid the grid in which the new configuration of cells will be stored,
 *					as the function calculates the neighbors and implements rules.
 *@param mode a char representation of the user-specified boundary condition to be
 *					passed onto the CountNeighbors() function.
 */
void GenerateNewGeneration(Grid<int> mainGrid, Grid<int> & newGrid, char mode) 
{	
	int numNeighbors = 0;
	for (int i = 0; i < mainGrid.numRows(); i ++) {
		for (int j = 0; j < mainGrid.numCols(); j++) {
			numNeighbors = CountNeighbors(i, j, mainGrid, mode);
			switch(numNeighbors) {
                case(3): newGrid(i, j) = mainGrid.getAt(i, j) + 1; break;
                case(2): 
                    if (mainGrid.getAt(i, j) == 0) {
                        newGrid(i, j) = mainGrid.getAt(i, j); break;
                    } else {
                        newGrid(i, j) = mainGrid.getAt(i, j) + 1; break;
                    }
                default: newGrid(i, j) = 0; break;
			}
		}
	}
}

/**
 * Function: DrawNewGeneration()
 * ------------------------------
 * Draws cells with the specified age, in accordance to the cell configuration
 * of a grid.
 *
 *@param newGrid the grid from which the function pulls information regarding
 *				the cell configuration from.
 */
void DrawNewGeneration(Grid<int> newGrid) 
{
	for (int i = 0; i < newGrid.numRows(); i++) {
		for (int j = 0; j < newGrid.numCols(); j++) {
			DrawCellAt(i, j, newGrid.getAt(i, j));
		}
	}
}

/**
 * Function: StableColony()
 * -------------------------
 * Determines whether or not the colony has stabilized. A colony has stabilized once
 * its cell configuration remains constant and no longer changes and all of its live cells
 * have reached a constant MaxAge.
 *
 *@param mainGrid the grid from which the function will be pulling information regarding
 *					the age of the cells.
 *@return bool of whether or not the colony has stabilized.
 */
bool StableColony(Grid<int> mainGrid) 
{	
	int deadCells = 0;
	for (int i = 0; i < mainGrid.numRows(); i ++) {
		for (int j = 0; j < mainGrid.numCols(); j ++) {
			if (mainGrid.getAt(i, j) == 0) deadCells ++;
			if (mainGrid.getAt(i, j) != 0 && mainGrid.getAt(i, j) < MaxAge) return false;
		}
	}
	if (deadCells == mainGrid.numRows() * mainGrid.numCols()) return true;
	return true;
}

/**
 * Function: RepeatCycle()
 * -------------------------
 * Once the user has done an action or something has happened to stop the current
 * simulation, the user will be given a chance of restarting the simulation with
 * a different cell configuration.
 *
 *@return bool of whether or not the user has chosen to continue with another
 *				simulation.
 */
bool RepeatCycle()
{
	cout << "Would you like to run another simulation? ";
	string repeat = ConvertToUpperCase(GetLine());
	while (repeat != "YES" && repeat != "NO") {
		cout << "Please answer 'Yes' or 'No'." << endl;
		cout << "Would you like to run another simulation? ";
		repeat = ConvertToUpperCase(GetLine());
	}
	return (repeat == "YES");
}

/**
 * Function: GenerateSimulation()
 * -------------------------------
 * Generates the entire simulation after processing various user inputs to determine
 * logistical details of the simulation. Also takes into consideration various user
 * actions or colony stabilization that could prompt the current simulation to stop.
 *
 *@param mainGrid the grid the function will use to pull information from regarding
 *					the current cell configuration.
 *@param newGrid the grid the functino will use to store new information regarding the
 *					new cell configuration of the colony in the next generation.
 *@param mode the char representation of the user-specified boundary condition that will
 *					determine how the simulation will handle boundary cases.
 *@param speed the int representation of the user-specified speed of the simulation.
 */
void GenerateSimulation(Grid<int> mainGrid, Grid<int> newGrid, char mode, int speed)
{
	while(true) {
		GenerateNewGeneration(mainGrid, newGrid, mode);
		DrawNewGeneration(newGrid);
		mainGrid = newGrid;
		UpdateDisplay();
		if (speed <= 2) {
			Pause(speed);
		} else {
			cout << "RETURN to continue (or 'quit' to end simulation): ";
			if (ConvertToLowerCase(GetLine()) == "quit") break;
		}
		if (StableColony(mainGrid)) {
			cout << "Colony stabilized." << endl;
			break;
		}
		if (MouseButtonIsDown()) break;
	}
    
}

void Welcome();

/**
 * Function: main()
 * ------------------
 * Serves as the entry point of the program. Introduces the user to the purpose of the program
 * and prompts the user to specify certain logistical details of the simulation, storing it away
 * for future reference. Also intializes the two grids that will be used extensively by most of
 * the other functions.
 *
 *@return 0 to indicate that the program has successfully completed.
 */
int main()
{
	SetWindowTitle("Life!");
	InitGraphics();
	bool ongoingSimulation = true;
	Welcome();
	while (ongoingSimulation) {
		Grid<int> mainGrid(1, 1);
		if (DetermineStartingColony() == 'Y') {
			mainGrid = AccessStartingColony();
		} else {
			mainGrid = RandomColony();
		}
		Grid<int> newGrid(mainGrid.numRows(), mainGrid.numCols());
		int speed = DetermineSpeed();
		char mode = DetermineMode();
		if (speed <= 2) cout << "Click and hold the mouse button on the graphics window to end the simulation." << endl;
		GenerateSimulation(mainGrid, newGrid, mode, speed);
		ongoingSimulation = RepeatCycle();
	}
	return 0;
}

/**
 * Function: Welcome()
 * --------------------
 * Introductory function that provides user with basic information regarding purpose of the simulation
 * as well as the set of pre-determined rules concerning life/death determinations of cells.
 */
void Welcome()
{
	SetPointSize(48);
	MovePen(GetWindowWidth()/2-TextStringWidth("Welcome To Life!")/2,GetWindowHeight()/2);
	DrawTextString("Welcome To Life!");
	UpdateDisplay();
	cout << "Welcome to the game of Life, a simulation of the life cycle of a bacteria colony." << endl;
	cout << "Cells live and die by the following rules:" << endl;
	cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
	cout << "\tLocations with 2 neighbors remain stable" << endl;
	cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
	cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl;
	cout << "In the animation, new cells are dark and lighten as they age." << endl;
	cout << "Hit RETURN when ready: ";
	GetLine();
}

