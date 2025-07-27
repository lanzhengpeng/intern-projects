### 【2390】report text string of gdt symbol 报告GD&T符号的文本字符串

#### 代码

```cpp
    /*HEAD REPORT_TEXT_STRING_OF_GD&T_SYMBOL*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：
UF_print_syslog是V18中的新功能，请只回答翻译，不要添加其他内容。 */  
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
            msg[133];  
        int  
            type, subtype;  
        tag_t  
            gdt_symbol_tag = NULL_TAG;  
        double  
            gdt_symbol_origin[3];  
        UF_DRF_gdt_symbol_info_t  
            *gdt_symbol_info;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(UF_ASSEM_ask_work_part(),  
                UF_drafting_entity_type, &gdt_symbol_tag))&&  
                (gdt_symbol_tag != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(gdt_symbol_tag, &type,  
                    &subtype));  
            UF_CALL(UF_UI_open_listing_window());  
                sprintf(msg,"type: %d  subtype: %d\n", type, subtype);  
                UF_CALL(UF_UI_write_listing_window(msg));  
            if(subtype == 4)  
            {  
                /*retrieve GD&T symbol information*/  
                UF_CALL(UF_DRF_ask_gdt_symbol_info(gdt_symbol_tag,  
                    gdt_symbol_origin, &gdt_symbol_info));  
                UF_CALL(UF_UI_open_listing_window());  
                sprintf(msg,"gdt_symbol_tag %d\n", gdt_symbol_tag);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                sprintf(msg,"text_string: %s\n",  
                    gdt_symbol_info->text_info->text->string);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                UF_CALL(UF_DRF_free_gdtsymbol(&gdt_symbol_info));  
            }  
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

> 这段代码主要用于在NX中循环遍历工作部件中的GD&T符号，并获取其文本字符串信息。具体步骤包括：
>
> 1. 包含必要的NX Open库头文件。
> 2. 定义UF_CALL宏，用于调用NX Open函数并报告错误。
> 3. 定义report_error函数，用于打印错误信息。
> 4. 定义do_it函数，用于循环遍历工作部件中的所有GD&T符号，并获取其文本字符串信息。主要步骤包括：调用UF_OBJ_cycle_objs_in_part遍历部件中的所有GD&T符号。获取每个GD&T符号的类型和子类型。如果子类型为4，则调用UF_DRF_ask_gdt_symbol_info获取GD&T符号的详细信息。打印GD&T符号的tag和文本字符串。释放GD&T符号的详细信息内存。
> 5. 调用UF_OBJ_cycle_objs_in_part遍历部件中的所有GD&T符号。
> 6. 获取每个GD&T符号的类型和子类型。
> 7. 如果子类型为4，则调用UF_DRF_ask_gdt_symbol_info获取GD&T符号的详细信息。
> 8. 打印GD&T符号的tag和文本字符串。
> 9. 释放GD&T符号的详细信息内存。
> 10. 定义ufusr函数，用于初始化NX Open库，调用do_it函数，然后终止NX Open库。
> 11. 定义ufusr_ask_unload函数，用于卸载用户函数模块。
>
> 总的来说，这段代码实现了在NX中获取GD&T符号文本字符串信息的功能。
>
