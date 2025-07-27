### 【2539】section solid with plane 对实体进行截面分析，选择一个平面作为截面

#### 代码

```cpp
    /*HEAD SECTION_SOLID_WITH_PLANE CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的功能。该功能用于打印系统日志。 */  
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
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *(int *)data)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_body_type( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_solid(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp,  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            irc,  
            one = 1,  
            num_curves,  
            ten = 10;  
        tag_t  
            body[1],  
            plane;  
        tag_t  
            curves[10];  
        while (((body[0] = select_a_solid("Select solid to section")) != NULL_TAG)  
            && ((plane = select_a_plane("Select section plane")) != NULL_TAG))  
        {  
            FTN (uf6524)( body, &one, &plane, &ten, &irc, &num_curves, curves );  
            for (ii=0; ii<num_curves; ii++)  
                UF_CALL(UF_DISP_add_item_to_display(curves[ii]));  
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

> 这段代码是用于NX的二次开发，实现了对实体进行切片并显示切片曲线的功能。
>
> 代码主要包含以下部分：
>
> 1. 包含头文件，引入了UF函数库和UI函数库等。
> 2. 定义了错误处理函数report_error，用于在调用UF函数出错时打印错误信息。
> 3. 定义了过滤函数filter_body_type，用于在选取对话框中过滤出实体。
> 4. 定义了设置选取掩码的函数mask_body_type，用于在选取对话框中只允许选取实体。
> 5. 定义了选取实体的函数select_a_solid，通过对话框让用户选择一个实体。
> 6. 定义了设置切片平面选取掩码的函数mask_for_planes。
> 7. 定义了选取切片平面的函数select_a_plane。
> 8. 定义了do_it函数，是主功能函数，循环让用户选择实体和平面，然后调用UF函数进行切片，并显示切片曲线。
> 9. 定义了ufusr函数，是NX调用的入口函数，初始化UF库，调用do_it函数，然后终止UF库。
> 10. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发模块不需要延迟卸载。
>
> 总体来说，这段代码实现了选取实体和平面，进行切片并显示切片曲线的功能，是一个典型的NX二次开发示例。
>
