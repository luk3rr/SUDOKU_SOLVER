/*
 * Filename: heuristics.h
 * Created on: April 11, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#ifndef HEURISTICS_H_
#define HEURISTICS_H_

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <limits>

#include "vertex.h"

namespace heuristics
{
    namespace distance
    {
        /**
         * @brief Heuristic functions to estimate the cost to reach the target node
         */
        enum class Heuristic
        {
            MANHATTAN,
            EUCLIDEAN,
            MINKOWSKI,
            HAMMING,
        };

        /**
         * @brief Euclidean distance between two vertices
         * @param source Pointer to the source vertex
         * @param target Pointer to the target vertex
         */
        template<typename typeG, typename typeT, typename typeD, std::size_t nDim>
        double_t Euclidean(graph::Vertex<typeG, typeT, typeD, nDim>* source,
                           graph::Vertex<typeG, typeT, typeD, nDim>* target)
        {
            double_t sum = 0;

            for (std::size_t i = 0; i < nDim; i++)
            {
                sum += std::pow(
                    std::abs(static_cast<double_t>(source->GetCoordinates().At(i)) -
                             static_cast<double_t>(target->GetCoordinates().At(i))),
                    2);
            }

            return std::sqrt(sum);
        }

        /**
         * @brief Manhattan distance between two vertices
         * @param source Pointer to the source vertex
         * @param target Pointer to the target vertex
         */
        template<typename typeG, typename typeT, typename typeD, std::size_t nDim>
        inline double_t Manhattan(graph::Vertex<typeG, typeT, typeD, nDim>* source,
                                  graph::Vertex<typeG, typeT, typeD, nDim>* target)
        {
            double_t sum = 0;
            for (std::size_t i = 0; i < nDim; i++)
            {
                sum += std::abs(static_cast<double_t>(source->GetCoordinates().At(i)) -
                                static_cast<double_t>(target->GetCoordinates().At(i)));
            }
            return sum;
        }

        /**
         * @brief Minkowski distance between two vertices
         * @param source Pointer to the source vertex
         * @param target Pointer to the target vertex
         * @param p Exponent of the Minkowski distance
         */
        template<typename typeG, typename typeT, typename typeD, std::size_t nDim>
        inline double_t Minkowski(graph::Vertex<typeG, typeT, typeD, nDim>* source,
                                  graph::Vertex<typeG, typeT, typeD, nDim>* target,
                                  double_t                                  p = 3)
        {
            double_t sum = 0;
            for (std::size_t i = 0; i < nDim; i++)
            {
                sum += std::pow(
                    std::abs(static_cast<double_t>(source->GetCoordinates().At(i)) -
                             static_cast<double_t>(target->GetCoordinates().At(i))),
                    p);
            }

            return std::pow(sum, 1 / p);
        }

        /**
         * @brief Hamming distance between two vertices
         * @param source Pointer to the source vertex
         * @param target Pointer to the target vertex
         */
        template<typename typeG, typename typeT, typename typeD, std::size_t nDim>
        inline double_t Hamming(graph::Vertex<typeG, typeT, typeD, nDim>* source,
                                graph::Vertex<typeG, typeT, typeD, nDim>* target)
        {
            double_t sum = 0;

            for (std::size_t i = 0; i < nDim; i++)
            {
                sum += std::abs(static_cast<double_t>(source->GetCoordinates().At(i)) -
                                static_cast<double_t>(target->GetCoordinates().At(i))) >
                       std::numeric_limits<double_t>::epsilon();
            }

            return sum;
        }

    } // namespace distance

} // namespace heuristics

#endif // HEURISTICS_H_
