### 【1953】report all reference sets 报告所有参考集

#### 代码

```cpp
    /*HEAD REPORT_ALL_REFERENCE_SETS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_fltr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。该函数用于打印系统日志。 */  
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
    static tag_t ask_next_of_type(tag_t part, int type, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object));  
        return (object);  
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
        while ((object = ask_next_of_type(part, type, object)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_ref_sets;  
        tag_t  
            *ref_sets,  
            part = UF_PART_ask_display_part();  
        char  
            ref_set_name[MAX_ENTITY_NAME_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        double  
            rr3[12];  
        UF_CALL(UF_UI_open_listing_window());  
        n_ref_sets = ask_all_of_type(part, UF_reference_set_type, &ref_sets);  
            for (ii = 0; ii < n_ref_sets; ii++)  
            {  
                UF_CALL(uc5811(ref_sets[ii], ref_set_name, rr3));  
                sprintf(msg, "Reference set %s has TAG of %d, Origin: %f, %f, %f\n",   
                    ref_set_name, ref_sets[ii], rr3[9], rr3[10], rr3[11]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
            }  
        if (n_ref_sets > 0) UF_free(ref_sets);  
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
> 1. 错误报告函数：定义了report_error函数，用于在函数调用失败时打印错误信息。
> 2. 内存分配和释放：提供了allocate_memory和make_an_array函数，用于申请和释放内存。
> 3. 对象查询：定义了ask_next_of_type和ask_all_of_type函数，用于遍历和查询指定类型的对象。
> 4. 参考集查询：do_it函数遍历当前显示部件的所有参考集，并打印参考集名称和TAG，以及原点坐标。
> 5. 主函数：ufusr函数负责初始化NX，调用do_it函数，并结束NX。
> 6. 卸载函数：ufusr_ask_unload函数声明了立即卸载。
>
> 总的来说，这段代码通过NX提供的API实现了参考集的查询和打印功能，并提供了错误处理、内存管理等辅助功能。
>
