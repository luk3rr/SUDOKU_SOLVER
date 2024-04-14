/*
 * Filename: solver.cc
 * Created on: April 12, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#include "solver.h"
#include "constants.h"
#include "grid_utils.h"
#include "vector.h"
#include "vertex.h"
#include <cstdint>
#include <iostream>

namespace sudoku
{
    Solver::Solver(uint16_t grid[GRID_SIZE][GRID_SIZE], Algorithm algorithm)
    {
        this->m_algorithm        = algorithm;
        this->m_vertexSolutionID = 0;
        this->m_expandedStates   = 0;

        for (int i = 0; i < GRID_SIZE; i++)
        {
            for (int j = 0; j < GRID_SIZE; j++)
            {
                this->m_startGrid[i][j] = grid[i][j];
            }
        }
    }

    Solver::~Solver() { }

    void Solver::CreateInitialState()
    {
        // Make sure the graph is empty
        this->m_graph.Destroy();

        // Create the root vertex
        graph::Vertex<uint16_t, uint16_t, Vector<State>> root(0, Vector<State>());
        root.SetLabel(graph::VertexLabel::UNVISITED);
        this->m_graph.AddVertex(root);
    }

    bool Solver::CheckSolution(graph::Vertex<uint16_t, uint16_t, Vector<State>>& vertex)
    {
        uint16_t currentGrid[GRID_SIZE][GRID_SIZE];

        grid::CopyGrid(this->m_startGrid, currentGrid);

        // Since each vertex stores a history of changes, it is possible to use
        // these changes to reconstruct the new current grid
        Vector<State> changes = vertex.GetData();

        grid::ApplyChanges(currentGrid, changes);

        return grid::IsSolved(currentGrid);
    }

    void Solver::ExpandNode(graph::Vertex<uint16_t, uint16_t, Vector<State>>& father)
    {
        uint16_t currentGrid[GRID_SIZE][GRID_SIZE];

        grid::CopyGrid(this->m_startGrid, currentGrid);

        // Since each vertex stores a history of changes, it is possible to use
        // these changes to reconstruct the new current grid
        Vector<State> changes = father.GetData();

        grid::ApplyChanges(currentGrid, changes);

        // Find the first empty cell to expand
        uint16_t row, col;
        grid::FindEmptyCell(currentGrid, row, col);

        for (uint16_t num = 1; num <= GRID_SIZE; num++)
        {
            // For each possible number, check if it is valid and expand the node
            if (grid::IsValid(currentGrid, row, col, num))
            {
                // Create a new state with the change
                changes.PushBack(State(Pair<uint16_t, uint16_t>(row, col), num));

                // Create a new vertex and add it to the graph
                graph::Vertex<uint16_t, uint16_t, Vector<State>> child(
                    this->m_graph.GetNumVertices() + 1,
                    changes);

                child.SetLabel(graph::VertexLabel::UNVISITED);

                this->m_graph.AddVertex(child);

                this->m_graph.AddEdge(father.GetID(), child.GetID());

                this->m_expandedStates++;
            }
        }
        father.SetLabel(graph::VertexLabel::PROCESSING);
    }

    void Solver::PrintState(graph::Vertex<uint16_t, uint16_t, Vector<State>>& vertex,
                            bool pythonStyle)
    {
        uint16_t currentGrid[GRID_SIZE][GRID_SIZE];

        grid::CopyGrid(this->m_startGrid, currentGrid);

        // Since each vertex stores a history of changes, it is possible to use
        // these changes to reconstruct the new current grid
        Vector<State> changes = vertex.GetData();

        grid::ApplyChanges(currentGrid, changes);

        if (pythonStyle)
        {
            grid::PrintGridPythonStyle(currentGrid);
        }
        else
        {
            grid::PrintGrid(currentGrid);
        }
    }

    bool Solver::BFS()
    {
        this->CreateInitialState();

        slkd::Queue<graph::Vertex<uint16_t, uint16_t, Vector<State>>*> queue;

        queue.Enqueue(&this->m_graph.GetVertices().At(0));

        // Auxiliar variables to make code most legible
        graph::Vertex<uint16_t, uint16_t, Vector<State>>* u = nullptr;
        graph::Vertex<uint16_t, uint16_t, Vector<State>>* v = nullptr;

        graph::Edge<uint16_t, uint16_t, Vector<State>>* uv;

        Vector<graph::Edge<uint16_t, uint16_t, Vector<State>>*> uAdjList;

        while (not queue.IsEmpty())
        {
            u = queue.Dequeue();

            // Expand the node, that is, generate all possible and valid children
            this->ExpandNode(*u);

            uAdjList = u->GetAdjacencyList();

            for (std::size_t i = 0; i < uAdjList.Size(); i++)
            {
                uv = uAdjList.At(i);

                // Get the pointer do neighbor vertex, since one end of the edge is
                // vertex u, and the other end is vertex v
                uv->GetVertices().GetFirst()->GetID() == u->GetID()
                    ? v = &this->m_graph
                               .GetVertices()[uv->GetVertices().GetSecond()->GetID()]
                    : v = &this->m_graph
                               .GetVertices()[uv->GetVertices().GetFirst()->GetID()];

                // Check if the solution was found
                if (this->CheckSolution(*v))
                {
                    this->m_vertexSolutionID = v->GetID();
                    return true;
                }

                if (v->GetLabel() == graph::VertexLabel::UNVISITED)
                    queue.Enqueue(v);
            }
        }

        return false;
    }

    bool
    Solver::IDDFS(std::size_t currentVertexID, std::size_t depth, std::size_t label)
    {
        // Auxiliar variables to make code most legible
        graph::Vertex<uint16_t, uint16_t, Vector<State>>* u = nullptr;
        graph::Vertex<uint16_t, uint16_t, Vector<State>>* v = nullptr;

        u = &this->m_graph.GetVertices().At(currentVertexID);

        if (this->CheckSolution(*u))
            return true;

        if (depth <= 0)
            return false;

        u->SetLabel(label);

        graph::Edge<uint16_t, uint16_t, Vector<State>>* uv;

        // Expand the node, that is, generate all possible and valid children
        this->ExpandNode(*u);

        Vector<graph::Edge<uint16_t, uint16_t, Vector<State>>*> uAdjList =
            u->GetAdjacencyList();

        for (std::size_t i = 0; i < uAdjList.Size(); i++)
        {
            // Edge uv (or vu, if is non-directed)
            uv = uAdjList.At(i);

            // Get the pointer do neighbor vertex, since one end of the edge is
            // vertex u, and the other end is vertex v
            uv->GetVertices().GetFirst()->GetID() == u->GetID()
                ? v = &this->m_graph
                           .GetVertices()[uv->GetVertices().GetSecond()->GetID()]
                : v = &this->m_graph
                           .GetVertices()[uv->GetVertices().GetFirst()->GetID()];

            // Check if the solution was found
            if (this->CheckSolution(*v))
            {
                this->m_vertexSolutionID = v->GetID();
                return true;
            }

            if (v->GetLabel() != label)
            {
                if (IDDFS(v->GetID(), --depth, label))
                    return true;
            }
        }
        return false;
    }

    bool Solver::IDDFS(std::size_t maxDepth)
    {
        for (std::size_t i = 0; i < maxDepth; i++)
        {
            this->CreateInitialState();

            if (IDDFS(0, i, i))
                return true;
        }

        return false;
    }

    // bool UCS(std::size_t sourceID, std::size_t targetID)
    //{
    //     bheap::PriorityQueue<
    //         graph::Vertex<uint16_t, uint16_t, Vector<State>, nDim>*,
    //         decltype(graph::compare::Vertex<uint16_t, uint16_t, Vector<State>,
    //         nDim>)> minPQueue;

    //    // Defines the infinity value for the uint16_t type
    //    uint16_t INFINITY_VALUE = std::numeric_limits<uint16_t>::max();

    //    // Set all vertices as not visited
    //    for (std::size_t i = 0; i < this->m_graph.GetVertices().Size(); i++)
    //    {
    //        this->m_graph.GetVertices().At(i).SetLabel(graph::VertexLabel::UNVISITED);
    //        this->m_graph.GetVertices().At(i).SetCurrentCost(INFINITY_VALUE);
    //        this->m_graph.GetVertices().At(i).SetEdge2Predecessor(nullptr);
    //    }

    //    this->m_graph.GetVertices().At(sourceID).SetCurrentCost(0);

    //    minPQueue.Enqueue(&graph.GetVertices().At(sourceID));

    //    // Auxiliar variables to make code most legible
    //    graph::Vertex<uint16_t, uint16_t, Vector<State>, nDim>* u = nullptr;
    //    graph::Vertex<uint16_t, uint16_t, Vector<State>, nDim>* v = nullptr;

    //    graph::Edge<uint16_t, uint16_t, Vector<State>, nDim>* uv;

    //    Vector<graph::Edge<uint16_t, uint16_t, Vector<State>, nDim>*> uAdjList;

    //    while (not minPQueue.IsEmpty())
    //    {
    //        u = minPQueue.Dequeue();

    //        u->SetLabel(graph::VertexLabel::VISITED);

    //        if (u->GetID() == targetID)
    //            break;

    //        uAdjList = u->GetAdjacencyList();

    //        for (std::size_t i = 0; i < uAdjList.Size(); i++)
    //        {
    //            uv = uAdjList.At(i);

    //            // Get the pointer do neighbor vertex, since one end of the edge is
    //            // vertex u, and the other end is vertex v
    //            uv->GetVertices().GetFirst()->GetID() == u->GetID()
    //                ? v = &graph.GetVertices()[uv->GetVertices().GetSecond()->GetID()]
    //                : v = &graph.GetVertices()[uv->GetVertices().GetFirst()->GetID()];

    //            if (v->GetLabel() == graph::VertexLabel::UNVISITED and
    //                Relax(u, v, uAdjList.At(i)))
    //            {
    //                minPQueue.Enqueue(v);
    //            }
    //        }
    //    }
    //}

    // bool Solver::AStar(heuristics::distance::Heuristic heuristic)
    //{
    //     bheap::PriorityQueue<
    //         graph::Vertex<uint16_t, uint16_t, Vector<State>>*,
    //         decltype(graph::compare::Vertex<uint16_t, uint16_t, Vector<State>>)>
    //         minPQueue;

    //    // Defines the infinity value for the uint16_t type
    //    uint16_t INFINITY_VALUE = std::numeric_limits<uint16_t>::max();

    //    // Set all vertices as not visited
    //    for (std::size_t i = 0; i < this->m_graph.GetVertices().Size(); i++)
    //    {
    //        this->m_graph.GetVertices().At(i).SetLabel(graph::VertexLabel::UNVISITED);
    //        this->m_graph.GetVertices().At(i).SetCurrentCost(INFINITY_VALUE);
    //        this->m_graph.GetVertices().At(i).SetHeuristicCost(0);
    //        this->m_graph.GetVertices().At(i).SetEdge2Predecessor(nullptr);
    //    }

    //    // Auxiliar variables to make code most legible
    //    graph::Vertex<uint16_t, uint16_t, Vector<State>>* u =
    //        &this->m_graph.GetVertices().At(sourceID);
    //    graph::Vertex<uint16_t, uint16_t, Vector<State>>* v = nullptr;
    //    graph::Vertex<uint16_t, uint16_t, Vector<State>>* t =
    //        &this->m_graph.GetVertices().At(targetID);

    //    graph::Edge<uint16_t, uint16_t, Vector<State>>* uv;

    //    Vector<graph::Edge<uint16_t, uint16_t, Vector<State>>*> uAdjList;

    //    this->m_graph.GetVertices().At(sourceID).SetCurrentCost(
    //        CalculateHeuristic(heuristic, u, t));
    //    this->m_graph.GetVertices().At(sourceID).SetHeuristicCost(
    //        CalculateHeuristic(heuristic, u, t));

    //    minPQueue.Enqueue(&graph.GetVertices().At(sourceID));

    //    while (not minPQueue.IsEmpty())
    //    {
    //        u = minPQueue.Dequeue();

    //        u->SetLabel(graph::VertexLabel::VISITED);

    //        if (u->GetID() == targetID)
    //        {
    //            // PrintPath(graph, u);
    //            break;
    //        }

    //        uAdjList = u->GetAdjacencyList();

    //        for (std::size_t i = 0; i < uAdjList.Size(); i++)
    //        {
    //            uv = uAdjList.At(i);

    //            // Get the pointer do neighbor vertex, since one end of the edge is
    //            // vertex u, and the other end is vertex v
    //            uv->GetVertices().GetFirst()->GetID() == u->GetID()
    //                ? v = &graph.GetVertices()[uv->GetVertices().GetSecond()->GetID()]
    //                : v = &graph.GetVertices()[uv->GetVertices().GetFirst()->GetID()];

    //            if (v->GetLabel() == graph::VertexLabel::UNVISITED)
    //            {
    //                v->SetHeuristicCost(CalculateHeuristic(heuristic, v, t));

    //                if (Relax(u, v, uAdjList.At(i)))
    //                {
    //                    minPQueue.Enqueue(v);
    //                }
    //            }
    //        }
    //    }
    //}

    // bool Solver::GreedyBFS(heuristics::distance::Heuristic heuristic)
    // {
    //     bheap::PriorityQueue<
    //         graph::Vertex<uint16_t, uint16_t, Vector<State>>*,
    //         decltype(graph::compare::VertexHeuristic<uint16_t, uint16_t,
    //         Vector<State>>)> minPQueue;

    //     // Set all vertices as not visited
    //     for (std::size_t i = 0; i < this->m_graph.GetVertices().Size(); i++)
    //     {
    //         this->m_graph.GetVertices().At(i).SetLabel(graph::VertexLabel::UNVISITED);
    //         this->m_graph.GetVertices().At(i).SetHeuristicCost(0);
    //         this->m_graph.GetVertices().At(i).SetEdge2Predecessor(nullptr);
    //     }

    //     // Auxiliar variables to make code most legible
    //     graph::Vertex<uint16_t, uint16_t, Vector<State>>* u = nullptr;
    //     graph::Vertex<uint16_t, uint16_t, Vector<State>>* v = nullptr;
    //     graph::Vertex<uint16_t, uint16_t, Vector<State>>* t = nullptr;

    //     // Check if source and target vertices are valid
    //     try
    //     {
    //         u = &this->m_graph.GetVertices().At(sourceID);
    //         v = nullptr;
    //         t = &this->m_graph.GetVertices().At(targetID);
    //     }
    //     catch (const std::out_of_range& e)
    //     {
    //         // sourceID or targetID are invalid, that is, they are not in the graph
    //         return false;
    //     }

    //     graph::Edge<uint16_t, uint16_t, Vector<State>>* uv;

    //     Vector<graph::Edge<uint16_t, uint16_t, Vector<State>>*> uAdjList;

    //     this->m_graph.GetVertices().At(sourceID).SetHeuristicCost(
    //         CalculateHeuristic(heuristic, u, t));

    //     minPQueue.Enqueue(&this->m_graph.GetVertices().At(sourceID));

    //     while (not minPQueue.IsEmpty())
    //     {
    //         u = minPQueue.Dequeue();

    //         u->SetLabel(graph::VertexLabel::VISITED);

    //         if (u->GetID() == targetID)
    //         {
    //             // PrintPath(graph, u);
    //             return true;
    //         }

    //         uAdjList = u->GetAdjacencyList();

    //         for (std::size_t i = 0; i < uAdjList.Size(); i++)
    //         {
    //             uv = uAdjList.At(i);

    //             // Get the pointer do neighbor vertex, since one end of the edge is
    //             // vertex u, and the other end is vertex v
    //             uv->GetVertices().GetFirst()->GetID() == u->GetID()
    //                 ? v = &this->m_graph
    //                            .GetVertices()[uv->GetVertices().GetSecond()->GetID()]
    //                 : v = &this->m_graph
    //                            .GetVertices()[uv->GetVertices().GetFirst()->GetID()];

    //             if (v->GetLabel() == graph::VertexLabel::UNVISITED)
    //             {
    //                 v->SetHeuristicCost(CalculateHeuristic(heuristic, v, t));
    //                 v->SetEdge2Predecessor(uv);

    //                 minPQueue.Enqueue(v);
    //             }
    //         }
    //     }

    //     return false;
    // }
    void Solver::PrintAlgorithm()
    {
        std::cout << "Algorithm: ";

        // Print letters according to the algorithm
        switch (m_algorithm)
        {
            case Algorithm::BFS:
                std::cout << "BFS" << std::endl;
                break;
            case Algorithm::IDDFS:
                std::cout << "IDDFS" << std::endl;
                break;
            case Algorithm::UCS:
                std::cout << "UCS" << std::endl;
                break;
            case Algorithm::A_STAR:
                std::cout << "A*" << std::endl;
                break;
            case Algorithm::GBFS:
                std::cout << "GREEDY" << std::endl;
                break;
            default:
                std::cout << "UNKNOWN" << std::endl;
                break;
        }
    }

    void Solver::Solve()
    {
        std::cout << "Solving the following grid:" << std::endl;
        grid::PrintGrid(this->m_startGrid);
        std::cout << std::endl;

        bool solved = false;

        // Check if the grid is already solved
        if (grid::IsSolved(this->m_startGrid))
        {
            grid::PrintGrid(this->m_startGrid);
            return;
        }
        else
        {
            switch (this->m_algorithm)
            {
                case Algorithm::BFS:
                    solved = this->BFS();
                    break;

                case Algorithm::IDDFS:
                    solved = this->IDDFS();
                    break;

                case Algorithm::UCS:
                    break;

                case Algorithm::A_STAR:
                    break;

                case Algorithm::GBFS:
                    break;

                default:
                    break;
            }
        }

        if (solved)
        {
            this->PrintState(this->m_graph.GetVertices().At(this->m_vertexSolutionID));
            std::cout << "Total expanded states: " << this->m_expandedStates
                      << std::endl;
        }
        else
        {
            std::cout << "No solution found" << std::endl;

            std::cout << "Total expanded states: " << this->m_expandedStates
                      << std::endl;
        }
    }

} // namespace sudoku
