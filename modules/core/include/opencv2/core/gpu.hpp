/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef __OPENCV_CORE_GPU_HPP__
#define __OPENCV_CORE_GPU_HPP__

#ifndef __cplusplus
#  error gpu.hpp header must be compiled as C++
#endif

#include "opencv2/core.hpp"
#include "opencv2/core/gpu_types.hpp"

namespace cv { namespace gpu
{

//////////////////////////////// GpuMat ///////////////////////////////

// Smart pointer for GPU memory with reference counting.
// Its interface is mostly similar with cv::Mat.

class CV_EXPORTS GpuMat
{
public:
    //! default constructor
    GpuMat();

    //! constructs GpuMat of the specified size and type
    GpuMat(int rows, int cols, int type);
    GpuMat(Size size, int type);

    //! constucts GpuMat and fills it with the specified value _s
    GpuMat(int rows, int cols, int type, Scalar s);
    GpuMat(Size size, int type, Scalar s);

    //! copy constructor
    GpuMat(const GpuMat& m);

    //! constructor for GpuMat headers pointing to user-allocated data
    GpuMat(int rows, int cols, int type, void* data, size_t step = Mat::AUTO_STEP);
    GpuMat(Size size, int type, void* data, size_t step = Mat::AUTO_STEP);

    //! creates a GpuMat header for a part of the bigger matrix
    GpuMat(const GpuMat& m, Range rowRange, Range colRange);
    GpuMat(const GpuMat& m, Rect roi);

    //! builds GpuMat from Mat. Perfom blocking upload to device
    explicit GpuMat(const Mat& m);

    //! destructor - calls release()
    ~GpuMat();

    //! assignment operators
    GpuMat& operator =(const GpuMat& m);

    //! allocates new GpuMat data unless the GpuMat already has specified size and type
    void create(int rows, int cols, int type);
    void create(Size size, int type);

    //! decreases reference counter, deallocate the data when reference counter reaches 0
    void release();

    //! swaps with other smart pointer
    void swap(GpuMat& mat);

    //! pefroms blocking upload data to GpuMat
    void upload(const Mat& m);

    //! downloads data from device to host memory (Blocking calls)
    void download(Mat& m) const;

    //! returns deep copy of the GpuMat, i.e. the data is copied
    GpuMat clone() const;

    //! copies the GpuMat content to "m"
    void copyTo(GpuMat& m) const;

    //! copies those GpuMat elements to "m" that are marked with non-zero mask elements
    void copyTo(GpuMat& m, const GpuMat& mask) const;

    //! sets some of the GpuMat elements to s, according to the mask
    GpuMat& setTo(Scalar s, const GpuMat& mask = GpuMat());

    //! converts GpuMat to another datatype with optional scaling
    void convertTo(GpuMat& m, int rtype, double alpha = 1, double beta = 0) const;

    void assignTo(GpuMat& m, int type=-1) const;

    //! returns pointer to y-th row
    uchar* ptr(int y = 0);
    const uchar* ptr(int y = 0) const;

    //! template version of the above method
    template<typename _Tp> _Tp* ptr(int y = 0);
    template<typename _Tp> const _Tp* ptr(int y = 0) const;

    template <typename _Tp> operator PtrStepSz<_Tp>() const;
    template <typename _Tp> operator PtrStep<_Tp>() const;

    //! returns a new GpuMat header for the specified row
    GpuMat row(int y) const;

    //! returns a new GpuMat header for the specified column
    GpuMat col(int x) const;

    //! ... for the specified row span
    GpuMat rowRange(int startrow, int endrow) const;
    GpuMat rowRange(Range r) const;

    //! ... for the specified column span
    GpuMat colRange(int startcol, int endcol) const;
    GpuMat colRange(Range r) const;

    //! extracts a rectangular sub-GpuMat (this is a generalized form of row, rowRange etc.)
    GpuMat operator ()(Range rowRange, Range colRange) const;
    GpuMat operator ()(Rect roi) const;

    //! creates alternative GpuMat header for the same data, with different
    //! number of channels and/or different number of rows
    GpuMat reshape(int cn, int rows = 0) const;

    //! locates GpuMat header within a parent GpuMat
    void locateROI(Size& wholeSize, Point& ofs) const;

    //! moves/resizes the current GpuMat ROI inside the parent GpuMat
    GpuMat& adjustROI(int dtop, int dbottom, int dleft, int dright);

    //! returns true iff the GpuMat data is continuous
    //! (i.e. when there are no gaps between successive rows)
    bool isContinuous() const;

    //! returns element size in bytes
    size_t elemSize() const;

    //! returns the size of element channel in bytes
    size_t elemSize1() const;

