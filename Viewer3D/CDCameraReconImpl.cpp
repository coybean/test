#include "CDCameraReconImpl.h"
#include <onnxruntime_cxx_api.h>	 // C或c++的api

#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include<opencv2/ximgproc/disparity_filter.hpp>
// 定义点云类型
typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointCloud<PointT> PointCloud;
using namespace Ort;
using namespace std;

wstring stringTowstring(string str)
{
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    if (len == 0)
        return wstring(L"");
    wchar_t* wct = new wchar_t[len];
    if (!wct)
        return std::wstring(L"");

    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wct, len);
    wstring wstr(wct);
    delete[] wct;
    wct = NULL;
    return wstr;
}

string wstringTostring(wstring wstr)
{
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (len == 0)
        return string("");
    char* psz = new char[len];
    if (!psz)
        return string("");
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, psz, len, NULL, NULL);
    string str(psz);
    delete[] psz;
    psz = NULL;
    return str;
}

typedef struct IOStruct
{
    typedef enum IO { INPUT, OUTPUT };
    IO io;
    string name;
    ONNXTensorElementDataType type;
    std::vector<int64_t> shape;
    size_t elementCount;//每一个（输入输出）数据里面有多少个值（元素）
    void* data;
}*pIOStruct;
class ONNXParserner
{
public:
    ONNXParserner();
    ~ONNXParserner() {};
    void init(wstring modelPath, wstring modelPathMain);
    void predict(cv::Mat leftImage, cv::Mat rightImage,cv::Mat& resultImage);
public:
    Env env;
    SessionOptions sessionOptions;
    Session* ort_session;
    Session* ort_sessionMain;
    AllocatorWithDefaultOptions allocator;   // 配置输入输出节点内存

    vector<string> input_names;  // 定义一个字符指针vector
    vector<string> output_names; // 定义一个字符指针vector
    vector<vector<int64_t>> input_node_dims; //输入数据的个数和维度
    vector<vector<int64_t>> output_node_dims; // 输出数据的个数和维度

    int numInputNodes;//输入节点个数（几个输入）
    int numOutputNodes;//输出节点个数（几个输出）

    vector<IOStruct>input_output;
    vector<IOStruct>input_outputMain;
    float* input_Left; //输入图片，HWC
    float* input_Right; //输入图片，HWC
    float* output;
    float* input_LeftMain; //输入图片，HWC
    float* input_RightMain; //输入图片，HWC
    float* outputMain;
};

ONNXParserner::ONNXParserner()
{
    input_Left=nullptr; //输入图片，HWC
    input_Right = nullptr; //输入图片，HWC
    output = nullptr;
    input_LeftMain = nullptr; //输入图片，HWC
    input_RightMain = nullptr; //输入图片，HWC
    outputMain = nullptr;
}
void ONNXParserner::init(wstring modelPath,wstring modelPathMain)
{
    sessionOptions = SessionOptions();//session配置
    sessionOptions.SetGraphOptimizationLevel(ORT_ENABLE_BASIC);
    env = Ort::Env(ORT_LOGGING_LEVEL_ERROR, "onnxPredict");//onnxruntime env
    //OrtSessionOptionsAppendExecutionProvider_CUDA(sessionOptions, 0);
     ort_session = new Session(env, (const wchar_t*)&modelPath[0], sessionOptions);//创建session
     ort_sessionMain = new Session(env, (const wchar_t*)&modelPathMain[0], sessionOptions);//创建session

     numInputNodes = ort_sessionMain->GetInputCount();  //输入输出节点数量
     numOutputNodes = ort_sessionMain->GetOutputCount();
     //获取模型输入结构信息
     for (int i = 0; i < numInputNodes; i++)
     {
         IOStruct iostruct;
         AllocatedStringPtr  input_name_ptr = ort_sessionMain->GetInputNameAllocated(i, allocator);
         //input_names.push_back(string(input_name_ptr.get()));		// 输入数据名称
         Ort::TypeInfo input_type_info = ort_sessionMain->GetInputTypeInfo(i);   // 输入数据信息（类型，大小）
         ConstTensorTypeAndShapeInfo input_tensor_info = input_type_info.GetTensorTypeAndShapeInfo();  //
         //std::vector<int64_t> input_dims = input_tensor_info.GetShape();    // 输入shape
        // ONNXTensorElementDataType type = input_tensor_info.GetElementType();//输入类型
        // input_node_dims.push_back(input_dims);	// 保存

         iostruct.io = IOStruct::IO::INPUT;
         iostruct.name = string(input_name_ptr.get());
         iostruct.shape = input_tensor_info.GetShape();
         iostruct.type = input_tensor_info.GetElementType();
         iostruct.elementCount = input_tensor_info.GetElementCount();
         input_output.push_back(iostruct);
     }

     //获取模型输出结构信息
     for (int i = 0; i < numOutputNodes; i++)
     {
         AllocatedStringPtr  output_name_ptr = ort_sessionMain->GetOutputNameAllocated(i, allocator);
        // output_names.push_back(string(output_name_ptr.get()));
         Ort::TypeInfo output_type_info = ort_sessionMain->GetOutputTypeInfo(i);
         auto output_tensor_info = output_type_info.GetTensorTypeAndShapeInfo();
         //auto output_dims = output_tensor_info.GetShape();
         //output_node_dims.push_back(output_dims);

         IOStruct iostruct;
         iostruct.io = IOStruct::IO::OUTPUT;
         iostruct.name = string(output_name_ptr.get());
         iostruct.shape = output_tensor_info.GetShape();
         iostruct.type = output_tensor_info.GetElementType();
         iostruct.elementCount = output_tensor_info.GetElementCount();
         input_output.push_back(iostruct);
     }
}

