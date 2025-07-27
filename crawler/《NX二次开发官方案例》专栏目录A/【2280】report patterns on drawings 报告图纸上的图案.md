### 【2280】report patterns on drawings 报告图纸上的图案

#### 代码

```cpp
    /*HEAD REPORT_PATTERNS_ON_DRAWINGS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_patt.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数，用于打印系统日志。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static logical find_patterns_on_drawing( tag_t dv )  
    {  
        int  
            type = 0,  
            subtype = 0,  
            layer = 0,   
            status = 0,  
            view_dep = 0;  
        tag_t  
            pattern = NULL_TAG;  
        logical  
            flag = FALSE;  
        double  
            data[13];  
        char  
            name[MAX_ENTITY_NAME_SIZE+1] = { "" },  
            sfn[UF_CFI_MAX_PATH_NAME_SIZE] = { "" };  
        do  
        {  
            UF_CALL( UF_VIEW_cycle_objects ( dv,   
                     UF_VIEW_VISIBLE_OBJECTS, &pattern ));  
            if( NULL_TAG == pattern ) break;  
            UF_OBJ_ask_type_and_subtype( pattern, &type, &subtype );  
            if( type == UF_pattern_type && subtype == UF_pattern_subtype )  
            {  
                flag = TRUE;  
                WRITENZ( uc5822( pattern, name, sfn, &view_dep,   
                                         &layer, &status, data ));  
                WRITE_D( pattern );   
                WRITE_S( name );  
                WRITE_S( sfn );  
            }  
        } while ( NULL_TAG != pattern );  
        return( flag );  
    }  
    static void process_this_part( void )  
    {  
        int  
            inx = 0,  
            num_dwgs = 0;  
        tag_t  
            *drawings = NULL,  
            dwg_view = NULL_TAG;  
        char   
            dwg_name[ 30 + 1 ] = { "" };  
        UF_CALL( UF_DRAW_ask_drawings( &num_dwgs, &drawings ));  
        for( inx = 0; inx < num_dwgs; inx++ )  
        {  
            UF_CALL(UF_DRAW_open_drawing ( drawings[inx] ));  
            UF_OBJ_ask_name( drawings[inx], dwg_name );   
            WRITE( "\n\nDrawing: " );  
            WRITE( dwg_name ); WRITE( "\n" );  
            UF_CALL( UF_DRAW_ask_view_of_drawing (drawings[inx], &dwg_view ));  
            find_patterns_on_drawing( dwg_view );  
        }  
        if( num_dwgs > 0 ) UF_free( drawings );  
    }  
    static void do_it(void)  
    {  
        tag_t  
            current_dwg = NULL_TAG,  
            disp_part = UF_PART_ask_display_part();  
        if( NULL_TAG == disp_part ) return;  
        UF_CALL( UF_DRAW_ask_current_drawing ( &current_dwg ));  
        UF_CALL( UF_DISP_set_display( UF_DISP_SUPPRESS_DISPLAY ));  
        process_this_part( );   
        if( current_dwg != NULL_TAG )  
            UF_CALL( UF_DRAW_open_drawing ( current_dwg ));  
        UF_CALL( UF_DISP_set_display( UF_DISP_UNSUPPRESS_DISPLAY ));  
        UF_CALL( UF_DISP_regenerate_display());  
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

> 这段代码是用于在NX的绘图上查找图案的二次开发代码。主要功能和步骤如下：
>
> 1. 定义了错误报告函数report_error，用于在函数执行出错时打印错误信息。
> 2. 定义了辅助宏WRITE、WRITE_D、WRITENZ、WRITE_S，用于在列表窗口中打印信息。
> 3. 定义了辅助函数write_integer_to_listing_window和write_string_to_listing_window，用于在列表窗口中打印整型和字符串信息。
> 4. 定义了函数find_patterns_on_drawing，用于在给定的视图中循环查找图案对象，并打印图案信息。
> 5. 定义了函数process_this_part，用于获取当前零件的所有绘图，并打开每个绘图，调用find_patterns_on_drawing查找图案。
> 6. 定义了函数do_it，用于获取当前显示的零件，关闭显示，调用process_this_part查找图案，然后恢复显示。
> 7. 定义了ufusr函数，这是NX二次开发的入口函数，用于初始化、调用do_it、终止。
> 8. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 综上，这段代码通过在NX零件的绘图中查找图案，并打印相关信息，实现了图案报告的功能。
>
