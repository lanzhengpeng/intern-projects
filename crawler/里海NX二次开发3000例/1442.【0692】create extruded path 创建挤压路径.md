### 【0692】create extruded path 创建挤压路径

#### 代码

```cpp
    /*HEAD CREATE_EXTRUDED_PATH CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档描述，UF_print_syslog是V18版本新增的函数。该函数的用途是打印系统日志。 */  
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
      int obj_cnt;  
      char *offsets[2] = {"0.0", "0.0"};  
      UF_FEATURE_SIGN mode_sign = UF_NULLSIGN;  
      tag_t line1, line2, line3, line4, guide, *objects;  
      tag_t generators[4];  
      UF_CURVE_line_t line_coords;  
      line_coords.start_point[0] = 0.0;  
      line_coords.start_point[1] = 0.0;  
      line_coords.start_point[2] = 0.0;  
      line_coords.end_point[0] = 0.0;  
      line_coords.end_point[1] = 1.0;  
      line_coords.end_point[2] = 0.0;  
      UF_CURVE_create_line(&line_coords,&line1);  
    /*line#1*/  
      line_coords.start_point[0] = 0.0;  
      line_coords.start_point[1] = 1.0;  
      line_coords.start_point[2] = 0.0;  
      line_coords.end_point[0] = 2.5;  
      line_coords.end_point[1] = 3.5;  
      line_coords.end_point[2] = 0.0;  
      UF_CURVE_create_line (&line_coords,&line2);  
    /*line#2*/  
      line_coords.start_point[0] = 2.5;  
      line_coords.start_point[1] = 3.5;  
      line_coords.start_point[2] = 0.0;  
      line_coords.end_point[0] = 1.0;  
      line_coords.end_point[1] = 0.0;  
      line_coords.end_point[2] = 0.0;  
      UF_CURVE_create_line(&line_coords,&line3);  
    /*line 3*/  
      line_coords.start_point[0] = 1.0;  
      line_coords.start_point[1] = 0.0;  
      line_coords.start_point[2] = 0.0;  
      line_coords.end_point[0] = 0.0;  
      line_coords.end_point[1] = 0.0;  
      line_coords.end_point[2] = 0.0;  
      UF_CURVE_create_line (&line_coords,&line4);  
    /* line 4 译:翻译line 4: The problem has been resolved.

仅回答译文：问题已解决。 */  
      generators[0] = line1;  
      generators[1] = line2;  
      generators[2] = line3;  
      generators[3] = line4;  
      line_coords.start_point[0] = 2.5;  
      line_coords.start_point[1] = 3.5;  
      line_coords.start_point[2] = 0.0;  
      line_coords.end_point[0] = 2.5;  
      line_coords.end_point[1] = 3.5;  
      line_coords.end_point[2] = 1.0;  
      UF_CURVE_create_line(&line_coords,&guide);  
    /*guide*/  
      UF_CALL(UF_MODL_create_extrusion_path(generators, 4, &guide, 1, NULL,  
                                    offsets, line_coords.start_point, FALSE, TRUE,  
                                    mode_sign, &objects, &obj_cnt ));  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这是段NX二次开发代码，主要功能是创建一个拉伸路径并生成拉伸体。以下是代码的主要流程：
>
> 1. 定义了错误报告函数report_error，用于打印错误信息。
> 2. 定义了主函数do_it，用于执行创建拉伸路径和生成拉伸体的操作。
> 3. 在do_it函数中，首先创建4条直线，作为拉伸路径的生成器。
> 4. 创建一条竖直线作为拉伸路径的引导线。
> 5. 调用UF_MODL_create_extrusion_path函数，传入生成器和引导线，生成拉伸路径并返回生成的拉伸体对象数组。
> 6. 在ufusr函数中，初始化NX，调用do_it函数，然后终止NX。
> 7. 定义ufusr_ask_unload函数，返回立即卸载标志，用于卸载二次开发应用程序。
>
> 总体来说，这段代码实现了在NX中通过编程方式创建拉伸路径和生成拉伸体的功能。
>
