### 【2519】run windows dialog 在UF框架下运行Windows对话框的示例代码

#### 代码

```cpp
    #include <uf.h>  
    #include <windows.h>  
    #include <uf_ui.h>  
    ///////////////////////////////////////////////////////////////////////////////  
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  
    {  
        switch (message)  
        {  
        case WM_CLOSE:  
            DestroyWindow(hWnd);  
            break;  
        case WM_DESTROY:  
            UF_terminate();  
            PostQuitMessage(0);  
            break;  
        default:  
            return DefWindowProcA(hWnd, message, wParam, lParam);  
        }  
        return 0;  
    }  
    ///////////////////////////////////////////////////////////////////////////////  
    int ShowMyWindow()  
    {  
        WNDCLASSA wnd_class = { 0 };  
        wnd_class.lpfnWndProc = WndProc;  
        wnd_class.hInstance = GetModuleHandle(NULL);  
        wnd_class.lpszClassName = "myWindowClass";  
        HWND p = (HWND)UF_UI_get_default_parent();  
        RegisterClassA(&wnd_class);  
        HWND myWindow = CreateWindowA(wnd_class.lpszClassName, "My Window",  
            WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 640, 480,  
            p, 0, wnd_class.hInstance, NULL);  
        MSG msg = { 0 };  
        while (GetMessage(&msg, NULL, 0, 0) > 0)  
        {  
            TranslateMessage(&msg);  
            DispatchMessageA(&msg);  
        }  
        return 1;  
    }  
    ///////////////////////////////////////////////////////////////////////////////  
    extern DllExport void ufusr(char *parm, int *returnCode, int rlen)  
    {  
        if (UF_initialize() != 0)  
            MessageBox(NULL, L"UF_initialize error", L"Error", MB_OK);  
        ShowMyWindow();  
    }  
    ///////////////////////////////////////////////////////////////////////////////  
    extern int ufusr_ask_unload(void)  
    {  
        return(UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是用于NX Open UI的二次开发示例，主要功能如下：
>
> 1. 包含了UF和Windows的头文件，以支持NX和Windows的调用。
> 2. 定义了窗口过程函数WndProc，用于处理窗口消息，包括关闭窗口和销毁窗口。
> 3. 定义了ShowMyWindow函数，用于创建并显示一个窗口。该函数首先注册一个窗口类，然后创建一个窗口实例并显示。
> 4. 定义了ufusr函数，这是NX二次开发的入口函数。在该函数中，首先调用UF_initialize初始化NX环境，然后调用ShowMyWindow函数显示窗口。
> 5. 定义了ufusr_ask_unload函数，用于在NX卸载该插件时调用，返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
> 6. 整体上，这段代码通过UF和Windows API实现了NX界面下的一个简单窗口，展示了如何在NX中创建和显示自定义窗口。
>
