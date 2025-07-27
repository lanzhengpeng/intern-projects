### 【2776】set mask from outside sel function 设置外部选择功能的面具

#### 代码

```cpp
    /*HEAD SET_MASK_FROM_OUTSIDE_SEL_FUNCTION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog 是 V18 新增的函数。 */  
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
    static int mask_for_faces(UF_UI_selection_p_t select, void *mask)  
    {  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt, UF_UI_mask_t *sel_mask)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, sel_mask, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t   face;  
        UF_UI_mask_t mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        face = select_a_face("Select face to trim", &mask);  
        printf("Tag of face selected: %d\n", face);  
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

> 这段代码是一个NX二次开发代码示例，主要功能是选择一个面并进行高亮显示。以下是代码的主要组成部分：
>
> 1. 包含头文件：引入了必要的NX API头文件。
> 2. 报错函数：定义了report_error函数，用于在API调用失败时报告错误信息。
> 3. 面选择掩码函数：定义了mask_for_faces函数，用于设置选择掩码，以仅选择实体类型为面类型。
> 4. 选择面函数：定义了select_a_face函数，用于通过单选对话框选择一个面，并返回面的标签。
> 5. 主函数：定义了do_it函数，用于初始化NX API，选择一个面并打印出该面的标签。
> 6. 用户函数：定义了ufusr函数，作为NX二次开发的入口函数，调用do_it函数执行主要逻辑。
> 7. 卸载函数：定义了ufusr_ask_unload函数，用于在插件卸载时执行清理工作。
>
> 总的来说，这段代码通过选择对话框让用户选择一个面，然后获取并打印该面的标签。通过二次开发实现了交互式的选择和获取NX模型中的面信息。
>
