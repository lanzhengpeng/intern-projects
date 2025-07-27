### 【0904】create smart point offset from csys 创建从坐标系偏移的智能点

#### 代码

```cpp
    /*HEAD CREATE_SMART_POINT_OFFSET_FROM_CSYS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_so.h>  
    #include <uf_part.h>  
    #include <uf_modl_expressions.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中新增的功能，用于打印系统日志。 */  
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
    static int mask_for_csys(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_coordinate_system_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_csys(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a coordinate system",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_csys, NULL, &resp,  
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
            ii;  
        tag_t  
            smart_point = NULL_TAG,  
            csys = NULL_TAG,  
            xyz[3];   
        while ((csys = select_a_csys("Select csys")) != NULL_TAG)  
        {  
            for (ii = 0; ii < 3; ii++)  
            {  
                UF_CALL(UF_SO_create_scalar_double(csys,  
                    UF_SO_update_within_modeling, 1.0, &xyz[ii]));  
            }  
            UF_CALL(UF_SO_create_point_3_scalars_csys(csys, csys, xyz,  
                 UF_SO_update_within_modeling, &smart_point));  
            UF_CALL(UF_SO_set_visibility_option(smart_point, UF_SO_visible));  
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

> 这段NX二次开发代码的主要功能是创建一个智能点，该智能点在所选坐标系的原点。代码主要包括以下几个部分：
>
> 1. 引入相关头文件：这些头文件包含代码中需要使用的NX API函数原型。
> 2. 定义错误报告函数：这个函数用于在API调用失败时输出错误信息。
> 3. 定义选择坐标系的面具：这个函数用于在UI中选择坐标系时设置选择面具，只允许选择坐标系类型的对象。
> 4. 定义选择坐标系的函数：这个函数用于弹出一个对话框，让用户选择一个坐标系对象。
> 5. 定义执行主体逻辑的函数：在这个函数中，首先选择一个坐标系，然后创建3个标量对象表示坐标轴，接着创建一个智能点，位置在坐标系的原点，最后设置智能点的可见性。
> 6. ufusr函数：这是NX二次开发脚本的入口函数，在这里初始化NX系统，调用执行主体逻辑的函数，然后终止NX系统。
> 7. 卸载函数：这个函数用于在脚本卸载时返回立即卸载。
>
> 总的来说，这段代码实现了一个简单的NX二次开发功能，允许用户选择一个坐标系，并在该坐标系的原点创建一个智能点。
>
