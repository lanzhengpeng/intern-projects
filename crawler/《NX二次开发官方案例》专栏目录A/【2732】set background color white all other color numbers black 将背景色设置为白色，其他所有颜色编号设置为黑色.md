### 【2732】set background color white all other color numbers black 将背景色设置为白色，其他所有颜色编号设置为黑色

#### 代码

```cpp
    /*HEAD SET_BACKGROUND_COLOR_WHITE_ALL_OTHER_COLOR_NUMBERS_BLACK CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的说明，UF_print_syslog是V18版本中新增的函数。因此，只需要回答翻译结果，不要添加任何额外内容。 */  
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
    void set_rgb_color_for_number( int color_num, double redval, double grnval, double bluval )  
    {  
        double clr_values[3];  
        int colnum;  
        colnum = color_num;  
        clr_values[0] = redval;  
        clr_values[1] = bluval;  
        clr_values[2] = grnval;  
        UF_CALL(UF_DISP_set_color(colnum, UF_DISP_rgb_model, "", clr_values ));  
        UF_CALL(UF_DISP_load_color_table());  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
    /* The visualization preferences for the background will have to be set to 'plain'  
         as opposed to 'graduated', in order to see the affect */  
        set_rgb_color_for_number(UF_DISP_BACKGROUND_COLOR, 0.0, 0.0, 0.0 );  
        for (ii = 1; ii < UF_OBJ_MAX_COLOR; ii++)  
        {  
            set_rgb_color_for_number(ii, 1.0, 1.0, 1.0 );  
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

> 这段NX二次开发代码的主要功能是设置NX图形界面的背景颜色为白色，其他颜色编号为黑色。具体来说，代码的主要部分包括：
>
> 1. 包含了必要的NX开发头文件。
> 2. 定义了一个错误报告函数report_error，用于在发生UF函数调用错误时打印错误信息。
> 3. 定义了一个设置颜色值的函数set_rgb_color_for_number，用于设置指定颜色编号的颜色值。
> 4. 定义了一个do_it函数，用于设置背景颜色为黑色，其他颜色编号的颜色为白色。
> 5. 定义了UF函数ufusr，在NX启动时调用do_it函数，实现颜色的设置。
> 6. 定义了一个询问卸载函数ufusr_ask_unload，在NX卸载时调用。
>
> 总的来说，这段代码通过UF函数实现了NX图形界面的背景颜色和其他颜色编号的设置，从而实现了界面颜色的定制化。
>
