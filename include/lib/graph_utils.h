/*
 * Filename: graph_utils.h
 * Created on: November  2, 2023
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#ifndef GRAPH_UTILS_H_
#define GRAPH_UTILS_H_

#include <cstddef>
#include <cstdint>

#include "edge.h"
#include "graph.h"
#include "heuristics.h"
#include "vector.h"

/**
 * @brief A utility namespace providing various auxiliary functions for graph
 * operations.
 *
 * @tparam typeG The type for the cost of the graph's edges
 * @tparam typeT The data type to store vertex and edge attributes
 * @tparam nDim The dimensionality of the vertices
 */
namespace graph
{
    enum VertexLabel : uint32_t
    {
        UNVISITED,
        PROCESSING,
        VISITED
    };

    /**
     * @brief A utility namespace providing various auxiliary functions for
     * comparison operations
     */
    namespace compare
    {
        /**
         * @brief A function object to compare two vertices based on their
         * current cost
         */
        template<typename typeG, typename typeT, typename typeD, std::size_t nDim = 2>
        auto Vertex = [](const graph::Vertex<typeG, typeT, typeD, nDim>* v1,
                         const graph::Vertex<typeG, typeT, typeD, nDim>* v2) -> bool {
            return v1->GetCurrentCost() <= v2->GetCurrentCost();
        };

        /**
         * @brief A function object to compare two vertices based on only their
         * heuristic cost
         */
        template<typename typeG, typename typeT, typename typeD, std::size_t nDim = 2>
        auto VertexHeuristic =
            [](const graph::Vertex<typeG, typeT, typeD, nDim>* v1,
               const graph::Vertex<typeG, typeT, typeD, nDim>* v2) -> bool {
            return v1->GetHeuristicCost() <= v2->GetHeuristicCost();
        };

        /**
         * @brief A function object to compare two edges based on their cost
         */
        template<typename typeG, typename typeT, typename typeD, std::size_t nDim = 2>
        auto Edge = [](const graph::Edge<typeG, typeT, typeD, nDim>* e1,
                       const graph::Edge<typeG, typeT, typeD, nDim>* e2) -> bool {
            return e1->GetCost() <= e2->GetCost();
        };
    } // namespace compare

    /**
     * @brief Relax the edge (u, v)
     * @param u, v Pointers to vertices of this edge
     * @param uv Pointer to the edge (u, v)
     * @return True if edge was relaxed, False otherwise
     **/
    template<typename typeG, typename typeT, typename typeD, std::size_t nDim = 2>
    inline bool Relax(Vertex<typeG, typeT, typeD, nDim>* u,
                      Vertex<typeG, typeT, typeD, nDim>* v,
                      Edge<typeG, typeT, typeD, nDim>*   uv)
    {
        // Heuristic by default is 0
        if (v->GetCurrentCost() >
            ((u->GetCurrentCost() - u->GetHeuristicCost()) + uv->GetCost()))
        {
            v->SetCurrentCost((u->GetCurrentCost() - u->GetHeuristicCost()) +
                              uv->GetCost() + v->GetHeuristicCost());

            v->SetEdge2Predecessor(uv);
            return true;
        }

        return false;
    }

    namespace
    {
        /**
         * @brief Calculate the heuristic cost between two vertices
         * @param heuristic The heuristic function to be used
         * @param u Pointer to the first vertex
         * @param v Pointer to the second vertex
         * @return The heuristic cost between the two vertices
         */
        template<typename typeG, typename typeT, typename typeD, std::size_t nDim>
        inline double_t CalculateHeuristic(heuristics::distance::Heuristic    heuristic,
                                           Vertex<typeG, typeT, typeD, nDim>* u,
                                           Vertex<typeG, typeT, typeD, nDim>* v)
        {
            switch (heuristic)
            {
                case heuristics::distance::Heuristic::MANHATTAN:
                    return heuristics::distance::Manhattan(u, v);

                case heuristics::distance::Heuristic::EUCLIDEAN:
                    return heuristics::distance::Euclidean(u, v);

                case heuristics::distance::Heuristic::MINKOWSKI:
                    return heuristics::distance::Minkowski(u, v);

                case heuristics::distance::Heuristic::HAMMING:
                    return heuristics::distance::Hamming(u, v);

                default:
                    std::cerr << "Invalid heuristic function. Using Euclidean distance "
                                 "as default."
                              << std::endl;
                    return heuristics::distance::Euclidean(u, v);
            }
        }
    } // namespace

    /**
     * @brief Print the path from a vertex v to its most distant ancestor
     * @param graph The graph containing the vertices and edges
     * @param v Pointer to the vertex to be printed
     */
    template<typename typeG,
             typename typeT,
             typename typeD,
             std::size_t nDim,
             bool        directed>
    inline void PrintPath(Graph<typeG, typeT, typeD, nDim, directed>& graph,
                          Vertex<typeG, typeT, typeD, nDim>*          v)
    {
        // Create a vector to store the path
        Vector<std::size_t> path;
        Vector<typeG>       costs;

        Edge<typeG, typeT, typeD, nDim>* uv;

        path.PushBack(v->GetID());
        costs.PushBack(0);

        // Get the path
        while ((uv = v->GetEdge2Predecessor()))
        {
            uv->GetVertices().GetFirst()->GetID() == v->GetID()
                ? v = &graph.GetVertices()[uv->GetVertices().GetSecond()->GetID()]
                : v = &graph.GetVertices()[uv->GetVertices().GetFirst()->GetID()];

            path.PushBack(v->GetID());
            costs.PushBack(uv->GetCost());
        }

        // Print the path
        std::cout << "Path: ";
        for (int32_t i = path.Size() - 1; i >= 0; i--)
        {
            if (i == 0)
                std::cout << path[i] << std::endl;
            else
                std::cout << path[i] << " --:" << costs[i] << ":-> ";
        }
    }

    /**
     * @brief Get the adjacent vertex of a given vertex u through the edge uv
     * @param graph The graph containing the vertices and edges
     * @param u Pointer to the vertex u
     * @param uv Pointer to the edge uv
     * @return Pointer to the adjacent vertex of u through the edge uv
     **/
    template<typename typeG,
             typename typeT,
             typename typeD,
             std::size_t nDim,
             bool        directed>
    inline Vertex<typeG, typeT, typeD, nDim>*
    GetAdjacentVertex(Graph<typeG, typeT, typeD, nDim, directed>& graph,
                      Vertex<typeG, typeT, typeD, nDim>*          u,
                      Edge<typeG, typeT, typeD, nDim>*            uv)
    {
        // Get the pointer do neighbor vertex, since one end of the edge is
        // vertex u, and the other end is vertex v
        return uv->GetVertices().GetFirst()->GetID() == u->GetID()
                   ? &graph.GetVertex(uv->GetVertices().GetSecond()->GetID())
                   : &graph.GetVertex(uv->GetVertices().GetFirst()->GetID());
    }

} // namespace graph

#endif // GRAPH_UTILS_H_
