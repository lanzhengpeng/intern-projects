### 【0804】create offset relative datum plane 创建相对基准面偏置

#### 代码

```cpp
    /*HEAD CREATE_OFFSET_RELATIVE_DATUM_PLANE CCC UFUN */  
    #include <string.h>  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
    #include <uf_defs.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中新增的功能。文档要求只提供翻译结果，不添加任何其他内容。

因此，翻译结果为：

UF_print_syslog是V18版本新增的功能。 */  
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
    /* 里海 */  
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
    static void do_it(void)  
    {  
        double  
            offset_d = 3.14,  
            ref_point[3] = { 1,1,1 };  
        int  
            num_dplanes,  
            point_select[3] = { 0,0,0 },  
            which_plane = 1;  
        tag_t  
            dplane,  
            dplanes[2],  
            obj_eids[3];  
        char  
            offset_s[133];  
        sprintf(offset_s, "%f", offset_d);  
        while ((dplane = select_a_datum_plane("Select datum_plane")) != NULL_TAG)  
        {  
            obj_eids[0] = dplane;  
            UF_CALL(UF_MODL_create_relative_dplane(1, obj_eids, point_select,  
                which_plane, ref_point, "0.0", offset_s, &num_dplanes, dplanes));  
        }  
    }  
    /* 里海 */  
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

> 这段代码是用于NX的二次开发，主要功能是在NX中创建相对基准平面。具体来说，代码主要包括以下几个部分：
>
> 1. 头文件包含：包含必要的NX API头文件，如uf.h, uf_obj.h等。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 3. 基准面选择掩码：定义了一个函数mask_for_datum_planes，用于设置选择基准面的掩码，确保只能选择基准面。
> 4. 选择基准面：定义了一个函数select_a_datum_plane，用于在图形界面中选择一个基准面，并返回其tag。
> 5. 创建相对基准面：定义了一个函数do_it，用于实现创建相对基准面的逻辑。首先，它循环调用select_a_datum_plane选择基准面，然后调用UF_MODL_create_relative_dplane接口，根据参考点和偏置距离创建相对基准面。
> 6. UFusr函数：定义了UFusr函数作为程序的入口，初始化NX环境，调用do_it函数实现功能，然后终止NX环境。
> 7. 卸载函数：定义了卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总的来说，这段代码通过二次开发的方式，实现了在NX中方便地创建相对基准平面的功能。
>
