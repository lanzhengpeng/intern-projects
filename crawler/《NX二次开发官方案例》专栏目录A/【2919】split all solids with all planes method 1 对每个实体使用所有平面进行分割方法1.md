### 【2919】split all solids with all planes method 1 对每个实体使用所有平面进行分割方法1

#### 代码

```cpp
    /*HEAD SPLIT_ALL_SOLIDS_WITH_ALL_PLANES_METHOD_1 CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，V18版本引入了新的功能UF_print_syslog，该功能用于打印系统日志。文档中提到，只需要回答有关UF_print_syslog的译文，不需要添加任何废话。 */  
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
            ii, jj, kk,  
            n_bodies = 0,  
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
                /*  Something went wrong, try this plane again at the end 里海译:对不起，翻译出现错误，请稍后再试。 */  
                    temp = planes[ii];  
                    for (kk = ii; kk < n_planes - 1; kk++)  
                        planes[kk] = planes[kk+1];  
                    planes[kk] = temp;  
                    ii--;  
                    break;  
                }  
                else UF_free(split_bodies);  
            }  
            UF_free(bodies);  
            n_bodies = ask_all_solids(part, &bodies);  
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

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要功能是对NX部件中的所有实体进行平面切割。具体功能如下：
>
> 1. 首先定义了错误处理函数report_error，用于打印API调用失败的错误信息。
> 2. 定义了两个宏WRITE_D和WRITE_F，用于将整型和浮点型数据写入列表窗口。
> 3. 定义了内存分配函数allocate_memory，用于分配指定大小的内存。
> 4. 定义了创建数组函数make_an_array，用于将列表转换为数组。
> 5. 定义了ask_next_solid_body函数，用于遍历并获取NX部件中的下一个实体。
> 6. 定义了ask_all_solids函数，用于获取NX部件中所有的实体。
> 7. 定义了ask_all_of_type函数，用于获取NX部件中指定类型的所有对象。
> 8. 主函数do_it首先获取NX部件中的所有实体和所有平面，然后遍历每个平面，对每个实体进行切割，并释放切割后的新实体。在循环中，如果切割失败，则将当前平面移至最后重新切割。
> 9. ufusr函数是NX二次开发的标准入口函数，调用do_it函数执行切割操作。
> 10. ufusr_ask_unload函数返回立即卸载标志，表示该用户函数不需要保留在内存中。
>
> 综上所述，这段代码实现了NX部件中所有实体与所有平面的切割功能，具有较完善的错误处理和数据输出机制。
>
