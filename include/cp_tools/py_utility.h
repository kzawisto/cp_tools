/*
 * py_utility.h
 *
 *  Created on: Aug 17, 2018
 *      Author: krystian
 */

#pragma once


#include<Python.h>
#include "numpy/arrayobject.h"
#include<stdexcept>
#include<string>


namespace cp_tools {


inline int get_size(NPY_TYPES type) {
	switch(type){
		case NPY_TYPES::NPY_DOUBLE: return 8; break;
		case NPY_TYPES::NPY_INT: return 4; break;
		case NPY_TYPES::NPY_FLOAT: return 4; break;
	default:
		throw std::logic_error("Type not suported by get_size");
	}
}

inline
PyArrayObject * new_numpy_array_1D(long int dim, NPY_TYPES type = NPY_TYPES::NPY_DOUBLE) {
	  npy_intp dims[] = {dim};
	  void * buffer = malloc(dim * get_size(type));
	  PyObject *narray = PyArray_SimpleNewFromData(1, dims, type, buffer);
	  PyArray_ENABLEFLAGS((PyArrayObject*)narray, NPY_ARRAY_OWNDATA);
	  return (PyArrayObject*)PyArray_Return((PyArrayObject*) narray);
}

inline
PyObject * unicode_pystr(std::string s){
	return (PyObject*)PyUnicode_FromString(s.c_str());
}

}

