### 【0734】create intersection geometry between selected objects 创建选定对象之间的交点几何

#### 代码

```cpp
    /*  
    The tolerance specified may no have any effect.  See PRs 6547640 and 6711883.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* 里海 */  
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( "Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_add_faces_and_edges, NULL,  
            &resp, &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static int create_intersection_objects(tag_t obj1, tag_t obj2, double tol, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            point;  
        uf_list_p_t  
            object_list;  
        UF_MODL_intersect_info_p_t  
            *intersections;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        UF_CALL(UF_MODL_intersect_objects(obj1, obj2, tol, &n, &intersections));  
        for (ii = 0; ii < n; ii++)  
        {  
            switch (intersections[ii]->intersect_type)  
            {  
                case UF_MODL_INTERSECT_POINT:  
                    UF_CALL(UF_CURVE_create_point(  
                        intersections[ii]->intersect.point.coords, &point));  
                    UF_CALL(UF_MODL_put_list_item(object_list, point));  
                    break;  
                case UF_MODL_INTERSECT_COINCIDE:  
                    UF_CALL(UF_MODL_put_list_item(object_list,  
                        intersections[ii]->intersect.coincide.identifier));  
                    break;  
                case UF_MODL_INTERSECT_CURVE:  
                    UF_CALL(UF_MODL_put_list_item(object_list,  
                        intersections[ii]->intersect.curve.identifier));  
                    break;  
                default:  
                    break;  
            }  
            UF_free(intersections[ii]);  
        }  
        if (n > 0) UF_free(intersections);  
        return make_an_array(&object_list, objects);  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            object[2],  
            *objects;  
        double  
            tol;  
        UF_MODL_ask_distance_tolerance(&tol);  
        while (((object[0] = select_anything("Intersect - object 1")) != NULL_TAG)  
            && ((object[1] = select_anything("Intersect - object 2")) != NULL_TAG)  
            && prompt_for_a_number("Intersect", "Tolerance", &tol))  
        {  
            n = create_intersection_objects(object[0], object[1], tol, &objects);  
            if (n > 0) UF_free(objects);  
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

> 这段代码是一段NX Open C++ API的二次开发代码，其主要功能是创建两个任意NX对象(如实体或曲面)的交集。以下是代码的主要内容和功能：
>
> 1. 定义了ECHO和report_error两个宏，用于打印错误消息。
> 2. 定义了mask_add_faces_and_edges函数，用于设置选择过滤器，允许选择实体中的面和边。
> 3. 定义了select_anything函数，用于弹出一个选择对话框，让用户选择一个对象，并返回其tag。
> 4. 定义了allocate_memory和make_an_array两个函数，用于分配内存并从列表中提取对象tag。
> 5. 定义了create_intersection_objects函数，用于计算两个对象(如实体或曲面)的交集，并将结果存储在一个tag数组中。
> 6. 定义了prompt_for_a_number函数，用于弹出一个对话框，让用户输入一个数值。
> 7. 定义了do_it函数，用于循环执行选择两个对象、获取公差、计算交集、释放内存等操作。
> 8. 定义了ufusr和ufusr_ask_unload两个函数，这是NX二次开发的标准入口和卸载函数。
> 9. 使用UF_MODL_intersect_objects API计算两个对象的交集，包括点、重合边、相交曲线等。
> 10. 代码整体采用了面向对象的设计，定义了多个函数，每个函数负责一个独立的功能，提高了代码的可读性和可维护性。
>
> 以上是该代码的主要内容和功能的简要介绍。
>
