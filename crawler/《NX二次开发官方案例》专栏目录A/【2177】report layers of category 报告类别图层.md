### 【2177】report layers of category 报告类别图层

#### 代码

```cpp
    /*HEAD REPORT_LAYERS_OF_CATEGORY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_layer.h>  
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

注意：UF_print_syslog是在V18版本中新增的。 */  
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
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITEnD(n,X) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%d", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static tag_t ask_category_tag(char *name)  
    {  
    /*  Prior to V18 - do this instead  
        tag_t  
            object = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, UF_layer_category_type,  
            FALSE, &object));  
        return object;  
    */  
        tag_t  
            object;  
        UF_CALL(UF_LAYER_ask_category_tag(name, &object));  
        return object;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int ask_layers_in_category(char *category, int **layers)  
    {  
        int  
            n_layers = 0,  
            ii,  
            jj;  
        tag_t  
            cat_tag = ask_category_tag(category);  
        UF_LAYER_category_info_t  
            category_info;  
        if (cat_tag == NULL_TAG) return 0;  
        UF_CALL(UF_LAYER_ask_category_info(cat_tag, &category_info));  
        for (ii = 0; ii < UF_LAYER_MAX_LAYER; ii++)  
            if (category_info.layer_mask[ii] == TRUE) n_layers++;  
        UF_CALL(allocate_memory(n_layers * sizeof(int), (void *)layers));  
        for (ii = 0, jj = 0; ii < UF_LAYER_MAX_LAYER; ii++)  
            if (category_info.layer_mask[ii] == TRUE)  
                (*layers)[jj++] = ii + 1;  
        return n_layers;  
    }  
    static void do_it(void)  
    {  
        int  
            *layers,  
            n_layers;  
        char  
            cat_name[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        while (prompt_for_text("Enter category name", cat_name))  
        {  
            WRITE_S(cat_name);  
            n_layers = ask_layers_in_category(cat_name, &layers);  
            WRITE_D(n_layers);  
            WRITEnD(n_layers, layers);  
            if (n_layers > 0) UF_free(layers);  
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

> 这是NX Open C++开发的示例代码，主要功能是查询NX中特定图层类别下的所有图层。
>
> 代码的主要逻辑如下：
>
> 1. 定义了宏，用于打印错误信息、整数值、字符串等。
> 2. 定义了错误报告函数report_error，用于在发生错误时打印错误信息。
> 3. 定义了用于写入整数、字符串、整数数组的函数，用于在列表窗口中输出信息。
> 4. 定义了用于获取用户输入文本的函数prompt_for_text。
> 5. 定义了用于查询指定名称的图层类别标签的函数ask_category_tag。
> 6. 定义了用于申请内存的函数allocate_memory。
> 7. 定义了用于查询指定图层类别下的所有图层编号的函数ask_layers_in_category。
> 8. 定义了主函数do_it，用于循环获取用户输入的图层类别名称，并输出该类别下的所有图层编号。
> 9. 定义了ufusr函数，是NX二次开发的入口函数，在这里调用主函数do_it。
> 10. 定义了卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总的来说，这段代码通过交互式获取用户输入的图层类别名称，并输出该类别下的所有图层编号，实现了查询NX中特定图层类别下的所有图层编号的功能。
>
