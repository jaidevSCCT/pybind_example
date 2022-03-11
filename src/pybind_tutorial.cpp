#include "pybind_tutorial.h"
using namespace std;

/* Declare a global vector of SportsPlayer type.
* In ths vector example, we will add information provided from python.
*/
std::vector<SportsPlayer<std::string>> PlayersList;


/*
* This API has been implemented, based on the data used in python.
* If user wants to play with data in python, then this API should be modify accordingly.

* User should send the below format only from python or else program will throw exception
* {"player name": "R Jadeja", "sports type": "Cricket", "player age": 33}
*/
template <typename T>
void SportsPlayer<T>::addPlayers(py::dict playerProfile)
{
    /*
    * playerProfile.contains -> Check if the given item is contained within this object, i.e. item in obj.
    * In this example it is used to check whether the dict key is present or not
    * it will return false if not present.

    * .cast<>() or py::cast() -> In this kind of mixed code,
    * it is often necessary to convert arbitrary C++ types to Python and vice versa.
    * When conversion fails, both directions throw the exception cast_error.
    */
    if(playerProfile.contains(py::str("player name")) &&
    playerProfile.contains(py::str("sports type")) &&
    playerProfile.contains(py::str("player age")))
    {
        // fetch dictionary data from python to c++
        std::string l_pName = playerProfile["player name"].cast<py::str>();
        std::string l_sType = playerProfile["sports type"].cast<py::str>();
        int l_pAge = playerProfile["player age"].cast<int>();

        // create temp object of SportsPlayer from the data coming from python
        SportsPlayer<std::string> l_temp{l_pName,l_sType,l_pAge};

        /*Adding player information to global object
        * later we will retrieve and send back to python */
        PlayersList.push_back(l_temp);
        std::cout << "Player " + l_pName + " added successfully" << std::endl;
    }
    else // throw error if format is wrong or keys not present
        throw std::runtime_error("Input is different !");
}

/* The list basically return player information as dictionary
   * see call_sports_player() function in python_ex.py file */
template <typename T>
py::list SportsPlayer<T>::getPlayersList()
{
    py::list l_temp;
    // iterate over global object PlayersList
    for (auto it = std::begin (PlayersList); it != std::end (PlayersList); ++it)
    {
        /*Taking temporary pybind dict type to be updated into the list*/
        py::dict l_playerDetails;
        std::cout << "FETCHING " + it->m_PlayerName + " DETAILS" << std::endl;
        /* add player details keys to dictionary */
        l_playerDetails["player name"] = it->m_PlayerName;
        l_playerDetails["sports type"] = it->m_SportsType;
        l_playerDetails["player age"] = it->m_PlayerAge;
        /* add dictionary to list and send back to python */
        l_temp.append(l_playerDetails);
    }
    std::cout << "SIZE OF PLAYERLIST: " << l_temp.size() << std::endl;
    return l_temp;
}

