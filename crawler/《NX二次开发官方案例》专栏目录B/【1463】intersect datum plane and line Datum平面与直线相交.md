### 【1463】intersect datum plane and line Datum平面与直线相交

#### 代码

```cpp
    /*HEAD INTERSECT_DATUM_PLANE_AND_LINE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 新增的函数。该函数用于将错误消息打印到系统日志。 */  
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
    static int mask_for_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_line_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static tag_t select_a_line(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a line", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* qq3123197280 */  
    static int mask_for_datum_planes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_datum_plane_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static tag_t select_a_datum_plane(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a datum plane", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datum_planes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* qq3123197280 */  
    static void do_it(void)  
    {  
        int  
            n = 0,  
            ii = 0;  
        tag_t  
            my_dpln = NULL_TAG,  
            my_line = NULL_TAG;  
        char  
            msg[300] = { "" };  
        double  
            tol = 0.001;  
        UF_MODL_intersect_info_p_t  
            *ints;  
        my_dpln = select_a_datum_plane("Select a Datum Plane:");  
        my_line = select_a_line("Select_a_line:");  
        if( NULL_TAG == my_dpln || NULL_TAG == my_line ) return;  
        UF_UI_open_listing_window();  
        UF_CALL(UF_MODL_intersect_objects( my_dpln, my_line, tol, &n, &ints));  
        if (n > 0)  
        {  
            sprintf(msg,"\nFound %d intersection(s) between (%d) and (%d)\n",   
                                                          n, my_dpln, my_line);  
            UF_UI_write_listing_window( msg );                                               
            for (ii = 0; ii < n; ii++)  
            {  
            switch (ints[ii]->intersect_type)  
            {  
                case UF_MODL_INTERSECT_POINT:  
                    sprintf( msg, "  %d.  UF_MODL_INTERSECT_POINT\n", ii+1);  
                    UF_UI_write_listing_window( msg );  
                    sprintf( msg, "      coords = %f,%f,%f\n",  
                        ints[ii]->intersect.point.coords[0],  
                        ints[ii]->intersect.point.coords[1],  
                        ints[ii]->intersect.point.coords[2]);  
                    UF_UI_write_listing_window( msg );  
                    sprintf( msg, "      object 1 u parm = %f\n",  
                        ints[ii]->intersect.point.object_1_u_parm);  
                    UF_UI_write_listing_window( msg );  
                    sprintf( msg, "      object 1 v parm = %f\n",  
                        ints[ii]->intersect.point.object_1_v_parm);  
                    UF_UI_write_listing_window( msg );  
                    sprintf( msg, "      object 2 u parm = %f\n",  
                        ints[ii]->intersect.point.object_2_u_parm);  
                    UF_UI_write_listing_window( msg );  
                    sprintf( msg, "      object 2 v parm = %f\n",  
                        ints[ii]->intersect.point.object_2_v_parm);  
                break;  
                case UF_MODL_INTERSECT_COINCIDE:  
                    sprintf( msg, "  %d.  UF_MODL_INTERSECT_COINCIDE\n", ii+1);  
                    UF_UI_write_listing_window( msg );  
                    UF_CALL(UF_OBJ_delete_object(  
                        ints[ii]->intersect.coincide.identifier));  
                    sprintf( msg, "      object 1 1st parm = %f\n",  
                        ints[ii]->intersect.coincide.object_1_first_parm);  
                    UF_UI_write_listing_window( msg );  
                    sprintf( msg, "      object 1 2nd parm = %f\n",  
                        ints[ii]->intersect.coincide.object_1_second_parm);  
                    UF_UI_write_listing_window( msg );  
                    sprintf( msg, "      object 2 1st parm = %f\n",  
                        ints[ii]->intersect.coincide.object_2_first_parm);  
                    UF_UI_write_listing_window( msg );  
                    sprintf( msg, "      object 2 2nd parm = %f\n",  
                        ints[ii]->intersect.coincide.object_2_second_parm);  
                break;  
                case UF_MODL_INTERSECT_CURVE:  
                    sprintf( msg, "  %d.  UF_MODL_INTERSECT_CURVE\n", ii+1);  
                    UF_UI_write_listing_window( msg );  
                    UF_CALL(UF_OBJ_delete_object(  
                        ints[ii]->intersect.curve.identifier));  
                break;  
                default:  
                    sprintf( msg, "  %d.  Unknown intersection type\n", ii+1);  
                    UF_UI_write_listing_window( msg );  
                break;  
            }  
            UF_free(ints[ii]);  
            }  
        UF_free(ints);  
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

> 根据代码内容，这段代码实现了以下功能：
>
> 1. 定义了错误处理函数report_error，用于打印错误信息。
> 2. 定义了两个选择函数select_a_line和select_a_datum_plane，用于选择线和基准面。
> 3. 定义了计算函数do_it，用于计算线和基准面的交点信息。
> 4. 定义了ufusr函数，用于初始化和执行计算。
> 5. 定义了卸载函数ufusr_ask_unload。
>
> 主要流程为：用户选择一条线和一个基准面，程序计算两者交点，并将结果输出到列表窗口。交点类型包括点、重合、曲线等，并会输出交点坐标和相关参数。通过交互式选择和计算交点，实现了NX的二次开发功能。
>
