
#pragma once
#include <Python.h>
#include <cp_tools/py_utility.h>
#include <numpy/arrayobject.h>
#include <numpy/ndarraytypes.h>
#include <vector>

namespace cp_tools {
class NumpyArrWrapper {
public:
  PyArrayObject *arr = nullptr;
};
template <class T> class NumpyArray1Dim : public NumpyArrWrapper {
public:
  NumpyArray1Dim(PyArrayObject *arr) { this->arr = arr; }

  T &at(long i) { return *(T *)(arr->data + arr->strides[0] * i); }
  long size() { return arr->dimensions[0]; }
};
class NumpyArrayObjD : public NumpyArray1Dim<double> {

public:
  NumpyArrayObjD(int dim) : NumpyArray1Dim(nullptr) {
    arr = new_numpy_array_1D(dim, NPY_TYPES::NPY_DOUBLE);
  }
  NumpyArrayObjD(const std::vector<double> &vec) : NumpyArrayObjD(vec.size()) {
    for (unsigned int i = 0; i < vec.size(); ++i) {
      at(i) = vec.at(i);
    }
  }
  PyArrayObject *detach() {
    assert(arr != nullptr);
    PyArrayObject *arr_tmp = arr;
    arr = nullptr;
    return arr_tmp;
  }
  ~NumpyArrayObjD() { Py_XDECREF(arr); }
};
class NumpyArrayViewD : public NumpyArray1Dim<double> {
public:
  NumpyArrayViewD(PyArrayObject *arr) : NumpyArray1Dim<double>(arr) {
    if (arr->descr->type != 'd') {
      throw std::logic_error("Not array of type double");
    }
    if (arr->nd != 1) {
      throw std::logic_error(
          stringify("Not one dimmension but ", arr->nd, " instead."));
    }
  }
};
}
