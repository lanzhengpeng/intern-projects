### 【3019】ungroup all groups 解组所有组

#### 代码

```cpp
    /*HEAD UNGROUP_ALL_GROUPS CCC UFUN */  
    /*  
        UF_GROUP_ungroup_all and UF_GROUP_ungroup_top have issues - see PR 2104488  
        This program demonstrates a work around to ungroup all groups in a part.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_undo.h>  
    #include <uf_group.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
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
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            ii, jj,  
            n_groups,  
            n_members;  
        tag_t  
            *groups,  
            *members,  
            part = UF_ASSEM_ask_work_part();  
        set_undo_mark("Ungroup all groups");  
        n_groups = ask_all_of_type(part, UF_group_type, &groups);  
        for (ii = 0; ii < n_groups; ii++)  
        {  
            UF_CALL(UF_GROUP_ask_group_data(groups[ii], &members, &n_members));  
            for (jj = 0; jj < n_members; jj++)  
                UF_CALL(UF_GROUP_del_member_from_group(members[jj], groups[ii]));  
            if (n_members > 0) UF_free(members);  
        }  
        if (n_groups > 0)  
        {  
            UF_CALL(UF_OBJ_delete_array_of_objects(n_groups, groups, NULL));  
            UF_free(groups);  
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

> 这段NX Open C++代码的目的是实现一个功能：在NX部件中取消所有组的分组。代码主要包含以下几个部分：
>
> 1. 头文件包含：代码包含了必要的NX Open C++头文件，如uf.h、uf_ui.h等。
> 2. 错误报告宏：定义了ECHO和UF_CALL宏，用于输出错误信息。
> 3. 内存分配函数：实现了allocate_memory函数，用于申请内存。
> 4. 创建数组函数：实现了make_an_array函数，用于将链表转换为数组。
> 5. 查询类型函数：实现了ask_all_of_type函数，用于查询部件中所有指定类型的对象。
> 6. 设置撤销标记函数：实现了set_undo_mark函数，用于设置撤销标记。
> 7. 执行主逻辑：实现了do_it函数，用于取消所有组的分组。该函数首先获取所有组，然后遍历每个组，删除其所有成员，最后删除组本身。
> 8. UFusr函数：实现了NX Open C++标准接口函数UFusr，在NX启动时调用，执行取消所有分组逻辑。
> 9. 卸载询问函数：实现了ufusr_ask_unload函数，返回立即卸载。
>
> 总的来说，这段代码通过NX Open C++ API实现了一个取消NX部件中所有分组的功能，利用了链表、数组、撤销等机制。代码结构清晰，注释详细，易于理解和维护。
>
