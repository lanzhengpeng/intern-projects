### 【2422】report view dependent objects in drawing and member views 在图纸和成员视图中报告视图相关对象

#### 代码

```cpp
    /*HEAD REPORT_VIEW_DEPENDENT_OBJECTS_IN_DRAWING_AND_MEMBER_VIEWS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_draw.h>  
    #include <uf_view.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的函数。 */  
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
    static tag_t ask_next_drawing(tag_t part, tag_t drawing)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &drawing));  
        return (drawing);  
    }  
    static void report_view_dependent_objects(char *view)  
    {  
        int  
            irc,  
            subtype,  
            type;  
        tag_t  
            object = NULL_TAG;  
        printf("view dependent objects in %s\n", view);  
        while (((irc = uc6457(view, 2, &object)) == 0) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            printf("  tag = %d, type = %d, subtype = %d\n", object, type, subtype);  
        }  
        if (irc) ECHO(irc);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            dwg = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            view[MAX_ENTITY_NAME_SIZE+1],  
            drawing[MAX_ENTITY_NAME_SIZE+1];  
        while ((dwg = ask_next_drawing(part, dwg)) && (dwg != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_name(dwg, drawing));  
            printf("drawing = %s\n", drawing);  
            sprintf(view, "%s@0", drawing);  
            report_view_dependent_objects(view);  
            strcpy(view, "");  
            while (!UF_CALL(uc6499(drawing, view)) && (strcmp(view, "")))  
                report_view_dependent_objects(view);  
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

> 这段代码是一个用于NX的二次开发示例，其主要功能是遍历当前部件中的所有绘图，并报告每个绘图中的视图依赖对象。
>
> 主要步骤包括：
>
> 1. 包含所需的NX API头文件。
> 2. 定义错误报告函数report_error，用于打印函数调用失败时的错误信息。
> 3. 定义函数ask_next_drawing，用于遍历部件中的绘图。
> 4. 定义函数report_view_dependent_objects，用于报告指定视图中的视图依赖对象。
> 5. 定义函数do_it，遍历当前部件中的所有绘图，并报告每个绘图中的视图依赖对象。
> 6. 定义ufusr函数，用于在NX中初始化和执行do_it。
> 7. 定义ufusr_ask_unload函数，用于卸载用户函数。
>
> 总体来说，该代码利用NX的图形和视图API，实现了遍历绘图并报告视图依赖对象的功能。
>
