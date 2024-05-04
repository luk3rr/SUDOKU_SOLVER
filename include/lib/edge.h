/*
 * Filename: edge.h
 * Created on: September 17, 2023
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#ifndef EDGE_H_
#define EDGE_H_

#include <cstddef>

#include "pair.h"

#include "line.h"
#include "vertex.h"

namespace graph
{
    /**
     * @brief Represents an edge in a graph in n-dimensional space.
     *
     * This class inherits from the geom::Line class and extends it to represent an edge
     * connecting two vertices in a graph. It stores information about the connected
     * vertices and the cost associated with the edge.
     *
     * @tparam typeE Type of the edge cost (e.g., int, double, etc.).
     * @tparam typeT Type of the coordinates of the points (e.g., int, double, etc.).
     * @tparam typeD Type of the data stored in the vertices (e.g., int, double, vector,
     * etc.). By default, it is a boolean value to use less memory.
     * @tparam nDim The number of dimensions for the line (e.g., 2 for 2D, 3 for 3D). By
     * default, it is 2.
     */
    template<typename typeE,
             typename typeT,
             typename typeD   = bool,
             std::size_t nDim = 2>
    class Edge : public geom::Line<typeT, nDim>
    {
        private:
            // The id of the edge
            std::size_t m_id;

            // The cost of this edge
            typeE m_edgeCost;

        public:
            Edge(std::size_t                        id,
                 Vertex<typeE, typeT, typeD, nDim>* sideA,
                 Vertex<typeE, typeT, typeD, nDim>* sideB);

            Edge(std::size_t                        id,
                 Vertex<typeE, typeT, typeD, nDim>* sideA,
                 Vertex<typeE, typeT, typeD, nDim>* sideB,
                 typeE                              edgeCost);

            ~Edge();

            /**
             * @brief Copy constructor
             */
            Edge(const Edge& other);

            /**
             * @brief Copy assignment operator
             **/
            Edge& operator=(const Edge& other);

            /**
             * @brief Overload operator <
             **/
            bool operator<(const Edge& other);

            /**
             * @brief Set a new cost of the edge
             **/
            void SetCost(typeE newCost);

            /**
             * @return Id of the edge
             **/
            std::size_t GetID();

            /**
             * @return Cost of the edge
             **/
            typeE GetCost();

            /**
             * @return Pair<a*, b*>, where a, b are pointers to vertex
             **/
            Pair<Vertex<typeE, typeT, typeD, nDim>*, Vertex<typeE, typeT, typeD, nDim>*>
            GetVertices();
    };

    template<typename typeE, typename typeT, typename typeD, std::size_t nDim>
    Edge<typeE, typeT, typeD, nDim>::Edge(std::size_t                        id,
                                          Vertex<typeE, typeT, typeD, nDim>* sideA,
                                          Vertex<typeE, typeT, typeD, nDim>* sideB)
        : geom::Line<typeT, nDim>(*sideA, *sideB)
    {
        this->m_id       = id;
        this->m_edgeCost = 0;
    }

    template<typename typeE, typename typeT, typename typeD, std::size_t nDim>
    Edge<typeE, typeT, typeD, nDim>::Edge(std::size_t                        id,
                                          Vertex<typeE, typeT, typeD, nDim>* sideA,
                                          Vertex<typeE, typeT, typeD, nDim>* sideB,
                                          typeE                              edgeCost)
        : geom::Line<typeT, nDim>(*sideA, *sideB)
    {
        this->m_id       = id;
        this->m_edgeCost = edgeCost;
    }

    template<typename typeE, typename typeT, typename typeD, std::size_t nDim>
    Edge<typeE, typeT, typeD, nDim>::Edge(const Edge& other)
        : geom::Line<typeT, nDim>(other)

    {
        this->m_id       = other.m_id;
        this->m_edgeCost = other.m_edgeCost;
    }

    template<typename typeE, typename typeT, typename typeD, std::size_t nDim>
    Edge<typeE, typeT, typeD, nDim>::~Edge()
    { }

    template<typename typeE, typename typeT, typename typeD, std::size_t nDim>
    Edge<typeE, typeT, typeD, nDim>& Edge<typeE, typeT, typeD, nDim>::operator=(
        const Edge<typeE, typeT, typeD, nDim>& other)
    {
        if (this != &other)
        {
            geom::Line<typeT, nDim>::operator=(other);
            this->m_edgeCost = other.m_edgeCost;
        }

        return *this;
    }

    template<typename typeE, typename typeT, typename typeD, std::size_t nDim>
    bool Edge<typeE, typeT, typeD, nDim>::operator<(const Edge& other)
    {
        return this->m_edgeCost < other.m_edgeCost;
    }

    template<typename typeE, typename typeT, typename typeD, std::size_t nDim>
    void Edge<typeE, typeT, typeD, nDim>::SetCost(typeE newCost)
    {
        this->m_edgeCost = newCost;
    }

    template<typename typeE, typename typeT, typename typeD, std::size_t nDim>
    std::size_t Edge<typeE, typeT, typeD, nDim>::GetID()
    {
        return this->m_id;
    }

    template<typename typeE, typename typeT, typename typeD, std::size_t nDim>
    typeE Edge<typeE, typeT, typeD, nDim>::GetCost()
    {
        return this->m_edgeCost;
    }

    template<typename typeE, typename typeT, typename typeD, std::size_t nDim>
    Pair<Vertex<typeE, typeT, typeD, nDim>*, Vertex<typeE, typeT, typeD, nDim>*>
    Edge<typeE, typeT, typeD, nDim>::GetVertices()
    {
        // Make dynamic cast to get correct data from base class
        return Pair<Vertex<typeE, typeT, typeD, nDim>*,
                    Vertex<typeE, typeT, typeD, nDim>*>(
            dynamic_cast<Vertex<typeE, typeT, typeD, nDim>*>(this->GetA()),
            dynamic_cast<Vertex<typeE, typeT, typeD, nDim>*>(this->GetB()));
    }

} // namespace graph

#endif // EDGE_H_
