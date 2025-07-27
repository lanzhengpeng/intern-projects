### 【1582】make window for printf 创建用于printf的窗口

#### 代码

```cpp
    #include <stdio.h>  
    #include <windows.h>  
    #include <uf_exit.h>  
    #include <fcntl.h>  
    #include <io.h>  
    DllExport void ufsta (char *param, int *retcode, int paramLen)  
    {  
        int fd_out = 0;  
        int fd_err = 0;  
        AllocConsole();  
        SetConsoleTitleA("NX Standard Output");  
        fd_out = _open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE), (_O_WRONLY | _O_BINARY));  
        if (fd_out) {  
              _dup2(fd_out, 1);  
              _close(fd_out);  
              SetStdHandle(STD_OUTPUT_HANDLE, (HANDLE) _get_osfhandle(1));  
              *stdout = *(_fdopen(1, "wb"));  
              setvbuf(stdout, NULL, _IONBF, 0);  
        }  
        fd_err = _open_osfhandle((intptr_t)GetStdHandle(STD_ERROR_HANDLE), (_O_WRONLY | _O_BINARY));  
        if (fd_err) {  
              _dup2(fd_err, 2);  
              _close(fd_err);  
              SetStdHandle(STD_ERROR_HANDLE, (HANDLE) _get_osfhandle(2));  
              *stderr = *(_fdopen(2, "wb"));  
              setvbuf(stderr, NULL, _IONBF, 0);  
        }  
        fprintf(stdout, "\n");  
        fprintf(stdout, "NX Standard Output is directed to this window.\n");  
        fprintf(stderr, "NX Standard Error is directed to this window.\n");  
        fprintf(stderr, "\n");  
    }

```

#### 代码解析

> 这段代码是NX标准输出和标准错误重定向的示例。其主要功能如下：
>
> 1. 使用AllocConsole()创建控制台窗口，并将窗口标题设置为"NX Standard Output"。
> 2. 通过GetStdHandle()获取标准输出和标准错误句柄，然后使用_open_osfhandle()将句柄转换为文件描述符。
> 3. 通过_dup2()将文件描述符复制到1和2，分别对应标准输出和标准错误。这样可以将输出和错误重定向到控制台窗口。
> 4. 使用_setvbuf()设置无缓冲输出，以确保输出内容立即显示在控制台窗口中。
> 5. 通过fprintf()向标准输出和标准错误写入提示信息，确认输出和错误已重定向到控制台窗口。
> 6. 最后，关闭文件描述符，释放资源。
>
> 综上所述，这段代码的主要目的是将NX的标准输出和标准错误重定向到控制台窗口，以便于观察程序运行过程中的输出和错误信息。
>
