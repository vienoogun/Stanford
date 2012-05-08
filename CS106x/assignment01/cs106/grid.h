/*
 * File: grid.h
 * Last modified on Wed Apr  1 08:10:18 2009 by eroberts
 *      modified on Tue Jan  2 13:40:51 2007 by zelenski
 * -----------------------------------------------------
 * This interface defines a class template to store a homogeneous 2-D
 * indexed collection. The basic operations are similar to those
 * on C++ built-in multi-dimensional arrays with the added features of
 * dynamic resizing, deep copying, and bounds-checking on indexes.
 *
 * Here is some sample code showing use of a Grid of strings:
 *
 *      Grid<string> g(4, 6); // creates 4x6 grid, each string empty
 *
 *      g(0, 2) = "blue";   // can set elemes using () or setAt
 *      g.setAt(2, 3, "red");
 *
 *      for (int row = 0; row < g.numRows(); row++)
 *          for (int col = 0; col < g.numCols(); col++)
 *               cout << g(row, col) << " ";   // or g.getAt(row, col);
 *
 *
 * Access to individual elements in the grid is provided via the
 * setAt/getAt member functions, as well as an overloaded operator().
 * Either refers to cells by row, col location, indexes start at 0 in
 * each dimension.
 *
 *
 * Last updated: Thu Jan  2 14:07:49 2007 zelenski
 */

#ifndef _grid_h_
#define _grid_h_


/*
 * Class: Grid
 * ------------
 * The class Grid stores an indexed, two-dimensional collection.
 * It is supplied as a class template. The client specializes the grid
 * to hold values of a specific type. Because the class is defined
 * with private data members, clients have no access to the underlying
 * data members and can only manipulate a grid object through
 * its public member functions.
 */

template <typename ElemType>
class Grid {

 public:

    /*
     * Constructor: Grid
     * Usage: Grid<bool> grid;
     *        Grid<string> *gp = new Grid<string>;
     * -------------------------------------------
     * This no-argument constructor initializes a new empty grid with
     * 0 rows and 0 cols.
     */
    Grid();

    /*
     * Constructor: Grid
     * Usage: Grid<bool> grid(10, 20)
     *        Grid<string> *gp = new Grid<string>(10, 20);
     * ---------------------------------------------------
     * This two-argument constructor initializes a new grid with num
     * rows and cols. Each element in the grid has value equal to
     * the default for that element type (e.g. for strings it would
     * be empty string, for ints, the default value is uninitialized).
     * Raises an error if numRowsx or numCols is negative.
     */
    Grid(int numRows, int numCols);


    /*
     * Destructor: ~Grid
     * Usage: delete gp;
     * -----------------
     * The destructor deallocates storage associated with this grid.
     */
    ~Grid();


    /*
     * Member functions: numRows, numCols
     * Usage: nRows = grid.numRows();
     * ------------------------------
     * These member functions returns the number of rows or columns in
     * this grid.
     */
    int numRows();
    int numCols();


    /*
     * Member function: resize
     * Usage: grid.resize(5, 10);
     * --------------------------
     * This member function sets the number of rows and columns in this
     * grid to the specified values. Any previous grid contents are
     * discarded.   Each element in the  resized grid has value equal
     * to the default for that element type. Raises an error if numRows
     * or numCols is negative.
     */
    void resize(int numRows, int numCols);


    /*
     * Member function: getAt
     * Usage: val = grid.getAt(3, 10);
     * -------------------------------
     * This member function returns the element at specified row/col
     * location in this grid. Locations are indexed starting from 0.
     * Raises an error if row is outside range [0, numRows()-1] or if
     * col is outside range [0, numCols()-1].
     */
    ElemType getAt(int row, int col);

    /*
     * Member function: setAt
     * Usage: grid.setAt(3, 10, value);
     * --------------------------------
     * This member function replaces the element at the specified
     * row/col location in this grid with a new value.  The previous
     * value at that location is overwritten with the new value.
     * Raises an error if row is outside the range [0, numRows()-1]
     * or if col is outside the range [0, numCols()-1].
     */
    void setAt(int row, int col, ElemType value);


    /*
     * Member function: operator()
     * Usage: grid(0, 0) = grid(1, 1);
     * -------------------------------
     * This member function overloads () to access elements from
     * this grid. This allows the client to use function-like notation
     * to get/set individual elements. Returns a reference to the
     * element to allow in-place modification of values. Raises an
     * error if row is outside the range [0, numRows()-1] or if col is
     * outside the range [0, numCols()-1].
     */
    ElemType &operator()(int row, int col);

	class GridRow;

    /*
     * Member function: operator[]
     * Usage: grid[0][0] = grid[1][1];
     * -------------------------------
     * This member function overloads [] to access elements from
     * this grid. This allows the client to use C/Java-like notation
     * to get/set individual elements. Returns a reference to the
     * element to allow in-place modification of values. Raises an
     * error if row is outside the range [0, numGridRows()-1] or if col is
     * outside the range [0, numCols()-1].
     */
    GridRow operator[](int row);

