### 【0666】create datum plane at planar face 在平面面上创建基准面

#### 代码

```cpp
    /*HEAD CREATE_DATUM_PLANE_AT_PLANAR_FACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，可以得知UF_print_syslog是V18版本中的新功能，用于打印系统日志。 */  
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
    static int mask_for_planar_face(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_planar_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_planar_face, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            num_dplanes,  
            point_select[3] = { 0,0,0 };  
        tag_t  
            dplanes[2],  
            eids[3];  
        while ((eids[0] = select_planar_face("Select planar face")) != NULL_TAG)  
            UF_CALL(UF_MODL_create_relative_dplane(1, eids, point_select,  
                    1, NULL, "0", "0", &num_dplanes, dplanes));  
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

> 这段代码是一个NX Open的二次开发示例，其主要功能是在NX中创建一个基于平面面的相对Datum平面。代码主要分为以下几个部分：
>
> 1. 头文件包含：包含了必要的NX Open头文件，如uf.h, uf_ui.h等。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于打印函数调用失败时的错误信息。
> 3. 选择面筛选函数：定义了一个选择面筛选函数mask_for_planar_face，用于在NX选择对话框中仅显示平面面特征。
> 4. 选择平面面函数：定义了一个选择平面面函数select_planar_face，用于在NX中选择一个平面面。
> 5. 主函数：定义了主函数do_it，在函数中不断循环调用select_planar_face选择平面面，然后调用UF_MODL_create_relative_dplane函数创建一个基于该平面面的相对Datum平面。
> 6. UFusr入口函数：定义了UFusr入口函数，在NX启动时调用do_it函数。
> 7. 卸载函数：定义了卸载函数，在NX卸载时调用。
>
> 整体来说，这段代码通过NX Open API实现了在NX中选择平面面并创建相对Datum平面的功能，是NX二次开发的一个典型示例。
>
