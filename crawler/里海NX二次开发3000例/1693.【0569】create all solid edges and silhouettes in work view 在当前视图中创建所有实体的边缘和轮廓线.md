### 【0569】create all solid edges and silhouettes in work view 在当前视图中创建所有实体的边缘和轮廓线

#### 代码

```cpp
    /*HEAD CREATE_ALL_SOLID_EDGES_AND_SILHOUETTES_IN_WORK_VIEW CCC UFUN */  
    /*  This is an example program which demonstrates how to get around the fact  
        that there is no API function to:  
            Insert->Curve Operation->Extract...->All in Work View  
        An ER 4258561 has been filed requesting that this function be created.  
        In the meantime, the work around is to call GRIP and use the command  
        CRSEWV.  The GRIP program called using UF_call_grip contains only these  
        two lines:  
            CRSEWV  
            HALT  
    */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_layer.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 版本新增的函数。 */  
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
    static int ask_all_objects_on_work_layer(tag_t **objects)  
    {  
        int  
            work;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        UF_CALL(UF_LAYER_ask_work_layer(&work));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(work, &object)) &&  
            (object != NULL_TAG)) UF_CALL(UF_MODL_put_list_item(list, object));  
        return make_an_array(&list, objects);  
    }  
    static int whats_new(tag_t *old, int n_old, tag_t *now, int n_now, tag_t **new)  
    {  
        int  
            ii,  
            jj;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < n_now; ii++)  
        {  
            for (jj = 0; jj < n_old; jj++)  
                if (old[jj] == now[ii]) break;  
            if (jj == n_old) UF_CALL(UF_MODL_put_list_item(list, now[ii]));  
        }  
        return (make_an_array(&list, new));  
    }  
    static int find_next_empty_layer(int start)  
    {  
        int  
            ii;  
        tag_t  
            object;  
        for (ii = start; ii < 257; ii++)  
        {  
            object = NULL_TAG;  
            UF_CALL(UF_LAYER_cycle_by_layer(ii, &object));  
            if (object == NULL_TAG) return ii;  
        }  
        return 0;  
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
            empty,  
            n_after,  
            n_before,  
            n_new,  
            work;  
        tag_t  
            *after,  
            *before,  
            *new;  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_LAYER_ask_work_layer(&work));  
        if ((empty = find_next_empty_layer(work)) != 0)  
            UF_CALL(UF_LAYER_set_status(empty, UF_LAYER_WORK_LAYER));  
        n_before = ask_all_objects_on_work_layer(&before);  
    #ifdef _WIN32  
        UF_CALL(UF_call_grip("n:\\grip\\crsewv.grx", 0, NULL));  
    #else  
        UF_CALL(UF_call_grip("grip/crsewv.grx", 0, NULL));  
    #endif  
        n_after = ask_all_objects_on_work_layer(&after);  
        n_new = whats_new(before, n_before, after, n_after, &new);  
        set_highlight_object_array(n_new, new, TRUE);  
        sprintf(msg, "%d new curves created", n_new);  
        uc1601(msg, TRUE);  
        set_highlight_object_array(n_new, new, FALSE);  
        UF_free(after);  
        UF_free(before);  
        UF_free(new);  
        if (empty != 0) UF_CALL(UF_LAYER_set_status(work, UF_LAYER_WORK_LAYER));  
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

> 这段代码是NX的二次开发示例，其功能是在工作层上创建所有实体的轮廓和轮廓线。主要流程如下：
>
> 1. 定义了一些辅助函数，用于报告错误、分配内存、创建实体数组、获取工作层上的所有实体、找出新创建的实体等。
> 2. 主函数do_it()首先找到下一个空的工作层，并设置为当前工作层。
> 3. 获取当前工作层上的所有实体，并记录下来。
> 4. 调用GRIP程序执行命令CRSEWV，在工作层上创建所有实体的轮廓和轮廓线。
> 5. 再次获取当前工作层上的所有实体，并与之前的实体对比，找出新创建的实体。
> 6. 高亮显示新创建的实体，并统计数量。
> 7. 最后恢复原来的工作层设置。
> 8. ufusr()函数是NX调用的主函数，其中初始化NX，调用do_it()函数，然后终止NX。
> 9. ufusr_ask_unload()函数用于卸载用户自定义程序。
>
> 通过以上流程，代码实现了在工作层上创建所有实体的轮廓和轮廓线的功能。
>
