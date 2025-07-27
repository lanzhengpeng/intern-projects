### 【2863】show csys of selected object 显示选定对象的坐标系

#### 代码

```cpp
    /*HEAD SHOW_CSYS_OF_SELECTED_OBJECT CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_csys.h>  
    #define ECHO3F(X)  (printf("%s = %f, %f, %f\n", #X, *X,*(X+1),*(X+2)))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，用于在用户空间打印系统日志。 */  
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
    static tag_t select_point_near_anything(char *prompt, double cp[3])  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
    }  
    static void show_objects_csys(tag_t object, double where[3])  
    {  
        tag_t  
            mx_id;  
        double  
            mx[9];  
        if (!UF_CALL(UF_CSYS_ask_matrix_of_object(object, &mx_id)))  
        {  
            UF_CALL(UF_CSYS_ask_matrix_values(mx_id, mx));  
            display_conehead(where, &mx[0]);  
            display_conehead(where, &mx[3]);  
            display_conehead(where, &mx[6]);  
            ECHO3F(&mx[0]);  
            ECHO3F(&mx[3]);  
            ECHO3F(&mx[6]);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        double  
            pos[3];  
        while ((object = select_point_near_anything("Select object", pos))  
            != NULL_TAG) show_objects_csys(object, pos);  
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

> 这段代码是用于NX的二次开发，主要实现了以下功能：
>
> 1. 包含了标准库文件，用于调用NX提供的API函数。
> 2. 定义了宏函数ECHO3F，用于打印3个浮点数。
> 3. 定义了宏函数UF_CALL，用于调用NX的API函数，并检查错误，如果发生错误，会打印错误信息。
> 4. 定义了report_error函数，用于处理API调用返回的错误码，并打印错误信息。
> 5. 定义了select_point_near_anything函数，用于提示用户选择一个对象，并返回其标签和位置。
> 6. 定义了display_conehead函数，用于在指定位置显示一个圆锥头，用于表示方向。
> 7. 定义了show_objects_csys函数，用于显示指定对象的坐标系，通过显示3个互相垂直的圆锥头来表示。
> 8. 定义了do_it函数，循环让用户选择对象，并显示其坐标系。
> 9. 定义了ufusr函数，是二次开发的入口函数，用于初始化NX，执行主功能，然后终止NX。
> 10. 定义了ufusr_ask_unload函数，用于在卸载用户二次开发程序时立即卸载。
>
> 整体来说，这段代码实现了在NX中循环让用户选择对象，然后显示其坐标系的功能，通过圆锥头来表示坐标系的方向。
>
