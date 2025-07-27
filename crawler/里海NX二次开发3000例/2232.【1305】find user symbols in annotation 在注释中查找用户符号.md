### 【1305】find user symbols in annotation 在注释中查找用户符号

#### 代码

```cpp
    /*HEAD FIND_USER_SYMBOLS_IN_ANNOTATION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 版本新增的功能。它用于将日志消息输出到系统日志。 */  
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
    static tag_t ask_next_drafting_aid(tag_t part, tag_t draft)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type, &draft));  
        return (draft);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            jj,  
            n;  
        tag_t  
            draft = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            *emb_sym;  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        while ((draft = ask_next_drafting_aid(part, draft)) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_draft_aid_text_info(draft, &n, &text_info));  
            for (ii=0; ii<n; ii++)  
                for (jj=0; jj<text_info[ii].num_lines; jj++)  
                {  
                    emb_sym = strstr(text_info[ii].text[jj].string, "<%");  
                    if (emb_sym != NULL)  
                    {  
                        emb_sym = emb_sym + 2;  
                        *strchr(emb_sym, '>') = '\0';  
                        UF_CALL(UF_DISP_set_highlight(draft, TRUE));  
                        uc1601(emb_sym, TRUE);  
                        UF_CALL(UF_DISP_set_highlight(draft, FALSE));  
                    }  
                }  
            UF_CALL(UF_DRF_free_text(n, &text_info));  
        }  
    }  
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

> 这段代码是一个NX的二次开发示例，主要功能是在NX模型中的注释中查找用户定义的符号，并将找到的符号高亮显示。下面是代码的主要内容和功能：
>
> 1. 头文件：包含了一些NX二次开发常用的头文件，如uf.h、uf_obj.h、uf_object_types.h等。
> 2. 错误报告函数：report_error用于报告NX函数调用的错误，并将错误信息输出到系统日志和列表窗口。
> 3. 获取下一标注辅助对象：ask_next_drafting_aid用于遍历零件中的标注辅助对象。
> 4. 主要功能函数：do_it是主函数，用于遍历零件中的标注辅助对象，并获取它们的文本信息。然后，在文本中查找用户定义的符号，如果找到，则将对应的标注辅助对象高亮显示。
> 5. 符号查找：使用strstr函数在文本中查找符号，如果找到，则将符号高亮显示。
> 6. 用户自定义函数：uc1601用于高亮显示找到的符号，但这个函数没有在代码中实现，需要用户自行实现。
> 7. ufusr函数：这是NX二次开发的入口函数，用于初始化NX环境，调用do_it函数执行功能，然后终止NX环境。
> 8. 卸载函数：ufusr_ask_unload用于在二次开发模块卸载时执行一些清理工作。
>
> 总的来说，这段代码实现了在NX模型注释中查找并高亮用户定义符号的功能。用户可以根据需要实现uc1601函数，以完成符号高亮显示的具体功能。
>
