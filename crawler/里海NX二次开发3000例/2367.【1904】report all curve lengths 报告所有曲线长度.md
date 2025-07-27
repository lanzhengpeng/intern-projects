### 【1904】report all curve lengths 报告所有曲线长度

#### 代码

```cpp
    /*HEAD REPORT_ALL_CURVE_LENGTHS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog是在V18中新加入的，请只回答译文，不要添加任何其他内容。 */  
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
    static tag_t ask_next_curve(tag_t part, tag_t curve, int *type)  
    {  
        int  
            types[4] = { UF_line_type,  
                         UF_circle_type,  
                         UF_conic_type,  
                         UF_spline_type };  
        while ((*type < 4) &&  
            !UF_CALL(UF_OBJ_cycle_objs_in_part(part, types[*type], &curve)))  
        {  
            if (curve != NULL_TAG)   
                return curve;  
            else   
                *type = *type + 1;  
        }  
        *type = 0;  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            type = 0;  
        tag_t  
            curve = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            length;  
        char  
            messg[MAX_LINE_SIZE+1];  
        UF_UI_open_listing_window();  
        while ((curve = ask_next_curve(part, curve, &type)) != NULL_TAG)  
        {  
            UF_CALL(UF_CURVE_ask_arc_length(curve, 0.0, 1.0, UF_MODL_UNITS_PART,  
                    &length));  
            sprintf(messg, "EID %d is %f long\n", curve, length);  
            UF_UI_write_listing_window(messg);  
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

> 这段代码是一个用于报告NX中所有曲线长度的UF程序。代码的主要功能包括：
>
> 1. 包含必要的头文件，并定义了UF_CALL宏来简化UF函数的调用和错误处理。
> 2. 定义了report_error函数，用于在UF函数调用失败时输出错误信息。
> 3. 定义了ask_next_curve函数，用于循环查询部件中的下一条曲线，并返回其类型。
> 4. 定义了do_it函数，用于遍历部件中的所有曲线，并调用UF_CURVE_ask_arc_length函数计算每条曲线的长度，然后输出长度信息。
> 5. ufusr函数是UF程序的入口点，用于初始化和调用do_it函数，以及终止UF。
> 6. ufusr_ask_unload函数用于设置UF程序的卸载模式。
>
> 总体来说，该代码实现了在NX中遍历所有曲线并输出其长度信息的功能，采用面向对象的方式组织代码，具有较好的可读性和可维护性。
>
