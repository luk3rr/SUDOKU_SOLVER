/*
 * Filename: solver.h
 * Created on: April 12, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include <cstdint>

#include "constants.h"
#include "graph.h"
#include "graph_utils.h"
#include "grid_utils.h"
#include "heuristics.h"
#include "pair.h"
#include "priority_queue_bheap.h"
#include "queue_slkd.h"
#include "vector.h"
#include "vertex.h"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <pthread.h>

namespace sudoku
{
    /**
     * @brief Class that represents the solver of the sudoku puzzle
     */
    class Solver
    {
        private:
            uint16_t  m_startGrid[GRID_SIZE][GRID_SIZE]; /**< Initial grid */
            Algorithm m_algorithm; /**< Algorithm to solve the puzzle */

            std::size_t m_vertexSolutionID; /**< ID of the vertex that represents the
                                               solution */
            std::size_t m_expandedStates;   /**< Number of expanded states */

            // Each vertex stores a vector of pairs with the index of the row and column
            // of the grid and the change made at that position
            graph::Graph<uint16_t, uint16_t, Vector<State>, 2, true>
                m_graph; /**< Graph that
                  represents the
                  search tree */

            /**
             * @brief Create the initial state of the puzzle
             **/
            void CreateInitialState();

            /**
             * @brief Check if the state of the vertex is a solution
             * @param vertex Vertex to check if it is a solution
             * @return True if the vertex is a solution, false otherwise
             **/
            bool
            CheckSolution(graph::Vertex<uint16_t, uint16_t, Vector<State>>& vertex);

            /**
             * @brief Expands the node in the search tree by choosing an empty cell and
             * creating child nodes for all valid values for that cell
             * @param fatherVertex Father of the node to expand
             */
            void
            ExpandNode(graph::Vertex<uint16_t, uint16_t, Vector<State>>& fatherVertex);

            /**
             * @brief Print the state of the vertex
             * @param vertex Vertex to print the state
             * @param pythonStyle If true, print the state in a Python style
             **/
            void PrintState(graph::Vertex<uint16_t, uint16_t, Vector<State>>& vertex,
                            bool pythonStyle = false);

            /**
             * @brief Solve the puzzle using the Breadth-First Search algorithm
             * @return True if the puzzle was solved, false otherwise
             **/
            bool BFS();

            /**
             * @brief Auxiliary function to solve the puzzle using Iterative Deepening
             * Depth-First Search algorithm
             * @param currentVertexID ID of the current vertex
             * @param depth Depth of the search
             * @param label Label of the current vertex
             * @param True if the puzzle was solved, false otherwise
             **/
            bool
            IDDFS(std::size_t currentVertexID, std::size_t depth, std::size_t label);

            /**
             * @brief Solve the puzzle using the Iterative Deepening Depth-First Search
             * algorithm
             * @param maxDepth Maximum depth of the search. By default, it is set to
             * infinity
             * @return True if the puzzle was solved, false otherwise
             **/
            bool IDDFS(std::size_t maxDepth = GRID_SIZE * GRID_SIZE);

            /**
             * @brief Solve the puzzle using the Uniform Cost Search algorithm
             * @return True if the puzzle was solved, false otherwise
             **/
            bool UCS();

            /**
             * @brief Solve the puzzle using the A* algorithm
             * @param heuristic Heuristic to use in the A* algorithm
             * @return True if the puzzle was solved, false otherwise
             **/
            bool AStar(heuristics::distance::Heuristic heuristic =
                           heuristics::distance::Heuristic::EUCLIDEAN);

            /**
             * @brief Solve the puzzle using the Greedy Best-First Search algorithm
             * @return True if the puzzle was solved, false otherwise
             **/
            bool GreedyBFS(heuristics::distance::Heuristic heuristic =
                               heuristics::distance::Heuristic::EUCLIDEAN);

        public:
            /**
             * @brief Constructor
             * @param startGrid Initial grid
             * @param algorithm Algorithm to solve the puzzle
             */
            Solver(uint16_t startGrid[GRID_SIZE][GRID_SIZE], Algorithm algorithm);

            ~Solver();

            /**
             * @brief Print the algorithm used to solve the puzzle
             **/
            void PrintAlgorithm();

            /**
             * @brief Solve the puzzle
             **/
            void Solve();
    };
} // namespace sudoku

#endif // SOLVER_H_
