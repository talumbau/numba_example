#from cffi import FFI
from ctypes import cdll, Structure, c_int, c_double, c_uint, c_ubyte, POINTER, c_char_p, pointer, byref
import ctypes
from numba import cfunc, types, carray, jit
import numpy as np
import math

c_sig = types.void(types.CPointer(types.double),
                   types.CPointer(types.double),
                   types.intc, types.intc)

@cfunc(c_sig)
def my_callback(in_, out, m, n):
    in_array = carray(in_, (m, n))
    out_array = carray(out, (m, n))
    for i in range(m):
        for j in range(n):
            out_array[i, j] = 2 * in_array[i, j]

c_sig2 = types.void(types.CPointer(types.uchar),
                   types.CPointer(types.uchar),
                   types.intc, types.intc)


@jit
def create_gaussian_filter(kernel):
    # set standard deviation to 1.0
    sigma = 1.0
    r = 0.0
    s = 2.0 * sigma * sigma;
    x=y=i=j=0;
    # sum is for normalization
    sum_ = 0.0;
    # generate 5x5 kernel
    for x in range(-2,3):
        for y in range(-2,3):
            r = math.sqrt(x*x + y*y);
            kernel[x + 2][y + 2] = (math.exp(-(r*r)/s))/(math.pi * s);
            sum_ += kernel[x + 2][y + 2];

    # normalize the Kernel
    for i in range(5):
        for j in range(5):
            kernel[i][j] /= sum_;
 


gKernel = np.zeros((5,5))
create_gaussian_filter(gKernel)

@cfunc(c_sig2)
def callback_for_gaussian_2(in_, out, y, x):
    #in_array = carray(in_, (12, 9, 3), dtype=np.uint8)
    in_array = carray(in_, (1364, 1364, 3))
    out_array = carray(out, (1364, 1364, 3))
    #print(b"i'm here")
    for k in range(3):
        sum_ = 0.
        for p in range(-2,3):
            for q in range(-2,3):
                sum_ += gKernel[p+2,q+2] * in_array[y+p,x+q,k]
        #out_array[x, y, k] = sum_
        out_array[y, x, k] = sum_


c_sig3 = types.int64(types.int64, types.int64)

@cfunc(c_sig3)
def callback3(in_, out):
    return in_ + out



cdll.LoadLibrary("libifilter.dylib")
ifilter = cdll.LoadLibrary("libifilter.dylib")
tbc = ifilter.take_basic_callback
tbc.restype = c_double
d1 = c_double(9)
d2 = c_double(12)
cf = ctypes.CFUNCTYPE(c_int, c_int, c_int)
#tbc.argtypes = [c_int, cf]
out = tbc(d1, d2, callback3.ctypes)

int_p = POINTER(c_int)
i1 = c_int()
i2 = c_int()
i3 = c_int()
ip1 = pointer(i1)
ip2 = pointer(i2)
ip3 = pointer(i3)
array_type = c_ubyte * 1364 * 1364 * 3
#p_array_type = POINTER(array_type)
arg2 = array_type()
read_png = ifilter.read_png
read_png.restype = POINTER(c_ubyte)
read_png.argtypes = [c_char_p, int_p, int_p, int_p]
pic = read_png(b"austin_flights.png", ip1, ip2, ip3)
ifilter.apply_any_filter3(pic, byref(arg2), callback_for_gaussian_2.ctypes)
ifilter.write_png(byref(arg2), 1364, 1364)
