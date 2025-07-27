### 【1510】list all reference sets 列出所有引用集

#### 代码

```cpp
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_info_window.hxx>  
    #include <ug_reference_set.hxx>  
    #include <ug_iterator.hxx>  
    #include <uf.h>  
    #include <uf_ui.h>  
    static void do_it(void)  
    {  
        UgInfoWindow  info;  
        info.open();  
        for (UgIterator<UgReferenceSet> it; !it.isFinished(); it.findNext())  
        {  
            info.write((*it)->getName());  
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

> 这是一段NX Open C++的二次开发代码，其主要功能是遍历NX当前会话中的所有引用集，并将它们的名称打印到NX信息窗口中。
>
> 代码主要包括以下几个部分：
>
> 1. 头文件包含：代码包含了一些NX Open C++的头文件，如ug_exception.hxx, ug_session.hxx等，用于定义NX Open C++的类和函数。
> 2. do_it函数：该函数遍历当前NX会话中的所有引用集，并打印它们的名称。它首先打开一个NX信息窗口，然后使用一个UgIterator遍历所有引用集，并打印每个引用集的名称。
> 3. ufusr函数：这是NX二次开发的标准入口函数。它首先创建一个NX会话，然后调用do_it函数执行实际操作。如果在操作过程中捕获到异常，它会打印异常信息到NX信息窗口。
> 4. 异常处理：代码使用了try-catch块来捕获并处理异常，这样可以避免程序因为异常而崩溃。
>
> 总体来说，这段代码通过遍历引用集并打印名称，实现了对NX会话中所有引用集的查询和展示。它使用NX Open C++提供的类和接口，展示了NX二次开发的基本流程。
>
