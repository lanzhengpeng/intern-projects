### 【2541】section solid with planes 带有平面的实体

#### 代码

```cpp
    /*HEAD SECTION_SOLID_WITH_PLANES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文意，UF_print_syslog是在V18版本中新增的函数。 */  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
    }  
    static tag_t ask_next_plane(tag_t part, tag_t plane)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_plane_type, &plane));  
        return plane;  
    }  
    static int ask_body_faces(tag_t body, tag_p_t *body_faces)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *faces;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_body_faces(body, &face_list));  
        UF_CALL(UF_MODL_ask_list_count(face_list, &n));  
        *body_faces = (faces = (tag_t *)malloc(n * sizeof(tag_t)));  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_MODL_ask_list_item(face_list, ii, &faces[ii]));  
            if (UF_ASSEM_is_occurrence(body) && !UF_ASSEM_is_occurrence(faces[ii]))  
                faces[ii] = UF_ASSEM_find_occurrence(  
                    UF_ASSEM_ask_part_occurrence(body), faces[ii]);  
        }  
        UF_CALL(UF_MODL_delete_list(&face_list));  
        return n;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            jj,  
            n_faces,  
            num_intersections;  
        tag_t  
            body,  
            *faces,  
            part = UF_PART_ask_display_part(),  
            plane = NULL_TAG;  
        double  
            tol;  
        UF_MODL_intersect_info_p_t  
            *intersections;  
        UF_MODL_ask_distance_tolerance(&tol);  
        if ((body = ask_next_body(part, NULL_TAG)) != NULL_TAG)  
        {  
            n_faces = ask_body_faces(body, &faces);  
            while ((plane = ask_next_plane(part, plane)) != NULL_TAG)  
            {  
                UF_CALL(UF_DISP_set_highlight(plane, TRUE));  
                for (ii = 0; ii < n_faces; ii++)  
                {  
                    UF_CALL(UF_DISP_set_highlight(faces[ii], TRUE));  
                    if (!UF_CALL(UF_MODL_intersect_objects(faces[ii], plane, tol,  
                        &num_intersections, &intersections)))  
                    {  
                        for (jj = 0; jj < num_intersections; jj++)  
                            UF_free(intersections[jj]);  
                        if (num_intersections > 0) UF_free(intersections);  
                    }  
                    UF_CALL(UF_DISP_set_highlight(faces[ii], FALSE));  
                }  
                UF_CALL(UF_DISP_set_highlight(plane, FALSE));  
            }  
            free(faces);  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX Open C++代码实现了以下功能：
>
> 1. 初始化与终止：使用UF_initialize和UF_terminate函数初始化和终止NX环境。
> 2. 错误处理：通过自定义的report_error函数来报告错误，并使用宏UF_CALL来简化错误处理。
> 3. 遍历零件体：使用ask_next_body函数遍历零件中的所有实体，并返回下一个实体。
> 4. 遍历平面：使用ask_next_plane函数遍历零件中的所有平面。
> 5. 查询面：通过ask_body_faces函数查询实体的所有面，并返回面的数量和数组。
> 6. 交线计算：通过UF_MODL_intersect_objects函数计算每个面和平面的交线，并返回交点信息。
> 7. 高亮显示：使用UF_DISP_set_highlight函数高亮显示平面和面，以便在图形区中显示。
> 8. 内存释放：释放交线信息数组和面数组占用的内存。
> 9. NX Open C++ API调用：使用NX Open C++ API实现上述功能。
> 10. NX环境交互：在NX环境中通过UFusr函数入口点执行代码，并通过ufusr_ask_unload函数控制代码的卸载。
>
> 综上所述，这段代码实现了在NX环境中遍历零件的实体和平面，计算每个面和平面的交线，并在图形区中高亮显示交线，以进行交线分析。
>
