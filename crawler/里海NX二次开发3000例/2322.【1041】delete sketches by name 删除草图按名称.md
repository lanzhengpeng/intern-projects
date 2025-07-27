### 【1041】delete sketches by name 删除草图按名称

#### 代码

```cpp
    /*HEAD DELETE_SKETCHES_BY_NAME CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数。 */  
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
            feat,  
            part = UF_PART_ask_display_part(),  
            sketch;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1] = { "SKETCH_000" };  
        uf_list_p_t  
            sketch_list;  
        while (prompt_for_text("Enter name of sketch to delete", name))  
        {  
            sketch = NULL_TAG;  
            UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, UF_sketch_type,  
                FALSE, &sketch));  
            if (sketch != NULL_TAG)  
            {  
                UF_CALL(UF_MODL_ask_object_feat(sketch, &feat));  
                UF_CALL(UF_MODL_create_list(&sketch_list));  
                UF_CALL(UF_MODL_put_list_item(sketch_list, feat));  
                UF_CALL(UF_MODL_delete_feature(sketch_list));  
                UF_CALL(UF_MODL_delete_list(&sketch_list));  
            }  
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

> 这段代码是用于删除NX模型中特定名称的草图特征的二次开发代码。以下是代码的主要功能：
>
> 1. 包含必要的头文件，定义了函数报告错误信息。
> 2. 定义了一个报告错误函数，用于在函数执行失败时输出错误信息。
> 3. 定义了一个提示输入文本的函数，用于提示用户输入要删除的草图名称。
> 4. 主函数do_it中，循环提示用户输入要删除的草图名称，然后根据名称查询草图特征，并将该特征加入到特征列表中，最后删除该特征。
> 5. ufusr是NX调用的主函数，初始化NX环境，调用do_it函数执行主逻辑，然后终止NX环境。
> 6. ufusr_ask_unload函数用于在卸载该二次开发程序时返回立即卸载标志。
>
> 总体而言，这段代码实现了通过名称查询并删除NX模型中草图特征的功能。
>
