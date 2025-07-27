### 【1987】report and display assembly bounding box 显示和报告装配体的边界框

#### 代码

```cpp
    /*HEAD REPORT_AND_DISPLAY_ASSEMBLY_BOUNDING_BOX CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

注意：UF_print_syslog是在V18版本中新增的，请只提供翻译，不要添加任何无关内容。 */  
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
    #define WRITE6F(X)  (write_double_array_to_listing_window(#X, X, 2, 3))  
    /* qq3123197280 */  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    /* qq3123197280 */  
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
    /* qq3123197280 */  
    static void filter_extremes( tag_t object, double *lims )  
    {  
        tag_t  
            subent = NULL_TAG;  
        double  
            xp[3] = { 1., 0., 0. },  
            yp[3] = { 0., 1., 0. },  
            zp[3] = { 0., 0., 1. },  
            xm[3] = { -1., 0., 0.  },  
            ym[3] = { 0., -1., 0.  },  
            zm[3] = { 0., 0., -1.  },  
            point[3] = { 0., 0., 0.  };  
        UF_CALL( UF_MODL_ask_extreme( object, xp, yp, zp, &subent, point ));  
        if( point[0] > lims[0] ) lims[0] = point[0];  
        if( point[1] > lims[1] ) lims[1] = point[1];  
        if( point[2] > lims[2] ) lims[2] = point[2];  
        UF_CALL( UF_MODL_ask_extreme( object, yp, zp, xp, &subent, point ));  
        if( point[0] > lims[0] ) lims[0] = point[0];  
        if( point[1] > lims[1] ) lims[1] = point[1];  
        if( point[2] > lims[2] ) lims[2] = point[2];  
        UF_CALL( UF_MODL_ask_extreme( object, zp, xp, yp, &subent, point ));  
        if( point[0] > lims[0] ) lims[0] = point[0];  
        if( point[1] > lims[1] ) lims[1] = point[1];  
        if( point[2] > lims[2] ) lims[2] = point[2];  
    /*----------------------------------------------------------------------------*/          
        UF_CALL( UF_MODL_ask_extreme( object, xm, ym, zm, &subent, point ));  
        if( point[0] < lims[3] ) lims[3] = point[0];  
        if( point[1] < lims[4] ) lims[4] = point[1];  
        if( point[2] < lims[5] ) lims[5] = point[2];  
        UF_CALL( UF_MODL_ask_extreme( object, ym, zm, xm, &subent, point ));  
        if( point[0] < lims[3] ) lims[3] = point[0];  
        if( point[1] < lims[4] ) lims[4] = point[1];  
        if( point[2] < lims[5] ) lims[5] = point[2];  
        UF_CALL( UF_MODL_ask_extreme( object, zm, xm, ym, &subent, point ));  
        if( point[0] < lims[3] ) lims[3] = point[0];  
        if( point[1] < lims[4] ) lims[4] = point[1];  
        if( point[2] < lims[5] ) lims[5] = point[2];  
        return;  
    }  
    /* qq3123197280 */  
    static void display_bounding_box( double * lims )  
    {  
        double  
            box[6],  
            end[3],  
            start[3];  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        box[0] = lims[3];  
        box[1] = lims[4];  
        box[2] = lims[5];  
        box[3] = lims[0];  
        box[4] = lims[1];  
        box[5] = lims[2];  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[1];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[1];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[1];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[1];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[4];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[4];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[4];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[4];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
    }  
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            num_edges = 0,  
            num_faces = 0;  
        tag_t  
            body = NULL_TAG,  
            part = UF_PART_ask_display_part(),  
            tmp_edge = NULL_TAG,  
            tmp_face = NULL_TAG;  
        double  
            xtrms[6] = { 0., 0., 0., 1000000., 1000000., 1000000. };  
        uf_list_p_t  
            edge_list,  
            face_list;  
        /*-----------------------------------------  
        If you just want to use the absolute XYZ vectors for the box,  
        you can call UF_FLTR_ask_box_of_assy, and pass the result to  
        the display_bounding_box function.  
        -------------------------------------------*/  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            UF_CALL( UF_MODL_ask_body_edges( body, &edge_list ));  
            UF_CALL( UF_MODL_ask_list_count( edge_list, &num_edges ));  
            for( inx = 0; inx < num_edges; inx++ )  
            {  
                UF_CALL( UF_MODL_ask_list_item( edge_list, inx, &tmp_edge ));  
                filter_extremes( tmp_edge, xtrms );  
            }  
            UF_CALL( UF_MODL_ask_body_faces( body, &face_list ));  
            UF_CALL( UF_MODL_ask_list_count( face_list, &num_faces ));  
            for( inx = 0; inx < num_faces; inx++ )  
            {  
                UF_CALL( UF_MODL_ask_list_item( face_list, inx, &tmp_face ));  
                filter_extremes( tmp_face, xtrms );  
            }  
            UF_MODL_delete_list( &edge_list );      
            UF_MODL_delete_list( &face_list );      
        }     
        WRITE6F( xtrms );  
        display_bounding_box( xtrms );  
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

> 这段NX二次开发代码的主要功能是显示装配体的边界框。代码的关键组成部分包括：
>
> 1. 错误报告函数：report_error，用于在函数调用失败时报告错误。
> 2. 数组写入列表窗口函数：write_double_array_to_listing_window，用于将数组写入列表窗口。
> 3. 获取下一个体的函数：ask_next_body，用于遍历装配体中的所有体。
> 4. 过滤极值函数：filter_extremes，用于获取体的边界框极值。
> 5. 显示边界框函数：display_bounding_box，用于使用临时线显示边界框。
> 6. 主函数：do_it，用于遍历装配体中的所有体，并调用filter_extremes获取边界框，最后调用display_bounding_box显示边界框。
> 7. UFusr函数：ufusr，作为NX二次开发的入口函数，初始化后调用do_it，最后终止。
> 8. 卸载函数：ufusr_ask_unload，用于立即卸载用户函数库。
>
> 总体来说，这段代码通过遍历装配体中的所有体，获取每个体的边界框，并计算最终的装配体边界框，然后使用临时线显示出来。
>
