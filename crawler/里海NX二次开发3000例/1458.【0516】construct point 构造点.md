### 【0516】construct point 构造点

#### 代码

```cpp
    /*HEAD CONSTRUCT_POINT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_so.h>  
    #include <uf_csys.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档中的说明，UF_print_syslog是V18版本中新增的函数。文档要求只提供翻译，不要添加任何额外的评论。因此，UF_print_syslog在V18版本中是新增的函数。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
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
    static void report_ui_response(int response)  
    {  
        WRITE("response = ");  
        switch (response)  
        {  
            case UF_UI_PICK_RESPONSE:  
                WRITE_D(UF_UI_PICK_RESPONSE);  
                break;  
            case UF_UI_OK:  
                WRITE_D(UF_UI_OK);  
                break;  
            case UF_UI_CANCEL:  
                WRITE_D(UF_UI_CANCEL);  
                break;  
            case UF_UI_BACK:  
                WRITE_D(UF_UI_BACK);  
                break;  
            case UF_UI_APPLY:  
                WRITE_D(UF_UI_APPLY);  
                break;  
            case UF_UI_HELP:  
                WRITE_D(UF_UI_HELP);  
                break;  
            case UF_UI_OBJECT_SELECTED:  
                WRITE_D(UF_UI_OBJECT_SELECTED);  
                break;  
            case UF_UI_OBJECT_SELECTED_BY_NAME:  
                WRITE_D(UF_UI_OBJECT_SELECTED_BY_NAME);  
                break;  
            case UF_UI_CB_TERMINATE:  
                WRITE_D(UF_UI_CB_TERMINATE);  
                break;  
            default:  
                WRITE("Unknown ");  
                WRITE_D(response);  
                break;  
        }  
    }  
    static void report_point_base_method(int method)  
    {  
        WRITE("method = ");  
        switch (method)  
        {  
            case UF_UI_POINT_INFERRED:  
                WRITE_D(UF_UI_POINT_INFERRED);  
                break;  
            case UF_UI_POINT_CURSOR_POS:  
                WRITE_D(UF_UI_POINT_CURSOR_POS);  
                break;  
            case UF_UI_POINT_EXISTING_PT:  
                WRITE_D(UF_UI_POINT_EXISTING_PT);  
                break;  
            case UF_UI_POINT_END_PT:  
                WRITE_D(UF_UI_POINT_END_PT);  
                break;  
            case UF_UI_POINT_CONTROL_PT:  
                WRITE_D(UF_UI_POINT_CONTROL_PT);  
                break;  
            case UF_UI_POINT_INTERSECT_PT:  
                WRITE_D(UF_UI_POINT_INTERSECT_PT);  
                break;  
            case UF_UI_POINT_CENTER_PT:  
                WRITE_D(UF_UI_POINT_CENTER_PT);  
                break;  
            case UF_UI_POINT_ANGLE_PT:  
                WRITE_D(UF_UI_POINT_ANGLE_PT);  
                break;  
            case UF_UI_POINT_QUADRANT_PT:  
                WRITE_D(UF_UI_POINT_QUADRANT_PT);  
                break;  
            case UF_UI_POINT_ON_CURVE_PT:  
                WRITE_D(UF_UI_POINT_ON_CURVE_PT);  
                break;  
            case UF_UI_POINT_ON_SURFACE_PT:  
                WRITE_D(UF_UI_POINT_ON_SURFACE_PT);  
                break;  
            case UF_UI_POINT_OFFSET_CSYS_PT:  
                WRITE_D(UF_UI_POINT_OFFSET_CSYS_PT);  
                break;  
            case UF_UI_POINT_DIALOG:  
                WRITE_D(UF_UI_POINT_DIALOG);  
                break;  
            case UF_UI_POINT_NO_METHOD:  
                WRITE_D(UF_UI_POINT_NO_METHOD);  
                break;  
            case UF_UI_POINT_APPLICATION_METHOD:  
                WRITE_D(UF_UI_POINT_APPLICATION_METHOD);  
                break;  
            default:  
                WRITE("Unknown UF_UI_POINT_base_method_t - ");  
                WRITE_D(method);  
                break;  
        }  
    }  
    static logical is_item_on_list(uf_list_p_t object_list, tag_t item)  
    {  
        int  
            ii;  
        uf_list_p_t  
            temp;  
        for (ii = 0, temp = object_list; temp != NULL; temp = temp->next, ii++)  
            if (temp->eid == item) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        logical  
            is_dumb,  
            is_smart,  
            is_so_subclass;  
        int  
            resp = 0;  
        tag_t  
            pt = NULL_TAG,  
            part = UF_ASSEM_ask_work_part();  
        double  
            disp_abs[3],  
            disp_wcs[3],  
            loc[3] = { 0,0,0 },  
            work_abs[3];  
        UF_UI_POINT_base_method_t  
            method = UF_UI_POINT_INFERRED;  
        uf_list_p_t  
            point_list;  
        UF_CALL(UF_MODL_create_list(&point_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_point_type, &pt))  
            && (pt != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(point_list, pt));  
    /*  The "cue" does not display - see PR 2084132 译:"The 'cue' does not display - see PR 2084132" */  
        while (!UF_CALL(UF_UI_point_construct("Specify Point", &method, &pt,  
            loc, &resp)))  
        {  
            report_ui_response(resp);  
        /*  response may not be correct - see PR 5002067 译:根据PR 5002067，翻译response may not be correct，译文如下：

翻译可能不正确。 */  
            if ((resp == UF_UI_CANCEL) || (pt == NULL_TAG)) break;  
            report_point_base_method(method);  
            if (!is_item_on_list(point_list, pt))  
            {  
            /*  As of NX 5, a new point tag is always returned 译:截至NX 5，总是返回一个新的点标签。 */  
                WRITE("Returned newly created point\n");  
                UF_CALL(UF_SO_set_visibility_option(pt, UF_SO_visible));  
                UF_CALL(UF_MODL_put_list_item(point_list, pt));  
            }  
            else  
            {  
            /*  Pre NX 5, an existing point tag is returned if selected 译:在NX 5之前，如果选择了一个现有的点标签，就会返回它。 */  
                WRITE("Returned previously existing point\n");  
            }  
        /*  the point class is a smart subclass - this will always be true 译:The point class is a smart subclass - this will always be true. */  
            UF_CALL(UF_SO_is_subclass(pt, &is_so_subclass));  
            WRITE_L(is_so_subclass);  
        /*  NX 5 Settings-> Associative = On (always On by default) 译:NX 5 设置 -> 关联性 = 打开（默认始终打开） */  
            UF_CALL(UF_SO_is_so(pt, &is_smart));  
            WRITE_L(is_smart);  
        /*  NX 5 Settings-> Associative = Off (user manually turned Off) 译:NX 5设置->关联=关闭(用户手动关闭) */  
            UF_CALL(UF_SO_has_become_dumb(pt, &is_dumb));  
            WRITE_L(is_dumb);  
            WRITE3F(loc);  
            UF_CALL(UF_CURVE_ask_point_data(pt, work_abs));  
            WRITE3F(work_abs);  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_WORK_COORDS, work_abs,  
                UF_CSYS_ROOT_COORDS, disp_abs));  
            WRITE3F(disp_abs);  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_WORK_COORDS, work_abs,  
                UF_CSYS_ROOT_WCS_COORDS, disp_wcs));  
            WRITE3F(disp_wcs);  
            WRITE("\n");  
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

> 这段代码是一个NX的二次开发代码示例，主要功能是使用UF_UI_point_construct函数来交互式地构造点。
>
> 代码流程如下：
>
> 1. 首先初始化NX环境。
> 2. 创建一个零件点列表，用于存储零件中的所有点。
> 3. 调用UF_UI_point_construct函数，用户可以交互式地指定一个点。这个函数会返回点的标签、构造方法等信息。
> 4. 根据返回的响应码，判断用户是选择了一个点、取消操作还是返回了其他响应。
> 5. 如果用户选择了一个点，会判断这个点是否已经在点列表中。如果是新创建的点，会添加到列表中；如果是已存在的点，直接使用。
> 6. 输出点的构造方法、坐标等信息。
> 7. 检查点是否是智能点，输出相关信息。
> 8. 循环调用UF_UI_point_construct，直到用户取消。
> 9. 最后，结束NX环境。
>
> 该代码示例展示了如何使用NX的交互式点构造功能，并在用户每次指定点后输出相关信息。
>
