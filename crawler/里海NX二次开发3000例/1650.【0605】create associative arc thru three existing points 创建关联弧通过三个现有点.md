### 【0605】create associative arc thru three existing points 创建关联弧通过三个现有点

#### 代码

```cpp
    /*HEAD CREATE_ASSOCIATIVE_ARC_THRU_THREE_EXISTING_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* 里海 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本中新增的函数。该函数用于打印系统日志信息。在V18版本之前，并没有这个函数。 */  
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
    /* 里海 */  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    /* 里海 */  
    static int mask_for_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* 里海 */  
    static tag_t select_a_point(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a point", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_points, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static int mask_for_face_plane_datums(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_plane_type, 0, 0 },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* 里海 */  
    static int select_a_face_plane_datum(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_face_plane_datums, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static void do_it(void)  
    {  
        tag_t  
            p1 = NULL_TAG,  
            p2 = NULL_TAG,  
            p3 = NULL_TAG,  
            spt_pln = NULL_TAG,  
            new_arc_feat = NULL_TAG;  
        UF_CURVE_limit_t   
            limit[2];  
        UF_CURVE_limit_p_t   
            limit_p[2];  
        limit[0].limit_type = UF_CURVE_limit_value;  
        limit[1].limit_type = UF_CURVE_limit_value;  
        limit[0].value = 0.;  
        limit[1].value = 360.;  
        limit_p[0] = &limit[0];  
        limit_p[1] = &limit[1];        
        while( ((p1 = select_a_point("First Point:")) != NULL_TAG ) &&  
               ((p2 = select_a_point("Second Point:")) != NULL_TAG ) &&  
               ((p3 = select_a_point("Third Point:")) != NULL_TAG ) &&  
               ((spt_pln=select_a_face_plane_datum("Support Plane:"))!= NULL_TAG)   
          )  
        {  
            UF_CALL( UF_CURVE_create_arc_3point( p1, p2, p3, limit_p,  
                                          spt_pln, TRUE, &new_arc_feat ));  
            WRITE_D( new_arc_feat );  
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

> 这段代码是一个NX的二次开发示例，用于通过三个现有点创建关联弧。以下是代码的主要内容和步骤：
>
> 1. 包含必要的头文件：代码中包含了进行NX二次开发所需的头文件，如uf.h、uf_curve.h等。
> 2. 定义错误报告函数report_error：这个函数用于在出错时打印错误信息。
> 3. 定义写入整数到列表窗口的函数write_integer_to_listing_window：这个函数用于将整数输出到NX的列表窗口中。
> 4. 定义用于选择点的函数mask_for_points和select_a_point：这些函数用于设置选择过滤条件，并选择一个点。
> 5. 定义用于选择面/平面/基准平面的函数mask_for_face_plane_datums和select_a_face_plane_datum：这些函数用于设置选择过滤条件，并选择一个面、平面或基准平面。
> 6. 定义主函数do_it：这个函数用于通过三个现有点和一个支持平面创建一个关联弧。它会提示用户选择三个点和一个支持平面，然后创建关联弧，并输出弧的tag到列表窗口。
> 7. 定义ufusr函数：这是NX二次开发必须的入口函数，用于初始化和调用do_it函数。
> 8. 定义卸载函数ufusr_ask_unload：这个函数用于在卸载NX时执行清理操作。
>
> 综上所述，这段代码通过选择三个点和一个支持平面，创建一个关联弧，并输出弧的tag到列表窗口。它展示了NX二次开发中的一些常用函数，如选择过滤、创建曲线、错误处理等。
>