    /*
     * Class: Grid<ValType>::GridRow
     * -------------------------
     * This interface defines a nested class within the Grid template that
     * makes it possible to use traditional subscripting on Grid values.
     */
    class GridRow {
    public:
        GridRow();
        ElemType & operator[](int col);

    private:
        GridRow(Grid *gridRef, int index);
		Grid *gp;
        int row;
        friend class Grid;
    };
    friend class GridRow;


		/*
		 * SPECIAL NOTE: mapping/iteration support
		 * ---------------------------------------
		 * The Grid class supports both a mapping operation and an iterator which
		 * allow the client access to all elements one by one.  In general,
		 * these  are intended for _viewing_ elements and can behave
		 * unpredictably if you attempt to modify the grid's contents during
		 * mapping/iteration.
		 */

		/*
		 * Member function: mapAll
		 * Usage: grid.mapAll(Print);
		 * --------------------------
		 * This member function iterates through this grid's contents
		 * and calls the function fn once for each element.
		 */
		void mapAll(void (fn)(ElemType elem));


		/*
		 * Member function: mapAll
		 * Usage: grid.mapAll(PrintToFile, outputStream);
		 * ----------------------------------------------
		 * This member function iterates through this grid's contents
		 * and calls the function fn once for each element, passing
		 * the element and the client's data. That data can be of whatever
		 * type is needed for the client's callback.
		 */
		template <typename ClientDataType>
			void mapAll(void (fn)(ElemType elem, ClientDataType &data),
							ClientDataType &data);


		class Iterator;

		/*
		 * Member function: iterator
		 * Usage: itr = grid.iterator();
		 * --------------------------
		 * This member function creates an iterator that allows the client
		 * to iterate through the elements in this grid. The elements are
		 * accessed in order as determined by the elem comparison function.
		 */
		Iterator iterator();


		/*
		 * Class: Grid<ElemType>::Iterator
		 * -------------------------------
		 * This interface defines a nested class within the Grid template that
		 * provides iterator access to the Grid contents.
		 *
		 * The idiomatic code for accessing elements using an iterator is
		 * to create the iterator from the collection and then enter a loop
		 * that calls next() while hasNext() is true, like this:
		 *
		 *	Grid<int>::Iterator itr = grid.iterator();
		 *  while (itr.hasNext())
		 *      int num = itr.next();
		 */
		class Iterator {
			public:
				Iterator();
				bool hasNext();
				ElemType next();

			private:
				Iterator(Grid *gridRef);
				Grid *gp;
				int curRow;
				int curCol;
				long timestamp;
				friend class Grid;
		};
		friend class Iterator;

    /*
     * Deep copying support
     * --------------------
     * This copy constructor and operator= are defined to make a
     * deep copy, making it possible to pass/return grids by value
     * and assign from one grid to another. The entire contents of
     * the grid, including all elements, are copied. Each grid
     * element is copied from the original grid to the copy using
     * assignment (operator=). Making copies is generally avoided
     * because of the expense and thus, grids are typically passed by
     * reference, however, when a copy is needed, these operations
     * are supported.
     */
    const Grid &operator=(const Grid &rhs) ;
    Grid(const Grid &rhs);


 private:

    ElemType *elements;
    int nRows, nCols;
	long timestamp;

    void checkRange(int row, int col);
    void copyContentsFrom(const Grid &other);
};



/*
 * Because of the way C++ templates are compiled, we must put the class implementation in
 * the header file. This is a bit quirky and seems to contradict the principles of
 * encapsulation and abstraction. As a client of this class, you don't need to know
 * anything about the internals and shouldn't depend on any implementation details. In
 * fact, you probably should just stop reading right here!
 */

/*
 * Grid class implementation
 * ---------------------------
 * The Grid is internally managed as a dynamic array of elements.  The array itself
 * is one-dimensional, the logical separation into rows and columns is done
 * manually.  The layout is in row-major order, which is to say that the first entire
 * row is laid out contiguously, followed by the entire next row and so on. All access
 * is bounds-checked for safety.
 */

#include "genlib.h" // for Error
#include "strutils.h" // for IntegerToString
#include <cstdlib>// for NULL


template <typename ElemType>
Grid<ElemType>::Grid()
{
    nRows = 0;
    nCols = 0;
	timestamp = 0L;
    elements = NULL;
}

template <typename ElemType>
Grid<ElemType>::Grid(int numRows, int numCols)
{
    elements = NULL;
	timestamp = 0L;
    resize(numRows, numCols);
}

template <typename ElemType>
Grid<ElemType>::~Grid()
{
    delete[] elements;
}

template <typename ElemType>
int Grid<ElemType>::numRows()
{
    return nRows;
}

