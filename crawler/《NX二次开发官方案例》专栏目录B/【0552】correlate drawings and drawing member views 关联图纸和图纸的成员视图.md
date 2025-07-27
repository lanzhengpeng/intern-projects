### 【0552】correlate drawings and drawing member views 关联图纸和图纸的成员视图

#### 代码

```cpp
    /*HEAD CORRELATE_DRAWINGS_AND_DRAWING_MEMBER_VIEWS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本新增的函数。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static tag_t ask_next_drawing(tag_t part, tag_t drawing)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &drawing));  
        return (drawing);  
    }  
    static tag_t ask_next_view(tag_t part, tag_t view)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_view_type, &view));  
        return (view);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_views;  
        tag_t  
            drawing = NULL_TAG,  
            part = UF_PART_ask_display_part(),  
            view = NULL_TAG,  
            *views;  
        char  
            name[36],    
            msg[132] = { "" };  
        while ((drawing = ask_next_drawing(part, drawing)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(drawing, name));  
            sprintf( msg, "%s\n", name);  
            WRITE( msg );  
            UF_CALL(UF_DRAW_ask_views(drawing, &n_views, &views));  
            for (ii = 0; ii < n_views; ii++)  
            {  
                UF_CALL(UF_OBJ_ask_name(views[ii], name));  
                sprintf( msg, "  %s\n", name);  
                WRITE( msg );  
            }  
            if (n_views > 0) UF_free(views);  
        }  
        while ((view = ask_next_view(part, view)) != NULL_TAG)  
        {  
            if (!UF_DRAW_ask_drawing_of_view(view, &drawing)  
                && (drawing != NULL_TAG))  
            {  
                UF_CALL(UF_OBJ_ask_name(view, name));  
                sprintf( msg, "%s -> ", name);  
                WRITE( msg );  
                UF_CALL(UF_OBJ_ask_name(drawing, name));  
                sprintf( msg, "%s\n", name);  
                WRITE( msg );  
            }  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX二次开发示例，用于实现以下功能：
>
> 1. 初始化NX系统。
> 2. 获取当前显示的部件，并遍历其中的所有图纸。
> 3. 对于每个图纸，获取图纸名称，并遍历图纸中的所有视图。
> 4. 获取每个视图的名称，并打印图纸名称和视图名称。
> 5. 遍历部件中的所有视图，并获取每个视图对应的图纸名称，打印视图名称和对应的图纸名称。
> 6. 终止NX系统。
>
> 代码的关键点包括：
>
> 这段代码的目的是展示如何遍历部件中的图纸和视图，并获取它们的名称，打印出来。
>
