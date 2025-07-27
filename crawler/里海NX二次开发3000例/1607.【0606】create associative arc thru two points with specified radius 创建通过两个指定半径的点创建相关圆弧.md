### 【0606】create associative arc thru two points with specified radius 创建通过两个指定半径的点创建相关圆弧

#### 代码

```cpp
    /*HEAD CREATE_ASSOCIATIVE_ARC_THRU_TWO_POINTS_WITH_SPECIFIED_RADIUS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中的新功能。 */  
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
    /* 里海 */  
    static void do_it(void)  
    {  
        tag_t  
            p1 = NULL_TAG,  
            p2 = NULL_TAG,  
            spt_pln = NULL_TAG,  
            new_arc_feat = NULL_TAG;  
        double  
            radius = 0.;  
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
               ((prompt_for_a_number("Radius:", "Radius", &radius)) ==  TRUE ) &&  
               ((spt_pln=select_a_face_plane_datum("Support Plane:"))!= NULL_TAG)   
          )  
        {  
            UF_CALL( UF_CURVE_create_arc_point_point_radius( p1, p2, radius,   
                                       limit_p, spt_pln, TRUE, &new_arc_feat ));  
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

> 这段代码是NX的二次开发代码，主要用于在两个点之间创建一个指定半径的圆弧。下面是代码的主要内容和步骤：
>
> 1. 错误报告函数：report_error函数用于在函数调用失败时打印错误信息。
> 2. 选择点：select_a_point函数用于选择一个点，并返回点的tag。
> 3. 选择面/基准面：select_a_face_plane_datum函数用于选择一个面或基准面，并返回其tag。
> 4. 提示输入半径：prompt_for_a_number函数用于提示用户输入一个数值，并返回用户输入的数值。
> 5. 创建圆弧：do_it函数中包含一个循环，用于连续创建圆弧。在每次循环中，会先选择两个点、输入半径、选择支持面，然后调用UF_CURVE_create_arc_point_point_radius函数创建一个指定半径的圆弧。
> 6. UF初始化：ufusr函数负责在NX启动时初始化UF，并调用do_it函数。
> 7. 卸载函数：ufusr_ask_unload函数用于在NX关闭时卸载二次开发代码。
>
> 总的来说，这段代码通过UF函数，实现了在两个点之间创建一个指定半径的圆弧的功能。代码通过对话框和提示框实现了与用户的交互。
>
