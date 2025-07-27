### 【0089】alternate colors on curve and body objects 在曲线和实体对象上交替使用颜色

#### 代码

```cpp
    /*HEAD ALTERNATE_COLORS_ON_CURVE_AND_BODY_OBJECTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18中的新功能，用于打印系统日志。 */  
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
    static tag_t ask_next_curve_or_body(tag_t part, tag_t curve_or_body, int *type)  
    {  
        int  
            this_type,  
            this_subtype,  
            types[5] = { UF_line_type,  
                         UF_circle_type,  
                         UF_conic_type,  
                         UF_spline_type,  
                         UF_solid_type };  
        while ((*type < 5) &&  
            !UF_CALL(UF_OBJ_cycle_objs_in_part(part, types[*type], &curve_or_body)))  
        {  
            if (curve_or_body != NULL_TAG)   
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(curve_or_body, &this_type, &this_subtype));  
                switch (this_type)  
                {  
                    case UF_circle_type:  
                        return curve_or_body;  
                        break;  
                    case UF_line_type:  
                        return curve_or_body;  
                        break;  
                    case UF_conic_type:  
                        return curve_or_body;  
                        break;  
                    case UF_spline_type:  
                        return curve_or_body;  
                        break;  
                    case UF_solid_type:  
                        printf("found a solid type tag: %d\n", curve_or_body);  
                        if (this_subtype < 2)  
                        {  
                            printf("found a solid: tag: %d\n", curve_or_body);  
                            return curve_or_body;  
                        }  
                        break;  
                    default:  
                        break;  
                }  
            }  
            else   
                *type = *type + 1;  
        }  
        *type = 0;  
        return NULL_TAG;  
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
    static int ask_all_curves_and_bodies(tag_t part, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            obj_list;  
        int  
            type = 0;  
        UF_CALL(UF_MODL_create_list(&obj_list));  
        while ((object = ask_next_curve_or_body(part, object, &type )) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(obj_list, object));  
        return (make_an_array(&obj_list, objects));  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0,  
            n = 0;  
        tag_t  
            *objects,  
            part = UF_PART_ask_display_part();  
        n = ask_all_curves_and_bodies(part, &objects);  
        for (ii = 0; ii < n; ii++)   
        {  
            UF_CALL(UF_OBJ_set_color(objects[ii], (1 + ii%214) ));  
            printf("color: %d\n", (1 + ii%214));  
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

> 这段代码是用于NX的二次开发，主要功能是遍历零件中的所有曲线和实体，并将它们分别赋予不同的颜色。
>
> 主要步骤包括：
>
> 1. 初始化UF模块。
> 2. 获取当前显示的零件。
> 3. 遍历零件中的所有曲线和实体，将它们添加到列表中。
> 4. 将列表转换为数组。
> 5. 遍历数组，对每个对象设置不同的颜色。
> 6. 结束UF模块。
>
> 代码的关键点包括：
>
> 总体来说，这段代码实现了在NX中遍历零件对象并设置颜色的重要功能。
>
