### 【0161】ask boundary curves 询问边界曲线

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_draw.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITENZ(X) if (X != 0) ECHO("%s = %d\n", #X, X);  
    static tag_t select_drawing_member_view(char *prompt)  
    {  
        int  
            orig_cv,  
            resp;  
        tag_t  
            view_tag;  
        double  
            pos[3];  
        UF_CALL(UF_UI_ask_cursor_view(&orig_cv));  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_specify_screen_position(prompt, NULL, NULL, pos,  
            &view_tag, &resp));  
        UF_CALL(UF_UI_set_cursor_view(orig_cv));  
        if (resp == UF_UI_PICK_RESPONSE)  
            return view_tag;  
        else  
            return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            num_crvs = 0;  
        tag_t  
            topView = NULL_TAG;  
        UF_DRAW_view_boundary_t    
                *curves = NULL;  
        double   
            tolerance = 0.;  
        topView = select_drawing_member_view("Select drawing member view");  
        if( NULL_TAG == topView ) return;  
        UF_CALL( UF_DRAW_ask_boundary_curves( topView, &tolerance, &num_crvs, &curves ));  
        WRITE_D( num_crvs );  
        if( curves != NULL ) UF_free( curves );  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize()))   
        {  
            WRITE( "\n\nIs this program signed?\n\n" );  
            return;  
        }  
        else  
        {  
            do_it();  
        }  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一段用于NX二次开发的示例代码，主要功能包括：
>
> 1. 包含了NX二次开发常用的头文件，例如uf.h, uf_draw.h等。
> 2. 定义了一个ECHO宏，用于在列表窗口和系统日志中输出信息。
> 3. 定义了一个UF_CALL宏，用于调用NX的UF函数，并在出错时报告错误信息。
> 4. 定义了一个select_drawing_member_view函数，用于让用户选择一个绘图成员视图。
> 5. 定义了一个do_it函数，用于执行以下操作：调用select_drawing_member_view获取用户选择的视图。调用UF_DRAW_ask_boundary_curves获取视图的边界曲线。输出边界曲线的数量。
> 6. 调用select_drawing_member_view获取用户选择的视图。
> 7. 调用UF_DRAW_ask_boundary_curves获取视图的边界曲线。
> 8. 输出边界曲线的数量。
> 9. 定义了ufusr函数，作为NX二次开发的入口函数，在初始化NX后调用do_it函数执行主要逻辑，在结束时终止NX。
> 10. 定义了一个ufusr_ask_unload函数，用于卸载NX二次开发环境。
>
> 总的来说，这段代码展示了NX二次开发的基本框架，包括错误处理、视图选择和边界曲线获取等常用操作。
>
