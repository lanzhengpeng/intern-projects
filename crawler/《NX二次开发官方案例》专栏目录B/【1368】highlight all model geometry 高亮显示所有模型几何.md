### 【1368】highlight all model geometry 高亮显示所有模型几何

#### 代码

```cpp
    /*HEAD HIGHLIGHT_ALL_MODEL_GEOMETRY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_layer.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，可以得出以下信息：

- UF_print_syslog 是 V18 新增的函数。
- 这个函数用于将日志信息输出到系统日志中。
- 在文档中，UF_print_syslog 被调用了两次，分别用于输出错误信息和警告信息。

因此，UF_print_syslog 是 V18 版本中新增的一个函数，用于将日志信息输出到系统日志。 */  
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
    static int ask_all_model_geometry(tag_t **geometry)  
    {  
        int  
            flag,  
            ii,  
            old_layer_settings[UF_LAYER_MAX_LAYER],  
            subtype,  
            type;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            list;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        for (ii = 1; ii <= UF_LAYER_MAX_LAYER; ii++)  
            UF_CALL(UF_LAYER_ask_status(ii, &old_layer_settings[ii-1]));  
        for (ii = 1; ii <= UF_LAYER_MAX_LAYER; ii++)  
            UF_CALL(UF_LAYER_set_status(ii, UF_LAYER_ACTIVE_LAYER));  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(0, &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype != UF_solid_body_subtype)) continue;  
            if (type == UF_faceted_model_type) continue;  
            UF_CALL(uc6409(object, &flag, view_name));  
            if (!flag) UF_CALL(UF_MODL_put_list_item(list, object));  
        }  
        for (ii = 1; ii <= UF_LAYER_MAX_LAYER; ii++)  
            UF_CALL(UF_LAYER_set_status(ii, old_layer_settings[ii-1]));  
        return make_an_array(&list, geometry);  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
    }  
    static void do_it(void)  
    {  
        int  
            n_model;  
        tag_t  
            *model_objs;  
        char  
            msg[133];  
        if ((n_model = ask_all_model_geometry(&model_objs)) > 0)  
        {  
            set_highlight_object_array(n_model, model_objs, TRUE);  
            sprintf(msg, "%d model objects highlighted", n_model);  
            uc1601(msg, TRUE);  
            set_highlight_object_array(n_model, model_objs, FALSE);  
            UF_free(model_objs);  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 包含了NX提供的标准头文件，定义了UF函数的调用方式，并实现了错误报告函数report_error。
> 2. 定义了内存分配函数allocate_memory和创建实体数组函数make_an_array。
> 3. ask_all_model_geometry函数用于获取模型中所有实体的列表。
> 4. set_highlight_object_array函数用于设置实体的高亮显示。
> 5. do_it函数首先获取模型中的所有实体，然后高亮显示它们，再取消高亮显示。
> 6. ufusr是主函数，初始化NX环境，调用do_it函数，最后关闭NX环境。
> 7. ufusr_ask_unload函数用于卸载用户自定义函数。
>
> 总体来说，这段代码实现了获取模型中的所有实体并高亮显示的功能，展示了NX二次开发的基本流程。
>
