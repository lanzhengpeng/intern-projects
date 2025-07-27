### 【0591】create and unite 3 blocks 创建并合并3个块

#### 代码

```cpp
    /*HEAD CREATE_AND_UNITE_3_BLOCKS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：注意，UF_print_syslog是V18版本新增的功能，请只提供翻译，无需添加其他内容。 */  
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
            *sides_a[3] = { "1.0", "5.0", "1.0" },  
            *sides_b[3] = { "5.0", "1.0", "1.0" },  
            *sides_c[3] = { "1.0", "5.0", "1.0" };  
        double  
            corner_a[3] = { 0,0,0 },  
            corner_b[3] = { 0,2,0 },  
            corner_c[3] = { 4,0,0 };  
        tag_t  
            block_a,  
            block_b,  
            block_c,  
            solid_a,  
            solid_b,  
            solid_c;  
        UF_CALL(UF_MODL_create_block1(UF_NULLSIGN, corner_a, sides_a, &block_a));  
        UF_CALL(UF_MODL_create_block1(UF_NULLSIGN, corner_b, sides_b, &block_b));  
        UF_CALL(UF_MODL_create_block1(UF_NULLSIGN, corner_c, sides_c, &block_c));  
        UF_CALL(UF_MODL_ask_feat_body(block_a, &solid_a));  
        UF_CALL(UF_MODL_ask_feat_body(block_b, &solid_b));  
        UF_CALL(UF_MODL_ask_feat_body(block_c, &solid_c));  
        UF_CALL(UF_MODL_operations(solid_a, solid_b, UF_POSITIVE));  
        UF_CALL(UF_MODL_operations(solid_a, solid_c, UF_POSITIVE));  
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

> 这段NX二次开发代码的主要功能是创建三个块，并执行布尔运算将它们合并为一个整体。
>
> 代码首先定义了三个块的位置和尺寸，并使用UF_MODL_create_block1函数创建了这三个块。然后，它通过UF_MODL_ask_feat_body函数获取了每个块的实体，并将它们作为参数传递给UF_MODL_operations函数，以执行布尔运算。第一个块与第二个块执行了正布尔运算，接着第一个块与第三个块也执行了正布尔运算。最终，这三个块被合并为一个整体。
>
> 代码还包含了一个错误报告函数report_error，用于在出现错误时输出错误信息。
>
> 在ufusr函数中，首先调用UF_initialize初始化NX，然后执行do_it函数，最后调用UF_terminate终止NX。
>
> 总体来说，这段代码实现了创建块、获取块实体，以及执行布尔运算合并块的功能。
>
