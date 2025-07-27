### 【1340】get font index number 获取字体索引号

#### 代码

```cpp
    /*HEAD GET_FONT_INDEX_NUMBER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_ugfont.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新增功能。 */  
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
    static tag_t ask_font_table_entity(void)  
    {  
        tag_t  
            fte = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_font_table_type, &fte));  
        return (fte);  
    }  
    static int ask_font_index_number(char *font_name)  
    {  
        int  
            index;  
        tag_t  
            fte = ask_font_table_entity();  
        UF_CALL(UF_UGFONT_add_font(fte, &index, font_name));  
        return (index);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        WRITE_D(ask_font_index_number("leroy"));  
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

> 这段NX Open C++代码实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数出错时打印错误信息到系统日志和控制台。
> 2. 定义了一个获取当前活动部件的字体表实体的函数ask_font_table_entity。
> 3. 定义了一个根据字体名称获取字体索引号的函数ask_font_index_number，通过调用UF_UGFONT_add_font函数实现。
> 4. 定义了一个用于向列表窗口写入整数值的函数write_integer_to_listing_window。
> 5. 定义了一个主函数do_it，调用ask_font_index_number获取字体"leroy"的索引号，并输出到列表窗口。
> 6. 定义了ufusr函数，这是NX Open C++的入口函数，初始化环境后调用do_it，最后终止环境。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载，表示在关闭UF时无需等待。
>
> 总体而言，这段代码实现了获取指定字体名称在当前部件字体表中的索引号，并输出到列表窗口。通过封装UF函数，实现了NX Open C++对NX字体表的交互。
>
