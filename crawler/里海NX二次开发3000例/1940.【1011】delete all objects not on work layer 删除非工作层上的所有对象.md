### 【1011】delete all objects not on work layer 删除非工作层上的所有对象

#### 代码

```cpp
    /*HEAD DELETE_ALL_OBJECTS_NOT_ON_WORK_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_layer.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数。 */  
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
    static void set_all_layers_active(void)  
    {  
        int ii;  
        for (ii = 1; ii < 257; ii++)  
            UF_CALL(UF_LAYER_set_status(ii, UF_LAYER_ACTIVE_LAYER));  
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
    static int ask_all_objects_on_layer(int layer, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(layer, &object)) &&  
            (object != NULL_TAG)) UF_CALL(UF_MODL_put_list_item(list, object));  
        return make_an_array(&list, objects);  
    }  
    static void do_it(void)  
    {  
        int  
            n_objs,  
            work;  
        tag_t  
            *objs;  
        UF_CALL(UF_LAYER_ask_work_layer(&work));  
        set_all_layers_active();  
        if (work > 1)  
            UF_CALL(UF_LAYER_set_status(work-1, UF_LAYER_WORK_LAYER));  
        else  
            UF_CALL(UF_LAYER_set_status(work+1, UF_LAYER_WORK_LAYER));  
        n_objs = ask_all_objects_on_layer(0, &objs);  
        UF_CALL(UF_OBJ_delete_array_of_objects(n_objs, objs, NULL));  
        UF_free(objs);  
        UF_CALL(UF_LAYER_set_status(work, UF_LAYER_WORK_LAYER));  
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

> 这段代码是一个NX Open C++应用程序，用于删除除工作层以外的所有对象。其主要功能包括：
>
> 1. 初始化和错误报告：通过UF_initialize()函数初始化NX环境，并定义了report_error()函数用于打印错误信息。
> 2. 设置所有层为活动层：通过set_all_layers_active()函数将所有层的状态设置为活动层。
> 3. 创建对象列表并获取对象数组：ask_all_objects_on_layer()函数用于获取指定层上的所有对象，并返回一个对象数组。
> 4. 删除除工作层以外的所有对象：通过do_it()函数实现以下操作：获取当前工作层设置除当前工作层以外的所有层为活动层删除除当前工作层以外的所有层上的对象恢复当前工作层状态
> 5. 获取当前工作层
> 6. 设置除当前工作层以外的所有层为活动层
> 7. 删除除当前工作层以外的所有层上的对象
> 8. 恢复当前工作层状态
> 9. 主函数和卸载处理：ufusr()函数作为主函数调用do_it()，ufusr_ask_unload()用于处理应用程序卸载。
>
> 总体而言，这段代码利用NX Open C++ API，实现了NX环境中删除除工作层以外的所有对象的功能，并具有完善的错误处理和内存管理。
>
