### 【3075】update all drawings member views from grip 更新所有图纸的成员视图

#### 代码

```cpp
    /*  There is no GRIP method to update drawing member views.  The following is a very  
        simple program which will update all drawings out of date member views.  It can  
        be called from a GRIP program using:  
        XSPAWN/UFUN, 'C:\path\to\this\prog.dll'  
        HALT  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_drf.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static tag_t ask_next_drawing(tag_t part, tag_t drawing)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &drawing));  
        return (drawing);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            drawing = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            dname[MAX_ENTITY_NAME_SIZE+1];  
        while ((drawing = ask_next_drawing(part, drawing)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(drawing, dname));  
            UF_CALL(UF_DRF_update_views(dname, UF_DRF_UPDATE_FORCE, NULL));  
        }  
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

> 这段代码是一个简单的NX二次开发程序，用于更新NX部件中的所有绘图视图。具体功能包括：
>
> 1. 初始化和终止NX环境：代码首先调用UF_initialize()和UF_terminate()来初始化和终止NX环境。
> 2. 遍历部件中的所有绘图：通过调用UF_OBJ_cycle_objs_in_part()来遍历指定部件中的所有绘图对象。
> 3. 获取绘图名称：通过调用UF_OBJ_ask_name()获取当前遍历到的绘图的名称。
> 4. 更新绘图视图：调用UF_DRF_update_views()来强制更新指定绘图的所有视图，以确保绘图中的视图是最新的。
> 5. 错误处理：使用宏UF_CALL来捕获函数调用返回的错误码，并输出错误信息。
> 6. 程序入口点：定义了ufusr()作为程序的入口点，NX会调用这个函数来执行程序。
> 7. 立即卸载：定义了ufusr_ask_unload()函数，返回UF_UNLOAD_IMMEDIATELY表示程序执行完后立即卸载。
>
> 总体来说，这个程序通过遍历部件中的绘图并更新每个绘图的视图，以确保绘图显示最新内容，适用于NX二次开发。
>
