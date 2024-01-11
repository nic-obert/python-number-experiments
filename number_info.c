// This definition is needed for future-proofing your code
// see https://docs.python.org/3/c-api/arg.html#:~:text=Note%20For%20all,always%20define%20PY_SSIZE_T_CLEAN.
#define PY_SSIZE_T_CLEAN
#include <Python.h> /* Python API */
#include <stddef.h> /* offsetof() */

// Maximum number of digits a PyLongObject can have
#define MAX_LONG_DIGITS \
    ((PY_SSIZE_T_MAX - offsetof(PyLongObject, ob_digit))/sizeof(digit))


// Return MAX_LONG_DIGITS
PyObject* max_digit_count(PyObject* self, PyObject* args) {
    
    // Check that no arguments were passed, else raise an exception
    if (!PyArg_ParseTuple(args, "")) {
        PyErr_SetString(PyExc_TypeError, "Expected no arguments");
        return NULL;
    }

    // Create a PyLongObject from MAX_LONG_DIGITS (which is a long int)
    PyLongObject* result = PyLong_FromSsize_t((Py_ssize_t)MAX_LONG_DIGITS);

    return result;
}


// Return the size of a digit on the current platform
PyObject* digit_size(PyObject* self, PyObject* args) {
    
    // Check that no arguments were passed, else raise an exception
    if (!PyArg_ParseTuple(args, "")) {
        PyErr_SetString(PyExc_TypeError, "Expected no arguments");
        return NULL;
    }

    // Create a PyLongObject from PyLong_SHIFT (the bits to shift when parsing numbers)
    PyLongObject* result = PyLong_FromSsize_t((Py_ssize_t)PyLong_SHIFT);

    return result;
}


// Utility function to print a digit in binary to stdout
static void print_digit_binary(digit number)
{
    if (number >> 1) {
        print_digit_binary(number >> 1);
    }
    putc((number & 1) ? '1' : '0', stdout);
}


// Print information about a number object
PyObject* print_number_info(PyObject* self, PyObject* args) {
    
    // Pre-declare the arg variable
    PyObject* arg;

    // Parse and check the arguments
    // This function takes a single Python object ('O')
    if (!PyArg_ParseTuple(args, "O", &arg)) {
        PyErr_SetString(PyExc_TypeError, "Expected single argument");
        return NULL;
    }

    // Check if the argument is a PyLongObject
    if (!PyLong_CheckExact(arg)) {
        PyErr_SetString(PyExc_TypeError, "Expected an integer");
        return NULL;
    }
    // If it's a PyLongObject, then cast it to a PyLongObject
    PyLongObject* py_long = (PyLongObject*) arg;

    // Get the size of the number (in digits)
    int py_size = Py_SIZE(py_long);
    switch (py_size) {
        // Size 0 => number is 0 (even though it has one digit)
        case 0:
            printf("Zero\n");
            break;
        // Size 1 => number has one digit and is positive
        case 1:
            // Print the first element of the digit array
            // take only the first 30 bits with & PyLong_MASK
            printf("Abs: %d\nOne digit\nPositive\n", (digit)(py_long->ob_digit[0] & PyLong_MASK));
            break;
        // Size -1 => number has one digit and is negative
        case -1:
            // Print the first element of the digit array
            // take only the first 30 bits with & PyLong_MASK
            printf("Abs: %d\nOne digit\nNegative\n", (digit)(py_long->ob_digit[0] & PyLong_MASK));
            break;
        // All other sizes
        default:
            // Print the number object to stdout
            printf("Number: ");
            PyObject_Print(py_long, stdout, 0);
            printf("\n");

            // py_size > 0 => positive, py_size < 0 => negative, py_size == 0 => zero
            printf("Number of digits: %d\nSign: %s\n", py_size, py_size > 0 ? "Positive" : "Negative");

            printf("30-bit decimal digits (little endian): ");
            // Iterate over the digits
            for (int i = 0; i < py_size; i++) {
                assert(py_long->ob_digit[i] < PyLong_BASE);
                // Print the ith digit in decimal (only first 30 bits by &-ing with PyLong_MASK)
                printf("%d ", (digit)(py_long->ob_digit[i] & PyLong_MASK));
            }
            printf("\n");

            printf("30-bit binary digits (little endian): ");
            // Iterate over the digits
            for (int i = 0; i < py_size; i++) {
                assert(py_long->ob_digit[i] < PyLong_BASE);
                // Print the ith digit in binary
                print_digit_binary((digit)(py_long->ob_digit[i] & PyLong_MASK));
                printf(" ");
            }
            printf("\n");

            break;
    }    

    // Return None (nothing)
    Py_RETURN_NONE;
}


// array containing the module's methods' definitions
// put here the methods to export
// the array must end with a {NULL} struct
PyMethodDef module_methods[] = 
{
    {"print_number_info", print_number_info, METH_VARARGS, "Print information about a number object"},
    {"max_digit_count", max_digit_count, METH_VARARGS, "Return the maximum number of digits a number object can have"},
    {"digit_size", digit_size, METH_VARARGS, "Return the size of a digit in bits"},
    {NULL} // this struct signals the end of the array
};

// struct representing the module
struct PyModuleDef module =
{
    PyModuleDef_HEAD_INIT, // Always initialize this member to PyModuleDef_HEAD_INIT
    "number_info", // module name
    "Print information about a number object", // module description
    -1, // module size (more on this later)
    module_methods // methods associated with the module
};

// function that initializes the module
PyMODINIT_FUNC PyInit_number_info()
{
    return PyModule_Create(&module);
}


