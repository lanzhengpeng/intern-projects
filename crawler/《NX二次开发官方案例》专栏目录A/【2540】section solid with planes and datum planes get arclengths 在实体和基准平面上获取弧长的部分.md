### 【2540】section solid with planes and datum planes get arclengths 在实体和基准平面上获取弧长的部分

#### 代码

```cpp
    /*HEAD SECTION_SOLID_WITH_PLANES_AND_DATUM_PLANES_GET_ARCLENGTHS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog is new in V18 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); (UF_UI_write_listing_window(X))  
    static void report_arc_length(tag_t curve)  
    {  
        double  
            length;  
        char  
            msg[133];  
        UF_CALL(UF_CURVE_ask_arc_length(curve, 0.0, 1.0, UF_MODL_UNITS_PART,  
            &length));  
        sprintf(msg, "Curve tag %d has length: %f\n", curve, length);  
        WRITE(msg);  
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
    static tag_t ask_next_plane(tag_t part, tag_t plane, int *type)  
    {  
       int  
            types[2] = { UF_plane_type,  
                        UF_datum_plane_type};  
                    /*  UF_datum_plane_type}; 里海译:UF_datum_plane_type}; */  
        while ((*type < 2) &&  
            !UF_CALL(UF_OBJ_cycle_objs_in_part(part, types[*type], &plane)))  
        {  
            if (plane != NULL_TAG)  
                return plane;  
            else   
                *type = *type + 1;  
        }  
        *type = 0;  
        return NULL_TAG;  
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
    static void report_intersection_curves(UF_MODL_intersect_info_p_t intersection)  
    {  
        tag_t  
            curve;  
        curve = intersection->intersect.curve.identifier;  
        report_arc_length(curve);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            jj,  
            n_faces,  
            type = 0,  
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
            while ((plane = ask_next_plane(part, plane, &type )) != NULL_TAG)  
            {  
                UF_CALL(UF_DISP_set_highlight(plane, TRUE));  
                for (ii = 0; ii < n_faces; ii++)  
                {  
                    UF_CALL(UF_DISP_set_highlight(faces[ii], TRUE));  
                    if (!UF_CALL(UF_MODL_intersect_objects(faces[ii], plane, tol,  
                        &num_intersections, &intersections)))  
                    {  
                        for (jj = 0; jj < num_intersections; jj++)  
                        {  
                            report_intersection_curves(intersections[jj]);  
                            UF_free(intersections[jj]);  
                        }  
                        if (num_intersections > 0) UF_free(intersections);  
                    }  
                    UF_CALL(UF_DISP_set_highlight(faces[ii], FALSE));  
                }  
                UF_CALL(UF_DISP_set_highlight(plane, FALSE));  
            }  
            free(faces);  
        }  
    }  
    /* qq3123197280 */  
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

> 这段代码是一个NX的二次开发示例，主要功能是遍历装配中的实体和基准平面，计算它们之间的交线长度，并高亮显示相交的面和基准平面。
>
> 主要步骤包括：
>
> 1. 初始化NX环境。
> 2. 遍历装配中的实体，获取每个实体的所有面。
> 3. 遍历基准平面，对每个平面和实体面求交线。
> 4. 计算每条交线的长度。
> 5. 高亮显示相交的面和基准平面。
> 6. 释放内存并关闭NX环境。
>
> 该代码的关键点包括：
>
> 总体来说，这段代码通过NX提供的API实现了对装配体进行几何分析的功能，是NX二次开发的一个典型示例。
>
