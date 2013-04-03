#ifndef GRID_H_
#define GRID_H_

#include "gameplay.h"

using namespace gameplay;

static const unsigned int DEFAULT_LINE_COUNT = 81;

/**
 * Creates a new grid mesh.
 * 
 * @param lineCount The number of lines in the grid. (Rows or columns). Should be odd.
 * 
 * @return A new grid mesh or NULL if there was an error.
 */
Mesh* createGridMesh(unsigned int lineCount = DEFAULT_LINE_COUNT);

/**
 * Creates a model that contains a new grid mesh.
 * 
 * @param lineCount The number of lines in the grid. (Rows or columns). Should be odd.
 * 
 * @return A new model containing a grid mesh or NULL if there was an error.
 */
Model* createGridModel(unsigned int lineCount = DEFAULT_LINE_COUNT);

#endif
