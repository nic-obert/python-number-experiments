
// This definition is needed for future-proofing your code
// see https://docs.python.org/3/c-api/arg.html#:~:text=Note%20For%20all,always%20define%20PY_SSIZE_T_CLEAN.
#define PY_SSIZE_T_CLEAN
// The actual Python API
#include <Python.h>
#include <stddef.h>


#define MAX_LONG_DIGITS \
    ((PY_SSIZE_T_MAX - offsetof(PyLongObject, ob_digit))/sizeof(digit))


PyObject* max_digit_count(PyObject* self, PyObject* args) {
    
    if (!PyArg_ParseTuple(args, "")) {
        PyErr_SetString(PyExc_TypeError, "Expected no arguments");
        return NULL;
    }

    PyLongObject* result = PyLong_FromSsize_t((Py_ssize_t)MAX_LONG_DIGITS);

    return result;
}


PyObject* digit_size(PyObject* self, PyObject* args) {
    
    if (!PyArg_ParseTuple(args, "")) {
        PyErr_SetString(PyExc_TypeError, "Expected no arguments");
        return NULL;
    }

    PyLongObject* result = PyLong_FromSsize_t((Py_ssize_t)PyLong_SHIFT);

    return result;
}


static void print_digit_binary(digit number)
{
    if (number >> 1) {
        print_digit_binary(number >> 1);
    }
    putc((number & 1) ? '1' : '0', stdout);
}


PyObject* print_number_info(PyObject* self, PyObject* args) {
    
    PyObject* arg;

    if (!PyArg_ParseTuple(args, "O", &arg)) {
        PyErr_SetString(PyExc_TypeError, "Expected single argument");
        return NULL;
    }

    if (!PyLong_CheckExact(arg)) {
        PyErr_SetString(PyExc_TypeError, "Expected an integer");
        return NULL;
    }

    PyLongObject* py_long = (PyLongObject*) arg;

    int py_size = Py_SIZE(py_long);
    switch (py_size) {
        case 0:
            printf("Zero\n");
            break;
        case 1:
            printf("Abs: %d\nOne digit\nPositive\n", (digit)(py_long->ob_digit[0] & PyLong_MASK));
            break;
        case -1:
            printf("Abs: %d\nOne digit\nNegative\n", (digit)(py_long->ob_digit[0] & PyLong_MASK));
            break;
        default:
            printf("Number: ");
            PyObject_Print(py_long, stdout, 0);
            printf("\n");

            printf("Number of digits: %d\nSign: %s\n", py_size, py_size > 0 ? "Positive" : "Negative");

            printf("30-bit decimal digits (little endian): ");
            for (int i = 0; i < py_size; i++) {
                assert(py_long->ob_digit[i] < PyLong_BASE);
                printf("%d ", (digit)(py_long->ob_digit[i] & PyLong_MASK));
            }
            printf("\n");

            printf("30-bit binary digits (little endian): ");
            for (int i = 0; i < py_size; i++) {
                assert(py_long->ob_digit[i] < PyLong_BASE);
                print_digit_binary((digit)(py_long->ob_digit[i] & PyLong_MASK));
                printf(" ");
            }
            printf("\n");

            break;
    }    

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


