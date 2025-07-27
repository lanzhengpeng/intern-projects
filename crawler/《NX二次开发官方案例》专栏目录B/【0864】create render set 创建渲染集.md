### 【0864】create render set 创建渲染集

#### 代码

```cpp
    /*HEAD CREATE_RENDER_SET_AND_ADD_ALL_BODIES CCC UFUN */  
    /* Include files 译:Include files翻译为“包含文件”。 */  
    #include <stdarg.h>  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_defs.h>  
    #include <uf_disp.h>  
    #include <uf_draw_types.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[1025];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), 1024, format, args);  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                ECHO("Body Tag:%d, type: %d, subtype: %d", body, type, subtype);  
                if (UF_ASSEM_is_occurrence(body)) ECHO(", is occurrence.");  
                ECHO("\n\n");  
                return body;  
            }  
        }  
        return NULL_TAG;  
    }  
    /*****************************************************************************  
    **  Activation Methods  
    *****************************************************************************/  
    /*  Explicit Activation  
    **      This entry point is used to activate the application explicitly, as in  
    **      "File->Execute UG/Open->User Function..." */  
    extern DllExport void ufusr( char *parm, int *returnCode, int rlen )  
    {  
        UF_DRAW_render_prefs_t   
            render_parms;  
        tag_t   
            part = NULL_TAG,  
            body = NULL_TAG,  
            my_render_set = NULL_TAG;  
        /* Initialize the API environment 译:初始化API环境 */  
        if( UF_CALL(UF_initialize()) )   
        {  
            /* Failed to initialize 译:Failed to initialize */  
            return;  
        }  
        /* TODO: Add your application code here 译:翻译TODO: Add your application code here */  
        part = UF_PART_ask_display_part();  
        // Fails, see PR-1999319  
        //UF_CALL( UF_DRAW_ask_render_set_parms( NULL_TAG, &render_parms ) );  
        render_parms.hidden_line  = UF_DRAW_hidden_line_removal_on;  
        render_parms.hidden_line_color  = 0;  
        render_parms.hidden_line_font  = 0;  
        render_parms.hidden_line_width  = UF_OBJ_WIDTH_NORMAL;  
        render_parms.edge_hiding_edge = UF_DRAW_edge_hiding_edge_off;  
        render_parms.visible_line_color  = 0;  
        render_parms.visible_line_font  = 0;  
        render_parms.visible_line_width  = UF_OBJ_WIDTH_NORMAL;  
        render_parms.referenced_edges_only  = FALSE;  
        render_parms.edges_hidden_by_own_solid = TRUE;  
        UF_CALL( UF_DRAW_create_render_set("MyRenderSet", &render_parms, &my_render_set) );  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            UF_CALL( UF_DRAW_set_render_set_objects(my_render_set, 1, &body) );  
        }  
        /* Terminate the API environment 译:终止API环境 */  
        UF_CALL(UF_terminate());  
    }  
    /*****************************************************************************  
    **  Utilities  
    *****************************************************************************/  
    /* Unload Handler  
    **     This function specifies when to unload your application from Unigraphics.  
    **     If your application registers a callback (from a MenuScript item or a  
    **     User Defined Object for example), this function MUST return  
    **     "UF_UNLOAD_UG_TERMINATE". */  
    extern int ufusr_ask_unload( void )  
    {  
        return( UF_UNLOAD_IMMEDIATELY );  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 包含了必要的NX API头文件。
> 2. 定义了一个错误报告函数ECHO，用于在控制台窗口和NX日志中输出错误信息。
> 3. 定义了一个函数ask_next_body，用于遍历指定部件中的所有实体，并返回下一个实体的tag。
> 4. 定义了ufusr函数，作为程序的入口点。在该函数中：初始化NX API环境。获取当前显示的部件。创建一个渲染集，并设置渲染参数，如隐藏线显示、线宽等。遍历部件中的所有实体，并将它们添加到渲染集中。最后，终止NX API环境。
> 5. 初始化NX API环境。
> 6. 获取当前显示的部件。
> 7. 创建一个渲染集，并设置渲染参数，如隐藏线显示、线宽等。
> 8. 遍历部件中的所有实体，并将它们添加到渲染集中。
> 9. 最后，终止NX API环境。
> 10. 定义了ufusr_ask_unload函数，用于指定何时卸载该应用程序。
>
> 总体来说，这段代码实现了在NX中创建一个渲染集，并将一个部件中的所有实体添加到该渲染集的功能。
>
