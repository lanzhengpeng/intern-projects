### 【2864】show current view corners 显示当前视图的角点

#### 代码

```cpp
    /*HEAD SHOW_CURRENT_VIEW_CORNERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
    #include <uf_trns.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

注意：UF_print_syslog 是 V18 版本新增的功能，请只回答翻译内容，不要添加其他无关内容。 */  
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
    static void map_current_view2abs(double *c)  
    {  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            mx[12],  
            scale,  
            vw[12] = { 0,0,0 };  
        int  
            irc;  
        uc6430("", vw, &scale);  
        uc6433("", &vw[3]);  
        FTN(uf5940)(vw, abs, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void ask_current_view_corners(double corners[4][3])  
    {  
        double  
            bounds[4];  
        uc6450("", "", 0, 0);  
        UF_CALL(UF_VIEW_ask_xy_clip(NULL_TAG, bounds));  
        corners[0][0] = bounds[0];  
        corners[0][1] = bounds[2];  
        corners[0][2] = 0;  
        map_current_view2abs(&corners[0][0]);  
        corners[1][0] = bounds[1];  
        corners[1][1] = bounds[2];  
        corners[1][2] = 0;  
        map_current_view2abs(&corners[1][0]);  
        corners[2][0] = bounds[1];  
        corners[2][1] = bounds[3];  
        corners[2][2] = 0;  
        map_current_view2abs(&corners[2][0]);  
        corners[3][0] = bounds[0];  
        corners[3][1] = bounds[3];  
        corners[3][2] = 0;  
        map_current_view2abs(&corners[3][0]);  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        double  
            corners[4][3];  
        UF_OBJ_disp_props_t  
            color = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        ask_current_view_corners(corners);  
        for (ii = 0; ii < 4; ii++)  
            UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,  
                UF_DISP_USE_WORK_VIEW, &corners[ii][0], &color, UF_DISP_ASTERISK));  
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

> 这段NX二次开发代码的主要功能是显示当前视图的四个角点。以下是该代码的主要逻辑：
>
> 1. 包含头文件：代码中包含了必要的NX Open API头文件，如uf.h、uf_ui.h、uf_disp.h、uf_view.h、uf_obj.h和uf_trns.h。
> 2. 错误报告函数：定义了report_error函数，用于打印错误信息到系统日志窗口。
> 3. 视图变换函数：定义了map_current_view2abs函数，用于将视图坐标系中的点转换到绝对坐标系。
> 4. 获取当前视图角点：定义了ask_current_view_corners函数，用于获取当前视图的四个角点坐标，并转换到绝对坐标系。
> 5. 显示角点：定义了do_it函数，用于显示当前视图的四个角点。首先调用ask_current_view_corners函数获取角点坐标，然后调用UF_DISP_display_temporary_point函数在视图上显示这些点。
> 6. ufusr入口函数：定义了ufusr函数作为NX二次开发的入口函数，在初始化后调用do_it函数，然后终止。
> 7. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来看，该代码通过获取并显示当前视图的四个角点坐标，实现了对当前视图范围的直观展示。
>
