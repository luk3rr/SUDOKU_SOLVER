/*
 * Filename: line.h
 * Created on: June  5, 2023
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#ifndef LINE_H_
#define LINE_H_

#include <cstddef>

#include "point.h"

namespace geom
{
    /**
     * @brief Define a Line class in n-dimensional space.
     *
     * This class represents a line segment in an n-dimensional space, where n is
     * specified by the template parameter nDim. A line is defined by two points,
     * which are instances of the Point class.
     *
     * @tparam typeT Type of the coordinates of the points (e.g., int, double, etc.).
     * @tparam nDim The number of dimensions for the line (e.g., 2 for 2D, 3 for 3D).
     **/
    template<typename typeT, std::size_t nDim>
    class Line
    {
        private:
            Point<typeT, nDim>* m_a;
            Point<typeT, nDim>* m_b;

        public:
            /**
             * @brief Default constructor of the class
             */
            Line();

            /**
             * @brief Constructor overload of the class
             * @param a, b The two points that define the line
             */
            Line(Point<typeT, nDim>& a, Point<typeT, nDim>& b);

            virtual ~Line() = default;

            /**
             * @brief Operator == overload
             * @param other Line segment to be used in comparison
             * @return True if the line segments are the same, False otherwise
             */
            bool operator==(const Line& other) const;

            /**
             * @brief Set a value for point A
             * @param a New value for the point
             */
            void SetA(Point<typeT, nDim>& a);

            /**
             * @brief Set a value for point B
             * @param b New value for the point
             */
            void SetB(Point<typeT, nDim>& b);

            /**
             * @brief Get the value of point A
             * @return Point A
             */
            Point<typeT, nDim>* GetA();

            /**
             * @brief Get the value of point B
             * @return Point B
             */
            Point<typeT, nDim>* GetB();
    };

    template<typename typeT, std::size_t nDim>
    Line<typeT, nDim>::Line()
    {
    }

    template<typename typeT, std::size_t nDim>
    Line<typeT, nDim>::Line(Point<typeT, nDim>& a, Point<typeT, nDim>& b)
    {
        this->m_a = &a;
        this->m_b = &b;
    }

    template<typename typeT, std::size_t nDim>
    bool Line<typeT, nDim>::operator==(const Line& other) const
    {
        return (*this->m_a == *other.m_a and *this->m_b == *other.m_b);
    }

    template<typename typeT, std::size_t nDim>
    void Line<typeT, nDim>::SetA(Point<typeT, nDim>& a)
    {
        this->m_a = &a;
    }

    template<typename typeT, std::size_t nDim>
    void Line<typeT, nDim>::SetB(Point<typeT, nDim>& b)
    {
        this->m_b = &b;
    }

    template<typename typeT, std::size_t nDim>
    Point<typeT, nDim>* Line<typeT, nDim>::GetA()
    {
        return this->m_a;
    }

    template<typename typeT, std::size_t nDim>
    Point<typeT, nDim>* Line<typeT, nDim>::GetB()
    {
        return this->m_b;
    }
} // namespace geom

#endif // LINE_H_
