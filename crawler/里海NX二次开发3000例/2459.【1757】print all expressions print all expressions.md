### 【1757】print all expressions print all expressions

#### 代码

```cpp
    /*HEAD PRINT_ALL_EXPRESSIONS.CXX CCC UFUN */  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_info_window.hxx>  
    #include <ug_expression.hxx>  
    #include <ug_iterator.hxx>  
    #include <uf.h>  
    #include <uf_ui.h>  
    static void do_it(void)  
    {  
        UgInfoWindow  info;  
        info.open();  
        for (UgIterator<UgExpression> it; !it.isFinished(); it.findNext())  
        {  
            info.write((*it)->getLHS());  
            info.write(" = ");  
            info.write((*it)->getRHS());  
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

> 这段代码是NX Open C++的二次开发示例代码，其主要功能是遍历当前NX会话中的所有表达式，并打印每个表达式的左右两侧。
>
> 具体来说：
>
> 1. 包含了NX Open C++的几个核心头文件，如ug_exception.hxx, ug_session.hxx等，用于进行NX二次开发。
> 2. 定义了静态函数do_it，该函数负责遍历当前会话中的所有表达式，并打印每个表达式的左右两侧。
> 3. 在主函数ufusr中，首先创建了一个UgSession对象，表示当前的NX会话。
> 4. 然后调用do_it函数，遍历表达式并打印。
> 5. 使用try-catch捕获可能抛出的UgException异常，并打印错误信息。
> 6. ufusr函数是NX Open C++规定的主函数入口，param和retcode参数由NX调用时传入，paramLen为param的长度。
> 7. 代码首先打开一个信息窗口，然后遍历当前会话中的所有表达式，对于每个表达式，获取其左右两侧并打印出来。
> 8. 最后关闭信息窗口。
>
> 总的来说，这段代码展示了如何使用NX Open C++遍历表达式并打印其信息，是一个典型的NX二次开发示例。
>
