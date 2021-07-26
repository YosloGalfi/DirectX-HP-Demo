#include "System.h"

System::System(HINSTANCE hInstance)
{
    appHandle = this;

    this->hInstance = hInstance;
    this->hwnd = 0;
    this->scene = 0;
    this->hr = 0;
    this->screenWidth = 0;
    this->screenHeight = 0;
}

System::~System()
{
}

bool System::Initialize()
{
    /*
        CoInitialize so we can use WIC textures.
        Set screen width and height.
        Then initialize windows.
    */

    bool result;
    hr = CoInitialize(NULL);
    assert(SUCCEEDED(hr));

    //_crtBreakAlloc = 1940312;

    screenWidth = DEFAULT_SCREEN_WIDTH;
    screenHeight = DEFAULT_SCREEN_HEIGHT;

    InitializeWindows(screenWidth, screenHeight);

    /*
        Init directx 11 graphic stuff for project. (All on the scene)
        Input as parameter for picking stuff in graphic functions.
    */

    scene = new Scene();
    if (!scene)
        return false;

    result = scene->Initialize(screenWidth, screenHeight, hwnd);
    if (!result)
        return false;


    return true;
}

void System::Shutdown()
{
    // Release all graphics stuff.
    if (scene) {
        scene->Shutdown();
        delete scene;
        scene = 0;
    }

    // Remove all the window stuff.
    DestroyWindow(hwnd);
    hwnd = NULL;

    UnregisterClass(projectTitel, hInstance);
    hInstance = NULL;
    appHandle = NULL;
}

void System::Run()
{
    MSG msg = { 0 };
    ZeroMemory(&msg, sizeof(MSG));
    bool result, close_program = false;

    fpsTimer.Reset();

    /* Mainloop */
    while (!close_program)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                close_program = true;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            result = UpdateOnFrame();
            if (!result)
                close_program = true;
        }
    }
}

bool System::UpdateOnFrame()
{
    bool result;

    /* Get deltaTime for current frame. */
    fpsTimer.Frame();

    /* Graphic updates and renderpipeline. */
    result = scene->RenderFrame(fpsTimer.DeltaTime() / 2.0f);
    if (!result)
        return false;

    return true;
}

void System::InitializeWindows(int& width, int& height)
{
    const wchar_t CLASS_NAME[] = L"D3D11 Class";

    WNDCLASSEX wndclass;
    ZeroMemory(&wndclass, sizeof(WNDCLASSEX));

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hIcon = nullptr;
    wndclass.hIconSm = nullptr;
    wndclass.hbrBackground = nullptr;
    wndclass.lpszMenuName = nullptr;
    wndclass.lpszClassName = CLASS_NAME;

    if (!RegisterClassEx(&wndclass)) {
        MessageBox(0, L"Failed to 'RegisterClassEx'.", L"Windows Initialization Message", MB_ICONERROR);
    }

    // Resizes rect to clientsize
    RECT windowRect = { 0,0, width, height };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);


    this->hwnd = CreateWindowEx(0,
        CLASS_NAME,
        projectTitel,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr);

    assert(hwnd);
    ShowWindow(hwnd, SW_SHOW);
}

LRESULT CALLBACK System::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
    if (appHandle != nullptr)
        return appHandle->MessageHandler(hwnd, umessage, wparam, lparam);

    return DefWindowProc(hwnd, umessage, wparam, lparam);
}

LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch (umsg)
    {
        /* CLOSE */
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        break;
    }

    /* Default proc */
    default:
        return DefWindowProc(hwnd, umsg, wparam, lparam);
        break;
    }

    return 0;
}
