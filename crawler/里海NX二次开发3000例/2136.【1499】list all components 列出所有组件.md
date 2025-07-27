### 【1499】list all components 列出所有组件

#### 代码

```cpp
    /*HEAD LIST_ALL_FEATURES.CPP CCC UFUN */  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_assembly_node.hxx>  
    #include <ug_info_window.hxx>  
    #include <ug_iterator.hxx>  
    #include <uf.h>  
    #include <uf_ui.h>  
    static void do_it()  
    {  
        UgInfoWindow  info;  
        info.open();  
        for ( UgIterator <UgAssemblyNode> it; !it.isFinished(); it.findNext() )  
        {  
            info.write((*it)->getRelatedName());  
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

> 这段代码是一个NX二次开发示例，主要功能是遍历NX装配树，并显示每个节点的名称。具体来说，代码实现了以下功能：
>
> 1. 包含了必要的NX Open头文件，如ug_exception.hxx、ug_session.hxx等。
> 2. 定义了一个静态函数do_it()，用于遍历NX装配树并显示每个节点的名称。
> 3. do_it()函数中，首先创建一个UgInfoWindow对象，用于显示信息。然后使用UgIterator遍历装配树，迭代器类型为UgAssemblyNode。在循环中，获取当前节点的名称并显示在info窗口中。
> 4. 定义了ufusr()函数，这是NX二次开发的标准入口函数。函数中创建了一个UgSession对象，并调用do_it()函数。
> 5. ufusr()函数中的异常处理部分使用try-catch捕获UgException异常，并在捕获异常时显示错误信息。
> 6. ufusr()函数的参数param、retcode和paramLen是NX二次开发标准参数，但在此示例中并未使用。
> 7. 该代码是一个典型的NX Open二次开发示例，通过遍历装配树并显示节点名称，展示了NX Open提供的UgIterator迭代器、UgSession会话、UgInfoWindow信息窗口等的使用方法。
>
