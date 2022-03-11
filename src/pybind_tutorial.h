#ifndef PYBIND_TUTORIAL_H
#define PYBIND_TUTORIAL_H

#include <iostream>
#include <string>
#include <vector>
#include <iterator>

/*
* pybind11.h: Main header file of the C++11, python binding generator library
*/
#include <pybind11/pybind11.h>

/*
* numpy.h: Basic NumPy support, vectorize() wrapper
* Buffer protocol
* Arrays
* Structured types
* Vectorizing functions
* for more info
* https://pybind11.readthedocs.io/en/stable/advanced/pycpp/numpy.html
*/
#include <pybind11/numpy.h>

namespace py = pybind11;

/*
* Class SportsPlayer - Declaring a template class which has member as
* player name
* sports type
* player age
* The reason to choose template class is to provide an example
* that how to bind template class in pybind11 module.
*/

template <typename T>
class SportsPlayer
{
    public:
        T m_PlayerName {""};
        T m_SportsType {""};
        int m_PlayerAge {0};
     /**
     * Default constructor
     */
    SportsPlayer() = default;
     /**
     * Parameterized constructor:
     * @param[in] pName  name of player;
     * @param[in] sType  sports type;
     * @param[in] pAge  age oof player
     */
    SportsPlayer(const T& pName, const T& sType, const int& pAge )
    {
        this->m_PlayerName = pName;
        this->m_SportsType = sType;
        this->m_PlayerAge = pAge;
    }
    /*
     * addPlayers() function will take input from python as dictionary type
     * we will fetch the details in c++ and print it to terminal/console
     * @param[in] playerProfile  it will be python dictionary type;
     * @return void
    */
    void addPlayers(py::dict playerProfile);

    /*
    * getPlayersList() function will return list type to python
    * this list consist of dictionary of player information
    * see call_sports_player() function in python_ex.py file.

    * @return list of player information stored in dictionary
    */
    py::list getPlayersList();
};

#endif // PYBIND_TUTORIAL_H