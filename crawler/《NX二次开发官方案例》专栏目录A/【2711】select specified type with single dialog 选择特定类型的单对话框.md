### 【2711】select specified type with single dialog 选择特定类型的单对话框

#### 代码

```cpp
    /*HEAD SELECT_SPECIFIED_TYPE_WITH_SINGLE_DIALOG CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是在 V18 版本中新增的。 */  
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
    static int init_proc(UF_UI_selection_p_t select, void *user_data)  
    {  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1,  
            (UF_UI_mask_t *)user_data)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static void do_it(void)  
    {  
        int  
            sel,  
            resp;  
        tag_t  
            object,  
            view;  
        double  
            cursor[3];  
        char  
            messg[80],  
            options[3][38] = { "body", "face", "edge" };  
        UF_UI_mask_t  
            mask[3] = {  
            UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY,  
            UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE,  
            UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        while ((resp = uc1603("Select mask type", 0, options, 3)) != 2)  
        {  
            sel = resp - 5;  
            sprintf(messg, "Select a %s", options[sel]);  
            if (!UF_CALL(UF_UI_select_with_single_dialog(messg, options[sel],  
                UF_UI_SEL_SCOPE_NO_CHANGE, init_proc, &mask[sel], &resp, &object,  
                cursor, &view)) && (resp == UF_UI_OBJECT_SELECTED) ||  
                (resp == UF_UI_OBJECT_SELECTED_BY_NAME))  
            {  
                UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            }  
        }  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 初始化NX环境，包括错误报告函数的注册和NX的初始化。
> 2. 定义了选择不同类型的几何体（体、面、边）的选择掩码，并使用对话框让用户选择要选择的对象类型。
> 3. 根据用户的选择，初始化选择过程，设置对应的选择掩码，并提示用户选择一个对象。
> 4. 用户选择一个对象后，关闭高亮显示。
> 5. 循环让用户选择不同类型的对象，直到用户选择退出。
> 6. 结束NX环境。
>
> 该示例展示了如何使用NX的选择对话框让用户选择不同类型的几何对象，并处理选择结果。同时也演示了错误报告函数的编写，以及NX的初始化和退出。
>