/*
* The motivation to write this function is to provide example of different
* pybind11 datatypes.

* Function multipleArgsFunction - It will take args as 2d numpy array
* , user defined python class as object
* ,and tuple data
* Return -
* We will convert inputs from python to c++ and
* vice versa conversion will send back to python using tuple.
********************************************************************************************************

* reference - https://pybind11.readthedocs.io/en/stable/advanced/pycpp/numpy.html#arrays

* py::array_t ->
* By using py::array,we can restrict the function so that it only accepts NumPy arrays
* (rather than any type of Python object satisfying the buffer protocol).
* In many situations, we want to define a function which only accepts a NumPy array of a certain data type.
* This is possible via the py::array_t<T> template.
* For instance, the following below function requires the argument to be a NumPy 2D array
* containing unsigned char values.
********************************************************************************************************
* py::array::c_style -> It can be useful to require a function to only
* accept arrays using the C (row-major) , it is used to tell the pybind library
* to store multidimensional arrays in linear storage.
********************************************************************************************************
* py::array::forcecast -> Force a cast to the output type even if it cannot be done safely.
* Without this flag, a data cast will occur only if it can be done safely, otherwise an error is raised.
* The py::array::forcecast argument is the default value of the second template parameter.
********************************************************************************************************

* py::object, py::tuple -> pybind11's C++ object wrappers around Python types.
********************************************************************************************************

* This function is only called in one place (the PYBIND11_MODULE block),
* we didn’t create a header for the declaration of multipleArgsFunction.
* That means we need to make sure the names and signatures exactly match,
* because errors will only be caught by the linker (not the compiler),
* and linker error messages can be quite cryptic.
*/
template <typename T>
py::tuple multipleArgsFunction(
    py::array_t<T, py::array::c_style | py::array::forcecast>& numpyData,
    py::object pythonObj,
    py::tuple tupleData)
{
    // Convert numpy array to unsigned char data
    /*
    * .template keyword is compiler specific. Some will complain some not.
    * mutable_unchecked  -> provides direct access to array elements, 2 is dimensionality of the array(2d)
    */
    auto arrData = numpyData.template mutable_unchecked<2>();

    // python owned object pointer , shared pointer wrapper
    unsigned char* arrPtr = &(arrData(0,0));
    int arrLength = arrData.shape(0);
    int arrWidth= arrData.shape(1);
    std::cout << "PRINTING THE NUMPY DATA " << std::endl;
    int totalLen = arrLength * arrWidth;
    for(auto len = 0; len < totalLen; len++)
            std::cout<< (int)arrPtr[len]<< " , ";
    std::cout << std::endl;

    // Fetch python object and print
    std::cout << "PRINTING THE OBJECT DATA " << std::endl;
    // To call an object’s method/member, one can again use .attr to obtain access to the Python method/member.
    std::string playerName = pythonObj.attr("playerName").cast<py::str>();
    std::string sportsType = pythonObj.attr("sportsType").cast<py::str>();
    int playerAge = pythonObj.attr("playerAge").cast<int>();
    std::cout << "playerName: " << playerName << std::endl;
    std::cout << "sportsType: " << sportsType << std::endl;
    std::cout << "playerAge: " << playerAge << std::endl;

    // Fetch tuple details
    std::cout << "PRINTING THE TUPLE DATA " << std::endl;
    std::string vegName;
    if(tupleData.size() > 0)
    {
        // python tuple can be accessed using indexes
        vegName = tupleData[0].cast<py::str>();
        std::string fruitName = tupleData[1].cast<py::str>();
        float price = tupleData[2].cast<float>();

        std::cout << "Vegetable name: " << vegName << std::endl;
        std::cout << "Fruit name: " << fruitName << std::endl;
        std::cout << "Total price: " <<  price << std::endl;
    }

    // sending back same data
    // Python object that will free the allocated memory when destroyed:
    py::capsule freeMemoryAfterUse(arrPtr, [](void *arr) {
        unsigned char *arrPtr = reinterpret_cast<unsigned char *>(arr);
        std::cerr << "Element [1] = " << (int)arrPtr[1] << "\n";
        std::cerr << "freeing memory @ " << arr << "\n";
        delete[] arrPtr;
    });

    py::array_t<T, py::array::c_style | py::array::forcecast> outputArr(
    {arrLength, arrWidth}, // shape
    {arrWidth, 1}, // C-style contiguous strides
    arrPtr, // the data pointer
    freeMemoryAfterUse);

    return py::make_tuple(outputArr, vegName, playerName);
}


/*
* Modules should have exactly one source file with a PYBIND11_MODULE block,
* and usually that block should delegate the real work to functions defined in other source files
* (generally one for each C++ header to be wrapped).
*/

/*
* PYBIND11_MODULE - This macro creates the entry point that will be invoked when the Python interpreter
* imports an extension module.
* The module name is given as the fist argument and it should not be in quotes.
* The second macro argument defines a variable of type py::module_ which can be used to initialize the module.

* pybind_tutorial - the name of the module ,
* WARNING - The name used for the PYBIND11_MODULE(..., mod) macro must match the name of the file,
* otherwise an ImportError will be raised.

* m - pybind11 handle

* py::class_ - class_ creates bindings for a C++ class or struct-style data structure.
* init() is a convenience function that takes the types of a constructor’s parameters as
* template arguments and wraps the corresponding constructor

* m.def - The method module_::def() generates binding code that exposes the multipleArgsFunction() function to Python.
* class_.def - The method class_::def() generates binding code that exposes the addPlayers() function to Python.
* other class methods are class_::def_readwrite(), class_::def_readonly() and many more

* py::return_value_policy::copy - Create a new copy of the returned object, which will be owned by Python.
* This policy is comparably safe because the lifetimes of the two instances are decoupled.
*/
PYBIND11_MODULE(pybind_tutorial, m)
{
    const char* docStr = "Example of pybind11";
    // Get or set the object’s docstring
    m.doc() = docStr;

    // In C++11, the using keyword when used for type alias is identical to typedef.
    using PlayerClass = SportsPlayer<std::string>;

    py::class_<PlayerClass>(m, "SportsPlayer")
        .def(py::init<const std::string&, const std::string&, const int&>())
        .def("addPlayers", &PlayerClass::addPlayers)
        .def("getPlayersList", &PlayerClass::getPlayersList,py::return_value_policy::copy);

    m.def("multipleArgsFunction", &multipleArgsFunction<unsigned char>, "Example to take multiple args");
}