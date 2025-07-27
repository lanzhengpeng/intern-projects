### 【2920】split all solids with all planes method 2 将所有实体与所有平面分割方法2

#### 代码

```cpp
    /*HEAD SPLIT_ALL_SOLIDS_WITH_ALL_PLANES_METHOD_2 CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，UF_print_syslog是V18版本中新增的功能。在翻译中，只提供UF_print_syslog在V18版本中新增的答案，不添加任何其他内容。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static tag_t ask_next_solid_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                UF_CALL(UF_MODL_ask_body_type(body, &type));  
                if (type == UF_MODL_SOLID_BODY) return body;  
            }  
        }  
        return NULL_TAG;  
    }  
    static int ask_all_solids(tag_t part, tag_t **solids)  
    {  
        tag_t  
            solid = NULL_TAG;  
        uf_list_p_t  
            solid_list;  
        UF_CALL(UF_MODL_create_list(&solid_list));  
        while ((solid = ask_next_solid_body(part, solid)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(solid_list, solid));  
        return (make_an_array(&solid_list, solids));  
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
            err,  
            ii, jj, kk,  
            n_bodies,  
            n_planes,  
            n_split_bodies;  
        tag_t  
            *bodies,  
            part = UF_PART_ask_display_part(),  
            *planes,  
            *split_bodies,  
            temp;  
        UF_timer_t  
            timer;  
        UF_timer_values_t  
            times;  
        n_planes = ask_all_of_type(part, UF_plane_type, &planes);  
        WRITE_D(n_planes);  
        n_bodies = ask_all_solids(part, &bodies);  
        WRITE_D(n_bodies);  
        UF_CALL(UF_begin_timer(&timer));  
        for (ii = 0; ii < n_planes; ii++)  
        {  
            for (jj = 0; jj < n_bodies; jj++)  
            {  
                if (UF_MODL_split_body(1, &bodies[jj], planes[ii],  
                    &n_split_bodies, &split_bodies) && (ii < n_planes - 1))  
                {  
                /*  Something went wrong, try this plane again at the end 里海译:Something went wrong, try this plane again at the end. */  
                    temp = planes[ii];  
                    for (kk = ii; kk < n_planes - 1; kk++)  
                        planes[kk] = planes[kk+1];  
                    planes[kk] = temp;  
                    ii--;  
                    break;  
                }  
                else  
                {  
                    if (n_split_bodies > 1)  
                    {  
                        bodies = UF_reallocate_memory(bodies,  
                            (n_bodies + n_split_bodies - 1) * sizeof(tag_t), &err);  
                        for (kk = 1; kk < n_split_bodies; kk++)  
                            bodies[n_bodies + kk - 1] = split_bodies[kk];  
                        n_bodies = n_bodies + (n_split_bodies - 1);  
                    }  
                    UF_free(split_bodies);  
                }  
            }  
        }  
        if (n_planes > 0) UF_free(planes);  
        if (n_bodies > 0) UF_free(bodies);  
        WRITE_D(n_bodies);  
        UF_CALL(UF_end_timer(timer, &times));  
        WRITE_F(times.cpu_time);  
        WRITE_F(times.real_time);  
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

> 这是用于NX的二次开发代码，主要功能是使用所有平面分割所有实体。具体来说：
>
> 1. 初始化和错误处理：首先进行NX的初始化，并定义了错误处理函数report_error，用于打印错误信息。
> 2. 内存分配和对象列表操作：定义了内存分配函数allocate_memory和对象列表操作函数make_an_array，用于处理对象列表。
> 3. 查询所有实体和面：定义了函数ask_next_solid_body和ask_all_solids，用于查询当前部件的所有实体。同时，ask_all_of_type函数用于查询所有指定类型的对象。
> 4. 分割实体：在函数do_it中，遍历所有面，对每个面遍历所有实体进行分割，并将分割后的实体添加到实体列表中。使用了UF_MODL_split_body函数进行实体分割。
> 5. 计时和资源释放：函数do_it中还使用了计时函数UF_begin_timer和UF_end_timer来统计分割操作的时间，并在最后释放分配的内存。
> 6. 主函数和卸载函数：主函数ufusr中调用do_it函数执行分割操作，并在结束时进行NX的终止。卸载函数ufusr_ask_unload用于立即卸载该用户函数。
>
> 总体来说，这段代码实现了在NX中遍历所有面，使用每个面分割所有实体的功能，并在分割后更新实体列表，最后统计操作时间。
>
