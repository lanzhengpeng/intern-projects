### 【2418】report user defined symbol font info 报告用户定义符号字体信息

#### 代码

```cpp
    /*HEAD REPORT_USER_DEFINED_SYMBOL_FONT_INFO CCC UFUN */  
    /*----------------------------------------------------------  
    This requires an object of type UF_drafting_entity_type,  
                       subtype UF_draft_user_defined_subtype  
    ----------------------------------------------------------*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_defs.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static tag_t ask_next_user_defined_symbol(tag_t part, tag_t uds)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type,  
            &uds)) && (uds != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(uds, &type, &subtype));  
            if (subtype == UF_draft_user_defined_subtype) return uds;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        logical  
            is_sbf;  
        int  
            num_symbols = 0,  
            type = 0,  
            subtype = 0;  
        tag_t  
            ud_symbol_tag = NULL_TAG,  
            part = UF_ASSEM_ask_work_part();  
        UF_DRF_ud_symbol_font_info_t  
            *font_info;  
        while ((ud_symbol_tag = ask_next_user_defined_symbol(part, ud_symbol_tag))  
            != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_is_sbf_symbol(ud_symbol_tag, &is_sbf));  
            if (!is_sbf) continue;  
            WRITE_D( ud_symbol_tag );  
            if (!UF_CALL( UF_DRF_ask_ud_symbol_font_info( ud_symbol_tag,  
                                          &num_symbols, &font_info ))) continue;  
            UF_DISP_set_highlight( ud_symbol_tag, 1 );  
            WRITE_D( num_symbols );  
            WRITE_D( font_info->num_stroke );  
            WRITE_D( font_info->anchor_point );  
            WRITE_D( font_info->orientation_point );  
            WRITE_F( font_info->height );  
            WRITE_F( font_info->length );  
            WRITE_F( font_info->factor );  
            WRITE_D( font_info->stroke_info->x_length );  
            WRITE_D( font_info->stroke_info->y_length );  
            WRITE_D( font_info->stroke_info->pen_status );  
            uc1601( "Symbol is highlighted...", TRUE );  
            UF_DISP_set_highlight( ud_symbol_tag, 0 );  
            UF_CALL( UF_DRF_free_font( num_symbols, &font_info ));  
            WRITE( "\n\n" );  
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

> 这段NX二次开发代码实现了查询当前工作部件中所有用户自定义符号字体信息的功能，并使用NX的UF函数进行了实现。
>
> 主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在UF函数调用失败时输出错误信息。
> 2. 定义了write_integer_to_listing_window和write_double_to_listing_window函数，用于向NX的列表窗口输出整型和浮点型数据。
> 3. 定义了ask_next_user_defined_symbol函数，用于遍历当前工作部件中的所有用户自定义符号，并返回下一个符号的tag。
> 4. 定义了do_it函数，用于遍历所有用户自定义符号，并查询每个符号的字体信息，包括符号数量、笔画数量、锚点、方向点、高度、长度、缩放因子、笔画信息等，并将这些信息输出到列表窗口。
> 5. 定义了ufusr函数，作为NX的UF函数入口点，在函数内部初始化NX环境，调用do_it函数，然后终止NX环境。
> 6. 定义了ufusr_ask_unload函数，用于卸载NX加载的库文件。
>
> 综上所述，这段代码实现了查询和输出当前工作部件中所有用户自定义符号的字体信息的功能，通过遍历符号并使用NX提供的UF函数进行查询。
>