void ONNXParserner::predict(cv::Mat LeftImage,cv::Mat RightImage, cv::Mat& resultImage)
{
    cv::Mat LeftImageFloat, RightImageFloat;
    LeftImage.convertTo(LeftImageFloat, CV_32FC3);
    RightImage.convertTo(RightImageFloat, CV_32FC3);
    cv::Mat LeftImageSmall, LeftImageNormal;
    cv::Mat RightImageSmall, RightImageNormal;
    resize(LeftImageFloat, LeftImageSmall, cv::Size(input_output[0].shape[3]/2, input_output[0].shape[2]/2));
    resize(RightImageFloat, RightImageSmall, cv::Size(input_output[1].shape[3]/2, input_output[1].shape[2]/2));

    resize(LeftImageFloat, LeftImageNormal, cv::Size(input_output[0].shape[3], input_output[0].shape[2]));
    resize(RightImageFloat, RightImageNormal, cv::Size(input_output[1].shape[3], input_output[1].shape[2]));

    cv::Mat splitLeftImages[3];
    cv::Mat splitRightImages[3];
    split(LeftImageSmall, splitLeftImages);
    split(RightImageSmall, splitRightImages);
    if (input_Left == nullptr)
    {
        input_Left = new float[3 * LeftImageSmall.rows * LeftImageSmall.cols]; //输入图片，HWC
        input_Right = new float[3 * RightImageSmall.rows * RightImageSmall.cols]; //输入图片，HWC
        output = new float[1 * 2 * RightImageSmall.rows * RightImageSmall.cols] {0};
        input_LeftMain = new float[3 * LeftImageNormal.rows * LeftImageNormal.cols]; //输入图片，HWC
        input_RightMain = new float[3 * LeftImageNormal.rows * LeftImageNormal.cols]; //输入图片，HWC
        outputMain = new float[1 * 2 * LeftImageNormal.rows * LeftImageNormal.cols] {0};
    }


    memcpy(input_Left, splitLeftImages[2].data, sizeof(float) * LeftImageSmall.rows * LeftImageSmall.cols);
    memcpy(input_Left+ LeftImageSmall.rows * LeftImageSmall.cols, splitLeftImages[1].data, sizeof(float) * LeftImageSmall.rows * LeftImageSmall.cols);
    memcpy(input_Left+2* LeftImageSmall.rows * LeftImageSmall.cols, splitLeftImages[0].data, sizeof(float) * LeftImageSmall.rows * LeftImageSmall.cols);

    memcpy(input_Right, splitRightImages[2].data, sizeof(float) * RightImageSmall.rows * LeftImageSmall.cols);
    memcpy(input_Right + RightImageSmall.rows * RightImageSmall.cols, splitRightImages[1].data, sizeof(float) * RightImageSmall.rows * RightImageSmall.cols);
    memcpy(input_Right + 2 * RightImageSmall.rows * RightImageSmall.cols, splitRightImages[0].data, sizeof(float) * RightImageSmall.rows * RightImageSmall.cols);

    std::array<int64_t, 4> input_shape_{ 1, 3, RightImageSmall.rows , LeftImageSmall.cols };
    std::array<int64_t, 4> output_shape_{ 1, 2, RightImageSmall.rows , LeftImageSmall.cols };
    auto memory_info1 = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    Ort::Value input_Left_tensor = Ort::Value::CreateTensor<float>(memory_info1, input_Left, 3 * RightImageSmall.rows * RightImageSmall.cols, input_shape_.data(), input_shape_.size());
    Ort::Value input_Right_tensor = Ort::Value::CreateTensor<float>(memory_info1, input_Right, 3 * RightImageSmall.rows * RightImageSmall.cols, input_shape_.data(), input_shape_.size());
    Ort::Value output_tensor = Ort::Value::CreateTensor<float>(memory_info1, output, 2 * RightImageSmall.rows * RightImageSmall.cols, output_shape_.data(), output_shape_.size());

    Ort::IoBinding io_binding1(*ort_session);
    io_binding1.BindInput("left", input_Left_tensor);
    io_binding1.BindInput("right", input_Right_tensor);
    io_binding1.BindOutput("output", output_tensor);

    ort_session->Run(Ort::RunOptions{ nullptr }, io_binding1);

    cv::Mat splitLeftImagesMain[3];
    cv::Mat splitRightImagesMain[3];
    split(LeftImageNormal, splitLeftImagesMain);
    split(RightImageNormal, splitRightImagesMain);
    memcpy(input_LeftMain, splitLeftImagesMain[2].data, sizeof(float) * LeftImageNormal.rows * LeftImageNormal.cols);
    memcpy(input_LeftMain + LeftImageNormal.rows * LeftImageNormal.cols, splitLeftImagesMain[1].data, sizeof(float) * LeftImageNormal.rows * LeftImageNormal.cols);
    memcpy(input_LeftMain + 2 * LeftImageNormal.rows * LeftImageNormal.cols, splitLeftImagesMain[0].data, sizeof(float) * LeftImageNormal.rows * LeftImageNormal.cols);

    memcpy(input_RightMain, splitRightImagesMain[2].data, sizeof(float) * LeftImageNormal.rows * LeftImageNormal.cols);
    memcpy(input_RightMain + LeftImageNormal.rows * LeftImageNormal.cols, splitRightImagesMain[1].data, sizeof(float) * LeftImageNormal.rows * LeftImageNormal.cols);
    memcpy(input_RightMain + 2 * LeftImageNormal.rows * LeftImageNormal.cols, splitRightImagesMain[0].data, sizeof(float) * LeftImageNormal.rows * LeftImageNormal.cols);

    std::array<int64_t, 4> input_shape_Main{ 1, 3, LeftImageNormal.rows , LeftImageNormal.cols };
    std::array<int64_t, 4> input_flow_shape_Main{ 1, 2, LeftImageNormal.rows / 2 , LeftImageNormal.cols / 2 };
    std::array<int64_t, 4> output_shape_Main{ 1, 2, LeftImageNormal.rows , LeftImageNormal.cols };
    auto memory_infoMain = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    //float* hhh = new float[3 * LeftImageNormal.rows * LeftImageNormal.cols] {0};
    Ort::Value input_Left_tensorMain = Ort::Value::CreateTensor<float>(memory_infoMain, input_LeftMain, 3 * LeftImageNormal.rows * LeftImageNormal.cols, input_shape_Main.data(), input_shape_Main.size());
    Ort::Value input_Right_tensorMain = Ort::Value::CreateTensor<float>(memory_infoMain, input_RightMain, 3 * LeftImageNormal.rows * LeftImageNormal.cols, input_shape_Main.data(), input_shape_Main.size());
    Ort::Value input_flow_tensorMain = Ort::Value::CreateTensor<float>(memory_infoMain, output, 2 * LeftImageNormal.rows / 2 * LeftImageNormal.cols / 2, input_flow_shape_Main.data(), input_flow_shape_Main.size());
    Ort::Value output_tensorMain = Ort::Value::CreateTensor<float>(memory_infoMain, outputMain, 2 * LeftImageNormal.rows * LeftImageNormal.cols, output_shape_Main.data(), output_shape_Main.size());
    //memcpy(input_Left_tensorMain.GetTensorMutableRawData(), input_LeftMain, sizeof(float) * 3 * LeftImageNormal.rows * LeftImageNormal.cols);
    Ort::IoBinding io_bindingMain(*ort_sessionMain);

    io_bindingMain.BindInput(input_output[0].name.data(), input_Left_tensorMain);
    io_bindingMain.BindInput(input_output[1].name.data(), input_Right_tensorMain);
    io_bindingMain.BindInput(input_output[2].name.data(), input_flow_tensorMain);
    io_bindingMain.BindOutput(input_output[3].name.data(), output_tensorMain);

    ort_sessionMain->Run(Ort::RunOptions{ nullptr }, io_bindingMain);

    //try
    //{
    //	double start = GetTickCount();
    //	ort_session->Run(Ort::RunOptions{ nullptr }, io_binding);
    //	double end = GetTickCount();
    //	cout << (end - start) / 1000.0 << endl;
    //}
    //catch (Ort::Exception e)
    //{
    //	cout << e.what() << endl;
    //	return ;
    //}
    cv::Mat result = cv::Mat::zeros(LeftImageNormal.rows , LeftImageNormal.cols, CV_32FC1);
    memcpy(result.data, outputMain, sizeof(float) * LeftImageNormal.rows*LeftImageNormal.cols);
    resize(result, result, cv::Size(0, 0), 2, 2);
    normalize(result, result, 0, 255, cv::NORM_MINMAX);
    result.convertTo(result, CV_8UC1);
    cv::Mat colorImage;
    cv::applyColorMap(result, colorImage, cv::COLORMAP_INFERNO);
    resultImage = result.clone();
    imwrite("D:/colorImage.bmp", colorImage);
}



