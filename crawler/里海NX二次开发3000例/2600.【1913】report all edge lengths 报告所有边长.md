### 【1913】report all edge lengths 报告所有边长

#### 代码

```cpp
    //  WARNING:  See PR 4140900 - this does NOT output the correct numbers  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_iterator.hxx>  
    #include <ug_edge.hxx>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <iostream.h>  
    #include <stdio.h>  
    static void do_it(void)  
    {  
        char  
            messg[133];  
        for (UgIterator<UgEdge> it; !it.isFinished(); it.findNext())  
        {  
            (*it)->highlight();  
            sprintf(messg, "ArcLength = %f", (*it)->computeArcLength());  
            uc1601(messg, TRUE);  
            (*it)->unhighlight();  
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

> 这是NX二次开发中的一个用户自定义函数(ufusr)。该函数的主要功能是遍历当前NX部件中的所有边，并计算每条边的弧长，然后显示在NX的用户界面中。
>
> 具体步骤包括：
>
> 1. 包含必要的NX二次开发头文件。
> 2. 定义一个静态函数do_it，用于遍历所有边并计算弧长。
> 3. 在do_it函数中，使用UgIterator遍历所有边，并对每条边执行以下操作：调用highlight()函数高亮显示边。调用computeArcLength()函数计算边的弧长。使用sprintf生成包含弧长信息的字符串。调用uc1601()函数将字符串显示在NX用户界面中。调用unhighlight()函数取消边的高亮显示。
> 4. 调用highlight()函数高亮显示边。
> 5. 调用computeArcLength()函数计算边的弧长。
> 6. 使用sprintf生成包含弧长信息的字符串。
> 7. 调用uc1601()函数将字符串显示在NX用户界面中。
> 8. 调用unhighlight()函数取消边的高亮显示。
> 9. 在ufusr函数中，创建一个UgSession对象，用于管理NX会话。在try块中调用do_it函数，在catch块中捕获并打印异常。
> 10. ufusr函数的参数和返回值符合NX二次开发的标准要求。
>
> 总的来说，该代码实现了在NX中遍历边并显示每条边的弧长，采用了NX二次开发的相关API，并进行了异常处理。
>
