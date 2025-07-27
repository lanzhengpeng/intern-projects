### 【1520】list current search directories 列出当前的搜索目录

#### 代码

```cpp
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_info_window.hxx>  
    #include <ug_info_window.hxx>  
    #include <assembly_options.hxx>  
    #include <iostream.h>  
    #include <uf.h>  
    static void do_it(void)  
    {  
        int  
            ii;  
        AssemblyOptions  
            current;  
        UgInfoWindow  
            info;  
        std::vector < SearchDirectory >  
            theSearchDirs;  
        info.open();  
        theSearchDirs = current.getSearchDirectories();  
        for (ii = 0; ii < theSearchDirs.size(); ii++)  
        {  
            info.write(theSearchDirs[ii].getDirectoryPath());  
            info.write("\n");  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        try  
        {  
            UgSession my_session(true);  
            do_it();  
        }  
        catch (UgException oops)  
        {  
            UgInfoWindow  info;  
            info.open();  
            info.write(oops.askErrorText());  
            info.write("\n");  
            return;  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++的一个示例，实现了以下功能：
>
> 1. 定义了一个名为do_it的函数，用于获取当前装配选项中的搜索路径，并在信息窗口中显示出来。
> 2. 在ufusr函数中，创建一个UgSession对象，并调用do_it函数。如果在do_it函数中抛出异常，则捕获异常并在信息窗口中显示错误信息。
> 3. 使用了NX Open C++的UgSession、UgInfoWindow、AssemblyOptions等类，用于获取装配选项信息和在NX中打开信息窗口。
> 4. 使用了标准库中的vector和string，用于存储和操作搜索路径字符串。
> 5. 使用了异常处理机制，提高了代码的健壮性。
> 6. ufusr函数作为NX Open C++的UF函数，可以被NX调用执行，实现了与NX的交互。
>
> 这段代码是一个典型的NX Open C++二次开发示例，实现了NX软件装配模块的一个小功能。
>
