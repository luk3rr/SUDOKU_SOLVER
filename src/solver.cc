/*
 * Filename: solver.cc
 * Created on: April 12, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#include "solver.h"

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

    void
    Solver::GetVertexState(graph::Vertex<uint16_t, uint16_t, Vector<State>>& vertex,
                           uint16_t grid[GRID_SIZE][GRID_SIZE])
    {
        grid::CopyGrid(this->m_startGrid, grid);

        // Since each vertex stores a history of changes, it is possible to use
        // these changes to reconstruct the new current grid
        Vector<State> changes = vertex.GetData();

        grid::ApplyChanges(grid, changes);
    }

    uint16_t Solver::GenRandomCost()
    {
        unsigned     seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator(seed);

        std::uniform_int_distribution<int> distribution(1, GRID_SIZE + 1);

        return distribution(generator);
    }

    uint16_t Solver::CalculateAStarHeuristic(
        graph::Vertex<uint16_t, uint16_t, Vector<State>>& vertex)
    {

        uint16_t currentGrid[GRID_SIZE][GRID_SIZE];

        this->GetVertexState(vertex, currentGrid);

        uint16_t possibleNumbers = 0;

        Vector<State> changes = vertex.GetData();

        if (not changes.IsEmpty())
        {
            uint16_t row, col;

            row = changes.Back().GetFirst().GetFirst();
            col = changes.Back().GetFirst().GetSecond();

            for (uint16_t num = 1; num <= GRID_SIZE; num++)
            {
                if (grid::IsValid(currentGrid, row, col, num))
                    possibleNumbers++;
            }

            return possibleNumbers;
        }

        // If the vertex has no changes, that is, it is the root vertex, the cost
        // is GRID_SIZE
        return GRID_SIZE;
    }

    uint16_t Solver::CalculateGreedyBFSHeuristic(
        graph::Vertex<uint16_t, uint16_t, Vector<State>>& vertex)
    {

        uint16_t currentGrid[GRID_SIZE][GRID_SIZE];

        this->GetVertexState(vertex, currentGrid);

        uint16_t emptyCells = 0;

        for (uint16_t row = 0; row < GRID_SIZE; row++)
        {
            for (uint16_t col = 0; col < GRID_SIZE; col++)
            {
                if (currentGrid[row][col] == 0)
                    emptyCells++;
            }
        }

        return emptyCells;
    }

    void Solver::CreateInitialState()
    {
        // Make sure the graph is empty
        this->m_graph.Destroy();

        // Create the root vertex
        this->m_graph.AddVertex(Vector<State>())
            .SetLabel(graph::VertexLabel::UNVISITED);
    }

    bool Solver::CheckSolution(graph::Vertex<uint16_t, uint16_t, Vector<State>>& vertex)
    {
        uint16_t currentGrid[GRID_SIZE][GRID_SIZE];

        this->GetVertexState(vertex, currentGrid);

        return grid::IsSolved(currentGrid);
    }

    void Solver::ExpandNode(graph::Vertex<uint16_t, uint16_t, Vector<State>>& father)
    {
        uint16_t currentGrid[GRID_SIZE][GRID_SIZE];

        this->GetVertexState(father, currentGrid);
        Vector<State> changes = father.GetData();

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
                graph::Vertex<uint16_t, uint16_t, Vector<State>>& child =
                    this->m_graph.AddVertex(changes);

                child.SetCurrentCost(this->GenRandomCost());

                child.SetLabel(graph::VertexLabel::UNVISITED);

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

        this->GetVertexState(vertex, currentGrid);

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

        queue.Enqueue(&this->m_graph.GetVertex(0));

        // Auxiliar variables to make code most legible
        graph::Vertex<uint16_t, uint16_t, Vector<State>>* u = nullptr;
        graph::Vertex<uint16_t, uint16_t, Vector<State>>* v = nullptr;

        graph::Edge<uint16_t, uint16_t, Vector<State>>* uv;

        while (not queue.IsEmpty())
        {
            u = queue.Dequeue();

            // Expand the node, that is, generate all possible and valid children
            this->ExpandNode(*u);

            // Pair<first, second> = <ID, Edge>
            for (auto& pair : u->GetAdjacencyList())
            {
                // Edge uv (or vu, if is non-directed uv = vu)
                uv = pair.GetSecond();

                // Get the pointer do neighbor vertex, since one end of the edge is
                // vertex u, and the other end is vertex v
                uv->GetVertices().GetFirst()->GetID() == u->GetID()
                    ? v = &this->m_graph.GetVertex(
                          uv->GetVertices().GetSecond()->GetID())
                    : v = &this->m_graph.GetVertex(
                          uv->GetVertices().GetFirst()->GetID());

                // Check if the solution was found
                if (this->CheckSolution(*v))
                {
                    this->m_vertexSolutionID = v->GetID();
                    return true;
                }

                if (v->GetLabel() == graph::VertexLabel::UNVISITED)
                    queue.Enqueue(v);
            }

            // After expanding a vertex, since we won't visit it again, we remove the
            // node from the graph to save memory
            this->m_graph.RemoveVertex(u->GetID());
        }

        return false;
    }

    bool Solver::IDDFS(std::size_t maxDepth)
    {
        // Auxiliar variables to make code most legible
        graph::Vertex<uint16_t, uint16_t, Vector<State>>* u = nullptr;
        graph::Vertex<uint16_t, uint16_t, Vector<State>>* v = nullptr;

        graph::Edge<uint16_t, uint16_t, Vector<State>>* uv;

        for (std::size_t depth = 1; depth <= maxDepth; depth++)
        {
            this->CreateInitialState();

            slkd::Stack<graph::Vertex<uint16_t, uint16_t, Vector<State>>*> stack;

            // The cost of a vertex is the depth in which it was visited (depth of the
            // search)
            this->m_graph.GetVertex(0).SetCurrentCost(0);
            this->m_graph.GetVertex(0).SetLabel(graph::VISITED);

            stack.Push(&this->m_graph.GetVertex(0));

            while (not stack.IsEmpty())
            {
                u = stack.Pop();

                // If the cost of the vertex is greater than the depth, we don't need to
                // expand it (since we are doing a depth-limited search)
                if (u->GetCurrentCost() > depth)
                    continue;

                this->ExpandNode(*u);

                // Pair<first, second> = <ID, Edge>
                for (auto& pair : u->GetAdjacencyList())
                {
                    // Edge uv (or vu, if is non-directed uv = vu)
                    uv = pair.GetSecond();

                    // Get the pointer do neighbor vertex, since one end of the edge is
                    // vertex u, and the other end is vertex v
                    uv->GetVertices().GetFirst()->GetID() == u->GetID()
                        ? v = &this->m_graph.GetVertex(
                              uv->GetVertices().GetSecond()->GetID())
                        : v = &this->m_graph.GetVertex(
                              uv->GetVertices().GetFirst()->GetID());

                    if (this->CheckSolution(*v))
                    {
                        this->m_vertexSolutionID = v->GetID();
                        return true;
                    }

                    // If the vertex has not been visited yet, we add it to the stack
                    // with the cost of the current vertex + 1
                    if (v->GetLabel() == graph::VertexLabel::UNVISITED)
                    {
                        v->SetLabel(u->GetCurrentCost() + 1);
                        stack.Push(v);
                    }
                }

                // After expanding a vertex, since we won't visit it again, we remove
                // the node from the graph to save memory
                this->m_graph.RemoveVertex(u->GetID());
            }
        }
        return false;
    }

    bool Solver::UCS()
    {
        // Create the root vertex of the graph
        this->CreateInitialState();

        bheap::PriorityQueue<
            graph::Vertex<uint16_t, uint16_t, Vector<State>>*,
            decltype(graph::compare::Vertex<uint16_t, uint16_t, Vector<State>>)>
            minPQueue;

        // Enqueue the root vertex
        minPQueue.Enqueue(&this->m_graph.GetVertex(0));

        // Auxiliar variables to make code most legible
        graph::Vertex<uint16_t, uint16_t, Vector<State>>* u = nullptr;
        graph::Vertex<uint16_t, uint16_t, Vector<State>>* v = nullptr;

        graph::Edge<uint16_t, uint16_t, Vector<State>>* uv;

        while (not minPQueue.IsEmpty())
        {
            u = minPQueue.Dequeue();

            u->SetLabel(graph::VertexLabel::VISITED);

            this->ExpandNode(*u);

            // Pair<first, second> = <ID, Edge>
            for (auto& pair : u->GetAdjacencyList())
            {
                // Edge uv (or vu, if is non-directed uv = vu)
                uv = pair.GetSecond();

                // Get the pointer do neighbor vertex, since one end of the edge is
                // vertex u, and the other end is vertex v
                uv->GetVertices().GetFirst()->GetID() == u->GetID()
                    ? v = &this->m_graph.GetVertex(
                          uv->GetVertices().GetSecond()->GetID())
                    : v = &this->m_graph.GetVertex(
                          uv->GetVertices().GetFirst()->GetID());

                if (this->CheckSolution(*v))
                {
                    this->m_vertexSolutionID = v->GetID();
                    return true;
                }

                if (v->GetLabel() == graph::VertexLabel::UNVISITED and Relax(u, v, uv))
                {
                    minPQueue.Enqueue(v);
                }
            }

            // After expanding a vertex, since we won't visit it again, we remove the
            // node from the graph to save memory
            this->m_graph.RemoveVertex(u->GetID());
        }
        return false;
    }

    bool Solver::AStar()
    {
        // Create the root vertex of the graph
        this->CreateInitialState();

        bheap::PriorityQueue<
            graph::Vertex<uint16_t, uint16_t, Vector<State>>*,
            decltype(graph::compare::Vertex<uint16_t, uint16_t, Vector<State>>)>
            minPQueue;

        // Auxiliar variables to make code most legible
        graph::Vertex<uint16_t, uint16_t, Vector<State>>* u = nullptr;
        graph::Vertex<uint16_t, uint16_t, Vector<State>>* v = nullptr;

        graph::Edge<uint16_t, uint16_t, Vector<State>>* uv;

        u = &this->m_graph.GetVertex(0);

        uint16_t heuristicCost = this->CalculateAStarHeuristic(*u);

        u->SetCurrentCost(heuristicCost);
        u->SetHeuristicCost(heuristicCost);

        minPQueue.Enqueue(u);

        while (not minPQueue.IsEmpty())
        {
            u = minPQueue.Dequeue();

            u->SetLabel(graph::VertexLabel::VISITED);

            this->ExpandNode(*u);

            // Pair<first, second> = <ID, Edge>
            for (auto& pair : u->GetAdjacencyList())
            {
                // Edge uv (or vu, if is non-directed uv = vu)
                uv = pair.GetSecond();

                // Get the pointer do neighbor vertex, since one end of the edge
                // vertex u, and the other end is vertex v
                uv->GetVertices().GetFirst()->GetID() == u->GetID()
                    ? v = &this->m_graph.GetVertex(
                          uv->GetVertices().GetSecond()->GetID())
                    : v = &this->m_graph.GetVertex(
                          uv->GetVertices().GetFirst()->GetID());

                if (this->CheckSolution(*v))
                {
                    this->m_vertexSolutionID = v->GetID();
                    return true;
                }

                if (v->GetLabel() == graph::VertexLabel::UNVISITED)
                {
                    v->SetHeuristicCost(this->CalculateAStarHeuristic(*v));

                    if (Relax(u, v, uv))
                    {
                        minPQueue.Enqueue(v);
                    }
                }
            }

            // After expanding a vertex, since we won't visit it again, we remove the
            // node from the graph to save memory
            this->m_graph.RemoveVertex(u->GetID());
        }
        return false;
    }

    bool Solver::GreedyBFS()
    {
        // Create the root vertex of the graph
        this->CreateInitialState();

        bheap::PriorityQueue<graph::Vertex<uint16_t, uint16_t, Vector<State>>*,
                             decltype(graph::compare::VertexHeuristic<uint16_t,
                                                                      uint16_t,
                                                                      Vector<State>>)>
            minPQueue;

        // Auxiliar variables to make code most legible
        graph::Vertex<uint16_t, uint16_t, Vector<State>>* u = nullptr;
        graph::Vertex<uint16_t, uint16_t, Vector<State>>* v = nullptr;

        graph::Edge<uint16_t, uint16_t, Vector<State>>* uv;

        u = &this->m_graph.GetVertex(0);

        uint16_t heuristicCost = this->CalculateGreedyBFSHeuristic(*u);
        u->SetHeuristicCost(heuristicCost);

        minPQueue.Enqueue(u);

        while (not minPQueue.IsEmpty())
        {
            u = minPQueue.Dequeue();

            u->SetLabel(graph::VertexLabel::VISITED);

            this->ExpandNode(*u);

            // Pair<first, second> = <ID, Edge>
            for (auto& pair : u->GetAdjacencyList())
            {
                // Edge uv (or vu, if is non-directed uv = vu)
                uv = pair.GetSecond();

                // Get the pointer do neighbor vertex, since one end of the edge
                // vertex u, and the other end is vertex v
                uv->GetVertices().GetFirst()->GetID() == u->GetID()
                    ? v = &this->m_graph.GetVertex(
                          uv->GetVertices().GetSecond()->GetID())
                    : v = &this->m_graph.GetVertex(
                          uv->GetVertices().GetFirst()->GetID());

                if (this->CheckSolution(*v))
                {
                    this->m_vertexSolutionID = v->GetID();
                    return true;
                }

                if (v->GetLabel() == graph::VertexLabel::UNVISITED)
                {
                    v->SetHeuristicCost(this->CalculateGreedyBFSHeuristic(*v));

                    minPQueue.Enqueue(v);
                }
            }

            // After expanding a vertex, since we won't visit it again, we remove the
            // node from the graph to save memory
            this->m_graph.RemoveVertex(u->GetID());
        }

        return false;
    }

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
        if (not grid::GridIsValid(this->m_startGrid))
        {
            std::cout << "Invalid grid t(-_-t)" << std::endl;
            grid::PrintGrid(this->m_startGrid);
            return;
        }

        std::cout << "Solving the following grid:" << std::endl;
        grid::PrintGrid(this->m_startGrid);
        std::cout << std::endl;

        bool solved = false;

        // Get total time in milliseconds
        auto start = std::chrono::high_resolution_clock::now();

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
                    solved = this->UCS();
                    break;

                case Algorithm::A_STAR:
                    solved = this->AStar();
                    break;

                case Algorithm::GBFS:
                    solved = this->GreedyBFS();
                    break;

                default:
                    break;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();

        if (solved)
        {
            std::cout << "Solution found :')\n" << std::endl;

            this->PrintState(this->m_graph.GetVertices().At(this->m_vertexSolutionID));
        }
        else
        {
            std::cout << "No solution found :(\n" << std::endl;
        }

        // Show algorithm used
        this->PrintAlgorithm();

        std::cout << "Total time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                         .count()
                  << " ms" << std::endl;
        std::cout << "Total expanded states: " << this->m_expandedStates << std::endl;
    }
} // namespace sudoku
