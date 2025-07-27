### 【2310】report select dimensions drawing and view info 报告选择尺寸图纸和视图信息

#### 代码

```cpp
    /*HEAD REPORT_SELECT_DIMENSIONS_DRAWING_AND_VIEW_INFO CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
    #include <uf_part.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog在V18版本中是新增的功能。它用于打印系统日志。 */  
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
    static int mask_for_dimensions(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_dimension_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dimension(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "Select a dimension",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dimensions, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t ask_tag_of_drawing(char *dwg_name)  
    {  
        tag_t  
            dwg = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, dwg_name, UF_drawing_type,  
            FALSE, &dwg));  
        return dwg;  
    }  
    #define WRITE(X)   (UF_UI_write_listing_window(X))  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            subtype,  
            status;  
        tag_t  
            dim,  
            dwg_tag;  
        double  
            origin[3];  
        char  
            *dwg_name,  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_DRF_dim_info_t  
            *info;  
        while ((dim = select_a_dimension("Verify drawing and view")) != NULL_TAG)  
        {  
            UF_CALL(uc6409(dim, &status, view_name));  
            WRITE_S(view_name);  
            dwg_name = strtok(view_name, "@");  
            WRITE_S(dwg_name);  
            dwg_tag = ask_tag_of_drawing(dwg_name);  
            WRITE_D(dwg_tag);  
            WRITE("\n");  
            UF_CALL(UF_DRF_ask_dim_info(dim, &subtype, origin, &info));  
            WRITE_D(info->dim_line_info->num_assoc_objs);  
            for (ii = 0; ii < info->dim_line_info->num_assoc_objs; ii++)  
            {  
                WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_object_view_tag);  
                UF_CALL(UF_OBJ_ask_name(info->dim_line_info->  
                    assoc_objs[ii].assoc_object_view_tag, view_name));  
                WRITE_S(view_name);  
            }  
            UF_CALL(UF_DRF_free_dimension(&info));  
            WRITE("\n\n");  
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

> 这段代码是一个NX Open C++的二次开发示例，其主要功能是选择一个尺寸，获取其所在的图纸和视图信息，并打印到日志窗口。
>
> 主要函数和功能介绍如下：
>
> 1. report_error：用于打印错误信息。
> 2. mask_for_dimensions：用于设置选择过滤条件，只选择尺寸。
> 3. select_a_dimension：弹出选择对话框，选择一个尺寸。
> 4. ask_tag_of_drawing：根据图纸名称查询图纸对象。
> 5. write_string_to_listing_window：打印字符串到日志窗口。
> 6. write_integer_to_listing_window：打印整数到日志窗口。
> 7. do_it：主要逻辑函数，循环选择尺寸，获取图纸和视图信息，打印到日志窗口。
> 8. ufusr：NX二次开发入口函数。
> 9. ufusr_ask_unload：询问卸载函数。
>
> 通过选择一个尺寸，该代码可以获取其所在图纸和视图信息，并打印到日志窗口，展示了NX二次开发中常用的函数和交互方式。
>