template <typename ElemType>
int Grid<ElemType>::numCols()
{
    return nCols;
}

template <typename ElemType>
void Grid<ElemType>::resize(int numRows, int numCols)
{
    if (numRows < 0 || numCols < 0) Error("Attempt to resize grid to invalid size (" +
                                          IntegerToString(numRows) + ", " + IntegerToString(numCols) + ")");
    if (elements) delete[] elements;
    nRows = numRows;
    nCols = numCols;
    elements = new ElemType[nRows * nCols];
	timestamp++;
}

template <typename ElemType>
ElemType Grid<ElemType>::getAt(int row, int col)
{
    return (*this)(row, col);
}

template <typename ElemType>
void Grid<ElemType>::setAt(int row, int col, ElemType value)
{
    (*this)(row, col) = value;
}

template <typename ElemType>
ElemType &Grid<ElemType>::operator()(int row, int col)
{
    checkRange(row,col);
    return elements[(row * nCols) + col];
}

template <typename ElemType>
typename Grid<ElemType>::GridRow Grid<ElemType>::operator[](int row)
{
    return GridRow(this, row);
}

template <typename ElemType>
const Grid<ElemType> & Grid<ElemType>::operator=(const Grid &rhs)
{
    if (this != &rhs) {
        delete[] elements;
        copyContentsFrom(rhs);
		timestamp++;
    }
    return *this;
}

template <typename ElemType>
Grid<ElemType>::Grid(const Grid &rhs)
{
    copyContentsFrom(rhs);
}

template <typename ElemType>
void Grid<ElemType>::checkRange(int row, int col)
{
    if (row < 0 || row >= numRows() || col < 0 || col >= numCols())
        Error("Attempt to access location (" + IntegerToString(row) + ", " + IntegerToString(col) +
              ") in a grid of size (" + IntegerToString(numRows()) + ", " + IntegerToString(numCols()) + ")");
}

template <typename ElemType>
void Grid<ElemType>::copyContentsFrom(const Grid &other)
{
    nRows = other.nRows;
    nCols = other.nCols;
    elements = new ElemType[nRows * nCols];
    for (int i = 0; i < nRows * nCols; i++)
        elements[i] = other.elements[i];
}

template <typename ElemType>
  void Grid<ElemType>::mapAll(void (fn)(ElemType))
	{
		long t0 = timestamp;
		for (int i = 0; i < nRows * nCols; i++) {
			if (timestamp != t0) {
				Error("Grid structure has been modified");
			}
			fn(elements[i]);
		}
	}

template <typename ElemType>
 template <typename ClientDataType>
  void Grid<ElemType>::mapAll(void (fn)(ElemType, ClientDataType&), ClientDataType &data)
	{
		long t0 = timestamp;
		for (int i = 0; i < nRows * nCols; i++) {
			if (timestamp != t0) {
				Error("Grid structure has been modified");
			}
			fn(elements[i], data);
		}
	}

/*
 * Grid::Iterator class implementation
 * ----------------------------------
 * The Iterator for Grid maintains a pointer to the original Grid and
 * an index into that vector that identifies the next element to return.
 */

template <typename ElemType>
  Grid<ElemType>::Iterator::Iterator()
	{
		gp = NULL;
	}

template <typename ElemType>
  typename Grid<ElemType>::Iterator Grid<ElemType>::iterator()
	{
		return Iterator(this);
	}


template <typename ElemType>
  Grid<ElemType>::Iterator::Iterator(Grid *gridRef)
	{
		gp = gridRef;
		curRow = 0;
		curCol = 0;
		timestamp = gp->timestamp;
	}

template <typename ElemType>
  bool Grid<ElemType>::Iterator::hasNext()
	{
		if (gp == NULL) Error("hasNext called on uninitialized iterator");
		if (timestamp != gp->timestamp) {
			Error("Grid structure has been modified");
		}
		return curRow < gp->numRows() && curCol < gp->numCols();
	}

template <typename ElemType>
  ElemType Grid<ElemType>::Iterator::next()
	{
		if (gp == NULL) Error("next called on uninitialized iterator");
		if (!hasNext()) Error("Attempt to get next from iterator where hasNext() is false");
		int row = curRow;
		int col = curCol++;
		if (curCol == gp->numCols()) {
			curCol = 0;
			curRow++;
		}
		return (*gp)[row][col];
	}

/* GridRow implementation */

template <typename ElemType>
Grid<ElemType>::GridRow::GridRow()
{
	/* Empty */
}

template <typename ElemType>
Grid<ElemType>::GridRow::GridRow(Grid *gridRef, int index)
{
	gp = gridRef;
	row = index;
}

template <typename ElemType>
ElemType & Grid<ElemType>::GridRow::operator[](int col)
{
	return (*gp)(row,col);
}


#endif /* _grid_h_ */
