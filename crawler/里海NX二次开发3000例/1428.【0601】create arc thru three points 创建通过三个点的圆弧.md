### 【0601】create arc thru three points 创建通过三个点的圆弧

#### 代码

```cpp
/*HEAD CREATE_ARC_THRU_THREE_POINTS.CPP CCC UFUN */
#include <ug_exception.hxx>
#include <ug_session.hxx>
#include <point3.hxx>
#include <ug_arc.hxx>
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
    double
        loc1[3],
        loc2[3],
        loc3[3];
    while (specify_point("Define point 1", loc1) &&
           specify_point("Define point 2", loc2) &&
           specify_point("Define point 3", loc3))
    {
        UgArc *an_arc =
            UgArc::create(FALSE, (Point3)loc1, (Point3)loc2, (Point3)loc3);
        an_arc->setColor(Red);
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

> 这段代码是一个NX Open C++程序，用于通过三个点创建一个圆弧。以下是代码的主要功能：
>
> 1. 函数定义：定义了specify_point函数，用于提示用户输入一个点的坐标，并返回用户是否成功输入。
> 2. 主函数：在do_it函数中，使用specify_point函数提示用户输入三个点的坐标，然后创建一个通过这三个点的圆弧，并设置其颜色为红色。循环继续直到用户取消输入。
> 3. 用户函数：定义了ufusr函数，这是NX Open的入口函数。它初始化一个UgSession，并调用do_it函数。如果发生异常，则捕获异常并显示错误信息。
> 4. 主要逻辑：通过循环提示用户输入三个点，创建圆弧，直到用户取消输入。圆弧的颜色设置为红色。
> 5. 错误处理：使用异常处理机制，在捕获异常时显示错误信息。
> 6. NX Open API：使用了NX Open C++ API来创建圆弧、设置颜色等，如UgArc、Point3、UgSession等。
> 7. 交互式输入：通过交互式提示用户输入点坐标，实现用户交互。
> 8. 二次开发：通过二次开发扩展了NX的功能，使用户可以通过简单输入创建圆弧。
>
> 总之，这段代码通过交互式输入创建一个通过三个点的圆弧，并提供了错误处理，是NX Open C++的一个典型二次开发示例。
>
