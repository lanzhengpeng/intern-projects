### 【1014】delete all sketches 删除所有草图

#### 代码

```cpp
    /*HEAD DELETE_ALL_SKETCHES CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本中的新增功能。

UF_print_syslog 是 V18 版本中的新功能，用于打印系统日志。 */  
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
    static tag_t ask_next_sketch(tag_t part, tag_t sketch)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch));  
        return (sketch);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat,  
            part = UF_PART_ask_display_part(),  
            sketch = NULL_TAG;  
        uf_list_p_t  
            sketch_list;  
        UF_CALL(UF_MODL_create_list(&sketch_list));  
        while ((sketch = ask_next_sketch(part, sketch)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_object_feat(sketch, &feat));  
            UF_CALL(UF_MODL_put_list_item(sketch_list, feat));  
        }  
        if (feat != NULL_TAG) UF_CALL(UF_MODL_delete_feature(sketch_list));  
        UF_CALL(UF_MODL_delete_list(&sketch_list));  
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

> 这段代码是一个NX的二次开发代码，主要功能是删除当前显示部件中的所有草图特征。具体来说，代码的主要逻辑如下：
>
> 1. 包含了NX Open的UF头文件，定义了UF_CALL宏来报告错误。
> 2. 定义了report_error函数，用于打印错误信息到系统日志和控制台窗口。
> 3. 定义了ask_next_sketch函数，用于遍历部件中的所有草图特征。
> 4. do_it函数实现了主要逻辑：首先创建一个特征列表，然后遍历部件中的所有草图特征，并将它们添加到特征列表中。最后调用UF_MODL_delete_feature删除特征列表中的所有特征。
> 5. ufusr函数是用户自定义的入口函数，初始化NX Open，调用do_it函数，然后终止NX Open。
> 6. ufusr_ask_unload函数用于卸载用户自定义的库。
>
> 综上所述，这段代码通过遍历当前显示的部件，收集所有草图特征，并一次性删除它们，实现了删除所有草图特征的功能。
>
