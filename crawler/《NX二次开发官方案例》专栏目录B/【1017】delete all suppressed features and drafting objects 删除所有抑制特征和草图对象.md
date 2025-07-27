### 【1017】delete all suppressed features and drafting objects 删除所有抑制特征和草图对象

#### 代码

```cpp
    /*HEAD DELETE_ALL_SUPPRESSED_FEATURES_AND_DRAFTING_OBJECTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_drf.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18中是一个新增的功能。

翻译：UF_print_syslog是在V18中新增的功能。 */  
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
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_all_of_type(tag_t part, int type, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            jj,  
            n;  
        tag_t  
            *draft_objs,  
            exp,  
            part = UF_PART_ask_display_part();  
        double  
            value;  
        uf_list_p_t  
            feature_list;  
        UF_CALL(UF_MODL_ask_suppress_list(&feature_list));  
        UF_CALL(UF_MODL_ask_list_count(feature_list, &n));  
        if (n > 0)  
            UF_CALL(UF_MODL_delete_feature(feature_list));  
        UF_CALL(UF_MODL_delete_list(&feature_list));  
        for (ii = UF_drafting_entity_type; ii <= UF_dimension_type; ii++)  
        {  
            n = ask_all_of_type(part, ii, &draft_objs);  
            for (jj = 0; jj < n; jj++)  
            {  
                UF_CALL(UF_DRF_ask_controlling_exp(draft_objs[jj], &exp));  
                if (exp == NULL_TAG) continue;  
                UF_CALL(UF_MODL_ask_exp_tag_value(exp, &value));  
                if (value == FALSE)  
                    UF_CALL(UF_OBJ_delete_object(draft_objs[jj]));  
            }  
            if (n > 0) UF_free(draft_objs);  
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

> 这段代码是用于NX的二次开发，主要实现以下功能：
>
> 1. 删除所有被抑制的特征和草图对象。首先，从NX模型中获取被抑制的特征列表，并删除这些特征。然后，遍历草图对象，获取其控制表达式，如果表达式值为FALSE，则删除该草图对象。
> 2. 使用UF_MODL模块来创建和操作对象列表，使用UF_OBJ模块来遍历和删除对象，使用UF_DRF模块来获取草图对象的控制表达式，使用UF_PART模块来获取当前显示的部件。
> 3. 使用宏UF_CALL来封装UF函数调用，并在调用失败时输出错误信息。
> 4. 定义了allocate_memory和make_an_array两个辅助函数，用于分配内存和从列表中提取对象数组。
> 5. 定义了ask_all_of_type函数，用于查询指定类型对象并返回其数组。
> 6. do_it函数中实现了删除被抑制特征和草图对象的主要逻辑。
> 7. ufusr函数作为NX的二次开发入口，调用UF初始化，执行do_it函数，然后调用UF终止。
> 8. ufusr_ask_unload函数用于卸载用户程序。
>
> 总的来说，这段代码通过遍历和删除的方式实现了清除NX模型中被抑制的特征和草图对象的功能。
>
