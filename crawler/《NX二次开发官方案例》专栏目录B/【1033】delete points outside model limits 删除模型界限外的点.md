### 【1033】delete points outside model limits 删除模型界限外的点

#### 代码

```cpp
    /*HEAD DELETE_POINTS_OUTSIDE_MODEL_LIMITS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：
注意：UF_print_syslog 是 V18 中的新增功能，请只回答译文，不要添加其他内容。 */  
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
    static tag_t ask_next_point(tag_t part, tag_t point)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_point_type, &point));  
        return (point);  
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
    static void do_it(void)  
    {  
        int  
            n_bad,  
            units;  
        tag_t  
            *bad_points,  
            part = UF_PART_ask_display_part(),  
            point = NULL_TAG;  
        double  
            limits[2] = { 1000000, 39370.08 },  
            limit,  
            pos[3];  
        char  
            messg[MAX_LINE_SIZE+1];  
        uf_list_p_t  
            point_list;  
        UF_UI_open_listing_window();  
        UF_CALL(UF_PART_ask_units(part, &units));  
        limit = limits[units - 1];  
        limits[0] = -limit;  
        limits[1] = limit;  
        UF_CALL(UF_MODL_create_list(&point_list));  
        while ((point = ask_next_point(part, point)) != NULL_TAG)  
        {  
            UF_CALL(UF_CURVE_ask_point_data(point, pos));  
            if ( (pos[0]<limits[0]) || (pos[1]<limits[0]) || (pos[2]<limits[0]) ||  
                 (pos[0]>limits[1]) || (pos[1]>limits[1]) || (pos[2]>limits[1]))  
            {  
                sprintf(messg, "EID = %d at %f\t%f\t%f\n",  
                    point, pos[0], pos[1], pos[2]);  
                UF_CALL(UF_MODL_put_list_item(point_list, point));  
                UF_UI_write_listing_window(messg);  
            }  
        }  
        n_bad = make_an_array(&point_list, &bad_points);  
        if (n_bad > 0)  
        {  
            UF_CALL(UF_OBJ_delete_array_of_objects(n_bad, bad_points, NULL));  
            UF_free(bad_points);  
            sprintf(messg, "\nDeleted %d bad points.\n", n_bad);  
            UF_UI_write_listing_window(messg);  
        }  
        else  
            UF_UI_write_listing_window("No bad points found.\n");  
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

> 这段代码是一个NX的二次开发例程，主要功能是删除模型中超出指定坐标范围外的点。
>
> 主要步骤包括：
>
> 1. 初始化UF模块。
> 2. 获取当前显示的部件，并获取其单位。
> 3. 根据单位确定坐标范围限制。
> 4. 遍历部件中的所有点，判断是否超出坐标范围，如果是，则将其添加到列表中。
> 5. 删除列表中的所有点，并释放内存。
> 6. 终止UF模块。
>
> 代码通过UF系统函数实现了NX的交互式开发，使用UF_OBJ、UF_CURVE、UF_MODL等模块进行对象操作。主要功能是遍历模型中的点，判断其坐标是否超出指定范围，并将超出范围的点删除。如果发现超出范围的点，会将其添加到列表中，最后统一删除。
>