    //! returns element type
    int type() const;

    //! returns element type
    int depth() const;

    //! returns number of channels
    int channels() const;

    //! returns step/elemSize1()
    size_t step1() const;

    //! returns GpuMat size : width == number of columns, height == number of rows
    Size size() const;

    //! returns true if GpuMat data is NULL
    bool empty() const;

    /*! includes several bit-fields:
    - the magic signature
    - continuity flag
    - depth
    - number of channels
    */
    int flags;

    //! the number of rows and columns
    int rows, cols;

    //! a distance between successive rows in bytes; includes the gap if any
    size_t step;

    //! pointer to the data
    uchar* data;

    //! pointer to the reference counter;
    //! when GpuMat points to user-allocated data, the pointer is NULL
    int* refcount;

    //! helper fields used in locateROI and adjustROI
    uchar* datastart;
    uchar* dataend;
};

//! Creates continuous GPU matrix
CV_EXPORTS void createContinuous(int rows, int cols, int type, GpuMat& m);

//! Ensures that size of the given matrix is not less than (rows, cols) size
//! and matrix type is match specified one too
CV_EXPORTS void ensureSizeIsEnough(int rows, int cols, int type, GpuMat& m);

CV_EXPORTS GpuMat allocMatFromBuf(int rows, int cols, int type, GpuMat& mat);

//////////////////////////////// CudaMem ////////////////////////////////
// CudaMem is limited cv::Mat with page locked memory allocation.
// Page locked memory is only needed for async and faster coping to GPU.
// It is convertable to cv::Mat header without reference counting
// so you can use it with other opencv functions.

// Page-locks the matrix m memory and maps it for the device(s)
CV_EXPORTS void registerPageLocked(Mat& m);

// Unmaps the memory of matrix m, and makes it pageable again.
CV_EXPORTS void unregisterPageLocked(Mat& m);

class CV_EXPORTS CudaMem
{
public:
    enum  { ALLOC_PAGE_LOCKED = 1, ALLOC_ZEROCOPY = 2, ALLOC_WRITE_COMBINED = 4 };

    CudaMem();
    CudaMem(const CudaMem& m);

    CudaMem(int rows, int cols, int type, int _alloc_type = ALLOC_PAGE_LOCKED);
    CudaMem(Size size, int type, int alloc_type = ALLOC_PAGE_LOCKED);


    //! creates from cv::Mat with coping data
    explicit CudaMem(const Mat& m, int alloc_type = ALLOC_PAGE_LOCKED);

    ~CudaMem();

    CudaMem& operator = (const CudaMem& m);

    //! returns deep copy of the matrix, i.e. the data is copied
    CudaMem clone() const;

    //! allocates new matrix data unless the matrix already has specified size and type.
    void create(int rows, int cols, int type, int alloc_type = ALLOC_PAGE_LOCKED);
    void create(Size size, int type, int alloc_type = ALLOC_PAGE_LOCKED);

    //! decrements reference counter and released memory if needed.
    void release();

    //! returns matrix header with disabled reference counting for CudaMem data.
    Mat createMatHeader() const;
    operator Mat() const;

    //! maps host memory into device address space and returns GpuMat header for it. Throws exception if not supported by hardware.
    GpuMat createGpuMatHeader() const;
    operator GpuMat() const;

    //returns if host memory can be mapperd to gpu address space;
    static bool canMapHostMemory();

    // Please see cv::Mat for descriptions
    bool isContinuous() const;
    size_t elemSize() const;
    size_t elemSize1() const;
    int type() const;
    int depth() const;
    int channels() const;
    size_t step1() const;
    Size size() const;
    bool empty() const;


    // Please see cv::Mat for descriptions
    int flags;
    int rows, cols;
    size_t step;

    uchar* data;
    int* refcount;

    uchar* datastart;
    uchar* dataend;

    int alloc_type;
};


//////////////////////////////// CudaStream ////////////////////////////////
// Encapculates Cuda Stream. Provides interface for async coping.
// Passed to each function that supports async kernel execution.
// Reference counting is enabled

class CV_EXPORTS Stream
{
public:
    Stream();
    ~Stream();

    Stream(const Stream&);
    Stream& operator =(const Stream&);

    bool queryIfComplete();
    void waitForCompletion();

    //! downloads asynchronously
    // Warning! cv::Mat must point to page locked memory (i.e. to CudaMem data or to its subMat)
    void enqueueDownload(const GpuMat& src, CudaMem& dst);
    void enqueueDownload(const GpuMat& src, Mat& dst);

