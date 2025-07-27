### 【2743】set creation color of solid type 设置实体类型的创建颜色

#### 代码

```cpp
    /*HEAD SET_CREATION_COLOR_OF_SOLID_TYPE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的功能。 */  
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
    static void do_it(void)  
    {  
        char  
            *sides_a[3] = { "1.0", "5.0", "1.0" };  
        double  
            corner_a[3] = { 0,0,0 };  
        tag_t  
            block_a;  
       UF_CALL(UF_OBJ_set_cre_color(UF_solid_type, UF_all_subtype,  
            UF_OBJ_solid_body_property, UF_OBJ_GRAY));  
        UF_CALL(UF_MODL_create_block1(UF_NULLSIGN, corner_a, sides_a, &block_a));      
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

> 这段NX二次开发代码的主要功能如下：
>
> 1. 包含头文件，用于调用NX提供的API函数。
> 2. 定义了report_error函数，用于在调用API失败时报告错误。
> 3. 定义了do_it函数，用于创建一个灰色长方体块，并设置其颜色为灰色。
> 4. 定义了ufusr函数，用于初始化NX环境，调用do_it函数，然后终止NX环境。
> 5. 定义了ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 总体来说，这段代码的主要功能是创建一个灰色长方体块，并通过NX提供的API来设置其颜色。同时，代码包含了错误处理机制，确保在调用API失败时可以报告错误。
>
