### 【2832】set wcs by selected object 将WCS设置为选定对象

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_csys.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* qq3123197280 */  
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( "Select an object", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_add_faces_and_edges, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void set_wcs_by_object(tag_t object)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            csys,  
            mx;  
        double  
            origin[3] = { 0.0, 0.0, 0.0 };  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        if (subtype == UF_solid_edge_subtype)  
            UF_CALL(UF_MODL_create_curve_from_edge(object, &object));  
        UF_CALL(UF_CSYS_ask_matrix_of_object(object, &mx));  
        if (mx != NULL_TAG)  
        {  
            UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &csys));  
            UF_CALL(UF_CSYS_set_wcs(csys));  
        }  
        else  
        {  
            uc1601("Object has no matrix", TRUE);  
        }  
        if (subtype == UF_solid_edge_subtype)  
            UF_CALL(UF_OBJ_delete_object(object));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        while ((object = select_anything("Set WCS")) != NULL_TAG)  
            set_wcs_by_object(object);  
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

> 这段NX Open API代码的主要功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了一个函数mask_add_faces_and_edges，用于设置选择掩码，允许选择实体上的任意面和边。
> 3. 定义了一个函数select_anything，用于弹出一个选择对话框，用户可以交互式选择任意对象，并返回对象的tag。
> 4. 定义了一个函数set_wcs_by_object，根据用户选择的对象设置WCS坐标系。如果是边，先转换为曲线，然后获取该对象的变换矩阵，基于该矩阵创建一个临时坐标系并设为WCS。
> 5. 定义了一个函数do_it，用于循环调用select_anything获取用户选择的对象，并调用set_wcs_by_object设置WCS。
> 6. 定义了ufusr函数作为二次开发程序的入口点，调用UF_initialize初始化环境，调用do_it执行主体功能，最后调用UF_terminate终止环境。
> 7. 定义了ufusr_ask_unload函数，用于立即卸载二次开发程序。
>
> 总体来说，这段代码实现了交互式选择实体对象并基于该对象设置WCS坐标系的功能。
>
