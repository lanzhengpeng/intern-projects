### 【2508】report tag of specified feature 指定特征的标签报告

#### 代码

```cpp
    /*HEAD REPORT_TAG_OF_SPECIFIED_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，可以翻译如下：

注意：UF_print_syslog 是在 V18 版本中新增的。 */  
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
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == 3) return feat;  
        return NULL_TAG;  
    }  
    static tag_t ask_tag_of_feature(char name[MAX_ENTITY_NAME_SIZE+1])  
    {  
        tag_t  
            feat = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            *this_name;  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
        {  
            if (!UF_CALL(UF_MODL_ask_feat_name(feat, &this_name)))  
            {  
                if (!strcmp(name, this_name))  
                    return(feat);  
            }  
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
    static void do_it(void)  
    {  
        tag_t  
            feat = NULL_TAG;  
        char  
            msg[133],  
            feat_name[MAX_ENTITY_NAME_SIZE+1] = { "REVOLVED(0)" };  
        while (prompt_for_text("Enter feature name", feat_name))  
        {  
            if ((feat = ask_tag_of_feature(feat_name)) != NULL_TAG)  
            {  
                sprintf(msg, "\'%s\' has tag: %d \n", feat_name, feat);  
                uc1601(msg, TRUE);  
            }  
            else   
            {  
                sprintf(msg, "\'%s\' not found\n", feat_name);  
                uc1601(msg, TRUE);  
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

> 这段代码是用于NX的二次开发，其主要功能是获取指定特征名称的特征tag。
>
> 代码的主要流程如下：
>
> 1. 包含必要的NX Open头文件。
> 2. 定义了宏UF_CALL，用于记录函数调用和错误码。
> 3. 定义了report_error函数，用于打印错误信息。
> 4. 定义了ask_next_feature函数，用于遍历零件中的所有特征，并返回下一个可用特征。
> 5. 定义了ask_tag_of_feature函数，用于根据特征名称查询特征tag。
> 6. 定义了prompt_for_text函数，用于获取用户输入的特征名称。
> 7. 定义了do_it函数，用于主循环，不断获取用户输入的特征名称，并查询对应的特征tag。
> 8. 定义了ufusr函数，是二次开发程序的入口，调用do_it函数。
> 9. 定义了ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 通过以上流程，代码实现了根据特征名称查询特征tag的功能，并且提供了用户交互界面，方便用户进行查询。
>
