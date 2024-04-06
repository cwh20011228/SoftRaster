// OpenGLDemo1.cpp : 定义应用程序的入口点。

#include "framework.h"
#include "OpenGLDemo1.h"
#include "Canvas.h"
#include <cmath>
#include "Image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define MAX_LOADSTRING 100

//TODO: 在glm库中，rotate(旋转)和translate(平移），都是基于3D世界进行操作的。4*4的矩阵
// 要用三维模拟二维，二维（以(0,0)旋转），三维则以（0，0，1）z轴旋转

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HWND hWnd;                  //  窗口句柄，指向主窗口
int wWidth = 801;           //  长
int wHeight = 601;          // 宽
HDC hDC;
HDC hMem;

//TODO: 全局变量:
GT::Canvas* _canvas=nullptr;
GT::Image* _image = nullptr;
GT::Image* _bkImage = nullptr;
GT::Image* _zoomImage = nullptr;
GT::Image* _zoomImageSimple = nullptr;

float speed = 0.01;     // 速度
float _angle = 0.0f;    // 角度
float _xCam = 0.0f;     // 摄像机的位置


// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);   // 注册窗口类的函数
BOOL                InitInstance(HINSTANCE, int);           // 初始化
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);    // 窗口过程函数，处理窗口信息
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);      // 对话框的回调函数

void Render();  // 绘图函数

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,             //当前实例的句柄
                     _In_opt_ HINSTANCE hPrevInstance,      // 前一个实例的句柄
                     _In_ LPWSTR    lpCmdLine,              // 命令行参数
                     _In_ int       nCmdShow)               // 窗口的显示状态
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OPENGLDEMO1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OPENGLDEMO1));

    /****************创建绘图用的位图*********************/

    void* buffer = 0;   // buffer指向位图
    hDC = GetDC(hWnd);
    hMem = ::CreateCompatibleDC(hDC);   

    BITMAPINFO bmpInfo;
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = wWidth;
    bmpInfo.bmiHeader.biHeight = wHeight;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;   //实际的存储方式为bgr
    bmpInfo.bmiHeader.biSizeImage = 0;
    bmpInfo.bmiHeader.biXPelsPerMeter = 0;
    bmpInfo.bmiHeader.biYPelsPerMeter = 0;
    bmpInfo.bmiHeader.biClrUsed = 0;
    bmpInfo.bmiHeader.biClrImportant = 0;

    HBITMAP hBmp = CreateDIBSection(hDC, &bmpInfo, DIB_RGB_COLORS, (void**)&buffer, 0, 0);      // 创建buffer的内存
    SelectObject(hMem, hBmp);

    memset(buffer, 0, wWidth * wHeight * 4);    // 清空buffer为0
    
    /****************创建绘图用的位图*********************/
    
    _canvas = new GT::Canvas(wWidth, wHeight, buffer);
    //TODO: 读取图片  
    _image = GT::Image::readFromFile("../res/carma.png"); //D:/OpenGLDemo1/res/sun.jpg
    //_image->setAlpha(0.5);
    _zoomImage = GT::Image::zoomImage(_image, 3, 3);
    _zoomImageSimple= GT::Image::zoomImageSimple(_image, 3, 3);


    // TODO:读入背景图片
    _bkImage = GT::Image::readFromFile("../res/bk.jpg");

    /****************创建绘图用的位图*********************/


    MSG msg;    // 定义一个消息结构，接收和处理窗口信息

    // 主消息循环:
    while (true)
    {
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            Render();
        }
    }

    return (int) msg.wParam;
}


// 绘图函数
void Render()
{
    // 屏幕的范围800*600(Width*Height)
    _canvas->clear();
    
    GT::Point ptArray[] =
    {
        {0.0,0.0,0.0,GT::RGBA(255,0,0),GT::floatV2(0,0)},
        {400.0,0.0,0.0,GT::RGBA(0,255,0),GT::floatV2(1.0,0)},
        {400.0,300.0,0.0,GT::RGBA(0,0,255),GT::floatV2(1.0,1.0)}
    };


    for (int i = 0; i < 3; i++)
    {
        glm::vec4 ptv4(ptArray[i].m_x, ptArray[i].m_y, ptArray[i].m_z, 1);

        // rMat是旋转矩阵
        glm::mat4 rMat(1.0f);

        rMat = glm::rotate(rMat, glm::radians(_angle), glm::vec3(1, 0, 0));

        // tMat是平移矩阵
        glm::mat4 tMat(1.0f);
        tMat = glm::translate(tMat,glm::vec3(100,300,0));

        // 先旋转后平移   矩阵的乘法实际上是按照从右到左的顺序进行的
        //ptv4 = tMat* rMat * ptv4;
        
        // vMat是观察矩阵
        glm::mat4 vMat(1.0f);
        // glm::vec3(0, 0, 100)是eye在世界坐标系下的坐标   
        // glm::vec3(0, 0, 0)是看向的那个点   
        // glm::vec3(0, 1, 0) 是头的方向
        vMat = glm::lookAt(glm::vec3(_xCam, 0, 250), glm::vec3(_xCam, 0, 0), glm::vec3(0, 1, 0));
        
        ptv4 = vMat * ptv4;

        ptArray[i].m_x = ptv4.x;
        ptArray[i].m_y = ptv4.y;
        ptArray[i].m_z = ptv4.z;

    }
    
    _angle += 2;
    _xCam += 5;

    _canvas->gtVertexPointer(2, GT::GT_FLOAT, sizeof(GT::Point), (GT::byte*)ptArray);
    _canvas->gtColorPointer(1, GT::GT_FLOAT, sizeof(GT::Point), (GT::byte*)&ptArray[0].m_color);
    _canvas->gtTexCoordPointer(2, GT::GT_FLOAT, sizeof(GT::Point), (GT::byte*)&ptArray[0].m_uv);
    _canvas->gtDrawArray(GT::GT_TRIANGLE, 0, 3);



    // 开启纹理贴图
    _canvas->enableTexture(true);
    // 绑定背景图片_bkImage

    _canvas->bindTexture(_bkImage);

    // 在这里画到设备上，hMem相当于缓冲区
    BitBlt(hDC, 0, 0, wWidth, wHeight, hMem, 0, 0, SRCCOPY);
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OPENGLDEMO1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_OPENGLDEMO1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

  /* hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP,
      CW_USEDEFAULT, 0, wWidth,wHeight , nullptr, nullptr, hInstance, nullptr);*/
   hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP,
       (GetSystemMetrics(SM_CXSCREEN) - wWidth) / 2, // X-coordinate for centering
       (GetSystemMetrics(SM_CYSCREEN) - wHeight) / 2, // Y-coordinate for centering
       wWidth, wHeight, nullptr, nullptr, hInstance, nullptr);


   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            //// 填充背景色为白色
            //HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));
            //FillRect(hdc, &ps.rcPaint, hBrush);
            
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
