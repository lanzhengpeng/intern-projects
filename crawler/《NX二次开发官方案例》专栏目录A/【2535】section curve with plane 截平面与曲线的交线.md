### 【2535】section curve with plane 截平面与曲线的交线

#### 代码

```cpp
    /*HEAD SECTION_CURVE_WITH_PLANE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文，翻译如下：

UF_print_syslog 是 V18 中新增的。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static int mask_for_planes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_plane_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_plane(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_planes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static logical report_curve_intersections(tag_t curve1,tag_t curve2,double tol)  
    {  
        int  
            ii,  
            n;  
        UF_MODL_intersect_info_p_t  
            *intersections;  
        char  
            name1[MAX_ENTITY_NAME_SIZE+1] = { "" },  
            name2[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        UF_OBJ_ask_name(curve1, name1);  
        UF_OBJ_ask_name(curve2, name2);  
        if (!UF_CALL(UF_MODL_intersect_objects(curve1, curve2, tol, &n,  
            &intersections)))  
        {  
            printf("\nFound %d intersection(s) between %s (%d) and %s (%d)\n", n,  
                name1, curve1, name2, curve2);  
            for (ii = 0; ii < n; ii++)  
            {  
                switch (intersections[ii]->intersect_type)  
                {  
                    case UF_MODL_INTERSECT_POINT:  
                        display_temporary_point(  
                            intersections[ii]->intersect.point.coords);  
                        printf("  %d.  UF_MODL_INTERSECT_POINT\n", ii+1);  
                        printf("      coords = %f,%f,%f\n",  
                            intersections[ii]->intersect.point.coords[0],  
                            intersections[ii]->intersect.point.coords[1],  
                            intersections[ii]->intersect.point.coords[2]);  
                        printf("      object 1 u parm = %f\n",  
                            intersections[ii]->intersect.point.object_1_u_parm);  
                        printf("      object 1 v parm = %f\n",  
                            intersections[ii]->intersect.point.object_1_v_parm);  
                        printf("      object 2 u parm = %f\n",  
                            intersections[ii]->intersect.point.object_2_u_parm);  
                        printf("      object 2 v parm = %f\n",  
                            intersections[ii]->intersect.point.object_2_v_parm);  
                    break;  
                    case UF_MODL_INTERSECT_COINCIDE:  
                        printf("  %d.  UF_MODL_INTERSECT_COINCIDE\n", ii+1);  
                        UF_CALL(UF_OBJ_delete_object(  
                            intersections[ii]->intersect.coincide.identifier));  
                        printf("      object 1 1st parm = %f\n",  
                            intersections[ii]->intersect.  
                            coincide.object_1_first_parm);  
                        printf("      object 1 2nd parm = %f\n",  
                            intersections[ii]->intersect.coincide.  
                            object_1_second_parm);  
                        printf("      object 2 1st parm = %f\n",  
                            intersections[ii]->intersect.coincide.  
                            object_2_first_parm);  
                        printf("      object 2 2nd parm = %f\n",  
                            intersections[ii]->intersect.coincide.  
                            object_2_second_parm);  
                    break;  
                    case UF_MODL_INTERSECT_CURVE:  
                        printf("  %d.  UF_MODL_INTERSECT_CURVE\n", ii+1);  
                        UF_CALL(UF_OBJ_delete_object(  
                            intersections[ii]->intersect.curve.identifier));  
                    break;  
                    default:  
                        printf("  %d.  Unknown intersection type\n", ii+1);  
                    break;  
                }  
                UF_free(intersections[ii]);  
            }  
            if (n > 0) UF_free(intersections);  
            return TRUE;  
        }  
        else  
            return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            irc;  
        tag_t  
            curve,  
            plane;  
        double  
            guess[3],  
            junk[3],  
            parm,  
            point[3],  
            tol;  
        UF_MODL_ask_distance_tolerance(&tol);  
        while (((curve = select_a_curve("Section - Select curve")) != NULL_TAG)  
            && ((plane = select_a_plane("Section - Select plane")) != NULL_TAG))  
        {  
            if (!report_curve_intersections(curve, plane, tol))  
            {  
                for (ii = 0; ii < 11; ii++)  
                {  
                    parm = ii/10.0;  
                    UF_CALL(UF_MODL_ask_curve_props(curve, parm, guess, junk, junk,  
                        junk, junk, junk));  
                    FTN(uf5931)(&curve, &plane, guess, &parm, junk, point, &irc);  
                    if (irc == 1) display_temporary_point(point);  
                }  
            }  
        }  
    }  
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

> 这段代码是NX的二次开发代码，其主要功能是找出两条曲线与一个平面的交点，并将交点高亮显示。以下是代码的主要功能介绍：
>
> 1. 错误处理：定义了一个错误处理函数report_error，用于输出错误信息。
> 2. 曲线和平面选择：定义了曲线选择函数select_a_curve和平面选择函数select_a_plane，通过对话框选择曲线和平面，并设置选择掩码。
> 3. 交点计算：定义了交点计算函数report_curve_intersections，用于找出两条曲线之间的交点，并调用UF_MODL_intersect_objects函数计算交点，同时将交点高亮显示。
> 4. 曲线和平面交点计算：在主函数do_it中，循环调用曲线选择函数和平面选择函数，当用户选择了一条曲线和一个平面后，调用交点计算函数计算它们与平面的交点，并将交点高亮显示。
> 5. NX初始化和终止：在ufusr函数中，调用UF_initialize进行NX初始化，然后调用do_it函数执行主体功能，最后调用UF_terminate进行NX终止。
> 6. 卸载函数：定义了卸载函数ufusr_ask_unload，用于设置立即卸载二次开发程序。
>
> 总体而言，这段代码实现了曲线和平面交点的查询和显示功能，用户可以通过选择曲线和平面，查看它们之间的交点位置。
>
