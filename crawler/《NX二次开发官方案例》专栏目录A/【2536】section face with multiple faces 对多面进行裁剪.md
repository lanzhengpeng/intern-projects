### 【2536】section face with multiple faces 对多面进行裁剪

#### 代码

```cpp
    /*HEAD SECTION_FACE_WITH_MULTIPLE_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中内容，UF_print_syslog在V18版本中是新增的功能。

所以翻译为：

注意：UF_print_syslog是V18版本新增的功能，请只回答翻译，不要添加其他内容。 */  
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
    /* qq3123197280 */  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static int select_faces(char *prompt, tag_t **faces)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select faces", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &cnt, faces));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*faces)[ii], FALSE));  
        return cnt;  
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
    static int create_intersection_objects(tag_t obj1, tag_t obj2, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            point;  
        double  
            tol;  
        uf_list_p_t  
            object_list;  
        UF_MODL_intersect_info_p_t  
            *intersections;  
        UF_MODL_ask_distance_tolerance(&tol);  
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
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_curves,  
            n_object_2,  
            jj,  
            type,  
            subtype;  
        tag_t  
            *curves,  
            object_1,  
            *object_2;  
        while (((object_1 = select_a_face("Select face to section")) != NULL_TAG) &&  
            ((n_object_2 = select_faces("Select section faces", &object_2)) > 0))  
        {  
            for (ii = 0; ii < n_object_2; ii++)  
            {  
                n_curves = create_intersection_objects(object_1, object_2[ii],  
                    &curves);  
                if (n_curves > 0)  
                    set_highlight_object_array(n_curves, curves, TRUE);  
                UF_free(curves);  
            }  
            UF_free(object_2);  
            UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
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

> 这段NX二次开发代码主要实现了一个交互式选择两个面，并计算它们交线（曲线）的功能。主要步骤如下：
>
> 1. 引入NX提供的头文件，定义了UF_CALL宏，用于错误报告。
> 2. 定义了mask_for_faces函数，设置选择过滤条件为实体类型下的任意面。
> 3. 定义了select_a_face函数，使用单选对话框让用户选择一个面。
> 4. 定义了select_faces函数，使用多选对话框让用户选择多个面。
> 5. 定义了allocate_memory、make_an_array等辅助函数，用于内存分配和链表操作。
> 6. 定义了create_intersection_objects函数，计算两个面的交线，并返回结果。
> 7. 定义了set_highlight_object_array函数，用于高亮显示交线。
> 8. 定义了do_it函数，循环让用户选择两个面，计算交线并高亮显示。
> 9. 定义了ufusr函数，初始化NX，调用do_it，结束NX。
> 10. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 整体来看，这段代码通过交互式选择面，计算交线并高亮显示，实现了面面之间的交线计算功能。
>
