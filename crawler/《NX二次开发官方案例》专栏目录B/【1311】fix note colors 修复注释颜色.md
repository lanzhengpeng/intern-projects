### 【1311】fix note colors 修复注释颜色

#### 代码

```cpp
    /*HEAD FIX_NOTE_COLORS CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新功能，用于打印系统日志。 */  
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
        tag_t  
            a_note = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_OBJ_disp_props_t  
            disp_props;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type,  
            &a_note)) && a_note)  
        {  
            UF_CALL(UF_OBJ_ask_display_properties(a_note, &disp_props));  
            UF_CALL(UF_OBJ_set_color(a_note, disp_props.color));  
        }  
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

> 这段代码是一个NX Open的二次开发示例，其主要功能是遍历当前显示部件中的所有草绘实体，并设置它们的颜色为系统默认颜色。
>
> 代码的主要组成部分包括：
>
> 1. 包含头文件：引入了NX Open的通用头文件，如uf.h、uf_obj.h、uf_object_types.h、uf_part.h等，以使用NX Open API。
> 2. UF_CALL宏：用于调用NX Open函数，并捕获错误码。如果函数返回错误码，会打印错误信息。
> 3. report_error函数：用于打印错误信息，包括错误码、调用函数、出错文件名和行号等。它还使用了UF_print_syslog函数，这是一个V18版本新增的函数，用于打印系统日志。
> 4. do_it函数：遍历当前显示部件中的所有草绘实体，并设置它们的颜色为系统默认颜色。
> 5. ufusr函数：初始化NX Open，调用do_it函数，然后终止NX Open。
> 6. ufusr_ask_unload函数：用于卸载用户程序。
>
> 总体而言，这段代码实现了遍历当前显示部件中的所有草绘实体，并设置它们的颜色为系统默认颜色的功能。
>
