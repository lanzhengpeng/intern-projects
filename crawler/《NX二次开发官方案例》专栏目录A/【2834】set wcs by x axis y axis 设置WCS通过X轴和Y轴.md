### 【2834】set wcs by x axis y axis 设置WCS通过X轴和Y轴

#### 代码

```cpp
    /*HEAD SET_WCS_BY_X_AXIS_Y_AXIS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_mtx.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，以下是关键点的翻译：

1. UF_print_syslog是V18版本新增的功能。
2. 请只回答翻译，不要添加任何额外内容。

翻译结果：
UF_print_syslog是V18版本新增的功能。 */  
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
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
        else return FALSE;  
    }  
    static void set_wcs_by_x_axis_yaxis(double *x_axis, double *y_axis,  
        double *origin)  
    {  
        double  
            csys[9];  
        tag_t  
            mx,  
            wcs;  
        UF_CALL(UF_MTX3_initialize(x_axis, y_axis, csys));  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
    }  
    static void do_it(void)  
    {  
        double  
            junk[3],  
            origin[3],  
            x_axis[3],  
            y_axis[3];  
        while (specify_vector("Specify X-axis", x_axis, origin) &&  
               specify_vector("Specify Y-axis", y_axis, junk))  
           set_wcs_by_x_axis_yaxis(x_axis, y_axis, origin);  
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

> 这段代码是一个NX二次开发示例，其主要功能是根据用户指定的X轴和Y轴向量以及原点，来设置新的工作坐标系(WCS)。具体功能如下：
>
> 1. 使用UF_MTX3_initialize函数，根据用户指定的X轴和Y轴向量，计算出新的坐标系的方向余弦矩阵。
> 2. 使用UF_CSYS_create_matrix函数，基于方向余弦矩阵创建一个矩阵。
> 3. 使用UF_CSYS_create_temp_csys函数，基于原点坐标和方向余弦矩阵，创建一个临时坐标系。
> 4. 使用UF_CSYS_set_wcs函数，将临时坐标系设置为新的工作坐标系。
> 5. 提供了两个自定义函数：specify_vector用于获取用户指定的向量，set_wcs_by_x_axis_yaxis用于设置新的工作坐标系。
> 6. 主函数do_it中循环调用specify_vector来获取X轴和Y轴向量，然后调用set_wcs_by_x_axis_yaxis来设置新的工作坐标系。
> 7. 提供了ufusr和ufusr_ask_unload两个函数，分别用于NX的初始化、执行主体功能和卸载。
> 8. 使用UF_PRINT_SYSLOG函数输出错误信息到系统日志，从而提供错误跟踪功能。
>
> 综上所述，该代码实现了在NX中根据用户指定的向量设置新的工作坐标系的功能。
>