CDCameraReconImpl::CDCameraReconImpl()
{
    m_pOnnxparser=new ONNXParserner();
    wstring modelPath = L"models/crestereo_without_flow.onnx";// stringTowstring(model_path);
    wstring modelPathMain = L"models/crestereo.onnx";// stringTowstring(model_path);
    m_pOnnxparser->init(modelPath,modelPathMain);
    //左相机内参
    Klc = (cv::Mat_<double>(3, 3) << 905.655, 0, 676.581, 0, 906.169, 377.293, 0, 0, 1);
    //左相机畸变系数
    dist_Lcamera = (cv::Mat_<double>(5, 1) << 0.0493835, -0.106548, 0.00018606, -0.000134464, 0.00816809);
    //右相机内参
    Krc = (cv::Mat_<double>(3, 3) << 904.228, 0, 640.345, 0, 903.935, 376.785, 0, 0, 1);
    //右相机畸变系数
    dist_Rcamera = (cv::Mat_<double>(5, 1) << 0.0439956, -0.0792753, 0.000150844, 0.000677424, -0.0169198);
    //两相机旋转矩阵
    Rotate_L2R = (cv::Mat_<double>(3, 3) << 0.999901, 0.00695672, 0.0122243, -0.00696112, 0.999976, 0.000317128, -0.0122218, -0.000402191, 0.999925);

    //两相机平移矩阵
    Trans_L2R = (cv::Mat_<double>(3, 1) << -160.045, 0, 0);

    ImgSize=cv::Size(1280, 720);

    cv::stereoRectify(Klc, dist_Lcamera, Krc, dist_Rcamera, ImgSize, Rotate_L2R, Trans_L2R, RL, RR, PL, PR, Q, cv::CALIB_ZERO_DISPARITY);
    //畸变校正映射计算
    initUndistortRectifyMap(Klc, dist_Lcamera, RL, PL, ImgSize, CV_32FC1, undistmap1l, undistmap2l);
    initUndistortRectifyMap(Krc, dist_Rcamera, RR, PR, ImgSize, CV_32FC1, undistmap1r, undistmap2r);
}

