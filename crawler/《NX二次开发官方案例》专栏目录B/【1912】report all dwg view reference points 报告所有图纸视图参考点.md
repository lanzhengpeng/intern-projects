### 【1912】report all dwg view reference points 报告所有图纸视图参考点

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_curve.h>  
    #include <uf_obj.h>  
    #include <uf_view.h>  
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
    #define RON(X) (report_object_name(#X, X))  
    static void report_object_name(char *what, tag_t object)  
    {  
        char  
            object_name[MAX_ENTITY_NAME_SIZE+1];  
        if (!UF_OBJ_ask_name(object, object_name))  
            ECHO("%s = %s\n", what, object_name);  
        else  
            ECHO("%s = unnamed\n", what);  
    }  
    #define WRITENZ(X) if (X != 0) ECHO("%s = %d\n", #X, X);  
    static void do_it(void)  
    {  
        int  
            ii, jj,  
            n_dwgs,  
            n_views;  
        tag_t  
            anchor,  
            *dwgs,  
            *views;  
        double  
            dwgpt[2],  
            modelpt[3],  
            scale;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_DRAW_ask_drawings(&n_dwgs, &dwgs));  
        for (ii = 0; ii < n_dwgs; ++ii)  
        {  
            RON(dwgs[ii]);  
            UF_CALL(UF_DRAW_ask_views(dwgs[ii], &n_views, &views));  
            for (jj = 0; jj < n_views; ++jj)  
            {  
                RON(views[jj]);  
                UF_CALL(UF_DRAW_ask_drawing_ref_pt(views[jj], dwgpt));  
                ECHO(" UF_DRAW_ask_drawing_ref_pt = %f,%f\n", dwgpt[0], dwgpt[1]);  
                UF_CALL(UF_OBJ_ask_name(views[jj], view_name));  
                WRITENZ(uc6442(view_name, modelpt, &scale));  
                ECHO(" uc6442 = %f,%f,%f\n", modelpt[0], modelpt[1], modelpt[2]);  
                UF_CALL(UF_DRAW_ask_view_anchor(views[jj], &anchor));  
                if (anchor != NULL_TAG)  
                {  
                    UF_CALL(UF_CURVE_ask_point_data(anchor, modelpt));  
                    ECHO(" UF_DRAW_ask_view_anchor = %.3f,%.3f,%.3f\n",  
                        modelpt[0], modelpt[1], modelpt[2]);  
                }  
            }  
            if (n_views > 0) UF_free(views);  
        }  
        if (n_dwgs > 0) UF_free(dwgs);  
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

> 这段代码是一个NX Open的二次开发示例，主要功能是遍历当前文档的所有图纸和视图，并打印出相关信息。
>
> 主要函数和功能点包括：
>
> 1. ECHO- 用于打印信息到日志窗口和系统日志。
> 2. UF_CALL- 调用NX的API函数，并打印错误信息。
> 3. RON- 打印对象的名字。
> 4. WRITENZ- 打印非零整数值。
> 5. do_it- 主要功能函数，遍历所有图纸和视图，打印图纸和视图的名字、参照点、视图锚点等信息。
> 6. ufusr- NX的回调函数，初始化NX，调用do_it，然后终止NX。
> 7. ufusr_ask_unload- NX卸载时调用的函数。
>
> 这段代码主要用于演示如何遍历NX文档中的图纸和视图，并获取相关信息，为NX的二次开发提供了基础示例。
>
