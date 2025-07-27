### 【2921】split all solids with all planes method 3 将所有实体与所有平面进行分割方法3

#### 代码

```cpp
    /*HEAD SPLIT_ALL_SOLIDS_WITH_ALL_PLANES_METHOD_3 CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog is new in V18。 */  
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
    static uf_list_p_t list_all_solids(tag_t part)  
    {  
        tag_t  
            solid = NULL_TAG;  
        uf_list_p_t  
            solid_list;  
        UF_CALL(UF_MODL_create_list(&solid_list));  
        while ((solid = ask_next_solid_body(part, solid)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(solid_list, solid));  
        return solid_list;  
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
            ii, jj,  
            n_bodies,  
            n_planes,  
            n_split_bodies;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *planes,  
            *split_bodies,  
            temp_tag;  
        uf_list_p_t  
            body_list,  
            temp;  
        UF_timer_t  
            timer;  
        UF_timer_values_t  
            times;  
        n_planes = ask_all_of_type(part, UF_plane_type, &planes);  
        WRITE_D(n_planes);  
        body_list = list_all_solids(part);  
        UF_CALL(UF_MODL_ask_list_count(body_list, &n_bodies));  
        WRITE_D(n_bodies);  
        UF_CALL(UF_begin_timer(&timer));  
        for (ii = 0; ii < n_planes; ii++)  
        {  
            for (temp = body_list; temp != NULL; temp = temp->next)  
            {  
                if (UF_MODL_split_body(1, &temp->eid, planes[ii],  
                    &n_split_bodies, &split_bodies) && (ii < n_planes - 1))  
                {  
                /*  Something went wrong, try this plane again at the end 里海译:翻译如下：

出现了一些问题，请在最后再尝试一次该平面。 */  
                    temp_tag = planes[ii];  
                    for (jj = ii; jj < n_planes - 1; jj++)  
                        planes[jj] = planes[jj+1];  
                    planes[jj] = temp_tag;  
                    ii--;  
                    break;  
                }  
                else  
                {  
                    for (jj = 1; jj < n_split_bodies; jj++)  
                        UF_MODL_put_list_item(body_list, split_bodies[jj]);  
                    UF_free(split_bodies);  
                }  
            }  
        }  
        if (n_planes > 0) UF_free(planes);  
        UF_CALL(UF_MODL_ask_list_count(body_list, &n_bodies));  
        if (n_bodies > 0) UF_CALL(UF_MODL_delete_list(&body_list));  
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

> 这段代码是用于NX的二次开发，其主要功能是遍历零件中的所有实体，并使用所有平面进行切割。具体步骤如下：
>
> 1. 获取当前显示的零件对象。
> 2. 遍历零件，列出所有实体，并存储在一个列表中。
> 3. 遍历零件，列出所有平面，并存储在一个数组中。
> 4. 对列表中的每个实体，使用数组中的每个平面进行切割。
> 5. 切割后，将新生成的实体添加到列表中。
> 6. 遍历完成后，输出实体的数量。
> 7. 计时切割过程，输出CPU时间和实际时间。
>
> 代码中定义了多个函数，包括错误报告、内存分配、实体列表的创建和删除、获取下一个实体、列出所有实体、获取所有指定类型的对象、执行切割操作等。通过这些函数，实现了遍历切割的功能。
>
> 总体来说，这段代码实现了对NX模型中的实体进行遍历，并使用所有平面进行切割的功能，为后续的模型处理提供了基础。
>
