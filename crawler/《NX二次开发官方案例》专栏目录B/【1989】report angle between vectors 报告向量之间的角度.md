### 【1989】report angle between vectors 报告向量之间的角度

#### 代码

```cpp
/*HEAD REPORT_ANGLE_BETWEEN_VECTORS CCC UFUN */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_vec.h>
#define ECHOF(X)   (printf("%s = %f\n", #X, X))
#define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))
static int report_error( char *file, int line, char *call, int irc)
{
    if (irc)
    {
        char err[133],
             msg[133];
        sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",
            irc, line, file);
        UF_get_fail_message(irc, err);
    /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中的新功能，用于在日志文件中记录系统事件，用户不需要回复任何废话，只需翻译即可。 */
        UF_print_syslog(msg, FALSE);
        UF_print_syslog(err, FALSE);
        UF_print_syslog("\n", FALSE);
        UF_print_syslog(call, FALSE);
        UF_print_syslog(";\n", FALSE);
        if (!UF_UI_open_listing_window())
        {
            UF_UI_write_listing_window(msg);
            UF_UI_write_listing_window(err);
            UF_UI_write_listing_window("\n");
            UF_UI_write_listing_window(call);
            UF_UI_write_listing_window(";\n");
        }
    }
    return(irc);
}
static void display_conehead(double origin[3], double vec[3])
{
    UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);
}
static void do_it(void)
{
    double
        big,
        minus_45[3] = { 1, -1, 0 },
        origin[3] = { 0, 0, 0 },
        plus_45[3] = { 1, 1, 0 },
        plus_x[3] = { 1, 0, 0 },
        plus_z[3] = { 0, 0, 1 },
        small;
    display_conehead(origin, plus_x);
    display_conehead(origin, plus_45);
    display_conehead(origin, minus_45);
    UF_VEC3_angle_between(plus_x, plus_45, plus_z, &small);
    ECHOF(small * RADEG);
    UF_VEC3_angle_between(plus_x, minus_45, plus_z, &big);
    ECHOF(big * RADEG);
}
/* qq3123197280 */
void ufusr(char *param, int *retcode, int paramLen)
{
    if (UF_CALL(UF_initialize())) return;
    do_it();
    UF_terminate();
}
int ufusr_ask_unload(void)
{
    return (UF_UNLOAD_IMMEDIATELY);
}

```

#### 代码解析

> 这段代码是一个NX Open C++用户函数，用于计算并显示两个向量之间的角度。
>
> 主要功能包括：
>
> 1. 使用UF_DISP_conehead函数在三个视图上显示圆锥头，分别代表三个向量。
> 2. 使用UF_VEC3_angle_between函数计算两个向量之间的角度。
> 3. 将角度转换为度数，并打印输出。
> 4. 使用宏ECHOF打印变量值。
> 5. 使用宏UF_CALL报告函数调用错误。
> 6. 使用函数report_error记录错误信息到日志文件和列表窗口。
> 7. 定义了函数do_it作为主要功能函数。
> 8. 定义了ufusr作为主函数。
> 9. 定义了ufusr_ask_unload用于卸载用户函数。
> 10. 使用RADEG宏将弧度转换为度数。
>
> 总体来说，这段代码通过计算两个向量之间的角度，并以度数形式打印输出，实现了计算并显示向量之间角度的功能。
>
