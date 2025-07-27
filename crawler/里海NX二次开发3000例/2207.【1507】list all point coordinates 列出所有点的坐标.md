### 【1507】list all point coordinates 列出所有点的坐标

#### 代码

```cpp
    /*HEAD LIST_ALL_POINT_COORDINATES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
    static tag_t ask_next_point(tag_t part, tag_t point)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_point_type, &point));  
        return (point);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            point = NULL_TAG;  
        double  
            pos[3];  
        char  
            messg[MAX_LINE_SIZE+1],  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_UI_open_listing_window();  
        while ((point = ask_next_point(part, point)) != NULL_TAG)  
        {  
            UF_CALL(UF_CURVE_ask_point_data(point, pos));  
            if (!UF_OBJ_ask_name(point, name))  
                sprintf(messg, "%f\t%f\t%f\t(%s)\n", pos[0], pos[1], pos[2], name);  
            else  
                sprintf(messg, "%f\t%f\t%f\n", pos[0], pos[1], pos[2]);  
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

> 这是段NX二次开发代码，主要功能是遍历当前显示部件中的所有点，获取它们的坐标，并将坐标输出到日志窗口。
>
> 代码主要包含以下几个部分：
>
> 1. 头文件：包含了必要的NX API头文件，如uf.h、uf_ui.h等。
> 2. 错误处理宏：定义了UF_CALL宏，用于在函数调用后检查返回码，如果有错误，则输出错误信息到日志窗口。
> 3. ask_next_point函数：用于遍历部件中的点，返回下一个点的tag。
> 4. do_it函数：遍历部件中的所有点，获取每个点的坐标，并输出到日志窗口。
> 5. ufusr函数：是NX二次开发的标准入口函数，在这里初始化NX API，调用do_it函数，然后终止NX API。
> 6. ufusr_ask_unload函数：用于在卸载用户函数时返回立即卸载标志。
>
> 整体来看，这段代码实现了遍历部件中的所有点并输出坐标的功能。通过日志窗口的输出，用户可以查看每个点的位置信息。
>
