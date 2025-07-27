### 【0855】create red points 在CAD程序中创建红色点

#### 代码

```cpp
    #include <iostream.h>  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_iterator.hxx>  
    #include <point3.hxx>  
    #include <ug_point.hxx>  
    #include <ug_info_window.hxx>  
    #include <uf.h>  
    #include <uf_ui.h>  
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        UgPoint *pPoint;  
        double  loc[3];  
        while (specify_point("Define point", loc))  
        {  
            pPoint = UgPoint::create ( (Point3) loc );  
            pPoint->setColor(Red);  
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

> 这段代码是用于NX的二次开发，主要功能是让用户交互式地输入点的位置，并在图形窗口中创建一个红色的点。下面是该代码的主要功能：
>
> 1. 引入相关头文件：包括UG NX的异常处理、会话、迭代器、点类、信息窗口和UF功能等头文件。
> 2. 定义交互式输入点函数：specify_point函数通过UF功能接收用户输入的点坐标，并返回布尔值表示是否输入了有效的点。
> 3. 定义创建点的主函数：do_it函数循环调用specify_point函数获取用户输入的点坐标，然后创建一个红色的UG点对象。
> 4. 定义UF函数：ufusr函数是NX调用的主函数。它创建一个UG会话，调用do_it函数执行点创建操作。如果发生异常，则弹出信息窗口显示错误信息。
> 5. 使用异常处理：通过try-catch块捕获并处理可能发生的异常，从而提高了代码的健壮性。
>
> 总体而言，这段代码通过交互式输入创建图形点，并且包含了必要的错误处理，为NX二次开发提供了一个基础的示例。
>
