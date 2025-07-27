### 【0888】create single facet model 创建单面模型

#### 代码

```cpp
    /*HEAD CREATE_SINGLE_FACET_MODEL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_facet.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中新增的功能。 */  
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
        int  
            adjacent_facet_ids[4] = { UF_FACET_NULL_FACET_ID, UF_FACET_NULL_FACET_ID,  
                UF_FACET_NULL_FACET_ID, UF_FACET_NULL_FACET_ID },  
            new_facet_id;  
        tag_t  
            model,  
            part = UF_PART_ask_display_part();  
        double  
            vertices[4][3] = { { 0, 0, 0 }, {10, 0, 0 }, {10, 5, 0 }, { 0, 5, 0 } };  
        UF_CALL(UF_FACET_create_model(part, &model));  
        UF_CALL(UF_FACET_add_facet_to_model(model, 4, vertices, NULL, adjacent_facet_ids,  
            &new_facet_id));  
        UF_CALL(UF_FACET_model_edits_done(model));  
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

> 这段NX二次开发代码主要用于创建单个面片模型。下面是代码的详细解释：
>
> 1. 头文件包含：代码包含了必要的NX API头文件，如uf.h、uf_ui.h、uf_facet.h、uf_part.h，以便调用NX提供的API函数。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于在API调用失败时报告错误信息。它会输出文件名、行号、错误码、错误信息等。
> 3. 面片模型创建：在do_it函数中，使用UF_FACET_create_model创建面片模型，指定显示部件为当前部件。
> 4. 面片添加：调用UF_FACET_add_facet_to_model向模型中添加一个四边形面片，指定顶点坐标、相邻面片ID等。
> 5. 面片模型编辑完成：调用UF_FACET_model_edits_done完成面片模型的编辑。
> 6. ufusr函数：这是NX二次开发的入口函数，在这里初始化NX环境，调用do_it完成面片模型创建，然后终止NX环境。
> 7. 卸载函数：提供了卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总体来说，这段代码实现了在NX中创建一个简单的四边形面片模型的功能。通过调用NX提供的面片建模API，实现了面片模型的创建、面片的添加和编辑完成。
>
