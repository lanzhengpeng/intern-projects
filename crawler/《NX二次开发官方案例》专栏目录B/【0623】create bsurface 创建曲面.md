### 【0623】create bsurface 创建曲面

#### 代码

```cpp
    /*HEAD CREATE_BSURFACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：
注意：UF_print_syslog 是在 V18 中新增的，请仅提供翻译，不要添加任何无关内容。 */  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            jj = 0,  
            knot_fixup,  
            pole_fixup;  
        tag_t  
            bsurf,  
            point;  
        double  
            poles[] = { -7.0, 1.0, 0.0, 1.0,  
                        -6.0, 3.0, 0.0, 1.0,  
                        -5.0, 4.5, 0.0, 1.0,  
                        -6.0, 6.0, 0.0, 1.0,  
                        -8.0, 7.0, 0.0, 1.0,  
                        -7.0, 1.0, 4.0, 1.0,  
                        -6.0, 3.0, 4.0, 1.0,  
                        -5.0, 4.5, 4.0, 1.0,  
                        -6.0, 6.0, 4.0, 1.0,  
                        -8.0, 7.0, 4.0, 1.0 },  
            uknot[] = {0.0, 0.0, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0, 1.0},  
            vknot[] = {0.0, 0.0, 1.0, 1.0} ;  
        /* Plotting the points just so you can see where they are 译:翻译：仅为了让你看到它们的位置，才绘制了这些点。 */  
        for (ii = 0; ii < 10; ii++)  
        {  
            UF_CALL(UF_CURVE_create_point(&poles[jj], &point));  
            jj = jj + 4;  
        }  
        if (!UF_CALL(UF_MODL_create_bsurf(5, 2, 4, 2, uknot, vknot, poles, &bsurf,  
            &knot_fixup, &pole_fixup)))  
        {  
            printf("knot_fixup = %d\n", knot_fixup);  
            printf("pole_fixup = %d\n", pole_fixup);  
        }  
    }  
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

> 这段代码是一个用于创建NX B样条曲面的二次开发示例。以下是代码的主要功能介绍：
>
> 1. 包含了必要的NX Open头文件，如uf.h、uf_ui.h、uf_modl.h、uf_curve.h等。
> 2. 定义了错误报告函数report_error，用于输出函数调用错误信息。
> 3. 定义了主函数do_it，用于创建B样条曲面。
> 4. 在do_it中，首先创建了一系列的点，并使用这些点来创建B样条曲面。
> 5. 使用UF_MODL_create_bsurf函数创建了一个5x2的B样条曲面，其中u方向有4个控制点，v方向有2个控制点。
> 6. 定义了ufusr函数，作为NX的二次开发入口函数。在ufusr中初始化和终止NX，并调用do_it函数。
> 7. 定义了ufusr_ask_unload函数，用于设置模块卸载方式。
>
> 总的来说，这段代码的主要功能是创建一个简单的B样条曲面，并提供了错误处理和NX二次开发的通用框架。
>
