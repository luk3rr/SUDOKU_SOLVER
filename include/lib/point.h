/*
 * Filename: point.h
 * Created on: June  5, 2023
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#ifndef POINT_H_
#define POINT_H_

#include <cstddef>
#include <ostream>

#include "vector.h"

namespace geom
{
    /**
     * @brief Define a point class with coordinates of type typeT in n-dimensional
     *space.
     *
     * This class represents a point in an n-dimensional space, where n is specified
     * by the template parameter nDim. The type of the point's coordinates is specified
     * by the template parameter typeT.
     *
     * @tparam typeT Type of the point coordinates (e.g., int, double, etc.).
     * @tparam nDim The number of dimensions for this point (e.g., 2 for 2D, 3 for 3D).
     **/
    template<typename typeT, std::size_t nDim>
    class Point
    {
        private:
            Vector<typeT> m_coordinates;

        public:
            /**
             * @brief Default constructor
             */
            Point();

            /**
             * @brief Class constructor overload
             * @param coordinates Point's coordinates
             */
            Point(Vector<typeT> coordinates);

            virtual ~Point() = default;

            /**
             * @brief Operator == overload
             * @param point Point to be used in comparison
             * @return True if the coordinates of the two points are equal element-wise,
             * False otherwise
             */
            bool operator==(const Point<typeT, nDim>& other) const;

            /**
             * @brief Operator << overload
             * @param os Output stream
             * @param point Point to be printed
             */
            friend std::ostream& operator<<(std::ostream&             os,
                                            const Point<typeT, nDim>& point)
            {
                os << "(";
                for (std::size_t i = 0; i < nDim; i++)
                {
                    os << point.m_coordinates[i];

                    if (i < nDim - 1)
                    {
                        os << ", ";
                    }
                }
                os << ")";
                return os;
            }

            /**
             * @brief Set a new set of coordinates for the point
             */
            void SetCoordinates(const Vector<typeT>& coordinates);

            /**
             * @return The set of coordinates for the point
             */
            Vector<typeT>& GetCoordinates();
    };

    template<typename typeT, std::size_t nDim>
    Point<typeT, nDim>::Point()
        : m_coordinates(nDim, 0)
    { }

    template<typename typeT, std::size_t nDim>
    Point<typeT, nDim>::Point(Vector<typeT> coordinates)
        : m_coordinates(nDim)
    {
        this->m_coordinates = coordinates;
    }

    template<typename typeT, std::size_t nDim>
    bool Point<typeT, nDim>::operator==(const Point& other) const
    {
        if (other.m_coordinates.Size() == this->m_coordinates.Size())
        {
            for (std::size_t i = 0; i < nDim; i++)
            {
                if (this->m_coordinates[i] != other.m_coordinates[i])
                {
                    return false;
                }
            }

            return true;
        }

        return false;
    }

    template<typename typeT, std::size_t nDim>
    void Point<typeT, nDim>::SetCoordinates(const Vector<typeT>& coordinates)
    {
        this->m_coordinates = coordinates;
    }

    template<typename typeT, std::size_t nDim>
    Vector<typeT>& Point<typeT, nDim>::GetCoordinates()
    {
        return this->m_coordinates;
    }
} // namespace geom

#endif // POINT_H_
