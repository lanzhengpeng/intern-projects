### 【3003】turn all points red 把所有点都变成红色

#### 代码

```cpp
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_iterator.hxx>  
    #include <ug_point.hxx>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <iostream.h>  
    static void do_it(void)  
    {  
        for (UgIterator<UgPoint> it; !it.isFinished(); it.findNext())  
        {  
            (*it)->setColor ( Red );  
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
            cout << oops.askErrorText().c_str() << endl;  
            return;  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发示例代码，主要功能是遍历NX中的所有点，并将它们的颜色设置为红色。
>
> 代码的主要逻辑包括：
>
> 1. 引入NX Open C++ API的头文件，如ug_exception.hxx、ug_session.hxx等。
> 2. 定义了一个do_it函数，用于遍历所有点并设置颜色。
> 3. do_it函数中使用UgIterator遍历所有点，通过findNext()方法获取下一个点，然后通过->操作符获取点的指针，并调用setColor(Red)方法设置颜色。
> 4. 在ufusr函数中，首先创建一个UgSession对象，表示与NX的会话。然后在try块中调用do_it函数，如果发生异常则捕获并打印错误信息。
> 5. ufusr函数是NX二次开发的入口函数，通过param参数接收命令行参数，通过retcode参数返回执行结果。
>
> 通过这段代码，可以实现遍历NX中的所有点，并将它们的颜色设置为红色，从而为NX的二次开发提供了颜色设置的示例。
>
