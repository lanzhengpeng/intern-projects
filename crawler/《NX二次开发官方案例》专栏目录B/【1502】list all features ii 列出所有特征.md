### 【1502】list all features ii 列出所有特征

#### 代码

```cpp
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_info_window.hxx>  
    #include <ug_feature.hxx>  
    #include <ug_iterator.hxx>  
    #include <uf.h>  
    #include <uf_ui.h>  
    static void do_it(void)  
    {  
        UgInfoWindow  info;  
        info.open();  
        for (UgIterator<UgFeatureObject> it; !it.isFinished(); it.findNext())  
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

> 这段代码是一个NX的二次开发示例，其主要功能是遍历当前NX部件中的所有特征，并将特征名称打印在信息窗口中。下面是代码的主要功能：
>
> 1. 包含了NX二次开发常用的头文件，如ug_exception.hxx、ug_session.hxx等。
> 2. 定义了do_it()函数，用于遍历当前NX部件的所有特征，并打印特征名称。
> 3. 在do_it()函数中，首先打开一个信息窗口，用于显示特征名称。
> 4. 使用UgIterator遍历所有特征，遍历过程中，使用info.write()函数将特征名称写入信息窗口。
> 5. ufusr()函数是NX二次开发的入口函数，其中try-catch用于捕获和处理异常。
> 6. 在ufusr()函数中，首先创建一个UgSession对象，用于管理NX会话。
> 7. 调用do_it()函数实现遍历和打印特征的功能。
> 8. 如果在执行过程中发生异常，捕获异常并打印异常信息。
>
> 这段代码实现了基本的NX特征遍历功能，通过信息窗口打印出特征名称，为后续的二次开发提供了基础。
>