CDCameraReconImpl::~CDCameraReconImpl()
{
    if(m_pOnnxparser!=nullptr)
    {
        delete m_pOnnxparser;
        m_pOnnxparser=nullptr;
    }
}



void CDCameraReconImpl::correctImage(cv::Mat srcImage, cv::Mat& dstImage)
{
    cv::remap(srcImage, dstImage, undistmap1l, undistmap2l, cv::INTER_LINEAR);
}

void CDCameraReconImpl::genDepthImage(cv::Mat leftCorrectImg, cv::Mat rightCorrectImg, cv::Mat& depthImg)
{
    m_pOnnxparser->predict(leftCorrectImg, rightCorrectImg, depthImg);
}

void CDCameraReconImpl::genPlyImg(cv::Mat leftCorrectImg,cv::Mat depthImgFloat)
{
    cv::Mat disp = depthImgFloat;//cv::imread(".\\res\\Depth\\1.pfm", -1);// filteredImg.clone();//
    cv::Mat xyz;
    reprojectImageTo3D(disp, xyz, Q, true, -1);                                //在实际求距离时，ReprojectTo3D出来的X / W, Y / W, Z / W都要乘以16
    int num = 1;
    char file_name[200];
    sprintf_s(file_name, ".\\PointCloud\\%u.ply", 1);
    PointCloud::Ptr __pointCloud(new PointCloud);
    for (int row = 10; row < 710; row++)
    {
        for (int col = 10; col < 1270; col++)
        {
            if (leftCorrectImg.at<cv::Vec3b>(row, col) == cv::Vec3b(0, 0, 0) || (int)xyz.at<cv::Vec3f>(row, col)[2] > 60000)
                continue;
            PointT p;

            p.x = (float)xyz.at<cv::Vec3f>(row, col)[0];
            p.y = (float)xyz.at<cv::Vec3f>(row, col)[1];
            p.z = (float)xyz.at<cv::Vec3f>(row, col)[2];
            p.b = leftCorrectImg.at<cv::Vec3b>(row, col)[0];
            p.g = leftCorrectImg.at<cv::Vec3b>(row, col)[1];
            p.r = leftCorrectImg.at<cv::Vec3b>(row, col)[2];

            __pointCloud->points.push_back(p);
        }
    }
    if (__pointCloud->points.empty())
        return;
    pcl::io::savePLYFile(file_name, *__pointCloud);
}
