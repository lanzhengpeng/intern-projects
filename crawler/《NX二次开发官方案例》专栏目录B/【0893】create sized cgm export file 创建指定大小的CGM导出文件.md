### 【0893】create sized cgm export file 创建指定大小的CGM导出文件

#### 代码

```cpp
    /* Create Scaled CGM 译:创建缩放CGM */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_std.h>  
    #include <uf_draw.h>  
    #include <uf_view.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：UF_print_syslog是V18版本新增的功能，请只回答翻译，不要添加无关的评论。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            pens[15] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},  
            rtrn_code = 0;  
        double   
            cgmSizeMode[2] ;  
        UF_DRAW_info_t   
            drawing_info;  
        tag_t       
            drawing_tag = NULL_TAG;  
        UF_OBJ_cycle_by_name( "SHT1", &drawing_tag );  
        UF_CALL( UF_DRAW_ask_drawing_info( drawing_tag, &drawing_info) );  
        WRITE_F( drawing_info.size.custom_size[0] );  
        WRITE_F( drawing_info.size.custom_size[1] );  
        cgmSizeMode[0] = drawing_info.size.custom_size[1];  
        cgmSizeMode[1] = drawing_info.size.custom_size[0];  
        if( drawing_info.units == UF_PART_METRIC )  
        {  
            cgmSizeMode[0] /= 25.4;  
            cgmSizeMode[1] /= 25.4;  
        }  
        WRITE_F( cgmSizeMode[0] );  
        WRITE_F( cgmSizeMode[1] );  
        // Parameter 1: 1=scale mode, 2=size mode         
        UF_CALL( UF_STD_set_cgm_size_mode( 2, cgmSizeMode ) );  
        UF_CALL( uc6410( "SHT1", "c:\\temp\\my_cgm", 2, 1, pens ) );  
        if( rtrn_code ) WRITE_D( rtrn_code );  
        /* We need to repeat the steps in V18 and NX1 only, fixed in NX2 译:We need to repeat the steps in V18 and NX1 only, fixed in NX2. */  
        UF_STD_set_cgm_size_mode( 2, cgmSizeMode );  
        rtrn_code = uc6410( "SHT1", "c:\\temp\\my_cgm", 2, 1, pens );  
        if( rtrn_code ) WRITE_D( rtrn_code );  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 包含了NX二次开发所需的头文件。
> 2. 定义了宏UF_CALL，用于调用NX的函数，并在出错时报告错误。
> 3. 定义了宏WRITE_D和WRITE_F，用于将整型和浮点型数据写入列表窗口。
> 4. 实现了函数do_it，用于执行以下操作：获取当前工作表的绘图信息，包括自定义尺寸。如果绘图单位是公制，则将自定义尺寸转换为英寸。设置CGM绘图的大小模式为自定义尺寸。调用函数uc6410将工作表导出为CGM文件。由于NX 18和NX 1的bug，需要重复设置CGM大小模式和调用uc6410。
> 5. 获取当前工作表的绘图信息，包括自定义尺寸。
> 6. 如果绘图单位是公制，则将自定义尺寸转换为英寸。
> 7. 设置CGM绘图的大小模式为自定义尺寸。
> 8. 调用函数uc6410将工作表导出为CGM文件。
> 9. 由于NX 18和NX 1的bug，需要重复设置CGM大小模式和调用uc6410。
> 10. 定义了ufusr函数，作为NX二次开发的入口函数，在其中调用do_it函数执行功能。
> 11. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发程序可以立即卸载。
>
> 总的来说，这段代码实现了获取当前工作表的自定义尺寸，将其设置为CGM绘图的大小，然后导出工作表为CGM文件的功能。
>
