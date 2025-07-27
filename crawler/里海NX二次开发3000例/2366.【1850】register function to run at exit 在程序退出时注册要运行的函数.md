### 【1850】register function to run at exit 在程序退出时注册要运行的函数

#### 代码

```cpp
    /*HEAD REGISTER_FUNCTION_TO_RUN_AT_EXIT CCC UFUN */  
    #include <stdio.h>  
    #include <uf_exit.h>  
    #ifdef _USRDLL  
    #include <windows.h>  
    #endif  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
    void MY_cleanup(void)  
    {  
        printf("In my_cleanup\n");  
    #ifdef _USRDLL  
        system("pause");  
    #endif  
    }  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
    #ifdef _USRDLL  
        AllocConsole();  
        freopen("conout$", "w", stdout);  
        setvbuf(stdout,NULL,_IONBF,2);  
    #endif  
        printf("In ufsta\n");  
        ECHO(atexit(MY_cleanup));  
    }

```

#### 代码解析

> 这是段NX Open C++的二次开发代码，主要包含以下几个关键部分：
>
> 1. 包含头文件：引入了stdio.h和uf_exit.h，分别用于标准输入输出和UF退出相关的函数声明。
> 2. 定义宏：ECHO用于打印变量名和值。
> 3. 定义清理函数：MY_cleanup用于在程序退出时执行清理工作，并打印提示信息。
> 4. UF状态函数：ufsta是用户自定义的UF状态函数，用于在NX启动时执行。它首先打开控制台输出，然后打印提示信息，并使用atexit函数注册了清理函数MY_cleanup，确保在NX退出时执行清理工作。
> 5. _USRDLL宏：通过判断是否定义了_USRDLL宏，来决定是否需要调用Windows API来打开控制台输出，并在函数末尾暂停。
> 6. 头文件uf_exit.h：提供了UF退出相关的函数声明。
>
> 综上所述，这段代码主要用于在NX启动和退出时执行自定义的打印和清理工作，通过UF状态函数实现。
>
