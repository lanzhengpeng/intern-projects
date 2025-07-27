### 【2785】set partial shading on for selected body 为选中的体设置部分阴影

#### 代码

```cpp
    /*HEAD SET_PARTIAL_SHADING_ON_FOR_SELECTED_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。 */  
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
    /* qq3123197280 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
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
        tag_t  
            body = NULL_TAG;  
        while( (body = select_a_body( "Select Body:" )) != NULL_TAG )  
        {  
            UF_CALL( UF_OBJ_set_partially_shaded( body, TRUE ));  
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

> 这是段NX二次开发代码，主要功能是选择一个实体，将其设置为部分阴影显示。
>
> 主要代码逻辑如下：
>
> 1. 包含必要的NX Open头文件，定义了UF_CALL宏用于调用NX Open函数，并报告错误。
> 2. 定义了一个report_error函数，用于在出错时打印错误信息。
> 3. 定义了一个mask_for_bodies函数，用于设置选择掩码，只选择实体。
> 4. 定义了一个select_a_body函数，用于弹出选择对话框，让用户选择一个实体。如果选择成功，返回实体标签，否则返回NULL_TAG。
> 5. 定义了一个do_it函数，循环调用select_a_body函数，直到用户取消选择。如果选择成功，调用UF_OBJ_set_partially_shaded函数，将实体设置为部分阴影显示。
> 6. 定义了ufusr函数，作为NX的回调函数。首先初始化NX，然后调用do_it函数执行主要逻辑，最后终止NX。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 通过以上代码，实现了选择实体并设置为部分阴影显示的功能。
>
