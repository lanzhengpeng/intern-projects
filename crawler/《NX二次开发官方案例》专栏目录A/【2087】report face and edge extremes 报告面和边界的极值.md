### 【2087】report face and edge extremes 报告面和边界的极值

#### 代码

```cpp
    /*HEAD REPORT_FACE_AND_EDGE_EXTREMES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，翻译为：

注意：UF_print_syslog 是在 V18 中新增的函数。 */  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    /* qq3123197280 */  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            num_edges = 0,  
            num_faces = 0;  
        tag_t  
            body = NULL_TAG,  
            tmp_edge = NULL_TAG,  
            tmp_face = NULL_TAG;  
        double  
            xtrms[6] = { 0., 0., 0., 1000000., 1000000., 1000000. };  
        uf_list_p_t  
            edge_list,  
            face_list;  
        while((body = select_a_body("Select a Body:")) != NULL_TAG )   
        {  
            UF_CALL( UF_MODL_ask_body_edges( body, &edge_list ));  
            UF_CALL( UF_MODL_ask_list_count( edge_list, &num_edges ));  
            WRITE_D( num_edges );  
            for( inx = 0; inx < num_edges; inx++ )  
            {  
                UF_CALL( UF_MODL_ask_list_item( edge_list, inx, &tmp_edge ));  
                filter_extremes( tmp_edge, xtrms );  
            }  
            UF_CALL( UF_MODL_ask_body_faces( body, &face_list ));  
            UF_CALL( UF_MODL_ask_list_count( face_list, &num_faces ));  
            WRITE_D( num_faces );  
            for( inx = 0; inx < num_faces; inx++ )  
            {  
                UF_CALL( UF_MODL_ask_list_item( face_list, inx, &tmp_face ));  
                filter_extremes( tmp_face, xtrms );  
            }  
            WRITE6F( xtrms );  
        }     
        UF_MODL_delete_list( &edge_list );      
        UF_MODL_delete_list( &face_list );      
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

> 这段代码是一个NX Open的UF程序，其主要功能是从NX模型中选取一个或多个实体，并获取其边和面的极值。具体来说：
>
> 1. 定义了错误处理函数report_error，用于在调用UF函数失败时输出错误信息。
> 2. 定义了整数和数组写入列表窗口的函数write_integer_to_listing_window和write_double_array_to_listing_window。
> 3. 定义了选取实体的函数select_a_body，它使用UF_UI_select_with_single_dialog对话框让用户选择一个实体。
> 4. 定义了获取实体极值的函数filter_extremes，它调用UF_MODL_ask_extreme函数来获取极值。
> 5. 定义了主函数do_it，它首先获取用户选择的实体，然后获取该实体的所有边和面，并调用filter_extremes函数来获取极值，最后输出到列表窗口。
> 6. 定义了ufusr入口函数，用于初始化和终止NX Open。
> 7. 定义了卸载函数ufusr_ask_unload。
>
> 总的来说，这段代码实现了从NX模型中选取实体并获取其边和面的极值的功能。
>
