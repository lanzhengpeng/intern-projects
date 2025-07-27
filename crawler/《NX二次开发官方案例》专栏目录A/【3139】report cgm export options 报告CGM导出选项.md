### 【3139】report cgm export options 报告CGM导出选项

#### 代码

```cpp
    /*HEAD REPORT_CGM_EXPORT_OPTIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_cgm.h>  
    #include <uf_draw.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    /* qq3123197280 */  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    /* qq3123197280 */  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    /* qq3123197280 */  
    static void report_cgm_export_options( UF_CGM_export_options_t export_options )  
    {  
        WRITE_D( export_options.colors );  
        switch ( export_options.colors )  
        {  
            case UF_CGM_AS_DISPLAYED_COLORS:  
                WRITE( "Apply the colors displayed on screen.\n" );  
                break;  
            case UF_CGM_PART_COLORS:  
                WRITE( "Apply the colors of the part color table.\n" );   
                break;  
            case UF_CGM_CUSTOM_PALETTE_COLORS:  
                WRITE( "Apply the session custom colors. \n" );  
                break;  
            case UF_CGM_BLACK_ON_WHITE:  
                WRITE( "Apply black to all geometry.\n" );   
                break;  
            case UF_CGM_COLOR_BY_WIDTH:  
                WRITE( "Apply the first three entries(representing\n" );  
                WRITE( "thin, normal & thick) of the session\n" );  
                WRITE( "custom colors. In otherwords the color is\n" );  
                WRITE( "determined by the density index.  All thin\n" );  
                WRITE( "objects will plot using the first color\n" );  
                WRITE( "entry, all normal objects will plot with\n" );  
                WRITE( "the second color entry, and all thick\n" );  
                WRITE( "objects will plot in the third color entry.\n" );   
        }  
        WRITE_D( export_options.widths );  
        switch ( export_options.widths )  
        {  
            case UF_CGM_STANDARD_WIDTHS:  
                WRITE("Apply 3 fixed line widths.\n" );  
                break;  
            case UF_CGM_SINGLE_WIDTH:  
                WRITE("Apply the single line width defined\n" );  
                WRITE("in the session custom widths. \n" );  
                break;  
            case UF_CGM_CUSTOM_3_WIDTHS:  
                WRITE("Apply custom widths per-width index, \n" );  
                WRITE("as defined in the session custom widths. \n" );   
                break;  
            case UF_CGM_CUSTOM_PALETTE_WIDTHS:  
                WRITE("Apply custom widths per-color index, \n" );   
                WRITE("as defined in the session custom widths. \n" );   
                break;  
            case UF_CGM_DEFAULT_FILE_WIDTHS:  
                WRITE("Apply line width settings specified in \n" );   
                WRITE("the CGM Defaults File (cgmdef.txt). \n" );   
        }  
        WRITE_D( export_options.vdc_mode );      
        switch ( export_options.vdc_mode )  
        {  
            case UF_CGM_INTEGER_VDC:  
                WRITE( "Use 16-bit integer coordinates.\n" );  
                break;  
            case UF_CGM_REAL_VDC:  
                WRITE( "Use 32-bit real coordinates.\n" );  
        }  
        WRITE_D( export_options.size.mode );  
        switch ( export_options.size.mode )  
        {  
            case UF_CGM_SIZE_BY_SCALE:       
                WRITE( "Specify size as a scale factor.\n" );  
                break;  
            case UF_CGM_SIZE_BY_DIMENSIONS:  
                WRITE( "Specify size as dimensions.\n" );  
        }  
        WRITE_F( export_options.size.scale );  
        WRITE_D( export_options.size.dimensions.units );  
        switch ( export_options.size.dimensions.units )  
        {  
            case UF_CGM_MILLIMETERS:       
                WRITE( "MILLIMETERS\n" );  
                break;  
            case UF_CGM_INCHES:       
                WRITE( "INCHES\n" );  
        }      
        WRITE_F( export_options.size.dimensions.dimensions[0] );  
        WRITE_F( export_options.size.dimensions.dimensions[1] );  
        WRITE_D( export_options.text_mode );  
        switch ( export_options.text_mode )  
        {  
            case UF_CGM_TEXT_AS_POLYLINES:       
                WRITE( "Record text as CGM polyline elements.\n");  
                break;  
            case  UF_CGM_TEXT_AS_CHARACTERS:       
                WRITE( "Record text as as CGM text elements.\n");   
        }      
        WRITE_D( export_options.fonts );  
        switch ( export_options.fonts )  
        {  
            case UF_CGM_1_CALS_FONT:  
                WRITE("Map all NX fonts to CALS font Hershey Simplex Roman.\n");    
                break;  
            case UF_CGM_4_CALS_FONTS:  
                WRITE("Map NX fonts 1 through 4 to CALS fonts\as follows: \n");   
                WRITE("    1: Hershey Simplex Roman\n");   
                WRITE("    2: Hershey Cartographic Roman\n");   
                WRITE("    3: Hershey Simplex Script\n");   
                WRITE("    4: Hershey Complex Italic\n");   
                WRITE("    Map all other NX fonts to CALS font\n");   
                WRITE("    Hershey Simplex Roman.\n");   
                break;  
            case UF_CGM_NX_FONTS:  
                WRITE("Record the names of NX fonts in the CGM.\n");   
                break;  
            case UF_CGM_DEFAULT_FILE_FONTS:  
                WRITE("Record fonts according to text font mapping\n");   
                WRITE("keywords specified in the CGM Defaults File\n");   
                WRITE("(cgmdef.txt).\n");   
            }       
        WRITE_F( export_options.tolerance );      
    }  
    static void do_it(void)  
    {  
        UF_CGM_export_options_t   
            export_options;  
        UF_UI_exit_listing_window();  
        UF_CALL(UF_CGM_ask_session_export_options( &export_options ));  
        report_cgm_export_options( export_options );  
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

> 这段代码是一个用于获取和报告NX中CGM图形导出选项的二次开发示例。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在发生错误时打印错误信息。
> 2. 定义了用于向列表窗口写入浮点数和整数的辅助函数。
> 3. 定义了一个函数report_cgm_export_options，用于报告CGM导出选项的每个字段的值。
> 4. 定义了一个函数do_it，用于获取当前会话的CGM导出选项，并调用report_cgm_export_options函数报告这些选项。
> 5. 定义了ufusr函数，作为程序的入口点。在ufusr函数中，首先调用UF_initialize进行初始化，然后调用do_it函数，最后调用UF_terminate终止程序。
> 6. 定义了ufusr_ask_unload函数，用于在卸载时立即卸载库。
>
> 该代码通过二次开发接口获取了NX的CGM图形导出选项，并以可读的方式打印出来，从而实现了对CGM导出选项的查看和报告。
>
