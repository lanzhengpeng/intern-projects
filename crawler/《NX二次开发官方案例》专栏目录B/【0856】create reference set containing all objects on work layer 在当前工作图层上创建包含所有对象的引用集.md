### 【0856】create reference set containing all objects on work layer 在当前工作图层上创建包含所有对象的引用集

#### 代码

```cpp
    /*HEAD CREATE_REFERENCE_SET_CONTAINING_ALL_OBJECTS_ON_WORK_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是在V18版本中新增的。所以只回答翻译，不要添加任何废话。 */  
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
    static logical is_immediate_child_component(tag_t component)  
    {  
        tag_t  
            inst = UF_ASSEM_ask_inst_of_part_occ(component),  
            parent = UF_ASSEM_ask_parent_of_instance(inst),  
            part = UF_ASSEM_ask_work_part();  
        if (parent == part) return TRUE;  
        return FALSE;  
    }  
    static int ask_all_local_objects_on_layer(int layer, tag_t **objects)  
    {  
        logical  
            is_transferable;  
        int  
            subtype,  
            type;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(layer, &object)) &&  
            (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_is_transferable(object, &is_transferable));  
            if (!is_transferable) continue;  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if ((type == UF_component_type && is_immediate_child_component(object))  
                || !UF_ASSEM_is_occurrence(object))  
                UF_CALL(UF_MODL_put_list_item(list, object));  
        }  
        return make_an_array(&list, objects);  
    }  
    static void do_it(void)  
    {  
        int  
            n_objects,  
            work;  
        tag_t  
            *objects,  
            ref_set;  
        double  
            abs[9] = { 1,0,0, 0,1,0, 0,0,1 },  
            zero[3] = { 0,0,0 };  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_LAYER_ask_work_layer(&work));  
        sprintf(name, "LAYER_%d", work);  
        n_objects = ask_all_local_objects_on_layer(work, &objects);  
        if (n_objects > 0)  
        {  
            UF_CALL(UF_ASSEM_create_ref_set(name, zero, abs, objects, n_objects,  
                &ref_set));  
            UF_free(objects);  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX Open C++ API的二次开发示例，其主要功能是创建一个参考集，该参考集包含当前工作层上的所有对象。以下是代码的主要功能和实现步骤：
>
> 1. 定义错误报告函数：report_error函数用于记录和打印API调用失败的错误信息。
> 2. 内存分配函数：allocate_memory用于分配指定大小的内存。
> 3. 数组创建函数：make_an_array用于从对象列表创建对象数组。
> 4. 组件判断函数：is_immediate_child_component用于判断组件是否为当前工作部件的直接子组件。
> 5. 对象查询函数：ask_all_local_objects_on_layer用于查询工作层上的所有对象，并将这些对象放入一个列表中。
> 6. 创建参考集函数：do_it用于创建一个参考集，其包含工作层上的所有对象。
> 7. API初始化和清理：ufusr函数负责初始化和清理API。
> 8. 卸载函数：ufusr_ask_unload返回立即卸载标志。
>
> 通过以上函数的配合使用，该代码实现了创建一个包含当前工作层上所有对象的参考集的功能。
>