    //! uploads asynchronously
    // Warning! cv::Mat must point to page locked memory (i.e. to CudaMem data or to its ROI)
    void enqueueUpload(const CudaMem& src, GpuMat& dst);
    void enqueueUpload(const Mat& src, GpuMat& dst);

    //! copy asynchronously
    void enqueueCopy(const GpuMat& src, GpuMat& dst);

    //! memory set asynchronously
    void enqueueMemSet(GpuMat& src, Scalar val);
    void enqueueMemSet(GpuMat& src, Scalar val, const GpuMat& mask);

    //! converts matrix type, ex from float to uchar depending on type
    void enqueueConvert(const GpuMat& src, GpuMat& dst, int dtype, double a = 1, double b = 0);

    //! adds a callback to be called on the host after all currently enqueued items in the stream have completed
    typedef void (*StreamCallback)(Stream& stream, int status, void* userData);
    void enqueueHostCallback(StreamCallback callback, void* userData);

    static Stream& Null();

    operator bool() const;

private:
    struct Impl;

    explicit Stream(Impl* impl);
    void create();
    void release();

    Impl *impl;

    friend struct StreamAccessor;
};

//////////////////////////////// Initialization & Info ////////////////////////

//! This is the only function that do not throw exceptions if the library is compiled without Cuda.
CV_EXPORTS int getCudaEnabledDeviceCount();

//! Functions below throw cv::Expception if the library is compiled without Cuda.

CV_EXPORTS void setDevice(int device);

CV_EXPORTS int getDevice();

//! Explicitly destroys and cleans up all resources associated with the current device in the current process.
//! Any subsequent API call to this device will reinitialize the device.
CV_EXPORTS void resetDevice();

enum FeatureSet
{
    FEATURE_SET_COMPUTE_10 = 10,
    FEATURE_SET_COMPUTE_11 = 11,
    FEATURE_SET_COMPUTE_12 = 12,
    FEATURE_SET_COMPUTE_13 = 13,
    FEATURE_SET_COMPUTE_20 = 20,
    FEATURE_SET_COMPUTE_21 = 21,
    FEATURE_SET_COMPUTE_30 = 30,
    FEATURE_SET_COMPUTE_35 = 35,

    GLOBAL_ATOMICS = FEATURE_SET_COMPUTE_11,
    SHARED_ATOMICS = FEATURE_SET_COMPUTE_12,
    NATIVE_DOUBLE = FEATURE_SET_COMPUTE_13,
    WARP_SHUFFLE_FUNCTIONS = FEATURE_SET_COMPUTE_30,
    DYNAMIC_PARALLELISM = FEATURE_SET_COMPUTE_35
};

// Checks whether current device supports the given feature
CV_EXPORTS bool deviceSupports(FeatureSet feature_set);

// Gives information about what GPU archs this OpenCV GPU module was
// compiled for
class CV_EXPORTS TargetArchs
{
public:
    static bool builtWith(FeatureSet feature_set);
    static bool has(int major, int minor);
    static bool hasPtx(int major, int minor);
    static bool hasBin(int major, int minor);
    static bool hasEqualOrLessPtx(int major, int minor);
    static bool hasEqualOrGreater(int major, int minor);
    static bool hasEqualOrGreaterPtx(int major, int minor);
    static bool hasEqualOrGreaterBin(int major, int minor);
private:
    TargetArchs();
};

// Gives information about the given GPU
class CV_EXPORTS DeviceInfo
{
public:
    // Creates DeviceInfo object for the current GPU
    DeviceInfo() : device_id_(getDevice()) { query(); }

    // Creates DeviceInfo object for the given GPU
    DeviceInfo(int device_id) : device_id_(device_id) { query(); }

    String name() const { return name_; }

    // Return compute capability versions
    int majorVersion() const { return majorVersion_; }
    int minorVersion() const { return minorVersion_; }

    int multiProcessorCount() const { return multi_processor_count_; }

    size_t sharedMemPerBlock() const;

    void queryMemory(size_t& totalMemory, size_t& freeMemory) const;
    size_t freeMemory() const;
    size_t totalMemory() const;

    // Checks whether device supports the given feature
    bool supports(FeatureSet feature_set) const;

    // Checks whether the GPU module can be run on the given device
    bool isCompatible() const;

    int deviceID() const { return device_id_; }

private:
    void query();

    int device_id_;

    String name_;
    int multi_processor_count_;
    int majorVersion_;
    int minorVersion_;
};

CV_EXPORTS void printCudaDeviceInfo(int device);

CV_EXPORTS void printShortCudaDeviceInfo(int device);

}} // cv::gpu

#include "opencv2/core/gpu.inl.hpp"

#endif /* __OPENCV_CORE_GPU_HPP__ */
