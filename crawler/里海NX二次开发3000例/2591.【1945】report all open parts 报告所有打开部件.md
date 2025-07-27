### 【1945】report all open parts 报告所有打开部件

#### 代码

```cpp
    /*HEAD REPORT_ALL_OPEN_PARTS.CPP CCC UFUN */  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_iterator.hxx>  
    #include <ug_part.hxx>  
    #include <ug_info_window.hxx>  
    #include <iostream.h>  
    static void do_it(void)  
    {  
        UgInfoWindow  info;  
        info.open();  
        for (UgIterator<UgPart> it; !it.isFinished(); it.findNext())  
        {  
            info.write((*it)->getFullName());  
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

> 这是段NX二次开发代码，主要功能是遍历当前NX会话中所有打开的部件并输出它们的名称。以下是代码的主要功能：
>
> 1. 引入了NX Open C++ API中的异常处理、会话、迭代器、部件和信息窗口等类。
> 2. 定义了一个静态函数do_it，用于遍历所有打开的部件并输出它们的名称。
> 3. 在ufusr函数中，首先创建一个会话对象my_session，然后调用do_it函数执行遍历操作。
> 4. 使用try-catch捕获异常，如果出现异常，则通过信息窗口输出异常信息。
> 5. ufusr函数是NX Open的二次开发入口函数，param和retcode参数会被NX Open自动填充，paramLen参数暂时未使用。
> 6. 整个代码通过NX Open API遍历并输出所有打开部件的名称，以实现报告所有打开部件的功能。
>
