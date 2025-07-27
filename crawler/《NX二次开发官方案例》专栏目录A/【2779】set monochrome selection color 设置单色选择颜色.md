### 【2779】set monochrome selection color 设置单色选择颜色

#### 代码

```cpp
    /*HEAD SET_MONOCHROME_SELECTION_COLOR CCC UFUN */  
    /*Changes the selection-/preselection color of pre-nx1 parts to nx1/nx2 defaults*/  
    /*Otherwise selected objects are not visible in monochrome mode*/  
    /*see IR-1336003*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                sprintf(err, "error %d at line %d in %s\n", irc, line, file);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n\n");  
            }  
            else  
            {  
                fprintf(stderr, "%s\n", err);  
                fprintf(stderr, "error %d at line %d in %s\n", irc, line, file);  
                fprintf(stderr, "%s;\n\n", call);  
    #ifdef _USRDLL  
                uc1601(err, TRUE);  
                sprintf(err, "error %d at line %d in %s\n", irc, line, file);  
                uc1601(err, TRUE);  
    #endif  
            }  
        }  
        return(irc);  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        UF_DISP_drawing_display_data_t   
            drawing_display;  
        UF_CALL( UF_DISP_ask_drawing_display ( &drawing_display ) );  
        WRITE_L(drawing_display.monochrome_display);  
        WRITE_L(drawing_display.show_widths);  
        WRITE_D(drawing_display.system_color);  
        WRITE_D(drawing_display.background_color);  
        WRITE_D(drawing_display.foreground_color);  
        WRITE_D(drawing_display.preselection_color);  
        drawing_display.preselection_color = 145;  
        drawing_display.system_color = 186;  
        UF_CALL( UF_DISP_set_drawing_display ( &drawing_display ) );  
        UF_CALL( UF_DISP_ask_drawing_display ( &drawing_display ) );  
        WRITE_L(drawing_display.monochrome_display);  
        WRITE_L(drawing_display.show_widths);  
        WRITE_D(drawing_display.system_color);  
        WRITE_D(drawing_display.background_color);  
        WRITE_D(drawing_display.foreground_color);  
        WRITE_D(drawing_display.preselection_color);  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        tag_t  
            part_tag = NULL_TAG;  
        char msg[133];  
        if( UF_CALL(UF_initialize()) ) return;  
        if ( part_tag=UF_ASSEM_ask_work_part() == NULL_TAG )  
        {  
            UF_CALL(UF_UI_open_listing_window());  
            sprintf(msg, "No Work Part...exiting");  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
        else do_it();  
        UF_terminate();  
    }  
    void main( int argc, char *argv[] )  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_PART_load_status_t   
                error_status;  
            tag_t  
                part_tag = NULL_TAG;  
            if( argc < 2)  
            {  
                printf("\n%s UG-Partname\n", argv[0]);  
                return;  
            }  
            if(! UF_CALL( UF_PART_open( argv[1], &part_tag, &error_status ) ) )  
            {  
                do_it();  
                UF_CALL( UF_PART_save() );  
            }  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是用于修改NX的显示设置的二次开发代码。主要功能包括：
>
> 1. 获取当前NX的绘图显示设置，包括单色显示、线宽显示、系统颜色、背景颜色、前景颜色和预选颜色。
> 2. 将预选颜色修改为145号颜色，系统颜色修改为186号颜色。
> 3. 重新设置NX的绘图显示设置。
> 4. 再次获取当前NX的绘图显示设置，并输出到列表窗口。
> 5. 在命令行执行时，如果指定了零件名，则打开该零件，修改其绘图显示设置，并保存。
> 6. 提供了错误报告函数report_error，用于在出错时输出错误信息。
> 7. 提供了逻辑和整数写入函数，用于将逻辑值和整数输出到列表窗口。
> 8. 提供了立即卸载函数，用于在NX关闭时立即卸载该程序。
>
> 总的来说，这段代码实现了NX绘图显示设置的修改，并且可以通过命令行参数打开指定零件进行设置，同时提供了错误处理和日志输出功能。
>
